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
	connect(m_ui.buttonApply, &QPushButton::clicked, this, &TFWidget::lockZRange);
	connect(m_ui.buttonReset, &QPushButton::clicked, this, &TFWidget::unlockZRange);
	connect(m_ui.radioModulus2, &QRadioButton::toggled, this, &TFWidget::changeModulus);
	m_pos = -1;
}

TFWidget::~TFWidget()
{
}


void TFWidget::lockZRange()
{
	double min = m_ui.lineZMin->text().toDouble();
	double max = m_ui.lineZMax->text().toDouble();

	if (max <= min)
		return;

	m_zRangeLocked = true;
	m_ui.cbNorm->setEnabled(false);
	m_ui.cbZScale->setEnabled(false);
	m_ui.buttonApply->setEnabled(false);
	m_ui.radioModulus->setEnabled(false);
	m_ui.radioModulus2->setEnabled(false);

	m_zmin = min;
	m_zmax = max;
	setZScale();
}

void TFWidget::unlockZRange()
{
	m_zRangeLocked = false;
	m_ui.cbNorm->setEnabled(true);
	m_ui.cbZScale->setEnabled(true);
	m_ui.buttonApply->setEnabled(true);
	m_ui.lineZMin->setText(QString().setNum(m_min, 'g', 2));
	m_ui.lineZMax->setText(QString().setNum(m_max, 'g', 2));
	m_ui.cbNorm->setEnabled(true);
	m_ui.cbZScale->setEnabled(true);
	m_ui.radioModulus->setEnabled(true);
	m_ui.radioModulus2->setEnabled(true);
	setZScale();
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
			//for (TFParam *p : m_tfComputations)	  // restor baseline data in the computation list.
			//	p->baselineData = m_baselineComputations.at(i++)->data;
		}
	}
	else {
		//for (TFParam *p : m_tfComputations)
		//	p->baselineData.clear();
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
		// add TF plot widget to the layout
		TFPlot *plot = new TFPlot(m_settings, &m_displaySettings, c);
		m_plots << plot;
		layout->addWidget(plot, row, 1);
		layout->addWidget(plot->leftWidget(), row, 0);
		layout->addWidget(plot->rightWidget(), row++, 2);
		connect(plot, SIGNAL(selectionDone(float, float)), this, SLOT(highlightSampleInterval(float, float)));
		connect(plot, SIGNAL(selectionMade(AwChannel *, float, int, int, float, int, int)), this, SIGNAL(selectionMade(AwChannel *, float, int, int, float, int, int)));
		connect(this, SIGNAL(freqScaleChanged(float, float, float)), plot, SLOT(updateFreqScale(float, float, float)));
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
	connect(m_ui.buttonReplot, &QPushButton::clicked, this, [this]() { compute2(m_pos, -1); });
	repaint();
}

void TFWidget::setMarkers(const AwMarkerList& markers)
{
	m_signalView->setMarkers(markers);
	m_ui.comboMarkers->setMarkers(markers);
}


void TFWidget::showColorMapScale(bool flag)
{
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
	for (auto p : m_plots)
		p->updateDisplaySettings();
}

void TFWidget::changeColorMap(int index)
{
	QComboBox *cb = (QComboBox *)sender();
	Q_ASSERT(cb != NULL);
	m_displaySettings.colorMap = cb->currentData(Qt::UserRole).toInt();

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
	applyNormalisation();
	if (!m_zRangeLocked) {
		m_ui.lineZMin->setText(QString().setNum(m_zmin, 'g', 2));
		m_ui.lineZMax->setText(QString().setNum(m_zmax, 'g', 2));
	}
}



void TFWidget::changeZScale(int index)
{
	m_displaySettings.zInterval = m_ui.cbZScale->currentData().toInt();
	setZScale();
}

void TFWidget::changeModulus(bool flag)
{
	m_displaySettings.values = flag ? DisplaySettings::Modulus2 : DisplaySettings::Modulus;
	applyNormalisation();
}

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
	compute2(m_pos, -1);
	emit freqScaleChanged(m_settings->freq_min, m_settings->freq_max, m_settings->step);
	repaint();
}

