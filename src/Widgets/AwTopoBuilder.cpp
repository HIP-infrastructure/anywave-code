#include <widget/AwTopoBuilder.h>
#include <layout/AwLayout.h>
#include "Aw2DGrid.h"
#include <graphics/AwColorMap.h>
#include <QImage>
#include <QPainter>
#include <QtMath>
#ifndef NDEBUG
#include <QDebug>
#endif

#ifdef Q_OS_WIN
#define isnan(x) (x != x)
#endif

// static init
AwTopoBuilder *AwTopoBuilder::m_instance = 0;


AwTopoBuilder *AwTopoBuilder::instance()
{
	if (!m_instance)
		m_instance = new AwTopoBuilder;
	return m_instance;
}

void AwTopoBuilder::destroy()
{
	if (m_instance)	{
		delete m_instance;
		m_instance = 0;
	}
}


//end of statics

AwTopoBuilder::AwTopoBuilder(QObject *parent)
	: QObject(parent)
{
	m_res = 200;
	// start a new thread and move the object to that thread.
	m_thread = new QThread(this);
	moveToThread(m_thread);
	m_thread->start();
}

AwTopoBuilder::~AwTopoBuilder()
{
	m_thread->exit();
	m_thread->wait();
}

void AwTopoBuilder::stop()
{
	AwTopoBuilder::destroy();
}


/** compute topography for 2D AwTopoWidget **/
void AwTopoBuilder::computeTopo(AwTopoWidget *widget, const QStringList& labels, double *values)
{
#ifndef NDEBUG
	qDebug() << Q_FUNC_INFO << "called" << endl;
#endif
	if (widget->layout() == NULL) 
		return;

	m_sensors.clear();
	int i = 0;
	foreach (QString s, labels) {	// build a sensor list in the same order than labels...
									// Assuming the values are also sorted in the same order
		AwSensor *sensor = widget->layout()->getSensorByName(s);
		if (sensor)		{
			sensor->setValue(values[i++]);
			m_sensors << sensor;
		}
	}
	
	// compute a grid with res = 100
	Aw2DGrid *grid = new Aw2DGrid(m_res, m_sensors);
	
	if (grid->status != Aw2DGrid::Ok)	{
		emit error(tr("2D grid interpolation error"));
		delete grid;
		return;
	}
	emit computationDone(widget, grid);

#ifndef NDEBUG
	qDebug() << Q_FUNC_INFO << "finished" << endl;
#endif
}

void AwTopoBuilder::updateBadSensors(AwMapWidget *widget, const QStringList& labels)
{
	if (widget->map() == NULL) 
		return;
	widget->map()->updateBadSensors(labels);
	emit computationDone(widget);
}

/** compute topography for 3D AwMapWidget **/
void AwTopoBuilder::computeTopo(AwMapWidget *widget, const QStringList& labels, float *values)
{
#ifndef NDEBUG
	qDebug() << Q_FUNC_INFO << "called" << endl;
#endif
	if (widget->map() == NULL) 
		return;
	if (labels.isEmpty())
		return;
	float *data = new float[labels.size()];
	memcpy(data, values, labels.size() * sizeof(float));
	widget->map()->update(data, labels);
	delete[] data;
	emit computationDone(widget);

#ifndef NDEBUG
	qDebug() << Q_FUNC_INFO << "finished" << endl;
#endif
}