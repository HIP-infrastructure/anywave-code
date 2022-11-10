#include "Spectral.h"
#include "SIWidget.h"
#include <QMessageBox>
#include <AwKeys.h>
#include "InputMarkersDial.h"
#include <sigpack.h>
#ifdef MKL
#include <fftw/fftw3.h>
#else
#include <fftw3.h>
#endif
#include "FFTIterations.h"
#include "PlotWidget.h"

using namespace sp;
#include <utils/json.h>
#include <matlab/AwMATLAB.h>
#include <matlab/AwMATLABStruct.h>
#include <QtConcurrent>


SpectralPlugin::SpectralPlugin()
{
	name = QString("Spectral");
	version = "1.0.1";
	category = "Process:Signal:Power Spectral Density";
	description = QString(tr("Compute and show Power Spectral Density."));
	type = AwProcessPlugin::Background;
	setFlags(Aw::ProcessFlags::ProcessHasInputUi | Aw::ProcessFlags::CanRunFromCommandLine | Aw::ProcessFlags::PluginAcceptsTimeSelections);
	m_helpUrl = "Power Spectral Density::Signal Processing::https://gitlab-dynamap.timone.univ-amu.fr/anywave/anywave/-/wikis/plugin_spectral";
	m_settings[keys::json_batch] = AwUtilities::json::fromJsonFileToString(":/args.json");
}

Spectral::Spectral()
{
	pdi.addInputChannel(AwProcessDataInterface::AnyChannels, 1, 0);
	// global flags
	setFlags(Aw::ProcessFlags::HasOutputUi);
	setInputModifiers(Aw::ProcessIO::modifiers::AcceptChannelSelection);
	setInputFlags(Aw::ProcessIO::GetDurationMarkers | Aw::ProcessIO::GetCurrentMontage);
	m_timeWindow = 1.;
	m_overlap = 0.5;
	m_windowing = Spectral::Hanning;
	m_fs = 0.;
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
	if (!pdi.input.settings.contains(keys::output_dir))
		pdi.input.settings[keys::output_dir] = pdi.input.settings.value(keys::data_dir);
	return 0;
}

bool Spectral::showUi()
{
	auto fd = pdi.input.settings.value(keys::file_duration).toFloat();
	bool manuallySelectedMarkers = modifiersFlags() & Aw::ProcessIO::modifiers::UserSelectedMarkers;

	// check for input markers (only duration markers is assumed here)
	if (!pdi.input.markers().isEmpty() && !manuallySelectedMarkers) {
		InputMarkersDial dlg(pdi.input.markers());
		if (dlg.exec() == QDialog::Accepted) {
			if (!dlg.m_skippedLabels.isEmpty())
				pdi.input.settings[keys::skip_markers] = dlg.m_skippedLabels;
			if (!dlg.m_usedLabels.isEmpty())
				pdi.input.settings[keys::use_markers] = dlg.m_usedLabels;
			if (dlg.m_skippedLabels.isEmpty() && dlg.m_usedLabels.isEmpty()) {
				if (QMessageBox::question(nullptr, "Data Length", "Compute on whole data?", QMessageBox::Yes | QMessageBox::No) ==
					QMessageBox::No)
					return false;
				else {
					pdi.input.clearMarkers();
					pdi.input.addMarker(new AwMarker("whole_data", 0., fd));
				}
			}
		}
	}
	SIWidget dlg;
	if (dlg.exec() != QDialog::Accepted)
		return false;
	pdi.input.settings["time_window"] = dlg.timeWindow;
	pdi.input.settings["overlap"] = dlg.overlap;
	pdi.input.settings["windowing"] = dlg.windowing;
	return true;
}

