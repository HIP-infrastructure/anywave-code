// AnyWave
// Copyright (C) 2013-2021  INS UMR 1106
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#ifndef AWSETTINGS_H
#define AWSETTINGS_H

#include <QObject>
#include <QSystemTrayIcon>
#include <AwMarker.h>
#include <QVariantMap>

class AwFileIO;
class AwDisplaySetup;
class AwMatlabInterface;

// defining keys for settings dictionnary
namespace aws {
	// path to dirs created by AnyWave in the user documents dir
	constexpr auto batch_dir = "batch_dir";
	constexpr auto montage_dir = "montage_dir";
	constexpr auto marker_rules_dir = "marker_rules_dir";
	constexpr auto plugins_dir = "plugins_dir";
	constexpr auto matlab_plugins_dir = "matlab_plugins_dir";
	constexpr auto python_plugins_dir = "python_plugins_dir";
	constexpr auto work_dir = "work_dir";
	constexpr auto log_dir = "log_dir";
	constexpr auto setup_dir = "setup_dir";
	constexpr auto settings_dir = "settings_dir";
	// common dirs
	constexpr auto home_dir = "home_dir";
	constexpr auto app_dir = "app_dir";
	constexpr auto app_resource_dir = "app_resource_dir";
	constexpr auto app_plugins_dir = "app_plugins_dir";
	constexpr auto app_matlab_plugins_dir = "app_matlab_plugins_dir";
	constexpr auto app_python_plugins_dir = "app_python_plugins_dir";
	constexpr auto matlab_mex_dir = "mex_dir";
	constexpr auto matlab_API_dir = "matlab_API_dir";	// the folder containing APi functions for matlab plugins (Starting with 2104)
	// plugins
	constexpr auto plugin_debug_mode = "plugin_debug";
	constexpr auto server_port = "server_port";
	// general
	constexpr auto gui_active = "gui_active";
	constexpr auto matlab_present = "matlab_present";
	constexpr auto system_path = "system_path";
	constexpr auto recent_files = "recent_files";
	constexpr auto recent_bids = "recent_bids";
	constexpr auto username = "username";
	constexpr auto auto_trigger_parsing = "auto_trigger_parsing";
	constexpr auto markerbar_mode_default = "markerbar_mode_default"; // classic or global mode
	// python
	constexpr auto python_exe = "python_exe";
	constexpr auto python_venv_dir = "python_venv_dir";
	constexpr auto python_detected = "python_detected";
	constexpr auto python_use_default = "python_use_default"; // true if using default system python 
	constexpr auto python_venv_alias = "python_venv_alias"; // name of venv to use
	constexpr auto python_venv_list = "python_venv_list"; 
	constexpr auto python_embeded_venv_dir = "python_embeded_venv_dir";
	// true if running ins version of anywave
	constexpr auto ins_version = "ins_version";
	constexpr auto check_updates = "check_updates";

	constexpr auto update_url = "update_url";
	constexpr auto ica_file = "ica_file";
	constexpr auto last_captured_file = "last_captured_file";
	constexpr auto total_cpu_cores = "total_cpu_cores";
	constexpr auto max_cpu_cores = "max_cpu_cores";
	constexpr auto max_recent_files = "max_recent_files";
	constexpr auto predefined_marker_file = "predefined_marker_file";
	constexpr auto itk_snap = "itk_snap";
	constexpr auto gardel = "gardel";
};

///
/// Global object containing all global settings of AnyWave
///

class AwSettings : public QObject
{
	Q_OBJECT

public:
	AwSettings(QObject *parent = 0);
	~AwSettings();

	static AwSettings * getInstance() {
		if (!m_instance)
			m_instance = new AwSettings();
		return m_instance;
	}

	void init();
	QVariant value(const QString& key);
	void setValue(const QString& key, const QVariant& value);
	
#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
	void createMatlabShellScript(const QString& path);
	void emptyMatlabShellScript();
#endif
	void closeFile();

	void quit();

	inline AwDisplaySetup *displaySetup() { return m_setup; }
	inline void setDisplaySetup(AwDisplaySetup *setup) { m_setup = setup; }
	inline QSystemTrayIcon *sysTray() { return m_sysTrayIcon; }
	inline AwMatlabInterface *matlabInterface() { return m_matlabInterface; }
	inline void setMatlabInterface(AwMatlabInterface *i) { m_matlabInterface = i; }
	AwFileIO* readerAt(int index);
	QStringList& topoLayouts(); 

	// recent files specific
	QString shortenFilePath(const QString& path);
	void addRecentFilePath(const QString& path);
	void addRecentBIDS(const QString& path);
	void removeRecentBIDS(const QString& path);
	void removeRecentFilePath(const QString& path);

	// predefined markers (Marker Inspector Tool)
	AwMarkerList loadPredefinedMarkers();
signals:
	void markersColorChanged(const QStringList& colors);
	void screenCalibrationChanged(float x, float y);
	void recentFilesUpdated(const QStringList&);
	void recentBIDSUpdated(const QStringList&);
	void timeRepresentationChanged(bool HMS); // if HMS is true that means we go for HMS representation
	void log(const QString& message);
public slots:
	void setAutoTriggerParsingOn(bool onoff);
	void savePredefinedMarkers(const AwMarkerList& markers);
protected:
	QVariantMap m_settings;
	QList<AwFileIO *> m_readers;
	AwFileIO *m_currentReader;
	AwDisplaySetup *m_setup;
	QSystemTrayIcon *m_sysTrayIcon;
	AwMatlabInterface *m_matlabInterface;
	QMap<QString, QString> m_pythonVenvs;
private:
	static AwSettings *m_instance;
};

#endif // AWSETTINGS_H
