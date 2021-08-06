#include "h2.h"
#include <filter/AwFiltering.h>
#include <utils/time.h>
#include <math/AwMath.h>
#include <AwCore.h>
#include <aw_armadillo.h>
#include <qmessagebox.h>
#include <QFileDialog>
#include <QtConcurrent>
#include <utils/json.h>
#include <AwKeys.h>

H2Plugin::H2Plugin()
{
	type = AwProcessPlugin::Background;
	setFlags(Aw::ProcessFlags::PluginAcceptsTimeSelections);
	category = "Process:Correlation:H2/R2 connectivity";
	name = QString("h2");
	version = "1.0.0";
	description = QString(tr("Compute H2/R2 connectivity graphs."));
	setFlags(Aw::ProcessFlags::ProcessHasInputUi | Aw::ProcessFlags::CanRunFromCommandLine);
	m_settings[keys::json_batch] = AwUtilities::json::fromJsonFileToString(":/h2/args.json");
	m_helpUrl = "h2/r2::Correlation::https://gitlab-dynamap.timone.univ-amu.fr/anywave/anywave/-/wikis/plugin_h2";
}

H2Plugin::~H2Plugin()
{
	
}

H2::H2() : AwProcess()
{
	// Inputs
	pdi.addInputChannel(-1, 2, 0);
	setInputFlags(Aw::ProcessIO::GetDurationMarkers	| Aw::ProcessIO::GetWriterPlugins);
	setInputModifiers(Aw::ProcessIO::modifiers::AcceptChannelSelection);
	m_winSize = 2; // 2s
	m_maxLag = (float)0.1;	// 0.1s lag
	m_nCells = 10;  // 10 cellules
	m_step = 1;	// pas de 1s
	m_scaleProgress = 0;
	m_ui = NULL;
	// we have localization support, so provide the prefix for all the language files.
	m_downSample = 0.;
	m_method = Global::Method::h2;
	m_currentBand.name = "n/a";
	m_currentBand.lp = m_currentBand.hp = 0.;
}

H2::~H2()
{
	clean();
	if (m_ui)
		delete m_ui;
	emit sendCommand(AwProcessCommand::RemoveLastHighlightedSection, QVariantList());
}



void H2::setMethodName(QString name)
{
	m_methodName = name.trimmed().toLower();
	if (name == "h2")
		m_method = Global::Method::h2;
	else
		m_method = Global::Method::r2;
}

void H2::setFreqBand(QString name, float hp, float lp)
{
	m_currentBand.name = name;
	m_currentBand.hp = hp;
	m_currentBand.lp = lp;
}


bool H2::showUi()
{
	// to make the plugin scriptable : DO NOT instantiate UI Objects in the object constructor.
	// Make it dynamically as a scripted plugin cannot access the main thread UI.
	if (m_ui == NULL)
		m_ui = new H2UI(this);
	m_ui->dataFolder = pdi.input.settings[keys::data_dir].toString();
	m_ui->sbTimeWindow->setValue(m_winSize);
	m_ui->sbMaxLag->setValue(m_maxLag);
	m_ui->sbStep->setValue(m_step);
	m_ui->samplingRate = pdi.input.channels().first()->samplingRate();
	m_ui->directory = pdi.input.settings[keys::working_dir].toString();
	m_ui->markers = pdi.input.markers();
	//if (modifiersFlags() & Aw::ProcessIO::modifiers::UserSelectedMarkers)
	//	m_ui->widgetInputData->hide();
	if (m_ui->exec() == QDialog::Accepted)	{
		m_maxLag = m_ui->sbMaxLag->value();
		m_winSize = m_ui->sbTimeWindow->value();
		m_step = m_ui->sbStep->value();
		m_method = m_ui->method;
		if (modifiersFlags() & Aw::ProcessIO::modifiers::UserSelectedMarkers)
			return true;
		auto fd = pdi.input.settings[keys::file_duration].toDouble();
		if (!m_ui->skippedLabels.isEmpty() || !m_ui->usedLabels.isEmpty()) {
			auto markers = AwMarker::duplicate(pdi.input.markers());
			auto inputMarkers = AwMarker::getInputMarkers(markers, m_ui->skippedLabels, m_ui->usedLabels, fd);
			if (inputMarkers.isEmpty()) {
				if (QMessageBox::question(0, "Data input", "No valid data sections defined. Use whole data?") == QMessageBox::No) {
					return false;
				}
				else {
					pdi.input.clearMarkers();
					pdi.input.addMarker(new AwMarker("whole_data", 0., fd));
				}
			}
			else {
				pdi.input.clearMarkers();
				pdi.input.setNewMarkers(inputMarkers);
			}
			AW_DESTROY_LIST(markers);
		}
		else {
			pdi.input.clearMarkers();
			pdi.input.addMarker(new AwMarker("whole_data", 0., fd));
		}

		return true;
	}
	return false;
}


