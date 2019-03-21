/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
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
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "Prefs/AwSettings.h"
#include <QSettings>
#include <QWidget>
#include <AwFileIO.h>
#include <QDir>
#include <QApplication>
#include <qthreadpool.h>
#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
#include <QTextStream>
#endif
AwSettings *AwSettings::m_instance = 0;

#define LINE_PX	200

AwSettings::AwSettings(QObject *parent)
	: QObject(parent)
{
	m_sysTrayIcon = new QSystemTrayIcon(this);
	m_sysTrayIcon->setIcon(QIcon(":images/AnyWave_icon.png"));
//	m_recentFilesMax = 15;
	m_settings["recentFilesMax"] = (int)15;
	m_currentReader = NULL;

	// load previously saved recent files
	QSettings settings;

	// load recent files
	int size = settings.beginReadArray("recentFiles");
	QStringList recentFiles, recentBIDS;
	for (int i = 0; i < size; i++)	{
		settings.setArrayIndex(i);
		recentFiles << settings.value("filePath").toString();
	}
	settings.endArray();
	// load recend BIDS
	size = settings.beginReadArray("recentBIDS");
	for (int i = 0; i < size; i++) {
		settings.setArrayIndex(i);
		recentBIDS << settings.value("BIDSPath").toString();
	}
	settings.endArray();

	m_settings["recentFiles"] = recentFiles;
	m_settings["recentBIDS"] = recentBIDS;
	m_settings["isMatlabPresent"] = false;

	auto isAutoTriggerParsingOn = settings.value("Preferences/autoTriggerParsing", true).toBool();
	m_settings["isAutoTriggerParsingOn"] = isAutoTriggerParsingOn;

	//// languages
	loadLanguage();
	// Cpu cores
	auto totalCPUCores = QThreadPool::globalInstance()->maxThreadCount();
	m_settings["totalCPUCores"] = totalCPUCores;
	auto maxCPUCores =  settings.value("general/cpu_cores", totalCPUCores).toInt();
	m_settings["maxCPUCores"] = maxCPUCores;

	bool checkForUpdates = settings.value("general/checkForUpdates", true).toBool();
	m_settings["checkForUpdates"] = checkForUpdates;

	m_matlabInterface = NULL;
	m_settings["predefinedMarkerFile"] = QString("marker_tool.mrk");

	m_fileInfo = Q_NULLPTR;

	auto appPath = QCoreApplication::applicationDirPath();
	m_settings["appDirPath"] = appPath;
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
	m_settings["appResourcePath"] = appPath;
#endif
#ifdef Q_OS_MAC
	m_settings["appResourcePath"] = QString("%1/../Resources").arg(appPath);
#endif

	// check for a version.txt in resources
	QString versionFile = QString("%1/version.txt").arg(m_settings["appResourcePath"].toString());
	if (QFile::exists(versionFile)) {
		QFile file(versionFile);
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			m_settings["majorVersion"] =  file.readLine();
			m_settings["minorVersion"] =  file.readLine();
			file.close();
		}
	}
}

