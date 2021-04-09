#include "TFWidget.h"
#ifdef MKL
#include <fftw/fftw3.h>
#else
#include <fftw3.h>
#endif
#include "TFWavelet2.h"
#include <QtConcurrent>
#include <graphics/AwColorMap.h>
#include <graphics/AwQwtColorMap.h>
#include <widget/AwMessageBox.h>
#include <AwCore.h>

static void compute_tf(TFParam *p);
static QMutex TFMutex;

TFWidget::TFWidget(TFSettings *settings, AwGUIProcess *process, QWidget *parent)
	: AwProcessGUIWidget(process, parent)
{  
	m_ui.setupUi(this);
	m_signalView = new AwBaseSignalView();
	m_signalView->setFlags(AwBaseSignalView::NoMarkerBar|AwBaseSignalView::ViewAllChannels|AwBaseSignalView::EnableMarking);
	m_signalView->setMinimumHeight(200);
	m_settings = settings;


	// init display settings
	// Colormap
	AwCMapNamesAndTypes names_types = AwColorMap::namesAndTypes();
	for (int i = 0; i < names_types.size(); i++) {
		m_ui.cbColormap->addItem(names_types.at(i).first, QVariant(names_types.at(i).second));
		if (names_types.at(i).second == m_displaySettings.colorMap)
			m_ui.cbColormap->setCurrentIndex(i);
	}
	connect(m_ui.cbColormap, SIGNAL(currentIndexChanged(int)), this, SLOT(changeColorMap(int)));

	// Norm.
	m_ui.cbNorm->addItem("No Normalisation");
	m_ui.cbNorm->addItem("Divisive");
	m_ui.cbNorm->addItem("ZScore");
	m_ui.cbNorm->setCurrentIndex(0);
	connect(m_ui.cbNorm, SIGNAL(currentIndexChanged(int)), this, SLOT(changeNormalization(int)));
	// Z Scale
	m_ui.cbZScale->addItem("-Max to Max", DisplaySettings::MaxToMax);
	m_ui.cbZScale->addItem("Min to Max", DisplaySettings::MinToMax);
	m_ui.cbZScale->addItem("Zero to Max", DisplaySettings::ZeroToMax);
	m_ui.cbZScale->setCurrentIndex(1);
	connect(m_ui.cbZScale, SIGNAL(currentIndexChanged(int)), this, SLOT(changeZScale(int)));

	// gains
//	connect(m_ui.sliderGain, SIGNAL(valueChanged(int)), this, SLOT(changeGain(int)));

	// init compute settings
	m_ui.sbXi->setValue(m_settings->xi);
	m_ui.checkBoxDIFF->setChecked(settings->useDIFF);
	m_ui.sbMin->setValue(m_settings->freq_min);
	m_ui.sbMax->setValue(m_settings->freq_max);
	m_ui.sbStep->setValue(m_settings->step);
	connect(m_ui.buttonCompute, SIGNAL(clicked()), this, SLOT(recompute()));
	connect(m_ui.checkBaseline, &QCheckBox::toggled, this, &TFWidget::toggleBaselineCorrection);

	// markers for baseline
	auto markers = AwMarker::getMarkersWithDuration(process->pdi.input.markers());

	m_ui.checkBaseline->setDisabled(markers.isEmpty());
	m_ui.comboMarkers->setDisabled(markers.isEmpty());
	m_ui.comboMarkers->setMarkers(markers);

	m_baselineComputed = false;
	m_zRangeLocked = false;
	m_min = m_max = 0;
	m_colorMapWidget = nullptr;

	// extra init
	m_ui.spinZMin->setMinimum(std::numeric_limits<double>::min());
	m_ui.spinZMin->setMaximum(std::numeric_limits<double>::max());
	m_ui.spinZMax->setMinimum(std::numeric_limits<double>::min());
	m_ui.spinZMax->setMaximum(std::numeric_limits<double>::max());
	connect(m_ui.buttonApply, &QPushButton::clicked, this, &TFWidget::lockZRange);
	connect(m_ui.buttonReset, &QPushButton::clicked, this, &TFWidget::unlockZRange);
}

TFWidget::~TFWidget()
{
}


void TFWidget::lockZRange()
{
	m_zRangeLocked = true;
	m_ui.cbNorm->setEnabled(false);
	m_ui.cbZScale->setEnabled(false);
	m_ui.buttonApply->setEnabled(false);
	double min = m_ui.spinZMin->value();
	double max = m_ui.spinZMax->value();
	applyMinMaxToAllPlots(min, max);
}

