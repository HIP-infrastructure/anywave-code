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
#ifndef AWDISPLAYSETUP_H
#define AWDISPLAYSETUP_H

#include <QObject>
#include <QSharedPointer>
//class AwViewSetup;
class AwViewSettings;

#include <widget/AwGraphicsObjects.h>

class AwDisplaySetup : public QObject
{
	Q_OBJECT

public:
	AwDisplaySetup(QObject *parent = 0);
	~AwDisplaySetup();
	
	enum Orientation { Horizontal, Vertical, Grid };
	enum UpdateFlags { AllFlags = 15, SynchViews = 1, SecsPerCm = 2, ViewNumber = 4, ViewOrientation = 8 };
	inline int orientation() { return m_orientation; }
	inline int numberOfViewSettings() { return m_viewSettings.count(); }
//	inline QList<AwViewSetup *>& viewSetups() { return m_ds; }
	inline QList<AwViewSettings*>& viewSettings() { return m_viewSettings; }
	inline void clearViewSettings() { m_viewSettings.clear(); }
	void removeViewSettings(int index);
	void setOrientation(int orientation);
	inline bool synchronizeViews() { return m_synchronize; }
	void setSynchronized(bool flag);
//	void setSecondsPerCm(float val);
	AwViewSettings* addViewSettings();
public slots:
	bool loadFromFile(const QString& path);
	bool saveToFile(const QString& path);


protected:
//	QString m_setupDir;
	QString m_name;
//	QString m_fullPath;
	int m_viewSetup;
	bool m_synchronize;
	bool m_hasBeenModified;
//	QList<AwViewSetup*> m_ds;
	QList<AwViewSettings*> m_viewSettings;
	int m_orientation;
};

#endif // AWDISPLAYSETUP_H
