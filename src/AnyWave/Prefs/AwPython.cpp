#include "AwPython.h"
#include <QFile>

AwPythonSettings AwPython::detect(const QString& dir)
{
	AwPythonSettings settings;
	QString activate, python;
#ifdef Q_OS_WIN
	activate = QString("%1\\Scripts\\activate.bat").arg(dir);
	python = QString("%1\\Scripts\\python.exe").arg(dir);
#endif
#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
	activate = QString("%1/bin/activate.sh").arg(dir);
	python = QString("%1/bin/python").arg(dir);
#endif
	if (!QFile::exists(activate)) {
		settings.setValue(python_settings::detected, false);
		return settings;
	}
	if (!QFile::exists(python)) {
		settings.setValue(python_settings::detected, false);
		return settings;
	}
	settings.setValue(python_settings::detected, true);
	settings.setValue(python_settings::activate_path, activate);
	settings.setValue(python_settings::python_path, python);

	return settings;
}