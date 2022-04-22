// AnyWave
// Copyright (C) 2013-2021  INS UMR 1106
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include "ica.h"
#include "ICASettings.h"
#include <QtMath>
#include <matlab/AwMATLAB.h>
#include <QFile>
#include <QDir>
#include <filter/AwFiltering.h>
#include <math/AwMath.h>
#include <filter/AwFilterSettings.h>
#include <QMessageBox>
#include <AwCore.h>
#include <AwEvent.h>

// use layout manager to store a layout in the matlab file
#include <layout/AwLayoutManager.h>
#include <layout/AwLayout.h>
#include <utils/json.h>
#include <AwKeys.h>
#include "infomax/ICAInfomax.h"
#include "sobi/ICASobi.h"


ICA::ICA()
{
	setInputFlags(/*Aw::ProcessIO::GetAsRecordedChannels |*/ Aw::ProcessIO::DontSkipBadChannels |
		Aw::ProcessIO::GetProcessPluginNames | Aw::ProcessIO::GetDurationMarkers | Aw::ProcessIO::GetWriterPlugins);
	setInputModifiers(Aw::ProcessIO::modifiers::IgnoreChannelSelection);
	pdi.addInputChannel(-1, 1, 0);
	pdi.addInputChannel(AwChannel::Source, 0, 0);
	m_isDownsamplingActive = true;
	m_hpf = m_lpf = m_notch = 0.;
	m_nComp = 0;
	auto infomax = new ICAInfomax(this);
	connect(infomax, SIGNAL(progressChanged(const QString&)), this, SIGNAL(progressChanged(const QString&)));
	m_algorithms << infomax;
#if defined(Q_OS_WIN) 
	auto sobi = new ICASobi(this);
	connect(sobi, SIGNAL(progressChanged(const QString&)), this, SIGNAL(progressChanged(const QString&)));
	m_algorithms << sobi;
#endif
}

ICAPlugin::ICAPlugin() : AwProcessPlugin()
{
    type = AwProcessPlugin::Background;
    category = "ICA:ICA Extraction";
	version = "2.2.1";
    name = QString("ICA");
    description = QString("Compute ICA");
	setFlags(Aw::ProcessFlags::ProcessHasInputUi | Aw::ProcessFlags::CanRunFromCommandLine);	
	m_settings[keys::json_batch] = AwUtilities::json::fromJsonFileToString(":/ica/json/batch.json");
	m_helpUrl = "ICA::https://gitlab-dynamap.timone.univ-amu.fr/anywave/anywave/-/wikis/plugin_ica";
}

ICA::~ICA()
{
	qDeleteAll(m_algorithms);
	qDeleteAll(m_rawChannels);
}

bool ICA::showUi()
{
	// request montage to DataManager
	QVariantMap settings;
	settings[keys::channels_source] = keys::channels_source_raw;
	// Use the synchronous call here as we are not yet running in a separate thread
	selectChannels(settings, &m_rawChannels);

	ICASettings ui(this);

	if (ui.exec() == QDialog::Accepted)	{
		auto args = pdi.input.settings;
		QString dir = pdi.input.settings.value(keys::output_dir).toString();
		// just testing to write a file in the output_dir 
		// m_filename is wrong here
		m_fileName = QString("%1/MEG_1Hz_120Hz_50c_ica.mat").arg(dir);
		QFile test(m_fileName);
		if (!test.open(QIODevice::WriteOnly)) {
			QMessageBox::critical(0, "Saving results", QString("Could not create %1").arg(m_fileName));
			return false;
		}
		test.close();
		QFile::remove(m_fileName);
		//pdi.input.settings.unite(ui.args);
		AwUniteMaps(pdi.input.settings, args);
		return true;
	}
	return false;
}

void ICA::init()
{

}


bool ICA::batchParameterCheck(const QVariantMap& hash)
{
	// this is an exhaustive test as we don't have a file open at this stage.
	// just checking for some parameters: 
	return hash.value(keys::comp).toInt() > 1;
}

