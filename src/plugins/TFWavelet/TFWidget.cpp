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
//#include "matlab/coder_array.h"
//#include "matlab/wavelet_dyn_morlet_preprocess.h"
//#include "matlab/wavelet_dyn_morlet_freqlist.h"


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
	m_computeBaseline = false;
	m_zRangeLocked = false;
	m_min = m_max = 0;
	m_colorMapWidget = nullptr;

	// extra init
	//m_ui.spinZMin->setMinimum(std::numeric_limits<double>::min());
	//m_ui.spinZMin->setMaximum(std::numeric_limits<double>::max());
	//m_ui.spinZMax->setMinimum(std::numeric_limits<double>::min());
	//m_ui.spinZMax->setMaximum(std::numeric_limits<double>::max());
	connect(m_ui.buttonApply, &QPushButton::clicked, this, &TFWidget::lockZRange);
	connect(m_ui.buttonReset, &QPushButton::clicked, this, &TFWidget::unlockZRange);

	m_pos = m_duration = 0.0;
}

TFWidget::~TFWidget()
{
	m_computationChannels.erase(m_computationChannels.begin(), m_computationChannels.end());
}


void TFWidget::lockZRange()
{
	m_zRangeLocked = true;
	m_ui.cbNorm->setEnabled(false);
	m_ui.cbZScale->setEnabled(false);
	m_ui.buttonApply->setEnabled(false);
//	double min = m_ui.spinZMin->value();
//	double max = m_ui.spinZMax->value();
	double min = m_ui.lineZMin->text().toDouble();
	double max = m_ui.lineZMax->text().toDouble();

	if (max <= min)
		return;

	m_ui.cbNorm->setEnabled(false);
	m_ui.cbZScale->setEnabled(false);
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
//	m_ui.spinZMin->setValue(m_min);
//	m_ui.spinZMax->setValue(m_max);
	m_ui.lineZMin->setText(QString().setNum(m_min, 'g', 2));
	m_ui.lineZMax->setText(QString().setNum(m_max, 'g', 2));
	m_ui.cbNorm->setEnabled(true);
	m_ui.cbZScale->setEnabled(true);
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
	m_min = m_plots.first()->min();
	m_max = m_plots.first()->max();
	for (TFParam *p : m_tfComputations) {
		auto plot = m_plots.at(i++);
		plot->setNewData(m_signalView->positionInFile(), p);
		m_min = std::min(plot->min(), m_min);
		m_max = std::max(plot->max(), m_max);
	}
	if (!m_zRangeLocked) {
		m_ui.lineZMin->setText(QString().setNum(m_min, 'g', 2));
		m_ui.lineZMax->setText(QString().setNum(m_max, 'g', 2));
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
	m_computationChannels.erase(m_computationChannels.begin(), m_computationChannels.end());
	m_computationChannels = AwChannel::clone(channels);


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

		// DEBUG CODE using QCustomPlot
		if (row == 1) {
			m_qcplot = new TFQCustomPlot(m_settings, &m_displaySettings, c);
			layout->addWidget(m_qcplot, row, 1);
			row++;
		}
		layout->addWidget(plot, row, 1);
		layout->addWidget(plot->leftWidget(), row, 0);
		layout->addWidget(plot->rightWidget(), row++, 2);

		

		//connect(m_ui.checkBoxFreqScale, SIGNAL(toggled(bool)), this, SLOT(showFreqScale(bool)));
		//connect(m_ui.checkBoxColormapScale, SIGNAL(toggled(bool)), this, SLOT(showColorMapScale(bool)));
		//connect(m_ui.checkBoxLogScale, SIGNAL(toggled(bool)), this, SLOT(switchLogScale(bool)));
		connect(plot, SIGNAL(selectionDone(float, float)), this, SLOT(highlightSampleInterval(float, float)));
		connect(plot, SIGNAL(selectionMade(AwChannel *, float, int, int, float, int, int)), this, SIGNAL(selectionMade(AwChannel *, float, int, int, float, int, int)));
		connect(this, SIGNAL(freqScaleChanged(float, float, float)), plot, SLOT(updateFreqScale(float, float, float)));
		//connect(plot, SIGNAL(applyMinMaxToAll(double, double)), this, SLOT(applyMinMaxToAllPlots(double, double)));
	}
	connect(m_ui.checkBoxFreqScale, SIGNAL(toggled(bool)), this, SLOT(showFreqScale(bool)));
	connect(m_ui.checkBoxColormapScale, SIGNAL(toggled(bool)), this, SLOT(showColorMapScale(bool)));
	connect(m_ui.checkBoxLogScale, SIGNAL(toggled(bool)), this, SLOT(switchLogScale(bool)));
	//auto nPlotRows = row - 1;
	// add color map widget to the right column and span it to all the rows used by TF plots
//	layout->addWidget(m_colorMapWidget, 1, 2, nPlotRows, 1);
	m_channels = channels;
	m_signalView->setChannels(channels);
	connect(m_signalView, SIGNAL(dataLoaded(float, float)), this, SLOT(compute2(float, float)));
//	connect(m_signalView, &AwBaseSignalView::positionChanged, this, &TFWidget::compute2);
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
////	updatePlots();
//	m_min = m_plots.first()->min();
//	m_max = m_plots.first()->max();
//	for (auto p : m_plots) {
//		p->updateDisplaySettings();
//		m_min = std::min(p->min(), m_min);
//		m_max = std::max(p->max(), m_max);
//	}
//	if (!m_zRangeLocked) {
//		m_ui.lineZMin->setText(QString().setNum(m_min, 'g', 2));
//		m_ui.lineZMax->setText(QString().setNum(m_max, 'g', 2));
//	}

	applyNormalisation();
	if (!m_zRangeLocked) {
		m_ui.lineZMin->setText(QString().setNum(m_zmin, 'g', 2));
		m_ui.lineZMax->setText(QString().setNum(m_zmax, 'g', 2));
	}
}



