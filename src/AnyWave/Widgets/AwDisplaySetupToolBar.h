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
#ifndef AWDISPLAYSETUPTOOLBAR_H
#define AWDISPLAYSETUPTOOLBAR_H

#include <QWidget>
#include <QToolBar>
#include "ui_AwDisplaySetupToolBar.h"

class AwDisplaySetupToolBar : public QWidget, public Ui::AwDisplaySetupToolBarUI
{
	Q_OBJECT

public:
	AwDisplaySetupToolBar(QStringList& setups, QWidget *parent = 0);
	~AwDisplaySetupToolBar();

	inline QToolBar *toolBar() { return m_toolBar; }

	void addSetup(const QString& setup);
	void removeSetup(const QString& setup);
protected:
	void changeEvent(QEvent *);
public slots:
	void setCurrentSetup(const QString& name);
	void retranslate();
signals:
	void setupChanged(const QString& name);
	void currentSetupSaveRequest();
	void currentSetupWillBeDeteted();
protected:
	QToolBar *m_toolBar;
	QStringList m_setups;	// name of all setups
};

#endif // AWDISPLAYSETUPTOOLBAR_H