void TFWidget::unlockZRange()
{
	m_zRangeLocked = false;
	m_ui.cbNorm->setEnabled(true);
	m_ui.cbZScale->setEnabled(true);
	m_ui.buttonApply->setEnabled(true);
	for (auto p : m_plots)
		p->resetZScale();
	m_ui.spinZMin->setValue(m_min);
	m_ui.spinZMax->setValue(m_max);
}

void TFWidget::toggleBaselineCorrection(bool flag)
{
	if (flag) {
		m_settings->baselineMarker = m_ui.comboMarkers->currentText();
		m_baselineComputed = false;
		auto markers = AwMarker::getMarkersWithLabel(m_process->pdi.input.markers(), m_settings->baselineMarker);
		if (markers.isEmpty())
			return;
		if (markers == m_baselineMarkers) { // baseline was already computed before
			m_baselineComputed = true;
			int i = 0;
			for (TFParam *p : m_tfComputations)	  // restor baseline data in the computation list.
				p->baselineData = m_baselineComputations.at(i++)->data;
		}
	}
	else {
		for (TFParam *p : m_tfComputations)
			p->baselineData.clear();
	}
}

void TFWidget::updateBaselineOptions()
{
	auto markers = AwMarker::getMarkersWithDuration(m_process->pdi.input.markers());
	m_ui.comboMarkers->setMarkers(markers);
	bool disabled = m_ui.comboMarkers->count() == 0;
	m_ui.checkBaseline->setDisabled(disabled);
	m_ui.comboMarkers->setDisabled(disabled);
}

void TFWidget::compute()
{
	m_signalView->scene()->removeHighLigthMarker();

	if (m_settings->useBaselineCorrection && !m_baselineComputed) {
		// gather markers with same label
		m_baselineMarkers = AwMarker::getMarkersWithLabel(m_process->pdi.input.markers(), m_settings->baselineMarker);
		if (!m_baselineMarkers.isEmpty()) {
			AwChannelList baselineChannels;
			AW_DESTROY_LIST(m_baselineComputations);
			for (auto c : m_channels) {
				auto chan = new AwChannel(c);
				TFParam *p = new TFParam();
				p->settings = m_settings;
				p->settings->useDIFF = false;  // on baseline computation, do not use special options like DIFF
				p->x = chan;
				baselineChannels << chan;
				m_baselineComputations << p;
			}
			m_process->requestData(&baselineChannels, &m_baselineMarkers);
			QFuture<void> future = QtConcurrent::map(m_baselineComputations, compute_tf);
			future.waitForFinished();

			for (auto i = 0; i < m_tfComputations.size(); i++) {
				m_tfComputations.at(i)->baselineData = m_baselineComputations.at(i)->data;
			}
			m_baselineComputed = true;
			AW_DESTROY_LIST(baselineChannels);
		}
	}
	
	QFuture<void> future = QtConcurrent::map(m_tfComputations, compute_tf);
	future.waitForFinished();
	int i = 0;

	// update plots data and compute global color map min and max.
	m_min = m_max = 0.;
	for (TFParam *p : m_tfComputations) {
		auto plot = m_plots.at(i++);
		plot->setNewData(m_signalView->positionInFile(), p);
		m_min = std::min(plot->min(), m_min);
		m_max = std::max(plot->max(), m_max);
	}
	if (!m_zRangeLocked) {
		m_ui.spinZMin->setValue(m_min);
		m_ui.spinZMax->setValue(m_max);
	}
//	for (auto plot : m_plots)
//		plot->setMinMax(m_min, m_max);

	//QList<double> rTicks[QwtScaleDiv::NTickTypes];
	//auto range = m_max - m_min;
	//auto step = std::abs(range / 4);
	//QwtInterval zInterval(m_min, m_max);
	//rTicks[QwtScaleDiv::MajorTick] << m_min << m_min + 1 * step << m_min + 2 * step << m_min + 3 * step << m_max;
	//QwtScaleDiv divR(rTicks[QwtScaleDiv::MajorTick].first(), rTicks[QwtScaleDiv::MajorTick].last(), rTicks);
	//m_colorMapWidget->setColorMap(zInterval, AwQwtColorMap::newMap(m_displaySettings.colorMap));
	//m_colorMapWidget->scaleDraw()->setScaleDiv(divR);
	//m_colorMapWidget->repaint();

	repaint();
}

