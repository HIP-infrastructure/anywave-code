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
#ifndef AWICAMANAGER_H
#define AWICAMANAGER_H

#include <QObject>
#include <AwChannel.h>
#include <QMap>
#include "AwICAChannel.h"
class AwICAComponents;

class AwICAManager : public QObject
{
	Q_OBJECT

public:
	AwICAManager(QObject *parent = 0);
	~AwICAManager();

	static AwICAManager *instance();

	void closeFile();
	int loadComponents(const QString& icaFile);
	AwICAComponents *getComponents(int type);
//	AwICAComponents **getAllComponents();
	AwICAChannelList getChannelsOfAllComponents();
	int numberOfComponents();
	bool reject(int type);
	void rejectComponents(int type, const AwChannelList& channels);
	bool containsComponents(int type);

	inline bool isFilteringActive() { return m_isFilteringOn; }
	void turnICAOff();
	QVector<int> getRejectedComponentsIndexes(int type);
	QMap<int, QVector<int> > getAllRejectedComponents();
	/** Get the labels of all rejected components **/
	QStringList getRejectedLabels();
public slots:
	void setICAFiletring(bool on) { m_isFilteringOn = on;  emit filteringSwitched(on); }
signals:
	void componentsLoaded();
	void filteringSwitched(bool active);
	void icaComponentsUnloaded();
	void componentsFiltersLoaded(float lp, float hp);
private:
	QString convertToMatlab(const QString& fileName);

//	AwICAComponents *m_comps[AW_CHANNEL_TYPES];  
	QMap<int, QSharedPointer<AwICAComponents>> m_componentsMap;
	static AwICAManager *m_instance;

	bool m_isFilteringOn;
};

#endif // AWICAMANAGER_H
