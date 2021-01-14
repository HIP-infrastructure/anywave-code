#include "SIWidget.h"
#include "FFTIterations.h"
#include <sigpack.h>
#ifdef MKL
#include <fftw3.h>
#else
#include "fftw/fftw3.h"
#endif
#include "PlotWidget.h"
using namespace sp;
#include <AwProcessInterface.h>
#include "Spectral.h"

#include <AwKeys.h>
#include <QPushButton>
#include <QMessageBox>


SIWidget::SIWidget(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	
	windowType = Spectral::Hanning;
	//connect(m_ui.buttonCompute, &QPushButton::clicked, this, &SIWidget::compute);
//	connect(m_ui.buttonShowPlots, &QPushButton::clicked, this, &SIWidget::showPlots);
	m_ui.comboWindow->addItem("None", -1);
	m_ui.comboWindow->addItem("Hanning", windowType);
	m_ui.comboWindow->addItem("Hamming", Spectral::Hamming);
	m_ui.comboWindow->setCurrentIndex(1);
}

SIWidget::~SIWidget()
{
	//for (auto plot : m_plotWidgets)
	//	plot->close();
	//
	//qDeleteAll(m_plotWidgets);
	//clear();
}

void SIWidget::clear()
{
	//auto values = m_results.values();

	//while (!values.isEmpty())
	//	delete values.takeFirst();
	//m_results.clear();
}

void SIWidget::accept()
{
	timeWindow = m_ui.spinWindow->value();
	overlap = m_ui.spinOverlap->value();
	windowType = m_ui.comboWindow->currentData().toInt();
	QDialog::accept();
}

void SIWidget::compute()
{
	clear();

	uword nfft, noverlap;
	auto fs = m_process->pdi.input.settings.value(keys::max_sr).toFloat();
	nfft = (uword)std::floor(m_ui.spinWindow->value() * fs);
	noverlap = (uword)std::floor(m_ui.spinOverlap->value() * fs);
	// loop over markers
	AwMarkerList badMarkers, goodMarkers;
	foreach(AwMarker *m, m_process->pdi.input.markers()) {
		uword nPts = (uword)std::floor(m->duration() * fs);
		if (nPts < nfft)
			badMarkers << m;
		else
			goodMarkers << m;
	}
	if (!badMarkers.isEmpty()) {
		if (QMessageBox::question(0, "Data input",
			"Some markers will be skipped because their length is too short regarding the time window specified.\nProceed anyway?") == QMessageBox::No)
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
		// get RAW data for all channels
		auto& channels = m_process->pdi.input.channels();
		m_process->requestData(&m_process->pdi.input.channels(), m, true);
		uword nPts = (uword)channels.first()->dataSize();
		// prepare window
		vec window;
		switch (m_window) {
		case SIWidget::Hanning:
			window = hanning(nfft);
			break;
		case SIWidget::Hamming:
			window = hamming(nfft);
			break;
		}
		if (m_window != SIWidget::None) {
			// normalize window with its RMS
			auto squared = window;
			squared.for_each([](vec::elem_type& val) { val *= val; });
			auto sqr_mean = std::sqrt(arma::mean(squared));
			window.for_each([sqr_mean](vec::elem_type& val) { val /= sqr_mean; });
		}
		
		for (auto channel : m_process->pdi.input.channels()) {
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
				if (m_window != SIWidget::None)
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
	}
	fftw_destroy_plan(plan);
	fftw_free(fftx);
	for (auto res : m_results.values()) {
		res->computePxx();
	}

	auto plot = new PlotWidget(m_results.values());
	m_plotWidgets << plot;
	plot->show();
	m_ui.buttonShowPlots->setEnabled(true);
}