void TFWidget::updatePlots() 
{
	//m_min = m_max = 0.;
	//for (auto p : m_plots) {
	//	p->updateDisplaySettings();
	//	// update plots data and compute global color map min and max.
	//	m_min = std::min(p->min(), m_min);
	//	m_max = std::max(p->max(), m_max);
	//}
	//for (auto plot : m_plots)
	//	plot->setMinMax(m_min, m_max);
	////QList<double> rTicks[QwtScaleDiv::NTickTypes];
	////auto range = m_max - m_min;
	////auto step = std::abs(range / 4);
	////QwtInterval zInterval(m_min, m_max);
	////rTicks[QwtScaleDiv::MajorTick] << m_min << m_min + 1 * step << m_min + 2 * step << m_min + 3 * step << m_max;
	////QwtScaleDiv divR(rTicks[QwtScaleDiv::MajorTick].first(), rTicks[QwtScaleDiv::MajorTick].last(), rTicks);
	////m_colorMapWidget->setColorMap(zInterval, AwQwtColorMap::newMap(m_displaySettings.colorMap));
	////m_colorMapWidget->scaleDraw()->setScaleDiv(divR);
	////m_colorMapWidget->repaint();
}

void TFWidget::setChannels(const AwChannelList& channels)
{
	// build layout for scroll area
	QGridLayout *layout = m_ui.signalsLayout;

	int row = 0;
	layout->addWidget(m_signalView, row++, 1);
	layout->setRowStretch(1, 0);
	layout->setColumnStretch(1, 1);

	//if (m_colorMapWidget == nullptr) {
	//	// building the colormap widget
	//	m_colorMapWidget = new QwtScaleWidget(QwtScaleDraw::RightScale, this);
	//	m_colorMapWidget->setContentsMargins(0, 0, 0, 0);
	//	m_colorMapWidget->setBorderDist(1, 1);
	//	m_colorMapWidget->setSpacing(5);
	//	m_colorMapWidget->setMargin(5);
	//	m_colorMapWidget->setColorBarEnabled(true);
	//	//m_colorMapWidget->setColorBarWidth(25);
	//	m_colorMapWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	//	m_colorMapWidget->hide();
	//}

	// Build params for computation and add plot to the layout
	for (auto c : channels) {
		TFParam *p = new TFParam();
		p->settings = m_settings;
		p->x = c;
		m_tfComputations << p;

		// add TF plot widget to the layout
		TFPlot *plot = new TFPlot(m_settings, &m_displaySettings, c);
		m_plots << plot;
		layout->addWidget(plot, row, 1);
		layout->addWidget(plot->leftWidget(), row, 0);
		layout->addWidget(plot->rightWidget(), row++, 2);
		

		//connect(m_ui.checkBoxFreqScale, SIGNAL(toggled(bool)), this, SLOT(showFreqScale(bool)));
		//connect(m_ui.checkBoxColormapScale, SIGNAL(toggled(bool)), this, SLOT(showColorMapScale(bool)));
		//connect(m_ui.checkBoxLogScale, SIGNAL(toggled(bool)), this, SLOT(switchLogScale(bool)));
		connect(plot, SIGNAL(selectionDone(float, float)), this, SLOT(highlightSampleInterval(float, float)));
		connect(plot, SIGNAL(selectionMade(AwChannel *, float, int, int, float, int, int)), this, SIGNAL(selectionMade(AwChannel *, float, int, int, float, int, int)));
		connect(this, SIGNAL(freqScaleChanged(float, float, float)), plot, SLOT(updateFreqScale(float, float, float)));
		connect(plot, SIGNAL(applyMinMaxToAll(double, double)), this, SLOT(applyMinMaxToAllPlots(double, double)));
	}
	connect(m_ui.checkBoxFreqScale, SIGNAL(toggled(bool)), this, SLOT(showFreqScale(bool)));
	connect(m_ui.checkBoxColormapScale, SIGNAL(toggled(bool)), this, SLOT(showColorMapScale(bool)));
	connect(m_ui.checkBoxLogScale, SIGNAL(toggled(bool)), this, SLOT(switchLogScale(bool)));
	auto nPlotRows = row - 1;
	// add color map widget to the right column and span it to all the rows used by TF plots
//	layout->addWidget(m_colorMapWidget, 1, 2, nPlotRows, 1);
	connect(m_signalView, SIGNAL(dataLoaded(float, float)), this, SLOT(compute()));
	m_signalView->setChannels(channels);
	m_channels = channels;
	repaint();
}

void TFWidget::setMarkers(const AwMarkerList& markers)
{
	m_signalView->setMarkers(markers);
	m_ui.comboMarkers->setMarkers(markers);
}


void TFWidget::showColorMapScale(bool flag)
{
//	m_colorMapWidget->setVisible(flag);
	for (auto p : m_plots)
		p->showColorMapScale(flag);
	repaint();
}

