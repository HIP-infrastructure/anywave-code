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
#ifndef AW_PROCESS_GUI_WIDGET_H
#define AW_PROCESS_GUI_WIDGET_H
#include <AwProcess.h>
#include <AwGlobal.h>
#include <QWidget>
class AwGUIProcess;

class AW_PROCESS_EXPORT AwProcessGUIWidget : public QWidget
{
	Q_OBJECT
public:
	AwProcessGUIWidget(AwGUIProcess *process, QWidget *parent) : QWidget(parent) { m_process = process; }
signals:
	void closed();
protected:
	/** reimplementing close event to send the closed signal to AnyWave **/
	void closeEvent(QCloseEvent *event);
	AwGUIProcess *m_process;
};

#endif