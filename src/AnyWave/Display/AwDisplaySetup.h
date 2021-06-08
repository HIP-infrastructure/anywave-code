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

class AwViewSetup;

class AwDisplaySetup : public QObject
{
	Q_OBJECT

public:
	AwDisplaySetup(const QString& name = QString(), QObject *parent = 0);
	// copy constructor
	AwDisplaySetup(AwDisplaySetup *source, QObject *parent = 0);
	~AwDisplaySetup();
	
	inline QString& name() { return m_name; }
	inline QString& path() { return m_fullPath; }
	AwViewSetup *newViewSetup();
	void setToDefault();
	enum Orientation { Horizontal, Vertical, Grid };
	enum UpdateFlags { AllFlags = 15, SynchViews = 1, SecsPerCm = 2, ViewNumber = 4, ViewOrientation = 8 };
	inline int orientation() { return m_orientation; }
	inline AwViewSetup* viewSetup(int index) { return m_ds.at(index); }
	inline QList<AwViewSetup *>& viewSetups() { return m_ds; }
	void setOrientation(int orientation);
	void deleteViewSetup(int index);
	void setName(const QString& name);
	inline void setModified() { m_hasBeenModified = true; }
	inline bool synchronizeViews() { return m_synchronize; }
	void setSynchronized(bool flag);
	void setSecondsPerCm(float val);
public slots:
//	bool load();
	bool loadFromFile(const QString& path);
//	bool save();
	bool saveToFile(const QString& path);


protected:
	QString m_setupDir;
	QString m_name;
	QString m_fullPath;
	int m_viewSetup;
	bool m_synchronize;
	bool m_hasBeenModified;
	QList<AwViewSetup *> m_ds;
	int m_orientation;
};

#endif // AWDISPLAYSETUP_H
