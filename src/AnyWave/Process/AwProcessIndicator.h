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
#ifndef AWPROCESSINDICATOR_H
#define AWPROCESSINDICATOR_H

#include <QWidget>
#include <QMenu>
#include <AwProcessInterface.h>

class AwProcessIndicatorWidget : public QWidget
{
	Q_OBJECT

public:
	AwProcessIndicatorWidget(AwProcess *process, QMenu *menu = 0, QWidget *parent = 0);
public slots:
	virtual void setIdle() = 0;
	virtual void setRunning() = 0;
	virtual void setFinished() = 0;
	virtual void setMessage(const QString& message) = 0;
	virtual void setProgression(int value) = 0;
	virtual void endDisplayProcess(AwProcess *proces);
	void closeIndicator();
signals:
	void closeRequest(AwProcessIndicatorWidget *piw);
	void processStopped(AwProcess *p);
protected:
	QMenu *m_menu;
	AwProcess *m_process;
};

class AwProcessIndicator : public QObject
{
	Q_OBJECT
public:
	AwProcessIndicator(QObject *parent = 0) : QObject(parent) {}
	virtual AwProcessIndicatorWidget *newWidget(AwProcess *process, QMenu *menu = NULL, QWidget *parent = 0) = 0;
};

#endif // AWPROCESSINDICATOR_H