int ICA::initParameters()
{
	auto args = pdi.input.settings;
	QStringList algoNames;
	for (auto const& algo : m_algorithms)
		algoNames << algo->name();

	m_isDownsamplingActive = false;
	// BIDS specific:
	// when launching ICA in batch mode and specifying ieeg as modality:
	// check for SEEG channels if none present check for EEG.
	if (args.value(keys::modality).toString().toLower() == "ieeg") {
		m_modality = AwChannel::SEEG;
		m_channels = AwChannel::getChannelsOfType(pdi.input.channels(), m_modality);
		if (m_channels.isEmpty()) {
			m_modality = AwChannel::EEG;
			m_channels = AwChannel::getChannelsOfType(pdi.input.channels(), m_modality);
		}
	}
	else {
		m_modality = AwChannel::stringToType(args.value(keys::modality).toString());
		if (m_modality == -1) {
			sendMessage(QString("modality: %1 invalid parameter").arg(m_modality));
			return -1;
		}
	}
	m_channels = AwChannel::getChannelsOfType(pdi.input.channels(), m_modality);

	if (args.contains(keys::use_seeg_electrode) && m_modality == AwChannel::SEEG) {
		QString label = args.value(keys::use_seeg_electrode).toString();
		sendMessage(QString("Will compute only on SEEG electrode:%1").arg(label));
		AwChannelList list;
		for (auto c : m_channels) {
			if (c->name().startsWith(label))
				list << c;
		}
		if (list.isEmpty()) {
			sendMessage(QString("use_seeg_electrode: the specified electrode is not present in file, specified montage or picked channels."));
			return -1;
		}
		m_channels = list;
	}

	m_channels = AwChannel::removeDoublons(m_channels);
	auto badLabels = pdi.input.settings[keys::bad_labels].toStringList();
	if (args.contains(keys::skip_bad_channels)) {
		if (!args.value(keys::skip_bad_channels).toBool())
			badLabels.clear();
	}
	m_samplingRate = m_channels.first()->samplingRate();
	if (args.contains(keys::downsampling))
		m_isDownsamplingActive = args.value(keys::downsampling).toBool();
	// check for bad labels 
	if (!badLabels.isEmpty()) {
		foreach(AwChannel *c, m_channels)
			if (badLabels.contains(c->name()))
				m_channels.removeAll(c);
	}
	m_nComp = m_channels.size();

	if (args.contains(keys::comp))
		m_nComp = args.value(keys::comp).toInt();

	// check algo
	// some algos have a fixed number of components (no PCA)
	m_selectedAlgo = m_algorithms.first();
	if (args.contains("algorithm")) {
		auto algoName = args.value("algorithm").toString().toLower().simplified();
		int index = algoNames.indexOf(algoName);
		if (index == -1) 
			sendMessage("The specified algorithm does not exist. Switching to default Infomax.");
		else
			m_selectedAlgo = m_algorithms.at(index);
	}
	if (m_nComp > m_channels.size()) {
		sendMessage("The specified number of components is greater dans the number of available channels in data. Aborted.");
		return -1;
	}

	if (m_nComp < 2) {
		sendMessage("Not enough components to compute. Aborted.");
		return -1;
	}

	sendMessage(QString("computing ica on file %1 and %2 channels...").arg(pdi.input.settings[keys::data_path].toString()).arg(args["modality"].toString()));

	m_lpf = args.value(keys::lp).toFloat();
	m_hpf = args.value(keys::hp).toFloat();
	m_notch = args.value(keys::notch).toFloat();
	AwFilterSettings filterSettings;
	filterSettings.set(m_channels.first()->type(), m_hpf, m_lpf, m_notch);
	filterSettings.apply(m_channels);

	if (!(modifiersFlags() & Aw::ProcessIO::modifiers::UseOrSkipMarkersApplied)) {
		pdi.input.clearMarkers();
		pdi.input.addMarker(new AwMarker("global", 0., args.value(keys::file_duration).toFloat()));
	}

    // Watch for memory exception
	try {

		// if the skip markers were selected then input markers already represent the data to load. 
		// if not than one marker should marked the entire data in pdi.input.markers.;
		sendMessage("Loading data...");
		requestData(&m_channels, &pdi.input.markers(), true);
		sendMessage("Done");
	}
	catch (std::bad_alloc& ba) {
		sendMessage("MEMORY ALLOCATION ERROR. Operation canceled.");
		return -1;
	}

	int decimate = 1;
	if (m_isDownsamplingActive) {
		decimate = 2;
		// compute decimate factor based on low pass filter
		if (m_lpf > 0) {
			float fc = m_lpf * 4;

			while (m_samplingRate / decimate > fc)
				decimate++;
			decimate--;
		}
		if (decimate > 1) {
			sendMessage(QString("Decimating data by a a factor of %1...").arg(decimate));
			try {
				AwFiltering::decimate(m_channels, decimate);
			}
			catch (const AwException& e)
			{
				sendMessage("Error during decimation of data. Aborted.");
				return -1;
			}
			sendMessage("Done.");
		}
	}
	else { // just filter the data
		sendMessage("Filtering...");
		AwFiltering::filter(&m_channels);
		sendMessage("Done.");
	}

	// check for nan values
	if (AwMath::isNanInChannels(m_channels)) {
		sendMessage("nan value detected in data. Computation aborted.");
		return -1;
	}

	qint64 nSamples = m_channels.first()->dataSize(); // getting total number of samples

	if (sqrt(nSamples / 30.) < m_nComp) {
		sendMessage(QString("Warning: Number of samples %1 for the number of components "
			"requested %2 may be insufficient.").arg(nSamples).arg(m_nComp));
	//	return -1;
	}

	// channels have been prepared.
	// prepare data matrix
	m = m_channels.size();
	n = nSamples;

	// if output_file exists => use it as the full path to result file
	if (args.contains(keys::output_file))
		m_fileName = args.value(keys::output_file).toString();
	else {
		// default file name is the basename of the data file.
		QFileInfo fi(pdi.input.settings[keys::data_path].toString());
		m_fileName = fi.fileName();
	}

	QString dir;
	// should always contain output_dir execpt if launched from command line with no output_dir option
	if (args.contains(keys::output_dir))
		dir = args.value(keys::output_dir).toString();
	else
		dir = pdi.input.settings[keys::data_dir].toString();

	// output_prefix will override this
	if (args.contains(keys::output_prefix))
		m_fileName = QString("%1%2").arg(args.value(keys::output_prefix).toString()).arg(m_fileName);

	QString mod = args.value(keys::modality).toString();
	if (args.contains(keys::output_suffix)) 
		m_fileName += QString("_algo-%1_mod-%2_hp-%3_lp-%4_notch-%5_comp-%6%7.mat").arg(m_selectedAlgo->name()).arg(mod).arg(m_hpf).arg(m_lpf).arg(m_notch).arg(m_nComp).arg(args.value(keys::output_suffix).toString());
	else // default suffix is _ica
	    m_fileName += QString("_algo-%1_mod-%2_hp-%3_lp-%4_notch-%5_comp-%6_ica.mat").arg(m_selectedAlgo->name()).arg(mod).arg(m_hpf).arg(m_lpf).arg(m_notch).arg(m_nComp);
	// generate full pathag
	m_fileName = QString("%1/%2").arg(dir).arg(m_fileName);

	// check if we have to export components time series
	if (args.contains(keys::save_comp_traces)) 
		m_componentsEEGFileName = m_fileName.replace(QRegularExpression("\\.[^.]*$"), ".vhdr");
	
	return 0;
}


