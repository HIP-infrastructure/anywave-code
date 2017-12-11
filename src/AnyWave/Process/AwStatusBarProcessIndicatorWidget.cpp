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
#include "AwStatusBarProcessIndicatorWidget.h"

AwStatusBarProcessIndicatorWidget::AwStatusBarProcessIndicatorWidget(AwProcess *process, QMenu *menu, QWidget *parent)
	: AwProcessIndicatorWidget(process, menu, parent)
{
	setupUi(this);
	m_isRunning = false;
	m_progress = -1;
	m_processName = m_process->plugin()->name;
	labelMessage->hide();
	if (!menu)
	{
		// Create default context menu
		m_menu = new QMenu(this);
		m_actStop = new QAction(tr("Stop"), m_menu);
		m_menu->addAction(m_actStop);
		connect(m_actStop, SIGNAL(triggered()), this, SLOT(stopProcess()));

		buttonMenu->setMenu(m_menu);
	}
}



void AwStatusBarProcessIndicatorWidget::updateMessage()
{
	QString message;

	if (m_isRunning)
		message = m_processName + ": " + m_message;
	else
		message = m_processName + ": " + m_message + QString(" (%1s)").arg((float)(m_process->executionTime() /  1000));

	labelProcess->setText(message);
	labelProcess->setScaledContents(true);
}


void AwStatusBarProcessIndicatorWidget::setProgression(int value)
{
	if (value >= 0 && value <= 100)
	{
		m_progress = value;
		QString progress = QString("%1%").arg(value);
		labelMessage->show();
		labelMessage->setText(progress);
		labelMessage->setScaledContents(true);
	}
	else
		labelMessage->hide();
}

void AwStatusBarProcessIndicatorWidget::setIdle()
{
	processIndicator->stopAnimation();
	m_message = QString(tr("Idle"));
	m_isRunning = false;
	updateMessage();
}

void AwStatusBarProcessIndicatorWidget::setRunning()
{
	processIndicator->startAnimation();
	m_message = QString(tr("Running..."));
	m_isRunning = true;
	updateMessage();
}

void AwStatusBarProcessIndicatorWidget::setFinished()
{
	processIndicator->stopAnimation();
	m_isRunning = false;
	// when a process is finished, we request that the widget will be closed
	emit closeRequest(this);
}

void AwStatusBarProcessIndicatorWidget::stopProcess()
{
	// We request the current process to be closed
	m_message = QString(tr("Stopping..."));
	m_isRunning = false;
	updateMessage();
	if (m_process->runMode() == AwProcessPlugin::Display)
	{
		emit processStopped(m_process);
		emit closeRequest(this);
	}
}



AwProcessIndicatorWidget* AwStatusBarProcessIndicator::newWidget(AwProcess *p, QMenu *menu, QWidget *parent)
{
	return new AwStatusBarProcessIndicatorWidget(p, menu, parent); 
}