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
#pragma once
#include <QObject>
#include <epoch/AwEpochTree.h>
#include "AwEpochVisuWidget.h"
#include "AwEpochAverageWidget.h"

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

	AwEpochTree *createCondition(const QString& name, const AwChannelList& channels, const QString& eventName, float pre, float post);
	void loadEpochFile(const QString& file);
	void saveEpochFile(const QString& file);
	void save();
	
	/** Select conditions on which the averaging will be computed. **/
	inline void selectConditions(QStringList& conditions) { m_selectedConditions = conditions; }
protected:
	void clean();
	void load();

	static AwEpochManager *m_instance;
	QHash<QString, AwEpochTree *> m_hashEpochs;
	float m_totalDuration;	// current file duration in seconds
	AwEpochVisuWidget *m_reviewWidget;
	QList<AwEpochAverageWidget *> m_avgWidgets;
	QString m_dataPath;
	QStringList m_selectedConditions;
};