void Spectral::compute()
{
	uword nfft, noverlap;
	m_fs = pdi.input.settings.value(keys::max_sr).toFloat();
	nfft = (uword)std::floor(m_timeWindow * m_fs);
	noverlap = (uword)std::floor(m_overlap * m_fs);
	// loop over markers
	AwSharedMarkerList badMarkers, goodMarkers;
	for(auto const& m : pdi.input.markers()) {
		uword nPts = (uword)std::floor(m->duration() * m_fs);
		if (nPts < nfft)
			badMarkers << m;
		else
			goodMarkers << m;
	}
	if (!badMarkers.isEmpty()) {
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
		uword nPts = (uword)std::floor(m->duration() * m_fs);
		uword start = 0;
		while (start + nfft < nPts) {
			nIterations++;
			start += noverlap;
		}
	}

	// temp vector to hold data for a time window
	vec timeWindowSignal = zeros(nfft);
	vec iteration = zeros(nIterationPnts);
	// fft plan that will be used for all fft transformations
	fftw_complex* fftx = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * nfft);
	fftw_plan plan = fftw_plan_dft_r2c_1d(nfft, timeWindowSignal.memptr(), fftx, FFTW_ESTIMATE);
	double fact = 2. / nfft;

	// now read data for each chunks and compute fft iterations
	for (auto const &m : goodMarkers) {
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
#ifndef COMPUTE_MP
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
#else
		int windowType = m_windowing;
		QList< FFTIterations*> list; // iterations to be done for all channels
		for (auto channel : pdi.input.channels()) {
			FFTIterations* fftIter = nullptr;
			if (m_results.contains(channel))
				fftIter = m_results.value(channel);
			else {
				fftIter = new FFTIterations(channel);
				fftIter->setNumberOfIterations(nIterations, nIterationPnts);
				m_results.insert(channel, fftIter);
			}
			list.append(fftIter);
		}

		//auto  compute = [window, nfft, noverlap, nIterationPnts, fact, plan, windowType](FFTIterations* iter) {
		auto  compute = [=](FFTIterations* iter) {
			vec timeWindowSignal = zeros(nfft);
			vec iteration = zeros(nIterationPnts);
			fftw_complex* fftx = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * nfft);
			uword start = 0;
			while (start + nfft < iter->channel()->dataSize()) {
				for (auto i = 0; i < nfft; i++)
					timeWindowSignal(i) = iter->channel()->data()[i + start];
				if (windowType != Spectral::None)
					timeWindowSignal = timeWindowSignal % window;

				m_mutex.lock();
				fftw_execute_dft_r2c(plan, timeWindowSignal.memptr(), fftx);
				m_mutex.unlock();
				uword j = 0;
				for (auto i = 1; i < nfft / 2; i++) {
					fftx[i][0] *= fact;
					fftx[i][1] *= fact;
					iteration(j++) = fftx[i][0] * fftx[i][0] + fftx[i][1] * fftx[i][1];
				}
				iter->appendIteration(iteration);
				start += noverlap;
			}
			fftw_free(fftx);
		};
		QFuture<void> res = QtConcurrent::map(list, compute);
		res.waitForFinished();
		sendMessage("Done.");
	}
	fftw_destroy_plan(plan);
	fftw_free(fftx);
	for (auto res : m_results.values()) {
		res->computePxx();
	}
#endif
}

void Spectral::runFromCommandLine()
{
	if (initialize() != 0) {
		sendMessage("Something went wrong while initializing. Aborting...");
		return;
	}
	compute();
	// save results to file
	saveResults();
}

void Spectral::run()
{
	if (initialize() != 0) {
		sendMessage("Something went wrong while initializing. Aborting...");
		return;
	}
	pdi.input.settings[keys::output_dir] = pdi.input.settings.value(keys::data_dir);
	compute();
	saveResults();
}

void Spectral::prepareOutputUi()
{
	pdi.output.widgets().append(new PlotWidget(m_results.values(), this));
}

void Spectral::saveResults()
{
	auto args = pdi.input.settings;
	QString baseFilename = args.value(keys::output_dir).toString();

	if (args.contains(keys::output_prefix)) {
		QString pref = args.value(keys::output_prefix).toString();
		baseFilename = QString("%1/%2_").arg(baseFilename).arg(pref);
	}
	else 
		baseFilename = QString("%1/spectrum_").arg(baseFilename);
	
	auto fs = pdi.input.settings.value(keys::max_sr).toFloat();
	auto nfft = (uword)std::floor(m_timeWindow * fs);
	auto noverlap = (uword)std::floor(m_overlap * fs);

	baseFilename += QString("fftwindow-%1_").arg(nfft);
	baseFilename += QString("fftoverlap-%1_").arg(noverlap);
	if (args.contains("windowing"))
		baseFilename += QString("window-%1").arg(args.value("windowing").toString());
	else {
		baseFilename += "window-hanning";
		args["windowing"] = QString("hanning");
	}
	if (args.contains(keys::output_suffix)) 
		baseFilename += QString("_%1").arg(args.value(keys::output_suffix).toString());
	
	baseFilename += ".mat";
	AwMATLABFile file;
	try {
		file.create(baseFilename);
		file.writeScalar("fft_window", (int)nfft);
		file.writeScalar("fft_overlap", (int)noverlap);
		file.writeString("windowing", args.value("windowing").toString());
		const char* fields[] = { "channel", "fft_iterations", "psd" };
		size_t dim[2] = { 1, (size_t)m_results.keys().size() };
		AwMATLABStruct s("psd", fields, 3, 2, dim);
		int count = 0;
		for (auto result : m_results.values()) {
			s.insertString("channel", result->channel()->fullName(), count);
			s.insertMatrix("fft_iterations", result->results(), count);
			vec psd = 10 * log10(result->pxx());
			s.insertVector("psd", psd, count);
			count++;
		}
		file.writeStruct(s);
		file.close();
	}
	catch (const AwException& e) {
		sendMessage(QString("Failed to write MATLAB file: %1 in %2").arg(e.errorString()).arg(e.origin()));
		return;
	}
}