void ICA::runFromCommandLine()
{
	try {
		if (initParameters() == 0) {
			m_selectedAlgo->setChannels(m_channels);
			m_selectedAlgo->setNumberOfComponents(m_nComp);
			m_selectedAlgo->run();
			m_mixing = m_selectedAlgo->mixing();
			m_unmixing = m_selectedAlgo->unmixing();
			saveToFile();
			if (!m_componentsEEGFileName.isEmpty()) {
				exportComponents();
			}
		}
	}
	catch (std::bad_alloc& ba) {
		sendMessage("MEMORY ALLOCATION ERROR. Operation canceled.");
		return;
	}
}

void ICA::run()
{
	try {
		if (initParameters() == -1)
			return;
		m_selectedAlgo->setChannels(m_channels);
		m_selectedAlgo->setNumberOfComponents(m_nComp);
		// check for extra settings in current algo
		auto map = m_selectedAlgo->getExtraSettings();
		if (!map.isEmpty())
			AwUniteMaps(pdi.input.settings, map);
		m_selectedAlgo->run();
		m_mixing = m_selectedAlgo->mixing();
		m_unmixing = m_selectedAlgo->unmixing();
	}
	catch (std::bad_alloc& ba) {
		sendMessage("MEMORY ALLOCATION ERROR. Operation canceled.");
		return;
	}
	if (!isAborted()) {
		saveToFile();
		//// tell AnyWave to load ICA components
		sendMessage("Sending event to AnyWave...");
		QSharedPointer<AwEvent> e = QSharedPointer<AwEvent>(new AwEvent());
		e->setId(AwEvent::LoadICAMatFile);
		QStringList args = { m_fileName };
		e->addValue("args", args);
		sendMessage("Open .mat file event");
		emit sendEvent(e);
		sendMessage("Done");
		// also send an event to open a signal view to visualise ICA components
		QSharedPointer<AwEvent> e2 = QSharedPointer<AwEvent>(new AwEvent());
		sendMessage("Adding new view for ICA channels");
		e2->setId(AwEvent::AddNewView);
		QStringList filters = { AwChannel::typeToString(AwChannel::ICA) };
		e2->addValue("filters", filters);
		emit sendEvent(e2);
		sendMessage("Done");
	}
}