void H2::clean()
{
	// clean channels associated to run
	for (auto r : m_runs)	{
		while (!r->params.isEmpty())
			delete r->params.takeFirst();
	}
	while (!m_runs.isEmpty())
		delete m_runs.takeFirst();
	while (!m_markers.isEmpty())
		delete m_markers.takeFirst();
	m_skippedMarkers.clear();
}


int H2::initialize()
{
	clean();
	QList<AwChannel *> list = pdi.input.channels();
	
	if (list.size() < 2) {
		m_error = QString("Less than two channels set as input. Aborted.");
		return -1;
	}

	// Merge input markers, that will implicilty duplicate them even if no merge is done.
	if (!pdi.input.markers().isEmpty()) {
		sendMessage("Markers received as input:");
		int count = 1;
		for (auto m : pdi.input.markers()) 
			sendMessage(QString("%1. %2").arg(count++).arg(m->label()));
		sendMessage("Checking that they all have a duration.");
		// remove single markers
		m_markers = AwMarker::duplicate(AwMarker::getMarkersWithDuration(pdi.input.markers()));
		sendMessage("Markers with duration:");
		//	m_markers = AwMarker::merge(m_markers);
		// if no duration markers => make one global
		if (m_markers.isEmpty()) {
			sendMessage("None => creating a Global marker to compute on the whole data.");
			m_markers << new AwMarker("Global", 0., pdi.input.reader()->infos.totalDuration());
		}
		else {
			count = 1;
			for (auto m : m_markers)
				sendMessage(QString("%1. %2").arg(count++).arg(m->label()));
		}
	}
	else {
		sendMessage("No markers received as input. Created a Global marker to compute on the whole data.");
		m_markers << new AwMarker("Global", 0., pdi.input.reader()->infos.totalDuration());
	}

	// generer les paires de canaux (on calcule le H2 sur toutes les combinaisons de canal possibles).
	h2_params *h2_p;

	// pre build runs with pair of channels.
	QStringList labels = AwChannel::getLabels(pdi.input.channels(), true);
	for (auto m : m_markers) {
		// check if at least one iteration of H2 can be computed
		qint64 minSize = (qint64)floor((m_winSize + m_maxLag) * list.first()->samplingRate());
		qint64 dataSize = (qint64)floor(m->duration() * list.first()->samplingRate());
		if (minSize > dataSize) {
			m_skippedMarkers.append(m);
			continue;
		}
		H2_run *run = new H2_run;
		run->samplingRate = list.first()->samplingRate();
		run->marker = m;
		run->h2_process = this;
		int indexOfChannel = -1;
		if (!m->targetChannels().isEmpty()) {
			QString targetChannel = m->targetChannels().first();
			indexOfChannel = labels.indexOf(targetChannel);
		}

		for (int i = 0; i < list.size() - 1; i++) {
			for (int j = i + 1; j < list.size(); j++) {
				// Build one pair
				h2_p = new h2_params;
				run->params.append(h2_p);
				h2_p->x = list.at(i);
				h2_p->y = list.at(j);
				h2_p->maxLag = (int)floor(m_maxLag * h2_p->x->samplingRate());
				h2_p->horizon = (int)floor(m_winSize * h2_p->x->samplingRate());
				h2_p->step = (int)(m_step * h2_p->x->samplingRate());
				h2_p->nbBins = m_nCells;
				h2_p->index_x = i;
				h2_p->index_y = j;
				h2_p->method = m_method;
				h2_p->h2_process = this;
				h2_p->triggeredChannel = indexOfChannel;
				// Build reversed pair
				h2_p = new h2_params;
				run->params.append(h2_p);
				h2_p->x = list.at(j);
				h2_p->y = list.at(i);
				h2_p->maxLag = (int)floor(m_maxLag * h2_p->x->samplingRate());
				h2_p->horizon = (int)floor(m_winSize * h2_p->x->samplingRate());
				h2_p->step = (int)floor(m_step * h2_p->x->samplingRate());
				h2_p->nbBins = m_nCells;
				h2_p->index_x = j;
				h2_p->index_y = i;
				h2_p->method = m_method;
				h2_p->h2_process = this;
				h2_p->triggeredChannel = indexOfChannel;
			}
		}
		m_runs << run;
	}
	return 0;
}