void TFWidget::showFreqScale(bool flag)
{
	for (auto p : m_plots)
		p->showFreqScale(flag);
	repaint();
}


void TFWidget::switchLogScale(bool flag)
{
	m_displaySettings.logScale = flag;
//	updatePlots();
	for (auto p : m_plots)
		p->updateDisplaySettings();
}

void TFWidget::changeColorMap(int index)
{
	QComboBox *cb = (QComboBox *)sender();
	Q_ASSERT(cb != NULL);
	m_displaySettings.colorMap = cb->currentData(Qt::UserRole).toInt();
	//updatePlots();
	for (auto p : m_plots)
		p->updateDisplaySettings();
}

void TFWidget::changeNormalization(int index)
{
	switch (index) {
	case 0:
		m_displaySettings.normalization = DisplaySettings::NoNorm;
		break;
	case 1:
		m_displaySettings.normalization = DisplaySettings::N10log10Divisive;
		break;
	case 2:
		m_displaySettings.normalization = DisplaySettings::ZScore;
		break;
	}
//	updatePlots();
	for (auto p : m_plots)
		p->updateDisplaySettings();
}

void TFWidget::changeZScale(int index)
{
	m_displaySettings.zInterval = m_ui.cbZScale->currentData().toInt();
	//updatePlots();
	for (auto p : m_plots)
		p->updateDisplaySettings();
}

void TFWidget::applyMinMaxToAllPlots(double min, double max)
{
	//TFPlot* plot = static_cast<TFPlot*>(sender());
	//if (plot == nullptr)
	//	return;
	//for (auto p : m_plots) {
	//	if (plot != p)
	//		p->setMinMaxZScale(min, max);
	//}

}

//void TFWidget::changeGain(int value)
//{
//	m_displaySettings.gain = (100 - (float)value) / 100;
//	//updatePlots();
//}

void TFWidget::highlightSampleInterval(float start, float duration)
{
	m_signalView->scene()->removeHighLigthMarker();
	m_signalView->scene()->addHighLigthMarker(tr("TF Selection"), start, duration);
}

void TFWidget::recompute()
{
	bool error = false;
	if (m_ui.sbMin->value() <= 0)
		error = true;
	if (m_ui.sbMin->value() > m_ui.sbMax->value())
		error = true;
	if (m_ui.sbXi->value() <= 1)
		error = true;
	if (error) {
		AwMessageBox::critical(this, tr("Parameters"), tr("Invalid parameters."));
		return;
	}
	m_settings->freqs.clear();
	m_settings->freq_min = m_ui.sbMin->value();
	m_settings->freq_max = m_ui.sbMax->value();
	m_settings->step = m_ui.sbStep->value();
	for (double f = m_settings->freq_min; f <= m_settings->freq_max; f += m_settings->step)
		m_settings->freqs << f;
	m_settings->xi = m_ui.sbXi->value();
	m_settings->useDIFF = m_ui.checkBoxDIFF->isChecked();
	m_settings->useBaselineCorrection = m_ui.checkBaseline->isChecked() && m_ui.checkBaseline->isEnabled();
	m_settings->baselineMarker = m_ui.comboMarkers->currentText();
	compute();
	emit freqScaleChanged(m_settings->freq_min, m_settings->freq_max, m_settings->step);
	repaint();
}