void TFWidget::changeZScale(int index)
{
	m_displaySettings.zInterval = m_ui.cbZScale->currentData().toInt();
	////updatePlots();
	//m_min = m_plots.first()->min();
	//m_max = m_plots.first()->max();
	//for (auto p : m_plots) {
	//	p->updateDisplaySettings();
	//	m_min = std::min(p->min(), m_min);
	//	m_max = std::max(p->max(), m_max);
	//}
	//if (!m_zRangeLocked) {
	//	m_ui.lineZMin->setText(QString().setNum(m_min, 'g', 2));
	//	m_ui.lineZMax->setText(QString().setNum(m_max, 'g', 2));
	//}
	setZScale();
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
	//compute2();
	emit freqScaleChanged(m_settings->freq_min, m_settings->freq_max, m_settings->step);
	repaint();
}

void TFWidget::compute2(float pos, float duration)
{
	if (m_computationChannels.isEmpty())
		return;

	if (pos == m_pos && m_duration == duration)
		return;
	m_pos = pos;
	m_duration = duration;
	Q_ASSERT(pos >= 0 && duration > 0);

	m_process->requestData(&m_computationChannels, pos, duration);
	m_signalView->scene()->removeHighLigthMarker();

	std::function<arma::mat(AwChannel*)> comp = [this](AwChannel* channel) {
		float sr = channel->samplingRate();
		auto nsamples = channel->dataSize();
		float sigma2 = 1.;
		float* x = channel->data();
		double* freq;
		double* Psi;
		double** psi_array;
		double** wt;
		float min_center_freq;
		float max_center_freq;
		double* s_array = nullptr;
		double min_scale, max_scale;
		// compute padding for data
		double wavelet_length = m_settings->xi / (2 * M_PI * m_settings->freq_min);
		qint64 padding = 3 * (qint64)std::ceil(wavelet_length * channel->samplingRate());
		qint64 n = nsamples + 2 * padding; // padding on each side
		qint64 half_n = n / 2;

		double* omega = new double[n];
		for (qint64 i = 0; i < half_n; i++)
			omega[i] = (double)i * (sr / n);
		for (qint64 i = half_n; i < n; i++)
			omega[i] = -((half_n - (i + 1)) * (sr / n));
		double* in;
		fftw_complex* fftx;
		// FFT(x)
		in = (double*)fftw_malloc(sizeof(double) * n);
		fftx = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * n);
		TFMutex.lock();
		fftw_plan plan = fftw_plan_dft_r2c_1d(n, in, fftx, FFTW_ESTIMATE);
		TFMutex.unlock();
		std::memset(in, 0, n * sizeof(double));

		// pad before and after the signal with zeros
		for (qint64 i = 0; i < nsamples; i++)
			in[i + padding] = x[i];

		// applying DIFF except when computing baselines
		if (this->m_settings->useDIFF && !this->m_computeBaseline)
			for (qint64 i = 1; i < n - 1; i++)
				in[i - 1] = in[i] - in[i - 1];

		fftw_execute_dft_r2c(plan, in, fftx);
		TFMutex.lock();
		fftw_destroy_plan(plan);
		TFMutex.unlock();
		fftw_free(in);
		min_center_freq = (2 * m_settings->tolerance * sqrt(sigma2) * sr * m_settings->xi) / n;
		max_center_freq = (sr * m_settings->xi / (m_settings->xi + m_settings->tolerance / sqrt(sigma2)));
		if (m_settings->freqs.isEmpty()) {
			float nvoice = 12;
			int exp;
			for (double i = frexp(min_center_freq, &exp); i < frexp(max_center_freq, &exp); i += 1 / nvoice)
				m_settings->freqs << i * i;
		}
		s_array = new double[m_settings->freqs.size()];
		for (int i = 0; i < m_settings->freqs.size(); i++)
			s_array[i] = m_settings->xi / m_settings->freqs.at(i);
		min_scale = m_settings->xi / max_center_freq;
		max_scale = m_settings->xi / min_center_freq;

		int nscale = m_settings->freqs.size();
		// MATLAB
		//scaleindices=find(s_array(:)'>=minscale & s_array(:)'<=maxscale);
		QVector<int> scale_indices;
		for (int i = 0; i < nscale; i++) {
			if (s_array[i] >= min_scale && s_array[i] <= max_scale)
				scale_indices << i;
		}
		freq = new double[n];
		Psi = new double[n];
		fftw_complex* c_in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * n);
		fftw_complex* c_out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * n);
		TFMutex.lock();
		fftw_plan plan_c2c = fftw_plan_dft_1d(n, c_in, c_out, FFTW_BACKWARD, FFTW_ESTIMATE);
		TFMutex.unlock();
		double m2max = 0;
		mat data = zeros(nscale, nsamples);
		for (auto index : scale_indices) {
			double s = s_array[index];
			for (int i = 0; i < n; i++) {
				freq[i] = (s * omega[i]) - m_settings->xi;
				Psi[i] = pow(4 * M_PI * sigma2, (double)(1 / 4)) * sqrt(s) * exp(-sigma2 / 2 * freq[i] * freq[i]);
			}
			// Multiplying complex values by Gaussian function
			for (int i = 0; i < n; i++) {
				c_in[i][0] = Psi[i] * fftx[i][0];
				c_in[i][1] = Psi[i] * fftx[i][1];
			}
			// compute ifft(fftx.*Psi)
			fftw_execute_dft(plan_c2c, c_in, c_out);
			for (int i = 0, j = padding; i < nsamples; i++)
				data(index, i) = c_out[i + j][0] * c_out[i + j][0] + c_out[i + j][1] * c_out[i + j][1];

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

		return data;
	};

	if (m_settings->useBaselineCorrection && !m_baselineComputed) {
		// gather markers with same label
		m_baselineMarkers = AwMarker::getMarkersWithLabel(m_process->pdi.input.markers(), m_settings->baselineMarker);
		if (!m_baselineMarkers.isEmpty()) {
			// create temp channel list by duplicating current channels list
			AwChannelList temp;
			for (auto c : m_channels) 
				temp << c->duplicate();
			m_computeBaseline = true;
			m_process->requestData(&temp, &m_baselineMarkers);
			QFuture<arma::mat> future = QtConcurrent::mapped(temp, comp);
			future.waitForFinished();
			m_computeBaseline = false;
			m_baselineComputed = true;
			temp.erase(temp.begin(), temp.end());

			m_baselines.clear();
			// get results
			for (auto &r : future) 
				m_baselines << r;
			

			//AW_DESTROY_LIST(m_baselineComputations);
			//for (auto c : m_channels) {
			//	auto chan = new AwChannel(c);
			//	TFParam* p = new TFParam();
			//	p->settings = m_settings;
			//	p->settings->useDIFF = false;  // on baseline computation, do not use special options like DIFF
			//	p->x = chan;
			//	baselineChannels << chan;
			//	m_baselineComputations << p;
			//}
		}
			//m_process->requestData(&baselineChannels, &m_baselineMarkers);
			//QFuture<void> future = QtConcurrent::map(m_baselineComputations, compute_tf);
			//future.waitForFinished();

			//for (auto i = 0; i < m_tfComputations.size(); i++) {
			//	m_tfComputations.at(i)->baselineData = m_baselineComputations.at(i)->data;
			//}
			//m_baselineComputed = true;
			//AW_DESTROY_LIST(baselineChannels);
		//}
	}
	m_rawTF.clear();

	// sequential computation using MATLAB Coder code
	// other version using armadillo
	for (auto c : m_computationChannels) {

		double Fs = static_cast<double>(c->samplingRate());
		double xi = static_cast<double>(m_settings->xi);
		double wt_len = xi / (2 * M_PI * m_settings->freq_min);
		vec freqlist(m_settings->freqs.size());
		for (auto i = 0; i < m_settings->freqs.size(); i++)
			freqlist(i) = m_settings->freqs.at(i);

		uword padding = static_cast<uword>(3 * std::ceil(wt_len * Fs));
		uword nsamples = c->dataSize();
		uword n = nsamples + 2 * padding;
		vec x(n);
		for (auto i = 0; i < padding; i++)
			x(i) = c->data()[0];
		for (auto i = 0; i < c->dataSize(); i++)
			x(i + padding ) = c->data()[i];
		for (auto i = 0; i < padding; i++)
			x(c->dataSize() - 1 + padding) = c->data()[c->dataSize() -1];
		if (m_settings->useDIFF) {
			vec tmp = diff(x);
			x = vec(tmp.n_elem + 1);
			x(0) = tmp(0);
			for (uword i = 1; i < x.n_elem; i++)
				x(i) = tmp(i - 1);
		}
		double sigma2 = 1.;
		vec omega(n);
		for (uword i = 0; i < n / 2; i++)
			omega(i) = i * Fs / n;
		double c = -std::ceil(n / 2);
		double j;
		for (uword i = n / 2, j = 0; i < n; i++, j = 1)
			omega(i) = (c + j) * Fs / n;
		//vec tmp1 = regspace(0, n / 2);
		//vec tmp2 = regspace(-std::ceil(n / 2), -1);
		//vec omega = join_vert(tmp1, tmp2);
		//omega = omega * Fs / n;

		// MATLAB
		// fftx = fft(x);
		// MATLAB
		cx_vec fftx(n);
		fftw_complex* out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * n);
		fftw_plan plan = fftw_plan_dft_r2c_1d(n, x.memptr(), out, FFTW_ESTIMATE);
		fftw_execute(plan);
		fftw_destroy_plan(plan);
		for (auto i = 0; i < fftx.n_elem; i++) {
			fftx(i) = cx_double(out[i][0], out[i][1]);
		}
		// MATLAB
		// tolerance = 0.5;
		// mincenterfreq = 2 * tolerance * sqrt(sigma2) * Fs * xi. / n;
		// maxcenterfreq = Fs * xi / (xi + tolerance / sqrt(sigma2));
		// MATLAB
		double tolerance = 0.5;
		double mincenterfreq = 2. * tolerance * std::sqrt(sigma2) * Fs * xi / n;
		double maxcenterfreq = Fs * xi / (xi + tolerance / std::sqrt(sigma2));

		// MATLAB
		// s_array = xi. / freqlist;
		// minscale = xi. / maxcenterfreq;
		// maxscale = xi. / mincenterfreq;
		// nscale = length(freqlist);
		// wt = zeros(n, nscale);
		//scaleindices = find(s_array(:)'>=minscale & s_array(:)' <= maxscale);
		// MATLAB
		vec s_array = xi / freqlist;
		double minscale = xi / maxcenterfreq;
		double maxscale = xi / mincenterfreq;
		auto nscale = freqlist.n_elem;

		mat wt(nsamples, nscale);  // real matrix to directly store the modulus value
		wt.fill(0.);
		uvec scaleindices = arma::find(s_array >= minscale && s_array <= maxscale);

		cx_vec in(n);
		
		vec ifftxPsi(nsamples); // WARNING: here were a taking over the padding and also computing modulus (no complex values)

		fftw_plan plan_c2c = fftw_plan_dft_1d(n, (fftw_complex*)in.memptr(), out, FFTW_BACKWARD , FFTW_ESTIMATE);

		for (auto i = 0; i < scaleindices.n_elem; i++) {
			uword kscale = scaleindices(i);
			double s = s_array(kscale);
			vec freq = s * omega - xi;
		//	Psi = realpow(4. * pi.*sigma2, 1 / 4) * sqrt(s) * exp(-sigma2 / 2 * freq.*freq);
			freq.for_each([](vec::elem_type& val) { val = val * val; });
			vec Psi(n);
			double tmp = pow(4 * M_PI * sigma2, 0.25) * std::sqrt(s);
			for (auto i = 0; i < n; i++) {
				Psi(i) = tmp * exp(-sigma2 / freq(i) * freq(i));
			}
		//	vec Psi = pow(4 * M_PI * sigma2, 1 / 4) * std::sqrt(s) * exp(-sigma2 / 2 * freq);

		//	vec Psi(n);

		//	wt(1:n, kscale) = abs(ifft(fftx.*Psi)). ^ 2;
			in = fftx % Psi;
			//fftw_plan plan_c2c = fftw_plan_dft_1d(n, (fftw_complex *)temp, (fftw_complex *)ifftxPsi, FFTW_BACKWARD, FFTW_ESTIMATE);
			fftw_execute(plan_c2c);
			for (auto i = 0; i < nsamples; i++) {
				ifftxPsi(i) = std::sqrt(out[i + padding][0] * out[i + padding][0] + out[i + padding][1] * out[i + padding][1]);
			}

			wt.col(kscale) = ifftxPsi;
		}
		fftw_free(out);

		//mat res = arma::abs(wt);
		//res.shed_rows(res.n_rows - padding - 1, res.n_rows - 1);
		//res.shed_rows(0, padding - 1);
		m_rawTF << wt.t();
		// nscale = length(freqlist);
		// wt = zeros(n, nscale);
		// scaleindices = find(s_array(:)'>=minscale & s_array(:)' <= maxscale);
 		//coder::array<double, 2U> data;
		//coder::array<double, 2U> data_preproc;
		//data.set_size(1, c->dataSize());
		//for (int idx0 = 0; idx0 < 1; idx0++) {
		//	for (int idx1 = 0; idx1 < data.size(1); idx1++) {
		//		// Set the value of the array element.
		//		// Change this value to the value that the application requires.
		//		data[idx1] = c->data()[idx1];
		//	}
		//}
		//double srate = static_cast<double>(c->samplingRate());
		//double doDiff = 0;
		//double xi = static_cast<double>(m_settings->xi);
		//if (m_settings->useDIFF)
		//	doDiff = 1.;
		//// prepare signal data (padding of 3s)
		//wavelet_dyn_morlet_preprocess(data, srate, doDiff, xi, m_settings->freq_min, 3., data_preproc);
		//// compute
		//coder::array<double, 2U> freqlist;
		//coder::array<double, 2U> wt;
		//freqlist.set_size(1, m_settings->freqs.size());
		//for (int idx0 = 0; idx0 < 1; idx0++) {
		//	for (int idx1 = 0; idx1 < freqlist.size(1); idx1++) {
		//		// Set the value of the array element.
		//		// Change this value to the value that the application requires.
		//		freqlist[idx1] = m_settings->freqs.at(idx1);
		//	}
		//}
		//wavelet_dyn_morlet_freqlist(data_preproc, srate, freqlist, xi, wt);

		//// avoid padded data
		//mat tmp = mat(wt.data(), wt.size(0), wt.size(1)).t();
		//double wt_len = xi / (2 * M_PI * m_settings->freq_min);
		//uword padding = static_cast<uword>( 3 * std::ceil(wt_len * srate));
		//tmp.shed_cols(tmp.n_cols - padding - 1, tmp.n_cols - 1);
		//tmp.shed_cols(0, padding - 1);
		//m_rawTF << tmp;
	}

	// threaded computation using original C++ code
	//QFuture<arma::mat> future = QtConcurrent::mapped(m_channels, comp);
	//future.waitForFinished();
	//for (auto &r : future)
	//	m_rawTF << r;

	m_results.clear();
	for (auto i = 0; i < m_rawTF.size(); i++) {
		QPair<mat, mat> pair;
		if (m_baselines.size() > i)
			pair.second = m_baselines.at(i);
		else
			pair.second = mat();
		pair.first = m_rawTF.at(i);
		m_results << pair;
	}

	// apply normalisation :
	applyNormalisation();
	//setZScale();

	//QFuture<void> future = QtConcurrent::map(m_tfComputations, compute_tf);
	//future.waitForFinished();
	//int i = 0;

	//// update plots data and compute global color map min and max.
	//m_min = m_plots.first()->min();
	//m_max = m_plots.first()->max();
	//for (TFParam* p : m_tfComputations) {
	//	auto plot = m_plots.at(i++);
	//	plot->setNewData(m_signalView->positionInFile(), p);
	//	m_min = std::min(plot->min(), m_min);
	//	m_max = std::max(plot->max(), m_max);
	//}
	//if (!m_zRangeLocked) {
	//	m_ui.lineZMin->setText(QString().setNum(m_min, 'g', 2));
	//	m_ui.lineZMax->setText(QString().setNum(m_max, 'g', 2));
	//}
}