void ICA::exportComponents()
{
	Q_ASSERT(!m_componentsEEGFileName.isEmpty());
	bool found = false; // find brain vision plugin
	AwFileIOPlugin *plugin = nullptr;
	for (auto p : pdi.input.writers) {
		if (p->name == "Brainvision Analyser Format") {
			found = true;
			plugin = p;
			break;
		}
	}
	if (!found) {
		sendMessage("Error: Brainvision plugin is not available. Could not export components traces.");
		return;
	}
	std::shared_ptr<AwFileIO> writer(plugin->newInstance());
	writer->infos.setChannels(AwChannel::duplicateChannels(m_channels));
	AwBlock* block = writer->infos.newBlock();
	// quick compute file duration
	float fileDuration = 0;
	for (auto m : pdi.input.markers())
		fileDuration += m->duration();
	block->setDuration(fileDuration);
	block->setSamples(std::floor(fileDuration * m_channels.first()->samplingRate()));
	if (writer->createFile(m_componentsEEGFileName) != AwFileIO::NoError) {
		sendMessage(QString("Error creating %1 file. No components traces exported").arg(m_componentsEEGFileName));
		return;
	}
	sendMessage("Saving components time series to vhdr file...");
	
	const float duration = 30.;
	fmat unmixing = conv_to<fmat>::from(m_unmixing);
	// read each input markers using chunks of 30s duration
	for (auto m : pdi.input.markers()) {
		float totalDuration = m->duration();
		float pos = m->start();
		float left = totalDuration;
		
		while (left) {
			float toRead = std::min(duration, left);
			requestData(&m_channels, pos, toRead);  // read and apply filters
			left -= toRead;
			pos += toRead;
			fmat X = AwMath::channelsToFMat(m_channels);
			fmat components = unmixing * X;
			X.clear();
			int index = 0;
			for (auto c : m_channels) {
				float* dest = c->newData(components.n_cols);
				for (uword c = 0; c < components.n_cols; c++)
					dest[c] = components(index, c);
				index++;
			}
			writer->writeData(&m_channels);
		}
	}
	sendMessage("Components time series exported.");
	writer->cleanUpAndClose();
}

void ICA::saveToFile()
{
	AwLayout *layout2D = nullptr, *layout3D = nullptr;
	int flags2D = 0, flags3D = 0;
	if (m_modality == AwChannel::EEG) {
		flags2D = AwLayout::EEG | AwLayout::L2D;
		flags3D = AwLayout::EEG | AwLayout::L3D;
	}
	if (m_modality == AwChannel::MEG) {
		flags2D = AwLayout::MEG | AwLayout::L2D;
		flags3D = AwLayout::MEG | AwLayout::L3D;
	}
	if (flags2D) {
		auto lm = AwLayoutManager::instance();
		layout2D = lm->guessLayout(pdi.input.reader(), flags2D);
	}
	if (flags3D) {
		auto lm = AwLayoutManager::instance();
		layout3D = lm->guessLayout(pdi.input.reader(), flags3D);
	}

	emit progressChanged("Saving to file...");
	AwMATLABFile file;
	try {
		file.create(m_fileName);
		file.writeString("modality", AwChannel::typeToString(m_modality));
		file.writeScalar("lpf", (double)m_lpf);
		file.writeScalar("hpf", (double)m_hpf);
		file.writeScalar("notch", (double)m_notch);
		file.writeScalar("sr", (double)m_samplingRate);
		file.writeMatrix("mixing", m_mixing);
		file.writeMatrix("unmixing", m_unmixing);
		file.writeStringCellArray(QString("labels"), AwChannel::getLabels(m_channels));
		if (layout2D)
			file.writeString("layout2D", layout2D->name());
		else
			file.writeString("layout2D", "n/a");
		if (layout3D)
			file.writeString("layout3D", layout3D->name());
		else
			file.writeString("layout3D", "n/a");
	}
	catch (const AwException& e) {
		sendMessage(QString("Error saving to .mat : %1").arg(e.errorString()));
		return;
	}
	sendMessage(QString("results saved to %1.").arg(m_fileName));
}