// compute function 
void compute_tf(TFParam *p)
{
	AwChannel *c = p->x;
	if (c->dataSize() == 0)
		return;

	//Base of algorithm
	//FFT of original signal to get complex values.
	//Multiply these values by the gaussian function computed.
	//Apply inverse FFT again on these values
	//Transform value into log(1 + (module� / max(module�)))

	// real number of samples
	quint32 nsamples = c->dataSize();
	float sr = c->samplingRate();
	float sigma2 = 1;
	float *x = c->data();
	double *freq;
	double *Psi;
	double **psi_array;
	double **wt;
	float min_center_freq;
	float max_center_freq;
	double *s_array;
	double min_scale, max_scale;
	// compute padding for data
	// Epsilon = 2pif0omega
	double wavelet_length = p->settings->xi / (2 * M_PI  * p->settings->freq_min);

	int padding = 3 * qCeil(wavelet_length * c->samplingRate());
	quint32 n = nsamples + 2 * padding; // padding on each side
	quint32 half_n = n / 2;

	// traduction du code MATLAB
	// omega = [(0:n/2) (-ceil(n/2)+1:-1)].*Fs/n; % CGB added ceil: TO BE CHECKED
	// omega = omega(:);
	double *omega = new double[n];
	for (int i = 0; i < half_n; i++)
		omega[i] = i * (sr / n);
	for (int i = half_n; i < n; i++)
		omega[i] = -((half_n - (i + 1)) * (sr / n));

	double *in;
	fftw_complex *fftx;
	// FFT(x)
	in = (double *)fftw_malloc(sizeof(double) * n);
	fftx = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * n);
	TFMutex.lock();
	fftw_plan plan = fftw_plan_dft_r2c_1d(n, in, fftx, FFTW_ESTIMATE);
	TFMutex.unlock();
	memset(in, 0, n * sizeof(double));

	// pad before and after the signal with zeros
	for (quint32 i = 0; i < nsamples; i++)
		in[i + padding] = x[i];

	// applying DIFF
	if (p->settings->useDIFF)
		for (int i = 1; i < n - 1; i++)
			in[i - 1] = in[i] - in[i - 1];

	fftw_execute_dft_r2c(plan, in, fftx);
	TFMutex.lock();
	fftw_destroy_plan(plan);
	TFMutex.unlock();
	fftw_free(in);
	switch (p->settings->wavelet)
	{
	case TFWavelet2::Gabor:
		min_center_freq = (2 * p->settings->tolerance * sqrt(sigma2) * sr * p->settings->xi) / n;
		max_center_freq = (sr * p->settings->xi / (p->settings->xi + p->settings->tolerance / sqrt(sigma2)));
		if (p->settings->freqs.isEmpty()) {
			float nvoice = 12;
			int exp;
			for (double i = frexp(min_center_freq, &exp); i < frexp(max_center_freq, &exp); i += 1 / nvoice)
				p->settings->freqs << i * i;
		}
		s_array = new double[p->settings->freqs.size()];
		for (int i = 0; i < p->settings->freqs.size(); i++)
			s_array[i] = p->settings->xi / p->settings->freqs.at(i);
		min_scale = p->settings->xi / max_center_freq;
		max_scale = p->settings->xi / min_center_freq;
		break;
	default:
		break;
	}
	// MATLAB: 
	// nscale = length(freqlist);
	// wt = zeros(n,nscale);
	// The matrix is n columns and nscale lines 
	int nscale = p->settings->freqs.size();
//	QVector<double> matrix(nscale * nsamples);

	p->data = zeros(nscale, nsamples);

	// MATLAB
	//scaleindices=find(s_array(:)'>=minscale & s_array(:)'<=maxscale);
	QVector<int> scale_indices;
	for (int i = 0; i < nscale; i++) {
		if (s_array[i] >= min_scale && s_array[i] <= max_scale)
			scale_indices << i;
	}
	//MATLAB
	//for kscale=scaleindices
	// s=s_array(kscale);
	freq = new double[n];
	Psi = new double[n];
	fftw_complex *c_in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * n);
	fftw_complex *c_out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * n);
	TFMutex.lock();
	fftw_plan plan_c2c = fftw_plan_dft_1d(n, c_in, c_out, FFTW_BACKWARD, FFTW_ESTIMATE);
	TFMutex.unlock();
	double m2max = 0;
	for (auto index : scale_indices) {
		double s = s_array[index];
		switch (p->settings->wavelet)
		{
		case TFWavelet2::Gabor:
			for (int i = 0; i < n; i++) {
				freq[i] = (s * omega[i]) - p->settings->xi;
				Psi[i] = pow(4 * M_PI * sigma2, (double)(1 / 4)) * sqrt(s) * exp(-sigma2 / 2 * freq[i] * freq[i]);
			}
			break;
		default:
			break;
		}

		// Multiplying complex values by Gaussian function
		for (int i = 0; i < n; i++) {
			c_in[i][0] = Psi[i] * fftx[i][0];
			c_in[i][1] = Psi[i] * fftx[i][1];
		}
		// compute ifft(fftx.*Psi)
		fftw_execute_dft(plan_c2c, c_in, c_out);

		// fill matrix
//		double *data = matrix.data();
//		auto data = p->data.memptr();

		//for (int i = 0, j = padding; i < nsamples; i++)
		//	data[index * nsamples + i] = c_out[i + j][0] * c_out[i + j][0] + c_out[i + j][1] * c_out[i + j][1];
		for (int i = 0, j = padding; i < nsamples; i++)
			p->data(index, i) = c_out[i + j][0] * c_out[i + j][0] + c_out[i + j][1] * c_out[i + j][1];

	}
	fftw_free(c_in);
	fftw_free(c_out);
	fftw_free(fftx);
	TFMutex.lock();
	fftw_destroy_plan(plan_c2c);
	TFMutex.unlock();
	delete[] Psi;
	delete[] freq;
	delete[] s_array;
	delete[] omega;

}