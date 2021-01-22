#include "Spectral.h"
#include "SIWidget.h"
#include <QMessageBox>
#include <AwKeys.h>
#include "InputMarkersDial.h"
#include <sigpack.h>
#ifdef MKL
#include <fftw3.h>
#else
#include "fftw/fftw3.h"
#endif
#include "FFTIterations.h"
#include "PlotWidget.h"

using namespace sp;


SpectralPlugin::SpectralPlugin()
{
	name = QString(tr("Spectral Informations"));
	category = "Process:Signal:Power Spectral Density";
	description = QString(tr("Compute and show spectral informations."));
	type = AwProcessPlugin::Background;
	setFlags(Aw::ProcessFlags::ProcessHasInputUi | Aw::ProcessFlags::CanRunFromCommandLine | Aw::ProcessFlags::PluginAcceptsTimeSelections);
	m_helpUrl = "https://gitlab-dynamap.timone.univ-amu.fr/anywave/anywave/-/wikis/plugin_spectral";
}

Spectral::Spectral()
{
	pdi.addInputChannel(AwProcessDataInterface::AnyChannels, 1, 0);
	// global flags
	setFlags(Aw::ProcessFlags::HasOutputUi);
	setInputFlags(Aw::ProcessIO::GetDurationMarkers | Aw::ProcessIO::AcceptChannelSelection
		| Aw::ProcessIO::GetCurrentMontage);
}

Spectral::~Spectral()
{
}

int Spectral::stringToWindowingType(const QString& str)
{
	QMap<QString, int> dict;
	dict.insert("none", Spectral::None);
	dict.insert("hanning", Spectral::Hanning);
	dict.insert("hamming", Spectral::Hamming);

	auto key = str.simplified().toLower();
	if (dict.contains(str))
		return dict.value(str);
	return Spectral::Hanning;
}

void Spectral::clean()
{
	for (auto iter : m_results.values())
		delete iter;
	m_results.clear();
}

int Spectral::initialize()
{
	clean();
	// we assume the input channels have been set by AnyWave regarding our input flags
	// we should have the user selection OR the as recorded channels included the bad ones.
	if (pdi.input.channels().isEmpty()) {
		sendMessage("No channels set as input. Aborting.");
		return -1;
	}
	auto fd = pdi.input.settings.value(keys::file_duration).toFloat();

	// init markers based on input.settings arguments (if any)
	QStringList usedMarkers, skippedMarkers;
	bool useMarkers = false, skipMarkers = false;
	if (pdi.input.settings.contains(keys::use_markers)) {
		usedMarkers = pdi.input.settings.value(keys::use_markers).toStringList();
		useMarkers = true;
	}
	if (pdi.input.settings.contains(keys::skip_markers)) {
		skippedMarkers = pdi.input.settings.value(keys::skip_markers).toStringList();
		skipMarkers = true;
	}
	if (skipMarkers || useMarkers) {
		auto markers = AwMarker::duplicate(pdi.input.markers());
		auto inputMarkers = AwMarker::getInputMarkers(markers, skippedMarkers, usedMarkers, fd);
		if (inputMarkers.isEmpty()) {
			pdi.input.clearMarkers();
			pdi.input.addMarker(new AwMarker("whole_data", 0., fd));
		}
		else {
			pdi.input.clearMarkers();
			pdi.input.setNewMarkers(inputMarkers);
		}
	}

	// we expect markers to be present, at least one global marker but we never know...
	if (pdi.input.markers().isEmpty()) 
		pdi.input.addMarker(new AwMarker("global", 0., pdi.input.settings.value(keys::file_duration).toFloat()));
	
	// check for required arguments and set default 
	m_windowing = Spectral::Hanning;
	if (!pdi.input.settings.contains("time_window")) {
		sendMessage("Missing --time_window argument.");
		return -1;
	}
	m_timeWindow = pdi.input.settings.value("time_window").toDouble();

	if (!pdi.input.settings.contains("overlap")) {
		sendMessage("Missing --overlap argument.");
		return -1;
	}
	m_overlap = pdi.input.settings.value("overlap").toDouble();

	if (m_timeWindow < 0 || m_overlap > m_timeWindow) {
		sendMessage(QString("Wrong values for time_window: %1 or orverlap:%2").arg(m_timeWindow).arg(m_overlap));
		return -1;
	}
	if (pdi.input.settings.contains("windowing")) 
		m_windowing = stringToWindowingType(pdi.input.settings.value("windowing").toString());
	
	return 0;
}