arma::mat TFWidget::computeFunction(AwChannel * c)
{
	if (c->dataSize() == 0)
		return arma::mat();
	float sr = c->samplingRate();
	auto nsamples = c->dataSize();
	float sigma2 = 1.;
	float* x = c->data();
	double* freq;
	double* Psi;
	double** psi_array;
	double** wt;
	float min_center_freq;
	float max_center_freq;
	double* s_array = nullptr;
	double min_scale, max_scale;
	// compute padding for data
	// Epsilon = 2pif0omega
	double wavelet_length = m_settings->xi / (2 * M_PI * m_settings->freq_min);
	qint64 padding = 3 * (qint64)std::ceil(wavelet_length * c->samplingRate());
	qint64 n = nsamples + 2 * padding; // padding on each side
	qint64 half_n = n / 2;
	// traduction du code MATLAB
    // omega = [(0:n/2) (-ceil(n/2)+1:-1)].*Fs/n; % CGB added ceil: TO BE CHECKED
    // omega = omega(:);
	double* omega = new double[n];
	for (qint64 i = 0; i < half_n; i++)
		omega[i] = (double)i * (sr / n);
	for (qint64 i = half_n; i < n; i++)
		omega[i] = -((half_n - (i + 1)) * (sr / n));
	double* in;
	fftw_complex* fftx;
	// FFT(x)
	in = (double*)fftw_malloc(sizeof(double) * n);
	fftx = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * n);
	TFMutex.lock();
	fftw_plan plan = fftw_plan_dft_r2c_1d(n, in, fftx, FFTW_ESTIMATE);
	TFMutex.unlock();
	std::memset(in, 0, n * sizeof(double));

	// pad before and after the signal with zeros
	for (qint64 i = 0; i < nsamples; i++)
		in[i + padding] = x[i];

	// applying DIFF except when computing baselines
	if (m_settings->useDIFF && !m_computeBaseline)
		for (qint64 i = 1; i < n - 1; i++)
			in[i - 1] = in[i] - in[i - 1];

	fftw_execute_dft_r2c(plan, in, fftx);
	TFMutex.lock();
	fftw_destroy_plan(plan);
	TFMutex.unlock();
	fftw_free(in);
	switch (m_settings->wavelet)
	{
	case TFWavelet2::Gabor:
		min_center_freq = (2 * m_settings->tolerance * sqrt(sigma2) * sr * m_settings->xi) / n;
		max_center_freq = (sr * m_settings->xi / (m_settings->xi + m_settings->tolerance / sqrt(sigma2)));
		if (m_settings->freqs.isEmpty()) {
			float nvoice = 12;
			int exp;
			for (double i = frexp(min_center_freq, &exp); i < frexp(max_center_freq, &exp); i += 1 / nvoice)
				m_settings->freqs << i * i;
		}
		s_array = new double[m_settings->freqs.size()];
		for (int i = 0; i < m_settings->freqs.size(); i++)
			s_array[i] = m_settings->xi / m_settings->freqs.at(i);
		min_scale = m_settings->xi / max_center_freq;
		max_scale = m_settings->xi / min_center_freq;
		break;
	default:
		break;
	}
	// MATLAB: 
	// nscale = length(freqlist);
	// wt = zeros(n,nscale);
	// The matrix is n columns and nscale lines 
	int nscale = m_settings->freqs.size();
	//	QVector<double> matrix(nscale * nsamples);

	

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
	fftw_complex* c_in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * n);
	fftw_complex* c_out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * n);
	TFMutex.lock();
	fftw_plan plan_c2c = fftw_plan_dft_1d(n, c_in, c_out, FFTW_BACKWARD, FFTW_ESTIMATE);
	TFMutex.unlock();
	double m2max = 0;
	mat data = zeros(nscale, nsamples);
	for (auto index : scale_indices) {
		double s = s_array[index];
		switch (m_settings->wavelet)
		{
		case TFWavelet2::Gabor:
			for (int i = 0; i < n; i++) {
				freq[i] = (s * omega[i]) - m_settings->xi;
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
			data(index, i) = c_out[i + j][0] * c_out[i + j][0] + c_out[i + j][1] * c_out[i + j][1];

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

	return data;
}

void TFWidget::applyNormalisation()
{
	int norm = m_displaySettings.normalization;

	std::function<arma::mat(const QPair<mat, mat>&)> applyNorm = [norm](const QPair<mat, mat>& result) {
		mat data = mat(result.first);
		mat baseline = result.second;
		switch (norm) {
			case DisplaySettings::N10log10Divisive:
				if (!baseline.is_empty()) {
					for (auto i = 0; i < data.n_rows; i++)
						data.row(i) = 10 * log10(data.row(i) / arma::mean(baseline.row(i)));
				}
				else {
					for (auto i = 0; i < data.n_rows; i++)
						data.row(i) = 10 * log10(data.row(i) / arma::mean(data.row(i)));
				}
				break;
			case DisplaySettings::NoNorm:
				break;
			case DisplaySettings::ZScore:
				if (!baseline.is_empty()) {
					for (auto i = 0; i < data.n_rows; i++) {
						data.row(i) -= arma::mean(baseline.row(i));
						data.row(i) /= arma::stddev(baseline.row(i));
					}
				}
				else {
					for (auto i = 0; i < data.n_rows; i++) {
						data.row(i) -= arma::mean(data.row(i));
						data.row(i) /= arma::stddev(data.row(i));
					}
				}
				break;
		}
		return data;
	};

	QFuture<arma::mat> future = QtConcurrent::mapped(m_results, applyNorm);
	future.waitForFinished();
	Q_ASSERT(future.resultCount() == m_plots.size());
	int i = 0;
	vec vmin(future.resultCount()), vmax(future.resultCount());
	m_normalizedTF.clear();
	for (auto &r : future) {
		vmin(i) = r.min();
		vmax(i) = r.max();
		m_normalizedTF << r;
		if (i == 0)
			m_qcplot->setNewData(r, m_signalView->positionInFile());
		m_plots.at(i++)->setDataMatrix(r, m_signalView->positionInFile());
	}
	m_min = vmin.min();
	m_max = vmax.max();
	if (!m_zRangeLocked) {
		m_zmin = m_min;
		m_zmax = m_max;
		m_ui.lineZMin->setText(QString().setNum(m_zmin, 'g', 2));
		m_ui.lineZMax->setText(QString().setNum(m_zmax, 'g', 2));
	}
	setZScale();
}

void TFWidget::setZScale()
{
	int i = 0;
	for (auto p : m_plots) {
		QwtInterval ZInterval;
		if (m_zRangeLocked) {
			ZInterval.setMinValue(m_zmin);
			ZInterval.setMaxValue(m_zmax);
		}
		else {
			double min = m_normalizedTF.at(i).min();
			double max = m_normalizedTF.at(i).max();
			double abs_max = std::max(std::abs(min), std::abs(max));
			i++;
			switch (m_displaySettings.zInterval) {
			case DisplaySettings::MinToMax:
				ZInterval = QwtInterval(min, max);
				break;
			case DisplaySettings::ZeroToMax:
				ZInterval = QwtInterval(0, max);
				break;
			case DisplaySettings::MaxToMax:
				ZInterval = QwtInterval(-abs_max, abs_max);
				break;
			}
		}
		p->updateZInterval(ZInterval);
	}
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