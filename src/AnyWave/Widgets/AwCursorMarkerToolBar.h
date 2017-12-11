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
#ifndef AWCURSORMARKERTOOLBAR_H
#define AWCURSORMARKERTOOLBAR_H

#include <QWidget>
#include <QToolBar>
#include "ui_AwCursorMarkerToolBar.h"

class AwCursorMarkerToolBar : public QWidget, public Ui::AwCursorMarkerToolBarClass
{
	Q_OBJECT

public:
	AwCursorMarkerToolBar(QWidget *parent = 0);
	~AwCursorMarkerToolBar();

	inline QToolBar *toolBar() { return m_toolBar; }
protected:
	void changeEvent(QEvent *);
public slots:
	void reset();
	void setCursorModeOn(bool flag);
	void setMarkerModeOn(bool flag);
	void setQTSModeOn(bool flag);
	void retranslate();
	void stopQTSMode();
signals:
	void markerModeChanged(bool);
	void cursorModeChanged(bool);
	void QTSModeChanged(bool);
protected:
	QToolBar *m_toolBar;
};

#endif // AWCURSORMARKERTOOLBAR_H
