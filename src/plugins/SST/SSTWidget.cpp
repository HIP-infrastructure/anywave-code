#include "SSTWidget.h"
#include <graphics/AwColorMap.h>
#include <qwt_scale_widget.h>
#include <qwt_scale_div.h>
#include <qwt_scale_engine.h>
#include <qwt_plot_layout.h>
#include <math.h>

QwtText LabelDraw::label(double value) const
{
	double dec;
	if (modf(value, &dec) == 0.5)
		if ((int)dec < m_labels.size())
			return QwtText(m_labels.at(dec));
	return QwtText("");
}

QwtText TimeDraw::label(double value) const
{
	double pos = m_currentPosition + value / m_sampleSecond;
	return QwtText(QString("%1s").arg(pos));
}


SSTWidget::SSTWidget(AwChannelList& channels, float duration, QWidget *parent)
	: QWidget(parent)
{
	m_ui.setupUi(this);
	m_dataDuration = 5.;
	m_plot = new SEEGPlot(channels, this);
	m_ui.verticalLayout->replaceWidget(m_ui.widget, m_plot);
	m_ui.comboBox->initToColorMap(AwColorMap::Jet);
	connect(m_ui.comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setNewCMap(int)));
	connect(m_ui.buttonBack, SIGNAL(clicked()), this, SLOT(goBack()));
	connect(m_ui.buttonForward, SIGNAL(clicked()), this, SLOT(goForward()));
	connect(m_ui.buttonUpdate, SIGNAL(clicked()), this, SLOT(updateData()));
	connect(m_ui.comboBoxPlots, SIGNAL(currentIndexChanged(int)), m_plot, SLOT(changePlot(int)));
	m_totalDuration = duration;
	m_position = 0.;
	m_step = 1.;
	m_channels = channels;
	buildYScale();
	updateData();
}

SSTWidget::~SSTWidget()
{
}


void SSTWidget::setNewCMap(int index)
{
	m_plot->changeCMap(m_ui.comboBox->selectedMap());
	
}

void SSTWidget::buildXScale()
{
	qint64 nSamples = m_channels.first()->dataSize();
	int sampleSecond = (int)floor(m_channels.first()->samplingRate());
	m_plot->setAxisScale(QwtPlot::xBottom, 0, nSamples - 1, sampleSecond);
	QList<double> ticks[QwtScaleDiv::NTickTypes];
	for (int i = 0; i < nSamples; i += sampleSecond)
		ticks[QwtScaleDiv::MajorTick] << (double)i;
	QwtScaleDiv div(ticks[QwtScaleDiv::MajorTick].first(), ticks[QwtScaleDiv::MajorTick].last(), ticks);
	m_plot->axisWidget(QwtPlot::xBottom)->scaleDraw()->setScaleDiv(div);
	TimeDraw *newTimeDrawX = new TimeDraw(sampleSecond, m_position);
	m_plot->setAxisScaleDraw(QwtPlot::xBottom, newTimeDrawX);
}

void SSTWidget::buildYScale()
{
	LabelDraw *newLabelDrawY = new LabelDraw(AwChannel::getLabels(m_channels));
	m_plot->setAxisScale(QwtPlot::yLeft, m_channels.size(), 0, 0.5);
	newLabelDrawY->enableComponent(QwtAbstractScaleDraw::ScaleComponent::Backbone, false);
	newLabelDrawY->enableComponent(QwtAbstractScaleDraw::ScaleComponent::Ticks, false);
	QList<double> ticks[QwtScaleDiv::NTickTypes];
	for (int i = 0; i < m_channels.size(); i++)
		ticks[QwtScaleDiv::MajorTick] << (double)i + 0.5;
	QwtScaleDiv div(ticks[QwtScaleDiv::MajorTick].first(), ticks[QwtScaleDiv::MajorTick].last(), ticks);
	m_plot->axisWidget(QwtPlot::yLeft)->scaleDraw()->setScaleDiv(div);
	m_plot->setAxisScaleDraw(QwtPlot::yLeft, newLabelDrawY);
	m_plot->axisScaleEngine(QwtPlot::yLeft)->setAttribute(QwtScaleEngine::Floating, true);
//	m_plot->plotLayout()->setAlignCanvasToScales(true);
}

void SSTWidget::updateData()
{
	m_dataDuration = m_ui.spinDuration->value();
	if (m_client.isConnected()) {
		m_client.requestData(&m_channels, m_position, m_dataDuration);
		buildXScale();
		buildYScale();
		m_plot->updateCurrentPlot();
	}
}


void SSTWidget::goBack()
{
	float pos = m_position - m_step;

	if (pos <= 0.)
		pos = 0;
	
	if (pos != m_position) {
		m_position = pos;
		updateData();
	}
}

void SSTWidget::goForward()
{
	float pos = m_position + m_step;

	if (pos > m_totalDuration)
		return;

	m_position = pos;
	updateData();
}