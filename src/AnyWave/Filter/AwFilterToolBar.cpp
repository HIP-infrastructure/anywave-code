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
#include "AwFilterToolBar.h"
//#include "AwFilteringManager.h"

AwFilterToolBar::AwFilterToolBar(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);
	connect(buttonFilter, SIGNAL(clicked()), this, SIGNAL(filterButtonClicked()));
	//m_buttonGroupICA.addButton(radioICA_ON, 0);
	//m_buttonGroupICA.addButton(radioICA_OFF, 1);
	//connect(&m_buttonGroupICA, SIGNAL(buttonToggled(int, bool)), this, SLOT(changeICAFiltering(int, bool)));
	//radioICA_ON->hide();
	//radioICA_OFF->hide();
	//labelICA->hide();
	m_toolBar = new QToolBar(tr("Filtering"));
	m_toolBar->setObjectName("Filtering");
	m_toolBar->addWidget(this);
	m_dontSwitch = true;
}

AwFilterToolBar::~AwFilterToolBar()
{

}

void AwFilterToolBar::changeEvent(QEvent *e)
{
	if (e)
		if (e->type() == QEvent::LanguageChange)
			retranslateUi(this);
}

//void AwFilterToolBar::retranslate()
//{
//	retranslateUi(this);
//}


void AwFilterToolBar::applyFilters()
{
//	buttonSwitch->update();
}

////// SLOTS


// Enable ICA Filtering (after ICA file has been loaded for example).
// This will set the ICA fileting options to default state (No ICA Filtering)
// and the button will be shown in the toolbar.
void AwFilterToolBar::enableICAFiltering()
{
	//m_blockICASwitch = true;
	//radioICA_ON->show();
	//radioICA_OFF->show();
	//labelICA->show();
	//radioICA_OFF->setChecked(true);
	//m_blockICASwitch = false;
}


void AwFilterToolBar::changeICAFiltering(int id, bool checked)
{
	//if (m_blockICASwitch)
	//	return;

	//if (checked && id == 0) // ICA Filters ON
	//	emit ICASwitchChanged(true);
	//else if (checked && id == 1)
	//	emit ICASwitchChanged(false);
}

void AwFilterToolBar::setICAFiltering(bool on)
{
	//if (on)
	//	radioICA_ON->setChecked(true);
	//else
	//	radioICA_OFF->setChecked(true);
}

// update radio buttons to match the current ICA filtering mode
void AwFilterToolBar::setICAMode(bool filter)
{
	//m_blockICASwitch = true;
	//if (filter)
	//	radioICA_ON->setChecked(true);
	//else
	//	radioICA_OFF->setChecked(true);
	//m_blockICASwitch = false;
}



void AwFilterToolBar::closeFile()
{
	//// reset toolbar to default mode
	//radioICA_ON->hide();
	//radioICA_OFF->hide();
	//labelICA->hide();
}