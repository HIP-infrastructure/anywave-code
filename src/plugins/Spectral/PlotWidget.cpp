#include "PlotWidget.h"
#include "Spectral.h"

PlotWidget::PlotWidget(const FFTs& ffts, AwBaseProcess *p, QWidget *parent)
	: AwProcessOutputWidget(p, parent)
{
	ui.setupUi(this);

	Spectral* process = qobject_cast<Spectral*>(p);
	auto fs = process->fs();

	// add a row with text explanation beyond the plot
	m_textBox = new QCPTextElement(ui.widget);
	m_textBox->setText("Click the left button on a signal and it will be highlighted in AnyWave.\nUse the mousewheel to zoom on x axis.");
	m_textBox->setFont(QFont("sans", 12, QFont::Bold));
	ui.widget->plotLayout()->insertRow(1);
	ui.widget->plotLayout()->addElement(1, 0, m_textBox);
	m_FFTs = ffts;
	// prepare x axis (Freq. Hz)
	auto length = ffts.first()->pxx().n_elem;

	QVector<double> x(length), y(length);
	vec arma_x = linspace(0, fs / 2, length);
	memcpy(x.data(), arma_x.memptr(), length * sizeof(double));
	
	ui.widget->xAxis->setLabel("Frequency (Hz)");
	ui.widget->yAxis->setLabel("dB");
	ui.widget->setInteractions(QCP::iSelectPlottables| QCP::iRangeZoom | QCP::iRangeDrag);
	double min = 0., max = 0.;
	uword count = 0;
	for (auto iter : ffts) {
		arma::vec pxx = 10 * log10(iter->pxx());
		min = std::min(min, pxx.min());
		max = std::max(max, pxx.max());
		memcpy(y.data(), pxx.memptr(), length * sizeof(double));
		auto graph = ui.widget->addGraph();
		//QColor color(20 + 200 / 4.0 * count, 70 * (1.6 - count / 4.0), 150, 150);
		graph->setName(iter->channel()->name());
		graph->setData(x, y);
		// selected color will be red
		auto decorator = graph->selectionDecorator();
		decorator->setPen(QColor(Qt::red));
	}
	// set axes ranges, so we see all data:
	m_xmin = 1;
	m_xmax = length + 1;
	ui.widget->xAxis->setRange(1, fs / 2);
	ui.widget->yAxis->setRange(min, max);
	ui.widget->axisRect()->setRangeZoom(Qt::Horizontal);
	connect(ui.widget, SIGNAL(plottableClick(QCPAbstractPlottable*, int, QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*, int)));

}

PlotWidget::~PlotWidget()
{

}


void PlotWidget::updateClicked()
{

}

void PlotWidget::resetClicked()
{

}

void PlotWidget::graphClicked(QCPAbstractPlottable* plottable, int dataIndex)
{
	Q_UNUSED(dataIndex);
	m_textBox->setText(QString("Channel %1 selected").arg(plottable->name()));
	QStringList labels = { plottable->name() };
	QVariantMap map;
	map["command"] = AwProcessCommand::HighlightChannels;
	map["channels"] = labels;
	emit sendCommand(map);
}