bool Spectral::showUi()
{
	auto fd = pdi.input.settings.value(keys::file_duration).toFloat();
	// check for input markers (only duration markers is assumed here)
	if (!pdi.input.markers().isEmpty()) {
		InputMarkersDial dlg(pdi.input.markers());
		if (dlg.exec() == QDialog::Accepted) {
			if (!dlg.m_skippedLabels.isEmpty())
				pdi.input.settings[keys::skip_markers] = dlg.m_skippedLabels;
			if (!dlg.m_usedLabels.isEmpty())
				pdi.input.settings[keys::use_markers] = dlg.m_usedLabels;
			if (dlg.m_skippedLabels.isEmpty() && dlg.m_usedLabels.isEmpty())
				if (QMessageBox::question(nullptr, "Data Length", "Compute on whole data?", QMessageBox::Yes | QMessageBox::No) ==
					QMessageBox::No)
					return false;
		}
		//	pdi.input.clearMarkers();
		//	pdi.input.addMarker(new AwMarker("whole_data", 0., fd));
		//	if (!dlg.m_skippedLabels.isEmpty() || !dlg.m_usedLabels.isEmpty()) {
		//		
		//		auto markers = AwMarker::duplicate(pdi.input.markers());
		//		auto inputMarkers = AwMarker::getInputMarkers(markers, dlg.m_skippedLabels, dlg.m_usedLabels, fd);
		//		if (inputMarkers.isEmpty()) {
		//			pdi.input.clearMarkers();
		//			pdi.input.addMarker(new AwMarker("whole_data", 0., fd));
		//		}
		//		else {
		//			pdi.input.clearMarkers();
		//			pdi.input.setNewMarkers(inputMarkers);
		//		}
		//	}
		//	else {
		//		if (QMessageBox::question(nullptr, "Data Length", "Compute on whole data?", QMessageBox::Yes | QMessageBox::No) ==
		//			QMessageBox::No)
		//			return;
		//		pdi.input.clearMarkers();
		//		pdi.input.addMarker(new AwMarker("whole_data", 0., fd));
		//	}
		//}
		//else 
		//	return false;
	}
//	else {
//		pdi.input.addMarker(new AwMarker("whole_data", 0., fd));
//	}
	//m_widget = new SIWidget(this);
	SIWidget dlg;
	if (dlg.exec() != QDialog::Accepted)
		return false;
	pdi.input.settings["time_window"] = dlg.timeWindow;
	pdi.input.settings["overlap"] = dlg.overlap;
	pdi.input.settings["windowing"] = dlg.windowing;
	
	////// check input channel
	//auto channels = pdi.input.channels();
	//if (channels.isEmpty()) {
	//	auto answer = QMessageBox::information(nullptr, "Power Spectral Density", "No channels selected, run for all channels?",
	//		QMessageBox::Yes | QMessageBox::No);
	//	if (answer == QMessageBox::Yes) {
	//		QVariantMap cfg;
	//		AwChannelList result;
	//		cfg.insert(keys::channels_source, keys::channels_source_raw);
	//		selectChannels(cfg, &result);
	//		if (result.isEmpty())
	//			return;
	//		pdi.input.setNewChannels(result);
	//	}
	//	else
	//		return false;
	//}

	// we assume the input channels have been set by AnyWave regarding our input flags
	// we should have the user selection OR the as recorded channels included the bad ones.


	return true;
}

