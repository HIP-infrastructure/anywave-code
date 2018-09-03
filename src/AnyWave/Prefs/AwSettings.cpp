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
	m_recentFilesMax = 10;
	m_currentReader = NULL;

	// load previously saved recent files
	QSettings settings;

	// load recent files
	int size = settings.beginReadArray("recentFiles");
	for (int i = 0; i < size; i++)	{
		settings.setArrayIndex(i);
		m_recentFiles << settings.value("filePath").toString();
	}
	settings.endArray();
	// load recend BIDS
	size = settings.beginReadArray("recentBIDS");
	for (int i = 0; i < size; i++) {
		settings.setArrayIndex(i);
		m_recentBIDS << settings.value("BIDSPath").toString();
	}
	settings.endArray();

	m_isMatlabPresent = false;
	m_isAutoTriggerParsingOn = settings.value("Preferences/autoTriggerParsing", true).toBool();

	//// languages
	loadLanguage();
	// Cpu cores
	totalCPUCores = QThreadPool::globalInstance()->maxThreadCount();
	maxCPUCores =  settings.value("general/cpu_cores", totalCPUCores).toInt();
	
	m_matlabInterface = NULL;
	m_pdfMarkerFile = "marker_tool.mrk";
	m_fileInfo = Q_NULLPTR;
}

AwSettings::~AwSettings()
{
	// save recent files
	QSettings settings;
	settings.beginWriteArray("recentFiles");
	for (int i = 0; i < m_recentFiles.size(); i++)	{
		settings.setArrayIndex(i);
		settings.setValue("filePath", m_recentFiles.at(i));
	}
	settings.endArray();
	// save recent BIDS
	settings.beginWriteArray("recentBIDS");
	for (int i = 0; i < m_recentBIDS.size(); i++) {
		settings.setArrayIndex(i);
		settings.setValue("BIDSPath", m_recentBIDS.at(i));
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
	currentIcaFile.clear(); 
	if (m_fileInfo)
		m_filterSettings.save(QString("%1.flt").arg(m_fileInfo->filePath()));
}



#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)

void AwSettings::createMatlabShellScript(const QString& path)
{
	QFile scriptFile(QDir::homePath() + "/AnyWave/matlab.sh");
	QTextStream stream(&scriptFile);

	if (scriptFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
        stream << "#!/bin/bash" << endl;
		stream << "MATLAB=" + path << endl;
#ifdef Q_OS_MAC
        stream << "DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$MATLAB/bin/maci64" << endl;
		stream << "export DYLD_LIBRARY_PATH" << endl;
#elif defined(Q_OS_LINUX)
        stream << "LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MATLAB/bin/glnxa64:$MATLAB/sys/os/glnxa64" << endl;
		stream << "export LD_LIBRARY_PATH" << endl;
#endif
		scriptFile.close();
	}
}

void AwSettings::emptyMatlabShellScript()
{
	QFile scriptFile(QDir::homePath() + "/AnyWave/matlab.sh");
	QTextStream stream(&scriptFile);
	if (scriptFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		stream << "#!/bin/sh" << endl;
		scriptFile.close();
	}
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
	if (m_recentFiles.contains(path))
		return;
	
	if (m_recentFiles.size() == m_recentFilesMax)
		m_recentFiles.removeLast();

	m_recentFiles.insert(0, path);
	QStringList result;
	for (auto s : m_recentFiles)
		result << shortenFilePath(s);

	emit recentFilesUpdated(result);
}

void AwSettings::addRecentBIDS(const QString& path)
{
	if (m_recentBIDS.contains(path))
		return;
	if (m_recentBIDS.size() == m_recentFilesMax)
		m_recentBIDS.removeLast();

	m_recentBIDS.insert(0, path);
	QStringList result;
	for (auto s : m_recentBIDS)
		result << shortenFilePath(s);

	emit recentBIDSUpdated(result);
}

void AwSettings::removeRecentFilePath(const QString& path)
{
	if (!m_recentFiles.contains(path))
		return;

	m_recentFiles.removeAll(path);
	QStringList result;
	foreach (QString s, m_recentFiles)
		result << shortenFilePath(s);
	emit recentFilesUpdated(result);
}

void AwSettings::removeRecentBIDS(const QString& path)
{
	if (!m_recentBIDS.contains(path))
		return;

	m_recentBIDS.removeAll(path);
	QStringList result;
	for (auto s : m_recentBIDS)
		result << shortenFilePath(s);
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
	if (markerRulesDir.isEmpty())
		return;
	if (markers.isEmpty()) {
		QFile::remove(m_pdfMarkerFile);
		return;
	}
	AwMarker::save(m_pdfMarkerFile, markers);
}

AwMarkerList AwSettings::loadPredefinedMarkers()
{
	if (markerRulesDir.isEmpty())
		return AwMarkerList();
	return AwMarker::load(m_pdfMarkerFile);
}
