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
#ifndef AW_FILTERSETTINGS_H
#define AW_FILTERSETTINGS_H

#include <QWidget>
#include "ui_AwFilterSettings.h"
#include <AwChannel.h>
using namespace Ui;
#include "AwFiltersManager.h"			

class AwFilterSettings : public QWidget, public FilterSettingsClass
{
	Q_OBJECT

public:
	AwFilterSettings(QWidget *parent = 0);

public slots:
	void apply();
	void show();
	void updateFilters();
	void closeFile();
	void setICASettings(int type, float hp, float lp);
	void setSourceSettings(int type, float hp, float lp);
	void reset();
signals:
	// Sent when the user clicks on the apply button.
	void filtersApplied();
protected:
	//float m_lp[AW_CHANNEL_TYPES], m_hp[AW_CHANNEL_TYPES];
	//float m_notch[AW_CHANNEL_TYPES];
};

#endif // FILTERSETTINGS_H
