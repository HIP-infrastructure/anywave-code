#include "PlotWidget.h"


PlotWidget::PlotWidget(const FFTs& ffts, AwBaseProcess *p, QWidget *parent)
	: AwProcessOutputWidget(p, parent)
{
	ui.setupUi(this);

	// add a row with text explanation beyond the plot
	m_textBox = new QCPTextElement(ui.widget);
	m_textBox->setText("Click on a signal to select it. The signal will be shown on AnyWave.\n");
	m_textBox->setFont(QFont("sans", 12, QFont::Bold));
	ui.widget->plotLayout()->insertRow(1);
	ui.widget->plotLayout()->addElement(1, 0, m_textBox);
	m_FFTs = ffts;
	// prepare x axis (Freq. Hz)
	auto length = ffts.first()->pxx().n_elem;

	QVector<double> x(length), y(length);
	for (auto i = 0; i < length; i++) {
		x[i] = (double)i + 1;
	}
	ui.widget->xAxis->setLabel("Frequency (Hz)");
	ui.widget->yAxis->setLabel(QString::fromLatin1("%1²/Hz").arg(ffts.first()->channel()->unit()));
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
	ui.widget->xAxis->setRange(1, length + 1);
	ui.widget->yAxis->setRange(min, max);
	connect(ui.widget, SIGNAL(plottableClick(QCPAbstractPlottable*, int, QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*, int)));
}

PlotWidget::~PlotWidget()
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