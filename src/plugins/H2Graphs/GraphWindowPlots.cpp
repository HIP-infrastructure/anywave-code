#include "GraphWindow.h"
#include <qcustomplot.h>


void GraphWindow::plotTimeCourses()
{
	QScreen *screen = QGuiApplication::primaryScreen();
	QRect geometry = screen->geometry();

	while (!m_plots.isEmpty()) {
		auto w = m_plots.takeFirst();
		w->close();
		delete w;
	}
	cube matrix = m_graphSet->correlationMatrix();
	auto const& positions = m_graphSet->positions();
	int xx = 10, yy = 80;
	const int plotWidth = 400;
	const int plotHeight = 300;
	for (auto const& pair : m_pairs) {
		// set widget geometry 
		QRect rect = QRect(xx, yy, plotWidth, plotHeight);
		auto tokens = pair.split('>');
		const QString& c1 = tokens.first();
		const QString& c2 = tokens.last();
		QCustomPlot* plot = new QCustomPlot;
		plot->setGeometry(rect);
		plot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		m_plots << plot;
		// get channel indexes in matrix
		int x = m_graphSet->labels.indexOf(c1);
		int y = m_graphSet->labels.indexOf(c2);
		Q_ASSERT(x != -1);
		Q_ASSERT(y != -1);
		QCPCurve* curve1 = new QCPCurve(plot->xAxis, plot->yAxis);
		QCPCurve* curve2 = new QCPCurve(plot->xAxis, plot->yAxis);
		QVector<QCPCurveData> data1(m_graphSet->totalIterations()), data2(m_graphSet->totalIterations());
		for (int i = 0; i < m_graphSet->totalIterations(); i++) {
			data1[i] = QCPCurveData(i, positions[i], matrix(x, y, i));
			data2[i] = QCPCurveData(i, positions[i], matrix(y, x, i));
		}
		plot->xAxis->setRange(QCPRange(positions.first(), positions.last()));
		plot->yAxis->setLabel(m_graphSet->method);
		plot->yAxis->setRange(QCPRange(0., 1.));
		curve1->data()->set(data1);
		curve2->data()->set(data2);
		curve1->setName(QString("%1 -> %2").arg(c1).arg(c2));
		curve2->setName(QString("%1 -> %2").arg(c2).arg(c1));
		curve1->setPen(QPen(Qt::blue));
		curve2->setPen(QPen(Qt::darkGreen));
		// set some basic customPlot config:
		plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
		plot->axisRect()->setupFullAxesBox();
		plot->rescaleAxes();
		QFont legendFont = font();
		legendFont.setPointSize(10);
		plot->legend->setFont(legendFont);
		plot->legend->setVisible(true);
	
		plot->show();
		plot->setWindowIcon(QIcon(":/images/AnyWave_icon.png"));
		plot->setWindowTitle(QString("correlation values in time, from %1s to %2s").arg(positions.first()).arg(positions.last()));
		xx += plotWidth + 10;
		if (xx + plotWidth > geometry.width()) {
			xx = 10;
			yy += plotHeight + 10;
		}
		if (yy + plotHeight > geometry.height()) {
			xx = 10;
			yy = 80;
		}
	}
}