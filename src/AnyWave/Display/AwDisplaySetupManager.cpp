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

////////////////////////////////////////////////////////////////////////////////////////////
// Static
////////////////////////////////////////////////////////////////////////////////////////////
AwDisplaySetupManager* AwDisplaySetupManager::m_instance = NULL;

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
	m_setupDir = AwSettings::getInstance()->value(aws::setup_dir).toString();
	m_currentSetup = NULL;
	if (!m_setupDir.isEmpty())	{
		// check for the existence of Default Setup
		if (QFile::exists(m_setupDir + "Default Setup.aws"))
			QFile::remove(m_setupDir + "Default Setup.aws");
		// parse setup directory for saved setups
		QStringList filter("*.aws");
		QDir dir(m_setupDir);

		dir.setNameFilters(filter);
		QStringList files = dir.entryList();

		// parse files to fill the hash table
		for (int i = 0; i < files.count(); i++)	{
			QString key = files[i];
			key = key.remove(".aws", Qt::CaseInsensitive);
			m_loadedSetups << key;
		}
	}

	// instantiate tool bar and make connections
	m_toolBar = new AwDisplaySetupToolBar(m_loadedSetups);
	// always add a Default Setup
	m_toolBar->addSetup("Default Setup");
	// set Default Setup as current active Setup
	m_toolBar->setCurrentSetup("Default Setup");
	connect(m_toolBar, SIGNAL(setupChanged(const QString&)), this, SLOT(changeSetup(const QString&)));
	connect(m_toolBar, SIGNAL(currentSetupSaveRequest()), this, SLOT(saveCurrentSetup()));
	connect(m_toolBar, SIGNAL(currentSetupWillBeDeteted()), this, SLOT(deleteCurrentSetup()));

	// always start with Default Setup
	resetToDefault();
}

AwDisplaySetupManager::~AwDisplaySetupManager()
{
	delete m_currentSetup;
}


QToolBar *AwDisplaySetupManager::toolBar() { return m_toolBar->toolBar(); }


