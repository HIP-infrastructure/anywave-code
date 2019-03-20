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

#include <filter/AwFilterSettings.h>

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

	inline void setSettings(const QString& key, const QVariant& value) { m_settings[key] = value; }
	inline QVariant getSettings(const QString& key) { return m_settings.value(key); }
	inline QString getString(const QString& key) { return m_settings.value(key).toString(); }
	inline bool getBool(const QString& key) { return m_settings.value(key).toBool(); }
	inline int getInt(const QString& key) { return m_settings.value(key).toInt(); }
	inline QStringList getStringList(const QString& key) { return m_settings.value(key).toStringList(); }

#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
	void createMatlabShellScript(const QString& path);
	void emptyMatlabShellScript();
#endif
	void closeFile();

	inline AwFileIO *currentReader() { return m_currentReader; }

	void setReader(AwFileIO *reader, const QString& path);

	inline AwFileInfo *fileInfo() { return m_fileInfo; }
	inline AwDisplaySetup *displaySetup() { return m_setup; }
	inline void setDisplaySetup(AwDisplaySetup *setup) { m_setup = setup; }
	inline QSystemTrayIcon *sysTray() { return m_sysTrayIcon; }
	inline AwMatlabInterface *matlabInterface() { return m_matlabInterface; }
	inline void setMatlabInterface(AwMatlabInterface *i) { m_matlabInterface = i; }
	inline AwFilterSettings& filterSettings() { return m_filterSettings; }
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
	void log(const QString& message);
public slots:
	void setAutoTriggerParsingOn(bool onoff);
	void savePredefinedMarkers(const AwMarkerList& markers);
protected:
	void loadLanguage();

	QMap<QString, QVariant> m_settings;
	
	QList<AwFileIO *> m_readers;
	AwFileInfo *m_fileInfo;
	QStringList m_languages;			// list of available languages
	QString m_language;					// current language
	QStringList m_locales;				// locales strings 

	AwFileIO *m_currentReader;
	AwDisplaySetup *m_setup;
	QSystemTrayIcon *m_sysTrayIcon;
	QTranslator m_anyWaveTranslator;
	QTranslator m_readWriteTranslator;
	QTranslator m_widgetTranslator;
	AwMatlabInterface *m_matlabInterface;
	// unique filter settings object.
	AwFilterSettings m_filterSettings;
private:
	static AwSettings *m_instance;
	
};

#endif // AWSETTINGS_H
