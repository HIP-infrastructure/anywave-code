#pragma once
#include <AwGlobal.h>
#include <QWidget>
#include <AwProcessInterface.h>

class AW_PROCESS_EXPORT AwProcessOutputWidget : public QWidget
{
	Q_OBJECT
public:
	explicit AwProcessOutputWidget(AwBaseProcess* process = nullptr, QWidget *parent = nullptr);
signals:
	// copy signals that a process can emit
	void log(const QString& log);
	void sendCommand(int command, QVariantList args);
	void sendCommand(const QVariantMap& );
	void sendMarkers(AwMarkerList* markers);
	void sendMarker(AwMarker* marker);
	void dataConnectionRequested(AwDataClient* client);
protected:
	AwBaseProcess* m_process;
};