AwSettings::~AwSettings()
{
	// save recent files
	QSettings settings;
	settings.beginWriteArray("recentFiles");
	auto recentFiles = m_settings["recentFiles"].toStringList();
	for (int i = 0; i < recentFiles.size(); i++)	{
		settings.setArrayIndex(i);
		settings.setValue("filePath", recentFiles.at(i));
	}
	settings.endArray();
	// save recent BIDS
	auto recentBIDS = m_settings["recentBIDS"].toStringList();
	settings.beginWriteArray("recentBIDS");
	for (int i = 0; i < recentBIDS.size(); i++) {
		settings.setArrayIndex(i);
		settings.setValue("BIDSPath", recentBIDS.at(i));
	}
	settings.endArray();
	if (m_fileInfo)
		delete m_fileInfo;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// LANGUAGE

void AwSettings::loadLanguage()
{
	QSettings settings;
	m_language = settings.value("general/locale", QString("en")).toString();
	QLocale locale = QLocale(m_language);
	QLocale::setDefault(locale);
	// create language path
	langPath = QApplication::applicationDirPath();
	langPath.append("/languages");
	QString languageName = QLocale::languageToString(locale.language());
	QString anywaveFile = langPath + "/" + QString("anywave_%1.qm").arg(m_language);
	QString qtFile = langPath + "/" + QString("qt_%1.qm").arg(m_language);
	QString graphicsFile = langPath + "/" + QString("awgraphicslib_%1.qm").arg(m_language);
	QString widgetsFile = langPath + "/" + QString("awwidgetslib_%1.qm").arg(m_language);
	// load translators
	if (translator.load(anywaveFile))
		qApp->installTranslator(&translator);
	if (translatorQt.load(qtFile))
		qApp->installTranslator(&translatorQt);
	if (translatorGraphics.load(graphicsFile))
		qApp->installTranslator(&translatorGraphics);
	if (translatorWidgets.load(widgetsFile))
		qApp->installTranslator(&translatorWidgets);
}

void AwSettings::loadLanguage(const QString& lang)
{
	if (m_language != lang) {
		QSettings settings;
		m_language = lang;
		QLocale locale = QLocale(m_language);
		QLocale::setDefault(locale);
		switchTranslator(translator, QString("anywave_%1.qm").arg(m_language));
		switchTranslator(translatorQt, QString("qt_%1.qm").arg(m_language));
		switchTranslator(translatorGraphics, QString("awgraphicslib_%1.qm").arg(m_language));
		switchTranslator(translatorWidgets, QString("awwidgetslib_%1.qm").arg(m_language));
		settings.setValue("general/locale", lang);
	}
}

void AwSettings::switchTranslator(QTranslator& translator, const QString& file)
{
	// remove the old translator
	qApp->removeTranslator(&translator);
	// load the new one and save it to preferences
	if (translator.load(file)) 
		qApp->installTranslator(&translator);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// LANGUAGE END

void AwSettings::closeFile()
{ 
//	currentIcaFile.clear(); 
	setSettings("currentIcaFile", QString());
	if (m_fileInfo)
		m_filterSettings.save(QString("%1.flt").arg(m_fileInfo->filePath()));
}



#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)

void AwSettings::createMatlabShellScript(const QString& path)
{
	QString scriptPath = QString("%1/AnyWave/matlab.sh").arg(m_settings["homeDir"].toString());
	QFile scriptFile(scriptPath);
	QTextStream stream(&scriptFile);

	if (scriptFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		stream << "#!/bin/bash" << endl;
		stream << "MATLAB=" << path << endl;
#ifdef Q_OS_MAC
		stream << "DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$MATLAB/bin/maci64" << endl;
		stream << "export DYLD_LIBRARY_PATH" << endl;
#elif defined(Q_OS_LINUX)
		stream << "LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/AnyWave/lib:$MATLAB/bin/glnxa64" << endl;
		stream << "export LD_LIBRARY_PATH" << endl;
#endif
		scriptFile.close();
		emit log(QString("Sucessfully created file %1.").arg(scriptPath));
	}
	else
		emit log(QString("Could not create %1 file").arg(scriptPath));
}

#endif

AwFileIO* AwSettings::readerAt(int index)
{
	if (m_readers.isEmpty())
		return NULL;

	if (index <= m_readers.size() && index >= 0)
		return m_readers.at(index);
	else
		return NULL;
}

void AwSettings::setReader(AwFileIO *reader, const QString& path)
{
	m_currentReader = reader;
	if (m_fileInfo) 
		delete m_fileInfo;
	m_fileInfo = new AwFileInfo(reader, path);
}


QString AwSettings::shortenFilePath(const QString& path)
{
	const int maxLength = 60;
	QString res;

	if (path.length() <= maxLength)
		res = path;
	else
		res = path.left(10) + "..." + path.right(47);
	return res;
}

void AwSettings::addRecentFilePath(const QString& path)
{
	auto recentFiles = m_settings["recentFiles"].toStringList();

	if (recentFiles.contains(path))
		return;
	
	if (recentFiles.size() == m_settings["recentFilesMax"].toInt())
		recentFiles.removeLast();

	recentFiles.insert(0, path);
	QStringList result;
	for (auto s : recentFiles)
		result << shortenFilePath(s);
	m_settings["recentFiles"] = recentFiles;

	emit recentFilesUpdated(result);
}

void AwSettings::addRecentBIDS(const QString& path)
{
	auto recentBIDS = m_settings["recentBIDS"].toStringList();
	if (recentBIDS.contains(path))
		return;
	if (recentBIDS.size() == m_settings["recentFilesMax"].toInt())
		recentBIDS.removeLast();

	recentBIDS.insert(0, path);
	QStringList result;
	for (auto s : recentBIDS)
		result << shortenFilePath(s);
	m_settings["recentBIDS"] = recentBIDS;
	emit recentBIDSUpdated(result);
}

void AwSettings::removeRecentFilePath(const QString& path)
{
	auto recentFiles = m_settings["recentFiles"].toStringList();
	if (!recentFiles.contains(path))
		return;

	recentFiles.removeAll(path);
	QStringList result;
	foreach (QString s, recentFiles)
		result << shortenFilePath(s);
	m_settings["recentFiles"] = recentFiles;
	emit recentFilesUpdated(result);
}

void AwSettings::removeRecentBIDS(const QString& path)
{
	auto recentBIDS = m_settings["recentBIDS"].toStringList();
	if (!recentBIDS.contains(path))
		return;

	recentBIDS.removeAll(path);
	QStringList result;
	for (auto s : recentBIDS)
		result << shortenFilePath(s);
	m_settings["recentBIDS"] = recentBIDS;
	emit recentBIDSUpdated(result);
}

void AwSettings::setAutoTriggerParsingOn(bool onoff)
{
	QSettings settings;
	settings.setValue("Preferences/autoTriggerParsing", onoff);
}

QStringList& AwSettings::topoLayouts()
{ 
	return m_currentReader->plugin()->layouts; 
}

//// Predefined Markers (Marker Inspector Tool)

void AwSettings::savePredefinedMarkers(const AwMarkerList& markers)
{
	auto markerRulesDir = m_settings["markerRulesDir"].toString();
	auto file = m_settings["predefinedMarkerFile"].toString();
	if (markerRulesDir.isEmpty())
		return;
	if (markers.isEmpty()) {
		QFile::remove(file);
		return;
	}
	AwMarker::save(file, markers);
}

AwMarkerList AwSettings::loadPredefinedMarkers()
{
	auto markerRulesDir = m_settings["markerRulesDir"].toString();
	auto file = m_settings["predefinedMarkerFile"].toString();
	if (markerRulesDir.isEmpty())
		return AwMarkerList();
	return AwMarker::load(file);
}
