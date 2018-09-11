/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
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
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <QWidget>
#include "ui_AwFilterGUI.h"
#include <filter/AwFilterSettings.h>

class AW_FILTER_EXPORT AwFilterGUI : public QWidget
{
	Q_OBJECT

public:
	AwFilterGUI(const AwFilterSettings& settings, QWidget *parent = Q_NULLPTR);
	~AwFilterGUI();

	void updateSettings(const AwFilterSettings& settings);
signals:
	void applyClicked(const AwFilterSettings& settings);
public slots:
	void switchFilters(bool flag);
	void apply();
	void addNewFilter(int type);
private:
	Ui::AwFilterGUI m_ui;
	AwFilterSettings m_settings, m_zeroFilters;
};