void AwDisplaySetupManager::setFilename(const QString &path)
{
	m_setupPath = path + ".display";

	if (QFile::exists(m_setupPath))
		loadSetup(m_setupPath);
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// SLOTS
/////////////////////////////////////////////////////////////////////////////////////////////////

void AwDisplaySetupManager::resetToDefault()
{
	AwDisplaySetup *setup = new AwDisplaySetup("Default Setup");
	setup->setToDefault();
	emit newSetupSelected(setup);
	// set current setup AFTER sending all the signals to update other objects.
	delete m_currentSetup;
	m_currentSetup = setup;
}

void AwDisplaySetupManager::changeSetup(const QString &newSetup)
{
	// check if setup name is not the same than the current one
	if (m_currentSetup->name() == newSetup)
		return;

	if (newSetup == "Default Setup") {
		AwDisplaySetup *setup = new AwDisplaySetup("Default Setup");
		setup->setToDefault();
		emit newSetupSelected(setup);
		delete m_currentSetup;
		m_currentSetup = setup;
		return;
	}

	if (newSetup == "Autosaved") {
		AwDisplaySetup *setup = new AwDisplaySetup("Autosaved");
		if (setup->loadFromFile(m_setupPath)) {
			emit newSetupSelected(setup);
			delete m_currentSetup;
			m_currentSetup = setup;
			return;
		}
		else { // load failed => reset to Default Setup
			setup->setToDefault();
			emit newSetupSelected(setup);
			delete m_currentSetup;
			m_currentSetup = setup;
			QMessageBox::critical(NULL, tr("Display Setup Error"), "Autosaved setup could not be loaded. Resetting to Default Setup.");
			return;
		}
	}

	if (m_loadedSetups.contains(newSetup)) {
		AwDisplaySetup *setup = new AwDisplaySetup(newSetup);
		auto dir = AwSettings::getInstance()->value(aws::setup_dir).toString();
		auto path  = QString("%1%2.aws").arg(dir).arg(newSetup);
		
		if (setup->loadFromFile(path)) {
			emit newSetupSelected(setup);
			delete m_currentSetup;
			m_currentSetup = setup;
			return;
		}
		else {
			QString message = QString("%1 could not be loaded").arg(newSetup);
			QMessageBox::critical(NULL, tr("Display Setup Error"), message);
			delete setup;
			return;
		}
	}
	QMessageBox::critical(NULL, tr("Display Setup Error"), "display setup not found");
}

void AwDisplaySetupManager::saveSettings()
{
	if (m_setupPath.isEmpty())
		return;

	// save setup in File setup
	QSystemTrayIcon *sysTray = AwSettings::getInstance()->sysTray();
	sysTray->show();

	if (m_currentSetup->saveToFile(m_setupPath))
		m_toolBar->addSetup("Autosaved");	
	else
		sysTray->showMessage(tr("Error"), tr("Display setup has been modified but could not be saved."));
}

void AwDisplaySetupManager::updateSetup(AwDisplaySetup *setup, int flags)
{
	if (m_setupPath.isEmpty())
		return;

	// save setup in File setup
	QSystemTrayIcon *sysTray = AwSettings::getInstance()->sysTray();
	sysTray->show();

	if (m_currentSetup->saveToFile(m_setupPath))
		m_toolBar->addSetup("Autosaved");	
	else
		sysTray->showMessage(tr("Error"), tr("Display setup has been modified but could not be saved."));

	if (flags &  AwDisplaySetup::SynchViews)
		if (!m_currentSetup->synchronizeViews())
			sysTray->showMessage(tr("Signal Views"), tr("Signal views have been desynchronized."));
}

void AwDisplaySetupManager::saveCurrentSetup()
{
	AwDSMSaveSetupUI dlg(m_currentSetup->name());
	if (dlg.exec() == QDialog::Accepted)	{
		if (dlg.name() != m_currentSetup->name())	{
			m_currentSetup->setName(dlg.name());
			m_loadedSetups << dlg.name();
			m_toolBar->addSetup(dlg.name());
			m_toolBar->setCurrentSetup(dlg.name());
		}
		if (!m_currentSetup->save())
			QMessageBox::critical(0, tr("File Error"), tr("The display setup could not be saved!"));
	}
}


void AwDisplaySetupManager::loadSetup(const QString& path)
{
	AwDisplaySetup *newSetup = new AwDisplaySetup();
	if (!newSetup->loadFromFile(path)) // Failed to load the setup
		newSetup->setToDefault();

	emit newSetupSelected(newSetup);
	delete m_currentSetup;
	m_currentSetup = newSetup;
	// if the setup is called Autosaved => we just loaded the display setup that was saved with the currently open data file.
	// update the UI accordingly.
	if (m_currentSetup->name() == "Autosaved")
		m_toolBar->addSetup(m_currentSetup->name()); // add Autosaved to the list of available setups.
	m_toolBar->setCurrentSetup(m_currentSetup->name());	
}

void AwDisplaySetupManager::saveAsNew()
{
	AwSaveAsNewSetupDialog *dlg = new AwSaveAsNewSetupDialog(m_loadedSetups);

	if (dlg->exec() == QDialog::Accepted) {
		// new name has been set
		m_currentSetup->setName(dlg->newName());
		saveCurrentSetup();
		
		m_loadedSetups << dlg->newName();
		m_toolBar->addSetup(dlg->newName());
		m_toolBar->setCurrentSetup(dlg->newName());
	}
	delete dlg;
}


void AwDisplaySetupManager::deleteCurrentSetup()
{
	// Do not delete Default Setup
	if (m_currentSetup->name() == "Default Setup")
		return;
	
	int index = -1;
	if (QFile::remove(m_currentSetup->path()))	{
		index = m_loadedSetups.indexOf(m_currentSetup->name());
		if (index != -1) {
			m_loadedSetups.removeOne(m_currentSetup->name());

			// Removing the setup in tool bar combo box should update anything and change the setup.
			m_toolBar->removeSetup(m_currentSetup->name());
		}
	}
}