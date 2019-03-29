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
#include "AwScriptWidget.h"
#include "ui_AwScriptWidget.h"
#include <QTime>

AwScriptWidget::AwScriptWidget(AwScript *script, QWidget *parent)
	: QWidget(parent)
{
	ui = new Ui::AwScriptWidgetUi();
	ui->setupUi(this);

	m_script = script;
	ui->labelName->setText(script->path());
	ui->buttonRemove->hide();
	ui->labelMessage->hide();
	ui->labelTime->hide();
	connect(ui->buttonShowLog, SIGNAL(clicked()), this, SLOT(showLog()));
	
	m_logUpdated = false;
	m_logWindow = new AwProcessLogWindow();
	m_logWindow->hide();
	m_timer = NULL;
	m_isActive = true;

	connect(script, SIGNAL(finished()), this, SLOT(setFinished()));

	connect(script, SIGNAL(message(const QString&)), m_logWindow, SLOT(appendLog(const QString&)));
	connect(script, SIGNAL(error(const QString&)), m_logWindow, SLOT(appendError(const QString&)));
	connect(script, SIGNAL(warning(const QString&)), m_logWindow, SLOT(appendWarning(const QString&)));
	connect(script, SIGNAL(message(const QString&)), this, SLOT(updateNewLog()));
	connect(script, SIGNAL(error(const QString&)), this, SLOT(updateNewLog()));
	connect(script, SIGNAL(warning(const QString&)), this, SLOT(updateNewLog()));
}

AwScriptWidget::~AwScriptWidget()
{
	delete ui;
	delete m_logWindow;
}

////////////////////////////////////////////////////////////////////////////////////////////////////:
/// SLOTS

void AwScriptWidget::removeMe()
{
	emit removeClicked(this);
}

void AwScriptWidget::updateNewLog()
{
	if (!m_logWindow->isVisible())	{	
		if (!m_logUpdated)	{
			m_timer = new QTimer(this);
			connect(m_timer, SIGNAL(timeout()), this, SLOT(makeShowLogBlink()));
			m_timer->start(500);
			m_logUpdated = true;
		}
	}
}

void AwScriptWidget::makeShowLogBlink()
{
	static bool flag = true;

	if (flag)
		ui->buttonShowLog->setStyleSheet("QPushButton{color: blue}");
	else
		ui->buttonShowLog->setStyleSheet("QPushButton{color: black}");
	flag = !flag;
}	

void AwScriptWidget::stop()
{
	setFinished();
}

void AwScriptWidget::setFinished()
{
	ui->labelMessage->show();
	ui->labelMessage->setText(tr("Finished"));
	ui->buttonRemove->show();
	ui->labelTime->show();
	ui->labelTime->setText(QTime::currentTime().toString());
	m_logWindow->appendLog(tr("Finished"));
	m_isActive = false;
}

void AwScriptWidget::showLog()
{
	m_logWindow->show();
	if (m_timer) {
		m_timer->stop();
		delete m_timer;
		m_timer = NULL;
	}
	ui->buttonShowLog->setStyleSheet("QPushButton{color: black}");
	ui->buttonShowLog->repaint();
}

void AwScriptWidget::setRunning()
{
	m_logWindow->appendLog("Started");
	ui->labelMessage->show();
	ui->labelMessage->setText(tr("Running..."));
}
