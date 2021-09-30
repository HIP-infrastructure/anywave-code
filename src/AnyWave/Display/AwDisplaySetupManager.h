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
#ifndef AWDISPLAYSETUPMANAGER_H
#define AWDISPLAYSETUPMANAGER_H

#include <QObject>
#include <QStringList>
#include <QToolBar>

class AwDisplaySetup;
class AwViewSetup;
class AwDisplaySetupToolBar;


class AwDisplaySetupManager : public QObject
{
	Q_OBJECT

public:
	AwDisplaySetupManager(QObject *parent = 0);
	~AwDisplaySetupManager();

	static AwDisplaySetupManager* instance();
	inline AwDisplaySetup* currentSetup() { return m_currentSetup; }
	inline QStringList& loadedSetups() { return m_loadedSetups; }
	QToolBar *toolBar();
	void  init(); //called when a file is open
public slots:
	/** change Display Setup Path (should be called only once when AnyWave starts **/
	void changeSetupDir(const QString& path) { m_setupDir = path; }
	void saveSettings();
	void updateSetup(AwDisplaySetup *view, int flags);
	/** Delete the current setup **/
	void deleteCurrentSetup();
	/** Import a setup configuration from file **/
	void loadSetup(const QString& path);
	/** Reset to default setup **/
	void resetToDefault();
signals:
	void newSetupSelected(AwDisplaySetup *setup);
	void setupChanged(AwDisplaySetup *setup, int flags);
private:
	QString m_setupDir;
	QString m_defaultSetupPath;
	static AwDisplaySetupManager *m_instance;
	AwDisplaySetup *m_currentSetup;
	QStringList m_loadedSetups;
	AwDisplaySetupToolBar *m_toolBar;
	QString m_setupPath;	// path to autosaved setup
};

#endif // AWDISPLAYSETUPMANAGER_H