int H2::saveToMatlab(const QString& fileName, H2_run *run)
{
	AwMATLABFile file;
	// write the method used to compute
	QString method = m_method == Global::Method::h2 ? QString("h2") : QString("r2");
	// write current frequency band used to compute
	QVector<double> freqBounds = { m_currentBand.hp, m_currentBand.lp };
	// get record data and time from reader plugin
	QTime time = AwUtilities::time::timeFromString(pdi.input.reader()->infos.recordingTime());
	QDate date = AwUtilities::time::dateFromString(pdi.input.reader()->infos.recordingDate());
	int h = time.hour(), m = time.minute(), s = time.second();
	int day = date.day(), month = date.month(), year = date.year();
	H2_run *first_run = m_runs.first(); // take the first run to init some vars.

	// electrode names
	QStringList labels;
	for (auto c : first_run->channels)
		labels << c->fullName();

	// h2 and lag matrices (MERGED VERSION)
	uword sx = first_run->channels.size();
	uword sy = sx;
	uword sz = 0;

	QList<H2_run*> runs; // local list to handle the case where we're only saving one run
	if (run)
		runs << run;
	else
		runs = m_runs;

	for (auto r : runs)
		sz += r->nIterations;
	cube h2mat(sx, sy, sz, arma::fill::zeros);
	cube lagmat(sx, sy, sz, arma::fill::zeros);
	quint32 sectionOffset = 0;
	int section_index = 0, channelNumber = -1, z = 0;
	QVector<int> time_vector(sz);
	QVector<int> section_vector(sz);
	QVector<int> channel_indexes(sz);
	QVector<double> time_vector_s(sz);
	int time_index = 0;

	for (auto r : runs) {
		for (auto p : r->params) {
			double *h2_data = p->h2.data();
			int *lag_data = p->lag.data();
			for (int iter = 0; iter < p->h2.size(); iter++) {
				h2mat(p->index_x, p->index_y, z + iter) = h2_data[iter];
				lagmat(p->index_x, p->index_y, z + iter) = (double)lag_data[iter];
			}
		}
		sectionOffset += r->params.first()->h2.size();
		channelNumber = r->params.first()->triggeredChannel + 1;
		for (int i = 0; i < r->nIterations; i++) {
			time_vector[time_index] = (int)floor((r->marker->start() + i * m_step) * m_currentSamplingRate);
			time_vector_s[time_index] = (double)(r->marker->start() + i * m_step);
			// add 1 to index as we export in a MATLAB file and indexes start at 1 in MATLAB
			channel_indexes[time_index] = channelNumber;
			section_vector[time_index++] = section_index;
		}
		section_index++;
		z += r->nIterations;
	}
	try {
		file.create(fileName);
		file.writeString("method", method);
		file.writeString("band", m_currentBand.name);
		file.writeVec("filters", freqBounds);
		file.writeScalar("hour", h);
		file.writeScalar("minute", m);
		file.writeScalar("second", s);
		file.writeScalar("day", day);
		file.writeScalar("month", month);
		file.writeScalar("year", year);
		file.writeScalar("aw_windowSize", m_winSize);
		// step
		file.writeScalar("aw_step", step());
		// maxLag
		file.writeScalar("aw_maxLag", maxLag());
		// starting Time
		file.writeScalar("aw_start", first_run->marker->start());
		file.writeStringCellArray("electrode_names", labels);
		file.writeMatrix("aw_h2", h2mat);
		file.writeMatrix("aw_lag", lagmat);
		file.writeVec("time", time_vector);
		file.writeVec("time_s", time_vector_s);
		file.writeVec("section", section_vector);
		file.writeVec("channel", channel_indexes);
		// write extra variables to facilitate reloading of graphs
		file.writeScalar("nb_section", m_runs.size());
		QVector<int> iterationsBySection;
		for (auto r : m_runs)
			iterationsBySection << r->nIterations;
		file.writeVec("section_iterations", iterationsBySection);
		file.writeScalar("samplingRate", m_currentSamplingRate);
		// save labels of markers used to make all the runs
		QStringList labels;
		for (auto r : m_runs) {
			labels << r->marker->label();
		}
		file.writeStringCellArray("sections", labels);
	}
	catch (const AwException& e) {
		sendMessage(QString("Failed to write MATLAB file: %1 in %2").arg(e.errorString()).arg(e.origin()));
		return -1;
	}
	return 0;
}


