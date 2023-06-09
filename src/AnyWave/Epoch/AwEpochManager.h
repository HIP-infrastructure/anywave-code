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
#include <epoch/AwEpochTree.h>
#include "AwEpochVisuWidget.h"
#include "AwEpochAverageWidget.h"
#include <filter/AwFilterSettings.h>
#include "Data/AwDataServer.h"

class AwEpochManager : public QObject
{
	Q_OBJECT
public:
	AwEpochManager();
	~AwEpochManager();

	QList<AwEpochTree *> conditions() { return m_hashEpochs.values(); }
	AwEpochTree *getCondition(const QString& name);
	static AwEpochManager *instance();
	static bool instanceExists() { return m_instance != NULL; }
	static void destroy();
	/** Open the create/edit conditions UI **/
	void create();
	/** Open the Review conditions UI **/
	void visualise();
	/** Open the averaging UI **/
	void average();
	void closeFile();

	AwEpochTree *createCondition(const QString& name, const AwChannelList& channels, 
		const QString& eventName, float pre, float post, const QString& artefact = QString());
	void loadEpochFile(const QString& file);
	void saveEpochFile(const QString& file);
	void save();
	/** Select conditions on which the averaging will be computed. **/
	inline void selectConditions(QStringList& conditions) { m_selectedConditions = conditions; }
protected:
	void clean();
	void load();
	AwFileIO *offlineFiltering(const AwChannelList& channels);

	static AwEpochManager *m_instance;
	QHash<QString, AwEpochTree *> m_hashEpochs;
	float m_totalDuration;	// current file duration in seconds
	AwEpochVisuWidget *m_reviewWidget;
	QList<AwEpochAverageWidget *> m_avgWidgets;
	QString m_dataPath;
	QStringList m_selectedConditions;
	AwFilterSettings m_filterSettings;
	AwMarkerList m_artefacts;
	AwDataServer *m_offlineDataServer;
};

/////////////////////////////////////////////////////////////////////
/// worker object to do the offline filtering
//

class OfflineFilterWorker : public QObject
{
	Q_OBJECT
public:
	OfflineFilterWorker(const AwChannelList& channels, const AwMarkerList& artefacts);
	~OfflineFilterWorker();
	AwFileIO *memIOReader() { return m_reader; }
public slots:
	void run();
signals:
	void finished();
protected:
	AwChannelList m_channels;
	AwMarkerList m_artefacts;
	AwFileIO *m_reader;	 // generated by run()
};