void Spectral::compute()
{
	uword nfft, noverlap;
	auto fs = pdi.input.settings.value(keys::max_sr).toFloat();
//	double timeWindow = pdi.input.settings.value("time_window").toDouble();
//	double overlap = pdi.input.settings.value("overlap").toDouble();
//	int windowType = pdi.input.settings.value("window").toInt();
	nfft = (uword)std::floor(m_timeWindow * fs);
	noverlap = (uword)std::floor(m_overlap * fs);
	// loop over markers
	AwMarkerList badMarkers, goodMarkers;
	for(AwMarker *m : pdi.input.markers()) {
		uword nPts = (uword)std::floor(m->duration() * fs);
		if (nPts < nfft)
			badMarkers << m;
		else
			goodMarkers << m;
	}
	if (!badMarkers.isEmpty()) {
		//if (QMessageBox::question(0, "Data input",
		//	"Some markers will be skipped because their length is too short regarding the time window specified.\nProceed anyway?") == QMessageBox::No)
		//	return;
		sendMessage("Some markers will be skipped because their length is too short regarding the time window specified.");
	}
	if (goodMarkers.isEmpty()) {
		sendMessage("No marked data set as input. Aborting...");
		return;
	}

	// pre calculate the number of fft iterations through all markers for a channel
	uword nIterations = 0;
	uword nIterationPnts = nfft / 2 - 1;
	for (auto m : goodMarkers) {
		uword nPts = (uword)std::floor(m->duration() * fs);
		uword start = 0;
		while (start + nfft < nPts) {
			nIterations++;
			start += noverlap;
		}
	}

	// temp vector to hold data for a time window
	vec timeWindowSignal = zeros(nfft);
	//vec data = zeros(nPts); // copy of chunk data
	vec iteration = zeros(nIterationPnts);
	// fft plan that will be used for all fft transformations
	fftw_complex* fftx = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * nfft);
	fftw_plan plan = fftw_plan_dft_r2c_1d(nfft, timeWindowSignal.memptr(), fftx, FFTW_ESTIMATE);
	double fact = 2. / nfft;

	// now read data for each chunks and compute fft iterations
	for (auto m : goodMarkers) {
		sendMessage(QString("Computing on chunk %1. starting at: %2s ending at: %3s").arg(m->label()).arg(m->start()).arg(m->end()));
		// get RAW data for all channels
		auto& channels = pdi.input.channels();
		requestData(&pdi.input.channels(), m, true);
		uword nPts = (uword)channels.first()->dataSize();
		// prepare window
		vec window;
		switch (m_windowing) {
		case Spectral::Hanning:
			window = hanning(nfft);
			break;
		case Spectral::Hamming:
			window = hamming(nfft);
			break;
		}
		if (m_windowing != Spectral::None) {
			// normalize window with its RMS
			auto squared = window;
			squared.for_each([](vec::elem_type& val) { val *= val; });
			auto sqr_mean = std::sqrt(arma::mean(squared));
			window.for_each([sqr_mean](vec::elem_type& val) { val /= sqr_mean; });
		}

		for (auto channel : pdi.input.channels()) {
			FFTIterations* fftIter = nullptr;
			if (m_results.contains(channel))
				fftIter = m_results.value(channel);
			else {
				fftIter = new FFTIterations(channel);
				fftIter->setNumberOfIterations(nIterations, nIterationPnts);
				m_results.insert(channel, fftIter);
			}

			uword start = 0;
			while (start + nfft < channel->dataSize()) {
				for (auto i = 0; i < nfft; i++)
					timeWindowSignal(i) = channel->data()[i + start];
				if (m_windowing != Spectral::None)
					timeWindowSignal = timeWindowSignal % window;

				fftw_execute_dft_r2c(plan, timeWindowSignal.memptr(), fftx);
				uword j = 0;
				for (auto i = 1; i < nfft / 2; i++) {
					fftx[i][0] *= fact;
					fftx[i][1] *= fact;
					iteration(j++) = fftx[i][0] * fftx[i][0] + fftx[i][1] * fftx[i][1];
				}
				fftIter->appendIteration(iteration);
				start += noverlap;
			}
		}
		sendMessage("Done.");
	}
	fftw_destroy_plan(plan);
	fftw_free(fftx);
	for (auto res : m_results.values()) {
		res->computePxx();
	}
}

void Spectral::runFromCommandLine()
{
	if (initialize() != 0) {
		sendMessage("Something went wrong while initialising. Aborting...");
		return;
	}
	compute();
	// save results to file

}

void Spectral::run()
{

//	// register our widget to auto close the plugin when the user closes the widget
////	registerGUIWidget(m_widget);
//	//// check input channel
//	auto channels = pdi.input.channels();
//	if (channels.isEmpty()) {
//		auto answer = QMessageBox::information(nullptr, "Power Spectral Density", "No channels selected, run for all channels?",
//			QMessageBox::Yes | QMessageBox::No);
//		if (answer == QMessageBox::Yes) {
//			QVariantMap cfg;
//			AwChannelList result;
//			cfg.insert(keys::channels_source, keys::channels_source_raw);
//			selectChannels(cfg, &result);
//			if (result.isEmpty())
//				return;
//			pdi.input.setNewChannels(result);
//		}
//		else
//			return; 
//	}
//	m_widget->show();
	if (initialize() != 0) {
		sendMessage("Something went wrong while initialising. Aborting...");
		return;
	}
	compute();
}

void Spectral::prepareOutputUi()
{
	pdi.output.widgets().append(new PlotWidget(m_results.values()));
}