void H2::setProgressNewRange(int start, int end)
{
	m_scaleProgress = 100 / (((float)end - (float)start) + 1);
}

void H2::setProgressValue(int value)
{
	emit progressChanged((int)((float)value * m_scaleProgress));
}

void H2::runFromCommandLine()
{
	auto args = pdi.input.settings;
	// get params from args
	QString m = args.value(h2::algorithm).toString().toLower();
	// default method is always h2
	QString algo = "h2";
	if (m == "r2") {
		m_method = Global::Method::r2;
		algo = "r2";
	}
	QString baseMATLABFile = algo;
	if (!args.contains(h2::time_window) || !args.contains(h2::max_lag) || !args.contains(h2::step)) {
		sendMessage("Missing crucial parameters for computation. Aborted.");
		return;
	}

	m_winSize = args[h2::time_window].toDouble();
	m_maxLag = args[h2::max_lag].toDouble();
	m_step = args[h2::step].toDouble();
	int decimateFactor = args.value(h2::downsampling_factor).toInt();
	if (decimateFactor < 1)
		decimateFactor = 1;

	if (initialize() == -1) {
		sendMessage(m_error);
		return;
	}

	if (m_runs.isEmpty()) {
		sendMessage("No run to compute. Check time window and step parameters against data size.");
		return;
	}

	QString dir;
	if (args.contains(keys::output_dir))
		dir = args.value(keys::output_dir).toString();
	else
		dir = pdi.input.settings[keys::data_dir].toString();

	// compute
	float sr = pdi.input.channels().first()->samplingRate();	// save sampling rate in case we use downsampling
	m_currentSamplingRate = sr;
	if (decimateFactor > 1) {
		m_currentSamplingRate = sr / (float)decimateFactor;
	}
	for (auto run : m_runs) {
		QString message = QString("Computing on section starting at %1s ending at %2s").arg(run->marker->start()).arg(run->marker->end());
		sendMessage(message);
		run->channels = pdi.input.channels();
		if (decimateFactor > 1) {
			sendMessage("Loading and downsampling data...");
			requestData(&pdi.input.channels(), run->marker->start(), run->marker->duration(), true);
			AwFiltering::downSample(pdi.input.channels(), decimateFactor);
			sendMessage("Done.");
		}
		else {
			sendMessage("Loading and filtering data...");
			requestData(&pdi.input.channels(), run->marker->start(), run->marker->duration());
			sendMessage("Done.");
		}
		sendMessage("Checking for NaN values...");
		if (AwMath::isNanInChannels(pdi.input.channels())) {
			sendMessage("NaN value detected in data. Aborting...");
			return;
		}
		sendMessage("OK.");
		// if down sampling, RECOMPUTE all parameters in samples
		if (decimateFactor > 1) {
			for (auto p : run->params) {
				p->maxLag = (int)floor(m_maxLag * m_currentSamplingRate);
				p->horizon = (int)floor(m_winSize * m_currentSamplingRate);
				p->step = (int)floor(m_step * m_currentSamplingRate);
			}
		}
		sendMessage("Computing...");
		// Check for the selected method
		QFuture<void> res = QtConcurrent::map(run->params, ComputeBackground);
		res.waitForFinished();
		sendMessage("Calculation complete.");
		if (decimateFactor > 1) {
			for (auto c : pdi.input.channels())
				c->setSamplingRate(sr);
		}
		run->nIterations = run->params.first()->h2.size();
	}

	float lp = args.value(keys::lp).toDouble();
	float hp = args.value(keys::hp).toDouble();

	bool saveOneFile = true;
	if (args.contains(h2::several_result_files)) {
		if (args.value(h2::several_result_files).toBool())
			saveOneFile = false;
	}


	if (args.contains(keys::output_prefix)) {
		QString pref = args.value(keys::output_prefix).toString();
		baseMATLABFile = QString("%1_").arg(pref);
	}
	else {
		QFileInfo fi(pdi.input.settings[keys::data_path].toString());
		baseMATLABFile = QString("%1_").arg(fi.baseName());
	}

	if (saveOneFile) {
		sendMessage("Saving to MATLAB file...");
		baseMATLABFile = QString("%1_algo-%2_hp-%3_lp-%4").arg(baseMATLABFile).arg(algo).arg(hp).arg(lp);
		QString file = QString("%1/%2.mat").arg(dir).arg(baseMATLABFile);

		m_currentBand.name = "Batch Mode";
		m_currentBand.lp = lp;
		m_currentBand.hp = hp;
		if (saveToMatlab(file) == -1) {
			sendMessage("Error when writing to MATLAB file.");
			return;
		}
	}
	else {
		QMap<QString, int> map;
		for (auto run : m_runs) {
			auto label = run->marker->label();
			if (!map.contains(label)) {
				map.insert(label, 1);
			}
			else {
				auto i = map.value(label);
				map.insert(label, ++i);
			}
			baseMATLABFile = QString("%1_algo-%2_hp-%3_lp-%4_sec-%5_num-%6").arg(baseMATLABFile).arg(algo).arg(hp).arg(lp).arg(label).arg(map.value(label));
			QString file = QString("%1/%2.mat").arg(dir).arg(baseMATLABFile);
			m_currentBand.name = "Batch Mode";
			m_currentBand.lp = lp;
			m_currentBand.hp = hp;
			if (saveToMatlab(file, run) == -1) {
				sendMessage(QString("Error when writing %1 file.").arg(baseMATLABFile));
			}
		}
	}
	sendMessage("Done.");
}


