#ifndef AWTOPOBUILDER_H
#define AWTOPOBUILDER_H
#include <AwGlobal.h>
#include <QObject>
#include <QThread>
#include <AwSensor.h>
#include <widget/AwTopoWidget.h>
#include <widget/AwMapWidget.h>

// this class is designed as a threaded object which computes 2D maps.
// AwTopoWidgets send topo request to this object and when the computation is done a message is sent back to the widget.

class AW_WIDGETS_EXPORT AwTopoBuilder : public QObject
{
	Q_OBJECT

public:
	static AwTopoBuilder *instance();
	static void destroy();
	void addConnection(AwTopoWidget *widget);
	void removeConnection(AwTopoWidget *widget);

	~AwTopoBuilder();
signals:
	void computationDone(AwTopoWidget *widget, Aw2DGrid *grid);
	void computationDone(AwMapWidget *widget);
	void error(const QString& error);
public slots:
	void computeTopo(AwTopoWidget *widget, const QStringList& labels, double *values);
	void computeTopo(AwMapWidget *widget, const QStringList& labels, float *values);
	void updateBadSensors(AwMapWidget *widget, const QStringList& badLabels);
	void stop();
protected:
	AwTopoBuilder(QObject *parent = 0);
	static AwTopoBuilder *m_instance;
	int m_res;	// 2D interpolation grid resolution
	QThread *m_thread;
	AwSensorList m_sensors;
};

#endif // AWTOPOBUILDER_H
