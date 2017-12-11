#include "AwEpochThumbCreator.h"
#include <epoch/AwEpoch.h>
#include <epoch/AwEpochTree.h>
#include "Data/AwDataServer.h"
#include <qwt_plot_curve.h>
#include <qwt_plot_layout.h>
#include <armadillo>
//#include <qthread.h>
using namespace arma;

AwEpochThumbCreator::AwEpochThumbCreator(QObject *parent) : AwDataClient(parent)
{
	//QThread *t = new QThread(this);
	//moveToThread(t);
	//AwDataServer::getInstance()->openConnection(this);
	//t->start();
}

AwEpochThumbCreator::~AwEpochThumbCreator()
{
	qDeleteAll(m_plots);
	AwDataServer::getInstance()->closeConnection(this);
	//thread()->exit(0);
	//thread()->wait();
}



void AwEpochThumbCreator::plot(AwEpoch *epoch)
{
	QwtPlot *plot = new QwtPlot();
	m_plots.append(plot);
	plot->setCanvasBackground(Qt::white);
	plot->resize(800, 600);
	plot->enableAxis(QwtPlot::xBottom, false);
	plot->enableAxis(QwtPlot::yLeft, false);
	plot->setFrameShape(QFrame::StyledPanel);
	plot->plotLayout()->setAlignCanvasToScales(true);
	AwEpochTree *cond = epoch->condition;
	AwChannelList channels = cond->channels();
	requestData(&cond->channels(), epoch->posAndDuration);
	//double min = 0., max = 0.;
	vec data(channels.first()->dataSize());
	vec x = linspace(0, channels.first()->dataSize() - 1, channels.first()->dataSize());
	for (auto c : channels) {
		QwtPlotCurve *curve = new QwtPlotCurve(c->name());
		data.zeros();
		for (qint64 i = 0; i < c->dataSize(); i++)
			data(i) = c->data()[i];
		//double d_min = data.min();
		//double d_max = data.max();
		//if (d_min < min)
		//	min = d_min;
		//if (d_max > max)
		//	max = d_max;

		curve->setSamples(x.memptr(), data.memptr(), (int)c->dataSize());
		curve->attach(plot);
	}
	//plot->setAxisScale(QwtPlot::yLeft, min, max);
	plot->replot();
	plot->show();
}