void TFWidget::compute2(float pos, float duration)
{
	// do nothing is position is unchanged

	// special case : duration = -1.0  => force to recompute all
	if (pos == m_pos && duration != -1)
		return;

	m_pos = pos;
	m_signalView->scene()->removeHighLigthMarker();

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

			// sequential
			m_baselines.clear();
			for (auto channel : temp) {
				m_baselines << computeFunction(channel);
			}
			m_computeBaseline = false;
			m_baselineComputed = true;
		}
	}
	m_rawTF.clear();

	// sequential computation using armadillo code based on MATLAB code
	for (auto c : m_channels) {
		m_rawTF << computeFunction(c);
	}

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
}

arma::mat TFWidget::computeFunction(AwChannel * chan)
{
	// armadillo version based on MATLAB code
	double Fs = static_cast<double>(chan->samplingRate());
	double xi = static_cast<double>(m_settings->xi);
	double wt_len = xi / (2 * M_PI * m_settings->freq_min);
	vec freqlist(m_settings->freqs.size());
	for (auto i = 0; i < m_settings->freqs.size(); i++)
		freqlist(i) = m_settings->freqs.at(i);

	uword padding = static_cast<uword>(3 * std::ceil(wt_len * Fs));
	uword nsamples = static_cast<uword>(chan->dataSize());
	uword n = nsamples + 2 * padding;
	vec x(n);
	for (auto i = 0; i < padding; i++)
		x(i) = chan->data()[0];
	for (auto i = 0; i < chan->dataSize(); i++)
		x(i + padding) = chan->data()[i];
	for (auto i = chan->dataSize(); i < chan->dataSize() + padding; i++)
		x(i + padding) = chan->data()[chan->dataSize() - 1];
	if (m_settings->useDIFF) {
		vec tmp = diff(x);
		x = vec(tmp.n_elem + 1);
		x(0) = tmp(0);
		for (uword i = 1; i < x.n_elem; i++)
			x(i) = tmp(i - 1);
	}
	double mi = x.min();
	double ma = x.max();
	double sigma2 = 1.;
	vec omega(n);
	for (uword i = 0; i < n / 2; i++)
		omega(i) = i * Fs / n;
	double c = -std::ceil(n / 2);
	double j;
	for (uword i = n / 2, j = 0; i < n; i++, j = 1)
		omega(i) = (c + j) * Fs / n;

	// MATLAB
	// fftx = fft(x);
	// MATLAB
	cx_vec fftx(n);
	fftx.fill(0);
	fftw_plan plan = fftw_plan_dft_r2c_1d(n, x.memptr(), (fftw_complex*)fftx.memptr(), FFTW_ESTIMATE);
	fftw_execute_dft_r2c(plan, x.memptr(), (fftw_complex*)fftx.memptr());
	fftw_destroy_plan(plan);
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
	cx_vec ifftxPsi(n);
	fftw_plan plan_c2c = fftw_plan_dft_1d(n, (fftw_complex*)in.memptr(), (fftw_complex*)ifftxPsi.memptr(), FFTW_BACKWARD, FFTW_ESTIMATE);
	for (auto i = 0; i < scaleindices.n_elem; i++) {
		uword kscale = scaleindices(i);
		double s = s_array(kscale);
		vec freq = s * omega - xi;
		vec Psi(n);
		double tmp = pow(4 * M_PI * sigma2, 0.25) * std::sqrt(s);
		for (auto i = 0; i < n; i++)
			Psi(i) = tmp * exp(-sigma2 / 2 * freq(i) * freq(i));

		in = fftx % Psi;
		fftw_execute(plan_c2c);
		vec modulus = abs(ifftxPsi);

		for (auto i = 0; i < nsamples; i++) {
			wt(i, kscale) = modulus(i + padding);
		}
	}
	fftw_destroy_plan(plan_c2c);
	return wt.t();
}

void TFWidget::applyNormalisation()
{
	int norm = m_displaySettings.normalization;
	int values = m_displaySettings.values;
	
	std::function<arma::mat(const QPair<mat, mat>&)> applyNorm = [norm, values](const QPair<mat, mat>& result) {
		mat data = mat(result.first);
		if (values == DisplaySettings::Modulus2) 
			data.for_each([](mat::elem_type& val) { val *= val; });

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
