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
#ifndef AWDISPLAYTOOLBAR_H
#define AWDISPLAYTOOLBAR_H

#include <QWidget>
#include <QToolBar>
#include "ui_AwDisplayToolBarv2.h"
#include "Display/AwDisplaySetup.h"

class AwDisplayToolBar : public QWidget, public Ui::AwDisplayToolBarClassv2
{
	Q_OBJECT

public:
	AwDisplayToolBar(QWidget *parent = 0);
	~AwDisplayToolBar();

	inline QToolBar *toolBar() { return m_toolBar; }
protected:
	void changeEvent(QEvent *);
public slots:
	void updateSetup(AwDisplaySetup *setup, int flags);
	void setNewSetup(AwDisplaySetup *setup);
	void addView();
protected slots:
	void setSynchronized(bool f);
	void changeAlignment(int index);
signals:
	void addViewClicked();
	void removeView();
	void alignHClicked();
	void alignVClicked();
	void setupChanged(AwDisplaySetup *setup, int flags);
	void captureClicked();

protected:
	AwDisplaySetup *m_setup;
	QToolBar *m_toolBar;
};

#endif // AWDISPLAYTOOLBAR_H
