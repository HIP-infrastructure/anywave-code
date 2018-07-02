/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef AWSETTINGS_H
#define AWSETTINGS_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QTranslator>
#include <AwMarker.h>
#include <AwFileInfo.h>

class AwFileIO;
class AwDisplaySetup;
class AwMatlabInterface;

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

	inline void setHomeDirectory(const QString& dir) { m_homeDirectory = dir; }
	inline void setSystemPath(const QString& path) { m_systemPath = path; }
	inline void setMATLABMexPath(const QString& path) { m_mexPath = path; }
	inline void setPythonModulePath(const QString& path) { m_pythonModulePath = path; }

#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
	void createMatlabShellScript(const QString& path);
	void emptyMatlabShellScript();
#endif
	void closeFile();

	// User folders paths
	QString montageDir;
	QString pluginDir;
	QString matlabPluginDir;
	QString pythonPluginDir;
	QString markerRulesDir;
	QString logDir;
	QString displaySetupDir;
	QString workingDir;
	QString currentIcaFile;	// hold the path of the current ICA matrix (empty if no ICA is loaded)
	// capture file
	QString lastCaptureFile;
	// url for update
	QString updateUrl;
	// the maximum of cores available and the maximum of core to use with AnyWave.
	int totalCPUCores, maxCPUCores;

	inline QString homeDirectory() { return m_homeDirectory; }
	inline QString MATLABMexPath() { return m_mexPath; }
	inline QString PythonModulePath() { return m_pythonModulePath; }

	inline QString systemPath() { return m_systemPath; }

	inline AwFileIO *currentReader() { return m_currentReader; }

	void setReader(AwFileIO *reader, const QString& path);

	inline AwFileInfo *fileInfo() { return m_fileInfo; }
	inline AwDisplaySetup *displaySetup() { return m_setup; }
	inline void setDisplaySetup(AwDisplaySetup *setup) { m_setup = setup; }
	inline QSystemTrayIcon *sysTray() { return m_sysTrayIcon; }
	inline void setMatlabPresent(bool flag = true) { m_isMatlabPresent = flag; }
	inline bool isMatlabPresent() { return m_isMatlabPresent; }
	inline bool isAutoTriggerParsingOn() { return m_isAutoTriggerParsingOn; }
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
	inline QStringList& recentFiles() { return m_recentFiles; }
	inline QStringList& recentBIDS() { return m_recentBIDS; }

	// predefined markers (Marker Inspector Tool)
	AwMarkerList loadPredefinedMarkers();

	// language specific
	/** Load the language chosen by the user or the default locale matching one **/
	QString& language() { return m_language;}
	void loadLanguage(const QString& lang);
	void switchTranslator(QTranslator& translator, const QString& file);
	QTranslator translator;			// AnyWave
	QTranslator translatorQt;		// Qt
	QTranslator translatorGraphics;	// Graphics Lib
	QTranslator translatorWidgets;	// Widgets Lib
	QString langPath;				// path to the folder containing the .qm files
signals:
	void markersColorChanged(const QStringList& colors);
	void screenCalibrationChanged(float x, float y);
	void recentFilesUpdated(const QStringList&);
	void recentBIDSUpdated(const QStringList&);
	void timeRepresentationChanged(bool HMS); // if HMS is true that means we go for HMS representation
public slots:
	void setAutoTriggerParsingOn(bool onoff);
	void savePredefinedMarkers(const AwMarkerList& markers);
protected:
	void loadLanguage();
	
	bool m_isMatlabPresent;
	bool m_isAutoTriggerParsingOn;
	QList<AwFileIO *> m_readers;
	AwFileInfo *m_fileInfo;
	QStringList m_languages;			// list of available languages
	QString m_language;					// current language
	QStringList m_locales;				// locales strings 
	QString m_homeDirectory;
	QString m_mexPath, m_pythonModulePath;

	AwFileIO *m_currentReader;
	AwDisplaySetup *m_setup;
	QSystemTrayIcon *m_sysTrayIcon;
	QStringList m_recentFiles, m_recentBIDS;
	qint32 m_recentFilesMax;
	QString m_pdfMarkerFile;			// Predefined markers file path
	QTranslator m_anyWaveTranslator;
	QTranslator m_readWriteTranslator;
	QTranslator m_widgetTranslator;
	AwMatlabInterface *m_matlabInterface;
	QString m_systemPath;
	// topo layouts for the current opened file
	QStringList m_topoLayouts;
private:
	static AwSettings *m_instance;
	
};

#endif // AWSETTINGS_H
