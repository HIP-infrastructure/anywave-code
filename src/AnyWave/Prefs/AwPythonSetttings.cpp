#include "AwPythonSettings.h"


QVariant AwPythonSettings::value(const QString& key)
{
	return m_settings.value(key);
}