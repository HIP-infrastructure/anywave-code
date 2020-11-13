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
#pragma once

#include <QObject>
#include <AwChannel.h>
#include <filter/AwFilterSettings.h>
#include <AwKeys.h>
#include <AwFileIO.h>
#include <AwChannel.h>
#include <QMutex>
class AwMontageManager;
class AwMarkerManager;
class AwDataServer;
class AwDataClient;

class AwDataManager : public QObject
{
	Q_OBJECT
		
public:
	static AwDataManager* instance();
	static AwDataManager* newInstance();

	enum Status { NoError = AwFileIO::NoError, FormatError = AwFileIO::WrongFormat, HeaderError = AwFileIO::BadHeader, 
		FileAccessError = AwFileIO::FileAccess, NoPluginFound };

	inline QVariant value(const QString& key) { return m_settings.value(key); }
	void closeFile();
	const AwChannelList& selectedChannels();
	const AwChannelList& montage();
	const AwChannelList& rawChannels();
	inline AwFileIO* reader() { return m_reader; }
	inline AwFilterSettings& filterSettings() { return m_filterSettings; }
	inline AwMontageManager* montageManager() { return m_montageManager; }
	inline AwMarkerManager* markerManager() { return m_markerManager; }
	inline AwDataServer* dataServer() { return m_dataServer; }
	// convenience methods
	inline bool isFileOpen() { return m_reader != nullptr; }
	inline QString dataDir() { return m_settings.value(keys::data_dir).toString(); }
	inline float totalDuration() { return m_settings.value(keys::file_duration).toFloat(); }
	inline QString dataFilePath() { return m_settings.value(keys::data_path).toString(); }
	inline bool canWriteTriggers() { return m_settings.value(keys::can_write_triggers).toBool(); }
	inline QString selFilePath() { return m_settings.value(keys::sel_file).toString(); }
	inline QString badFilePath() { return m_settings.value(keys::bad_file).toString(); }
	inline QString fltFilePath() { return m_settings.value(keys::flt_file).toString(); }
	inline QString mtgFilePath() { return m_settings.value(keys::montage_file).toString(); }
	inline QString mrkFilePath() { return m_settings.value(keys::marker_file).toString(); }
	inline QVariantMap& settings() { return m_settings; }

public slots:
	// selectChannels will generate a list of channels to be used when requested data.
	// it allows to select the channels to load depending on a list of key/values set in a variant map.
	void selectChannels(AwDataClient *client, const QVariantMap& settings, AwChannelList *channels);
	// convenience version of selectChannel that handle a json string as settings.
	void selectChannels(AwDataClient* client, const QString& settings, AwChannelList* channels);

	QStringList badLabels();
	int openFile(const QString& filePath = QString());
protected:
	AwDataManager();

	AwFileIO* m_reader;
	AwFilterSettings m_filterSettings;
	QVariantMap m_settings;
	AwChannelList m_tmp;
	static AwDataManager* m_instance;
	AwMontageManager* m_montageManager;
	AwMarkerManager* m_markerManager;
	AwDataServer* m_dataServer;
	QMutex m_mutex;
};