void H2::run()
{
	if (initialize() == -1) {
		sendMessage(m_error);
		return;
	}
	QList<AwChannel *> list = pdi.input.channels();

	if (!m_skippedMarkers.isEmpty()) {
		sendMessage("Some marked data cannot be used to compute the correlation due to insufficient data length.");
		sendMessage("At least window size + max lag length is required to compute one iteration.");
		sendMessage("Skipped markers are:");
		for (auto m : m_skippedMarkers)
			sendMessage(QString("Skipped marker %1 (%2-%3)").arg(m->label()).arg(m->start()).arg(m->end()));
	}
	if (m_runs.isEmpty()) {
		sendMessage("No run to compute. Check parameters and data selections length.");
		m_flags &= ~Aw::ProcessFlags::HasOutputUi;
		return;
	}
	computeRuns();
	if (!m_resultFiles.isEmpty()) {
		QVariantMap map;
		map["command"] = AwProcessCommand::LaunchProcess;
		map["process_name"] = QString("Correlation Graphs");
		QStringList args = { m_resultFiles };
		map["args"] = args;
		emit sendCommand(map);
	}
}

int H2::computeRuns()
{
	QFutureWatcher<void> watcher;

	connect(&watcher, SIGNAL(progressRangeChanged(int, int)), this, SLOT(setProgressNewRange(int, int)));
	connect(&watcher, SIGNAL(progressValueChanged(int)), this, SLOT(setProgressValue(int)));
	int runNumber = 1;
	// duplicate channels 
	AwChannelList dup = AwChannel::duplicateChannels(pdi.input.channels());
	// compute runs through selected frequency bands
	for (auto b : m_ui->bands.keys()) {
		QPair<float, float> value = m_ui->bands.value(b);
		if (b == "AnyWave") {
			auto filters = pdi.input.filterSettings.filters(dup.first()->type());
			if (!filters.isEmpty()) {
				value.first = filters[0];
				value.second = filters[1];
			}
			else {
				value.first = 0;
				value.second = 0;
			}
		}
		m_currentBand.name = b;
		m_currentBand.hp = value.first;
		m_currentBand.lp = value.second;
		// apply filters
		AwChannel::setFilters(dup, value.first, value.second);
		// compute all runs for the current frequency band
		if (m_ui->downSampling > 1)
			m_downSample = dup.first()->samplingRate() / (float)m_ui->downSampling;
		float sr = dup.first()->samplingRate();	// save sampling rate in case we use downsampling
		m_currentSamplingRate = sr;
		if (m_downSample)
			m_currentSamplingRate = m_downSample;

		// RESET THE RUNS DATA !!!!
		QEventLoop loop;
		int sectionCount = 0;
		for (auto run : m_runs) {
		
			run->channels = dup;  // keep a ref to the list
			// rebuild pair of channels with duplicated ones
			for (auto p : run->params) {
				p->x = dup.at(p->index_x);
				p->x->clearData();
				p->y = dup.at(p->index_y);
				p->y->clearData();
				p->h2.clear();
				p->lag.clear();
			}
			if (m_downSample) {
				sendMessage("Loading data...");
				requestData(&dup, run->marker->start(), run->marker->duration(), true);
				sendMessage(QString("Downsampling data to %1Hz...").arg(m_downSample));
				AwFiltering::downSample(dup, m_ui->downSampling);
				sendMessage("Done.");
			}
			else {
				sendMessage("Loading and filtering data...");
				requestData(&dup, run->marker->start(), run->marker->duration());
			//	saveSignals(AwChannel::duplicateChannels(dup));
			}
			sendMessage("Checking for NaN values...");
			if (AwMath::isNanInChannels(dup)) {
				sendMessage("NaN value detected in data. Aborting...");
				while (!dup.isEmpty())
					delete dup.takeFirst();
				return -1;
			}
			sendMessage("Done");

			// if down sampling, RECOMPUTE all parameters in samples
			if (m_downSample > 0.) {
				for (auto p : run->params) {
					p->maxLag = (int)floor(m_maxLag * m_downSample);
					p->horizon = (int)floor(m_winSize * m_downSample);
					p->step = (int)floor(m_step * m_downSample);
				}
			}
			sendMessage(QString("Computing section %1...(%2/%3)").arg(run->marker->label()).arg(++sectionCount).arg(m_runs.size()));
			watcher.setFuture(QtConcurrent::map(run->params, ComputeBackground));
			while (!watcher.isFinished() && !isAborted())
				loop.processEvents();
			if (isAborted()) {
				watcher.cancel();
				watcher.waitForFinished();
				sendMessage("Process aborted.");
				while (!dup.isEmpty())
					delete dup.takeFirst();
				return -1;
			}
			sendMessage("Calculation complete.");
			// if downsampling, reset sampling rate for the next run
			for (auto c : dup)
				c->setSamplingRate(sr);

			run->nIterations = run->params.first()->h2.size();
		}
		//// check for correctly computer H2  (if the selection is not long enough there might be no iterations 
		//// compute number of iterations for each run
		//for (auto r : m_runs) {
		//	r->nIterations = r->params.at(0)->h2.size();
		//}
		// AUTO SAVING TO MATLAB
		// Insert the method into the filename
		QString method = m_method == Global::Method::h2 ? QString("H2") : QString("R2");
		// The file name is based on the data file name with suffixes to indicate the frequency band used.
		QString LPString = m_currentBand.lp > 0 ? QString("%1Hz").arg(m_currentBand.lp) : QString("NoLP");
		QString HPString = m_currentBand.hp > 0 ? QString("%1Hz").arg(m_currentBand.hp) : QString("NoHP");

		// output_dir is always set by AnyWave:
		// the default output_dir is the data file dir when working on a file outside of a BIDS and is the derivatives path when 
		// working within a BIDS
		QString dir = pdi.input.settings[keys::output_dir].toString();
		QString baseFileName = pdi.input.settings.value(keys::data_file).toString();

		if (m_ui->saveInOneFile) {
			QString file = QString("%1_algo-%2_hp-%3_lp-%4.mat").arg(baseFileName).arg(method).arg(HPString).arg(LPString);
			sendMessage("Saving to MATLAB file...");
			auto filePath = QString("%1/%2").arg(dir).arg(file);
			int status = saveToMatlab(filePath);
			if (status == 0) {
				m_resultFiles << filePath;
				sendMessage("Done.");
			}
		}
		else {
			QMap<QString, int> map;
			for (auto run : m_runs) {
				auto label = run->marker->label();
				if (!map.contains(label)) {
					map.insert(label, 1);
				}
				else {
					auto i = map.value(label);
					map.insert(label, ++i);
				}
				baseFileName = QString("%1_algo-%2_hp-%3_lp-%4_sec-%5_num-%6").arg(baseFileName).arg(method).arg(HPString).arg(LPString).arg(label).arg(map.value(label));
				sendMessage(QString("Saving %1...").arg(baseFileName));
				auto filePath = QString("%1/%2").arg(dir).arg(baseFileName);
				if (saveToMatlab(filePath, run) == -1) {
					sendMessage(QString("Error when writing %1 file.").arg(filePath));
				}
				else {
					sendMessage("Done.");
					m_resultFiles << filePath;
				}

			}
		}
	}

	// destroy duplicated channels
	while (!dup.isEmpty())
		delete dup.takeFirst();

	return 0;
}


