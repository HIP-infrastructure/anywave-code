#pragma once
#include <QObject>

class AwCommandLogger : public QObject
{
	Q_OBJECT
public:
	AwCommandLogger(const QString& componentName, const QString& logFile = QString("command_line"));
	~AwCommandLogger();

	void sendLog(const QString& message);
signals:
	void log(const QString& message);
};