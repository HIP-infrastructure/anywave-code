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
#include "AwFilterSwitchButton.h"
//#include "AwFilteringManager.h"
#include "AwFiltersManager.h"

AwFilterSwitchButton::AwFilterSwitchButton(QWidget *parent)
	: QPushButton(parent)
{
	setText(tr("No Filters"));
	m_state = AwFilterSwitchButton::NoFilters;
	//AwFilteringManager *fm = AwFilteringManager::instance();

	//connect(fm, SIGNAL(filtersChanged()), this, SLOT(updateStatus()));
	connect(this, SIGNAL(clicked()), this, SLOT(handleClick()));

	updateStatus();
}

AwFilterSwitchButton::~AwFilterSwitchButton()
{

}

void AwFilterSwitchButton::updateStatus()
{
	AwFilteringManager *fm = AwFilteringManager::instance();
	if (!fm->anyFilterActive())
		noFilters();
	else 
		setOn();
}


void AwFilterSwitchButton::noFilters()
{
	setText(tr("No Filters"));
	m_state = AwFilterSwitchButton::NoFilters;
	setDisabled(true);
}


void AwFilterSwitchButton::setOn()
{
	setText(tr("Switch OFF"));
	m_state = AwFilterSwitchButton::On;
	setDisabled(false);
}

void AwFilterSwitchButton::setOff()
{
	setText(tr("Switch ON"));
	m_state = AwFilterSwitchButton::Off;
	setDisabled(false);
}

void AwFilterSwitchButton::handleClick()
{
	switch (m_state)
	{
	case AwFilterSwitchButton::On:
		AwFilteringManager::instance()->switchFilters(false);
		setOff();
		break;
	case AwFilterSwitchButton::Off:
		AwFilteringManager::instance()->switchFilters(true);
		setOn();
		break;
	}
}