void H2::saveSignals(const AwChannelList& channels)
{
	QStringList pluginNames;
	for (auto p : pdi.input.writers) 
		pluginNames << p->name;
	int index = pluginNames.indexOf("AnyWave ADES Format");
	if (index == -1)
		return;
	auto plugin = pdi.input.writers.value(index);
	auto writer = plugin->newInstance();

	AwChannelList sources = channels;
	writer->infos.setChannels(channels);
	auto block = writer->infos.newBlock();
	block->setDuration(channels.first()->dataSize() / channels.first()->samplingRate());
	block->setSamples(channels.first()->dataSize());
	QString filePath = QString("%1/%2_%3Hz_%4Hz").arg(pdi.input.settings[keys::data_dir].toString()).arg(m_currentBand.name).arg(m_currentBand.hp).arg(m_currentBand.lp);
	writer->createFile(filePath);
	writer->writeData(&sources);
	writer->cleanUpAndClose();
	delete writer;
}

void ComputeBackground(h2_params *p)
{
	// stop condition to check.
	if (p->h2_process->endOfData())
			return;

	// Recompute nb of cells
	qint64 length = p->x->dataSize();
	float *x, *y;

	x = p->x->data();
	y = p->y->data();

	// Channels may be empty for some reasons (virtual channels for example may not have been computed.
	if (x == NULL || y == NULL)
		return;

	// Compute the exact number of iterations 
	int step = 0, count = 0;
	while (p->horizon + p->maxLag + step <= length) {
		count++;
		step += p->step;
	}

	int i = 0;
	for (qint64 j = 0; j < count; j++)	{
		if (p->h2_process->isAborted())
			break;
		Q_ASSERT(i + p->maxLag < length);
		if (p->method == Global::Method::h2)
			computeH2(x + i, y + i, length - p->horizon - i * p->step, p->horizon, p->nbBins, p->maxLag, p->step, p->lag, p->h2);
		else
			computeR2(x + i, y + i, length - p->horizon - i * p->step, p->horizon, p->nbBins, p->maxLag, p->step, p->lag, p->h2);
		if (p->h2_process->endOfData())
			break;
		i+= p->step;
	}
}


