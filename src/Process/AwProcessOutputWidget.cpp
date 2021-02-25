#include <process/AwProcessOutputWidget.h>


AwProcessOutputWidget::AwProcessOutputWidget(AwBaseProcess* p, QWidget* w) : QWidget(w)
{
	m_process = p;
	// relay signals to process
	connect(this, SIGNAL(log(const QString&)), p, SIGNAL(log(const QString&)));
	connect(this, SIGNAL(sendCommand(int , QVariantList args)), p, SIGNAL(sendCommand(int, QVariantList args)));
	connect(this, SIGNAL(sendCommand(const QVariantMap&)), p, SIGNAL(sendCommand(const QVariantMap&)));
	connect(this, SIGNAL(sendMarkers(AwMarkerList *)), p, SIGNAL(sendMarkers(AwMarkerList*)));
	connect(this, SIGNAL(sendMarker(AwMarker*)), p, SIGNAL(sendMarker(AwMarker*)));
	connect(this, SIGNAL(dataConnectionRequested(AwDataClient*)), p, SIGNAL(dataConnectionRequested(AwDataClient*)));
}