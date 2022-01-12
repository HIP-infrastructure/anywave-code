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
#include "AwDisplaySetupManager.h"
#include "Prefs/AwSettings.h"
#include "Widgets/AwMarkersBar.h"
#include "AwSaveAsNewSetupDialog.h"
#include <QSystemTrayIcon>
#include <QMessageBox>
#include "AwDisplaySetup.h"
#include "AwViewSetup.h"
#include <QDir>
#include <QSettings>
#include "Widgets/AwDisplaySetupToolBar.h"
#include "AwDSMSaveSetupUI.h"
#include "Data/AwDataManager.h"

////////////////////////////////////////////////////////////////////////////////////////////
// Static
////////////////////////////////////////////////////////////////////////////////////////////
AwDisplaySetupManager* AwDisplaySetupManager::m_instance = nullptr;

AwDisplaySetupManager* AwDisplaySetupManager::instance()
{
	if (!m_instance)
		m_instance = new AwDisplaySetupManager();
	return m_instance;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


AwDisplaySetupManager::AwDisplaySetupManager(QObject *parent)
	: QObject(parent)
{
	m_currentSetup = NULL;
	// always start with Default Setup
	resetToDefault();
}

AwDisplaySetupManager::~AwDisplaySetupManager()
{

}


QToolBar *AwDisplaySetupManager::toolBar() { return m_toolBar->toolBar(); }



void AwDisplaySetupManager::init()
{
	m_setupPath = AwDataManager::instance()->dispFilePath();

	if (QFile::exists(m_setupPath)) 
		loadSetup(m_setupPath);
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// SLOTS
/////////////////////////////////////////////////////////////////////////////////////////////////

void AwDisplaySetupManager::resetToDefault()
{
	QSharedPointer<AwDisplaySetup> setup = QSharedPointer<AwDisplaySetup>(new AwDisplaySetup("Default Setup"));
	setup->setToDefault();
	emit newSetupSelected(setup.get());
	m_currentSetup = setup;
}

void AwDisplaySetupManager::saveSettings()
{
	if (m_setupPath.isEmpty())
		return;
	m_currentSetup->saveToFile(m_setupPath);
}

void AwDisplaySetupManager::updateSetup(AwDisplaySetup *setup, int flags)
{
	if (m_setupPath.isEmpty())
		return;

	// save setup in File setup
	QSystemTrayIcon *sysTray = AwSettings::getInstance()->sysTray();
	sysTray->show();
	m_currentSetup->saveToFile(m_setupPath);

	if (flags &  AwDisplaySetup::SynchViews)
		if (!m_currentSetup->synchronizeViews())
			sysTray->showMessage(tr("Signal Views"), tr("Signal views have been desynchronized."));
}

void AwDisplaySetupManager::loadSetup(const QString& path)
{
	QSharedPointer<AwDisplaySetup> newSetup = QSharedPointer<AwDisplaySetup>(new AwDisplaySetup());
	if (!newSetup->loadFromFile(path)) // Failed to load the setup
		newSetup->setToDefault();

	emit newSetupSelected(newSetup.get());
	m_currentSetup = newSetup;
}