double r2(float *x, float *y, int n)
{
	double sigmaXY = 0.;
	double sigmaX = 0.;
	double sigmaY = 0.;
	double sigmaX2 = 0.;
	double sigmaY2 = 0.;
	for (int i = 0; i < n; i++) {
		sigmaXY += (double)x[i] * (double)y[i];
		sigmaX += (double)x[i];
		sigmaY += (double)y[i];
		sigmaX2 += (double)x[i] * (double)x[i];
		sigmaY2 += (double)y[i] * (double)y[i];
	}
	double r = (n * sigmaXY - sigmaX * sigmaY) / sqrt((n * sigmaX2 - sigmaX * sigmaX) * (n * sigmaY2 - sigmaY * sigmaY));
	return r * r;
}

//
// nonlivar()
// routine to compute nonlinear variance measure 
//
// this is based on the C code written by Fabrice Wendling (https://perso.univ-rennes1.fr/fabrice.wendling/)
// The variance bug had been fixed.

double nonlinvar(float *x, float *y, int n, int nbCell, QVector<int>& indexes)
{
	int i, j, k;
	float *Yb;
	double minX, maxX, delta;
	double eps2 = 0., moyY = 0., varY = 0.;
	double a, b, f;
	int indiceInf, indiceSup;
	double limit;


	Yb = new float[nbCell];
	minX = x[indexes[0]];
	maxX = x[indexes[n - 1]];
	delta = (maxX - minX) / nbCell;

	// compute means of Y values for each bin
	i = 0;
	for (k = 0; k < nbCell; k++)
	{
		j = 0;
		limit = (minX + (k + 1) * delta);
		Yb[k] = 0.;
		while ((i < n) && x[indexes[i]] <= limit)
		{
			Yb[k] += y[indexes[i]];
			j++;
			i++;
		}
		moyY += Yb[k];
		if (j != 0)
			Yb[k] /= j;
	}
	moyY /= n;

	// on calcule les innovations
	// 1) recherche de l'indice inf. sur les X
	indiceInf = 0;
	while (x[indexes[indiceInf]] < (minX + delta / 2))
		indiceInf++;
	
	// 2) recherche de l'indice sup. sur les X
	indiceSup = n - 1;
	while (x[indexes[indiceSup]] > (maxX - delta / 2))
		indiceSup--;

	// 3) calcul de f(Xi) et des erreurs : som des [Yi - f(Xi)]^2
	i = indiceInf;
	for (k = 1; k < nbCell; k++)
	{
		a = (Yb[k] - Yb[k - 1]) / delta; 
		b = Yb[k - 1];
		limit = (minX + (k + 1) * delta - delta / 2);
		while (x[indexes[i]] < limit )
		{
			f = a * (x[indexes[i]] - (minX + k * delta - delta / 2)) + b;

			//inno[i] = Y[indice[i]] - f;
			//eps2 += inno[i] * inno[i];
			eps2 += (y[indexes[i]] - f) * (y[indexes[i]] - f);
			varY += (y[indexes[i]] - moyY) * (y[indexes[i]] - moyY);
			i++;
		}
	}
	eps2 /= (indiceSup - indiceInf);
	 varY /= (indiceSup - indiceInf);

	//// Calcul de la variance des Y
	//for (i = 0; i < n; i++) 
	//	varY += (y[i] - moyY) * (y[i] - moyY);

	//varY /= n;

	delete[] Yb;
	// Calcul du h2
	return (varY - eps2) / varY;
}

