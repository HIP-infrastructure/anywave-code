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
#pragma once

#include <QObject>
#include <AwChannel.h>
#include <filter/AwFilterSettings.h>
#include <AwKeys.h>
#include <AwFileIO.h>
#include <AwChannel.h>
#include <QMutex>
#include <thread>
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
	friend class AwBIDSManager;
	~AwDataManager();

	enum Status { NoError = AwFileIO::NoError, FormatError = AwFileIO::WrongFormat, HeaderError = AwFileIO::BadHeader, 
		FileAccessError = AwFileIO::FileAccess, NoPluginFound };

	inline QVariant value(const QString& key) { return m_settings.value(key); }
	void quit();
	void closeFile();
	const AwChannelList& selectedChannels();
	const AwChannelList& montage();
	const AwChannelList& rawChannels();
	inline AwFileIO* reader() { return m_reader; }
	inline AwFilterSettings& filterSettings() { return m_filterSettings; }
	inline AwMontageManager* montageManager() { return m_montageManager; }
	inline AwMarkerManager* markerManager() { return m_markerManager; }
	inline AwDataServer* dataServer() { return m_dataServer; }
	void applyFilters(const AwChannelList& channels);
	// convenience methods
	inline bool isFileOpen() { return m_reader != nullptr; }
	inline QString dataDir() { return m_settings.value(keys::data_dir).toString(); }
	inline float totalDuration() { return m_settings.value(keys::file_duration).toFloat(); }
	inline QString dataFilePath() { return m_settings.value(keys::data_path).toString(); }
	inline bool canWriteTriggers() { return m_settings.value(keys::can_write_triggers).toBool(); }
	inline QString selFilePath() { return m_settings.value(keys::sel_file).toString(); }
	inline QString levelFilePath() { return m_settings.value(keys::lvl_file).toString(); }
	inline QString lvlFilePath() { return m_settings.value(keys::lvl2_file).toString(); }
	inline QString badFilePath() { return m_settings.value(keys::bad_file).toString(); }
	inline QString fltFilePath() { return m_settings.value(keys::flt_file).toString(); }
	inline QString mtgFilePath() { return m_settings.value(keys::montage_file).toString(); }
	inline QString mrkFilePath() { return m_settings.value(keys::marker_file).toString(); }
	inline QString dispFilePath() { return m_settings.value(keys::disp_file).toString(); }
	inline QString currentMontageDir() { return m_settings.value(keys::current_montage_dir).toString(); }
	QString bidsDir();
	inline QVariantMap& settings() { return m_settings; }
	inline int status() { return m_status; }

	inline QString& errorString() { return m_errorString; }

	void setNewRootDirForSideFiles(const QString& dir);

signals:
	void finished();  // used for threading operations (only when opening file in GUI mode at present)
public slots:
	// selectChannels will generate a list of channels to be used when requested data.
	// it allows to select the channels to load depending on a list of key/values set in a variant map.
	void selectChannelsAsynch(AwDataClient *client, const QVariantMap& settings, AwChannelList *channels);
	// convenience version of selectChannel that handle a json string as settings.
	void selectChannelsAsynch(AwDataClient* client, const QString& settings, AwChannelList* channels);
	// synchronous version
	void selectChannels(AwDataClient* client, const QVariantMap& settings, AwChannelList* channels);
	void selectChannels(AwDataClient* client, const QString& settings, AwChannelList* channels);

	QStringList badLabels();
	int openFile(const QString& filePath = QString(), bool commandLineMode = false);
protected:
	AwDataManager();

	AwFileIO* m_reader;
	AwFilterSettings m_filterSettings;
	QVariantMap m_settings;
	AwChannelList m_tmp;
	int m_status;	// code returned after a call to openFile
	static AwDataManager* m_instance;
	AwMontageManager* m_montageManager;
	AwMarkerManager* m_markerManager;
	AwDataServer* m_dataServer;
	QString m_errorString;
	QMutex m_mutex;
};