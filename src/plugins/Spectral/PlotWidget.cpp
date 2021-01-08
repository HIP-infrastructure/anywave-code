#include "PlotWidget.h"


PlotWidget::PlotWidget(const FFTs& ffts, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	m_FFTs = ffts;
	// prepare x axis (Freq. Hz)
	auto length = ffts.first()->pxx().n_elem;

	QVector<double> x(length), y(length);
	for (auto i = 0; i < length; i++) {
		x[i] = i + 1;
	}
	ui.widget->xAxis->setLabel("Freq. (Hz)");
	ui.widget->yAxis->setLabel("uV²/Hz");
	uword count = 0;
	double min = 0., max = 0.;
	for (auto iter : ffts) {
		arma::vec pxx = 10 * log10(iter->pxx());
		min = std::min(min, pxx.min());
		max = std::max(max, pxx.max());
		memcpy(y.data(), pxx.memptr(), length * sizeof(double));
		ui.widget->addGraph();
		//QColor color(20 + 200 / 4.0 * count, 70 * (1.6 - count / 4.0), 150, 150);
		ui.widget->graph(count)->setData(x, y);
		//ui.widget->graph(count)->setPen(QPen(color));
		count++;
	}
	// set axes ranges, so we see all data:
	ui.widget->xAxis->setRange(1, length + 1);
	ui.widget->yAxis->setRange(min, max);
}

PlotWidget::~PlotWidget()
{

}