double computeR2(float *x, float *y, int n, int horizon, int nbCell, int max_lag, int step, QVector<int>& delay, QVector<double>& h2)
{
	int i;
	int decalMemo;
	double r2tamp, r2memo = -1e99;
	float *x1, *y1;

	y1 = y;
	// move x from 0 lag to max lag 
	for (i = 0; i < max_lag; i++) {
		x1 = x + i;
		r2tamp = r2(x1, y1, horizon);
		if (r2tamp > r2memo) {
			r2memo = r2tamp;
			decalMemo = i;  // positive lag (means x is before y)
		}
	}	
	// reset x to 0 position
	x1 = x;
	// now move y from 1 to max lag
	for (i = 1; i < max_lag; i++)
	{
		y1 = y + i;
		r2tamp = r2(x1, y1, horizon);
		if (r2tamp > r2memo) {
			r2memo = r2tamp;
			decalMemo = -i;  // positive lag (means x is before y)
		}
	}
	h2 << r2memo;
	delay << decalMemo;

	return r2memo;
}

//
// nonlivar()
// routine to compute nonlinear variance measure 
//
// this is based on the C code written by Fabrice Wendling (https://perso.univ-rennes1.fr/fabrice.wendling/)

double computeH2(float *x, float *y, int n, int horizon, int nbCell, int max_lag, int step, QVector<int>& delay, QVector<double>& h2)
{
	int i, k;
	float *x1, *y1; 
	int decalMemo;
	double h2tamp, h2memo;

	QVector<double> horizon_values;
	QList<double> horizon_indexes;
	
	QList<QPair<float, int> > values_indexes;

	// recompute nbins depending on the number of samples set in horizon.
	int nbins = nbCell;
	if (horizon < 129)
		nbins -= 4;
	else if (horizon < 257)
		nbins -= 3;
	else if (horizon < 513)
		nbins -= 2;

	y1 = y;
	h2memo = -1.E99;

	QVector<int> indexes;
	// move x from 0 lag to max lag 
	for (i = 0; i < max_lag; i++) {
		x1 = x + i;
		values_indexes.clear();

		for (k = 0; k < horizon; k++) {
			QPair<float, int> pair;
			pair.first = x1[k];
			pair.second = k;
			values_indexes << pair;
		}

		// sort values
		qSort(values_indexes.begin(), values_indexes.end());
		// build indexes from sorted values
		indexes.clear();
		indexes.reserve(horizon);
		for (k = 0; k < horizon; k++)
			indexes << values_indexes.at(k).second;
		
		h2tamp = nonlinvar(x1, y1, horizon, nbins, indexes);

		if (h2tamp > h2memo) {
			h2memo = h2tamp;
			decalMemo = i;  // positive lag (means x is before y)
		}
	}
	// reste x to 0 position
	x1 = x;
	// recompute indexes
	values_indexes.clear();

	for (k = 0; k < horizon; k++)
	{
		QPair<float, int> pair;
		pair.first = x1[k];
		pair.second = k;
		values_indexes << pair;
	}
	// sort values
	qSort(values_indexes.begin(), values_indexes.end());

	// now move y from 1 to max lag
	for (i = 1; i < max_lag; i++)
	{
		y1 = y + i;
		// sort values
		qSort(values_indexes.begin(), values_indexes.end());
		// build indexes from sorted values
		indexes.clear();
		indexes.reserve(horizon);
		for (k = 0; k < horizon; k++)
			indexes << values_indexes.at(k).second;
		
		h2tamp = nonlinvar(x1, y1, horizon, nbins, indexes);

		if (h2tamp > h2memo) {
			h2memo = h2tamp;
			decalMemo = -i;  // negative lag (means y is before x)
		}
	}

	h2 << h2memo;
	delay << decalMemo;

	return h2memo;
}
