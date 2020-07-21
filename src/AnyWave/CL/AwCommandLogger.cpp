#include "CL/AwCommandLogger.h"
#include "Debug/AwDebugLog.h"

AwCommandLogger::AwCommandLogger(const QString& component, const QString& logFile)
{
	AwDebugLog::instance()->connectComponent(component, this, logFile);
}

AwCommandLogger::~AwCommandLogger()
{
	AwDebugLog::instance()->disconnectComponent(this);
}

void AwCommandLogger::sendLog(const QString& message)
{
	emit log(message);
}