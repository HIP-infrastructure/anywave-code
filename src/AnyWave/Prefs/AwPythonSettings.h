#pragma once
#include <QVariant>

namespace python_settings {
	constexpr auto detected = "detected";
	constexpr auto activate_path = "activate_path";
	constexpr auto python_path = "python_path";
	constexpr auto lib_path = "lib_path";
}

class AwPythonSettings
{
public:
	QVariant value(const QString& key);
	inline void setValue(const QString& key, QVariant value) { m_settings[key] = value; }
	inline bool isDetected() { return m_settings.value(python_settings::detected).toBool(); }
protected:
	QVariantMap m_settings;
};