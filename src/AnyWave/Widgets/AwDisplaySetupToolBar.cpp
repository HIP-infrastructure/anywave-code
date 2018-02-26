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
#include "AwDisplaySetupToolBar.h"
#include "Display/AwDisplaySetupManager.h"
#include <QMenu>


AwDisplaySetupToolBar::AwDisplaySetupToolBar(QStringList& setups, QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);
	// get loaded setups
	m_setups = setups;
	comboSetups->addItems(setups);
	m_toolBar = new QToolBar(tr("Display Setups"));
	m_toolBar->setObjectName("Display Setup");
	m_toolBar->addWidget(this);

	connect(comboSetups, SIGNAL(currentIndexChanged(const QString&)), this, SIGNAL(setupChanged(const QString&)));

	QMenu *menu = new QMenu(buttonSave);
	QAction *action = new QAction(tr("Save"), menu);
	menu->addAction(action);
	connect(action, SIGNAL(triggered()), this, SIGNAL(currentSetupSaveRequest()));

	// insert separator then action "Delete"
	menu->addSeparator();
	action = new QAction(tr("Remove Setup"), menu);
	menu->addAction(action);
	connect(action, SIGNAL(triggered()), this, SIGNAL(currentSetupWillBeDeteted()));

	buttonSave->setMenu(menu);
}

AwDisplaySetupToolBar::~AwDisplaySetupToolBar()
{
	
}

void AwDisplaySetupToolBar::changeEvent(QEvent *e)
{
	if (e)
		if (e->type() == QEvent::LanguageChange)
			retranslateUi(this);
}


void AwDisplaySetupToolBar::addSetup(const QString& setup)
{
	if (!m_setups.contains(setup))
	{
		m_setups << setup;
		comboSetups->addItem(setup);
	}
}

void AwDisplaySetupToolBar::removeSetup(const QString &setup)
{
	for (qint32 i = 0; i < comboSetups->count(); i++)
		if (comboSetups->itemText(i) == setup)
		{
			comboSetups->removeItem(i);
			m_setups.removeAll(setup);
		}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SLOTS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AwDisplaySetupToolBar::setCurrentSetup(const QString& name)
{
	int index = m_setups.indexOf(name);
	if (index != -1)
		comboSetups->setCurrentIndex(index);
}

void AwDisplaySetupToolBar::retranslate()
{
	retranslateUi(this);
}