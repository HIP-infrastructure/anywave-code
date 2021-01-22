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
#include "AwProcessWidget.h"
#include <utils/time.h>

AwProcessWidget::AwProcessWidget(AwProcess *process, QWidget *parent)
	: QWidget(parent)
{
	m_ui.setupUi(this);
	m_process = process;
	m_ui.labelName->setText(process->plugin()->name);
	m_ui.buttonStop->hide();
	m_ui.buttonRemove->hide();
	m_ui.labelMessage->hide();
	m_ui.labelTime->hide();
	m_ui.progress->hide();
	m_ui.buttonShowResults->hide();
	connect(m_ui.buttonStop, SIGNAL(clicked()), this, SLOT(stop()));
	connect(m_ui.buttonRemove, SIGNAL(clicked()), this, SLOT(removeMe()));
	connect(m_ui.buttonShowResults, SIGNAL(clicked()), this, SLOT(showResults()));
	connect(m_ui.buttonShowLog, SIGNAL(clicked()), this, SLOT(showLog()));
	
	m_logUpdated = false;
	m_logWindow = new AwProcessLogWindow(this);
	m_logWindow->hide();
	m_timer = nullptr;
	m_isActive = true;
#ifdef Q_OS_MAC
	QMargins margins = layout()->contentsMargins();
	margins.setBottom(margins.bottom() - 6);
    layout()->setContentsMargins(margins);
#endif
	connect(process, SIGNAL(aborted()), this, SLOT(setFinished()));
	connect(process, SIGNAL(idle()), this, SLOT(setIdle()));
	connect(process, SIGNAL(finished()), this, SLOT(setFinished()));
	connect(process, SIGNAL(progressChanged(int)), this, SLOT(setProgression(int)));
	connect(process, SIGNAL(progressChanged(const QString&)), m_logWindow, SLOT(appendLog(const QString&)));
	connect(process, SIGNAL(error(const QString&)), m_logWindow, SLOT(appendError(const QString&)));
	connect(process, SIGNAL(warning(const QString&)), m_logWindow, SLOT(appendWarning(const QString&)));
	connect(process, SIGNAL(progressChanged(const QString&)), this, SLOT(updateNewLog()));
	connect(process, SIGNAL(error(const QString&)), this, SLOT(updateNewLog()));
	connect(process, SIGNAL(warning(const QString&)), this, SLOT(updateNewLog()));
	connect(process, SIGNAL(started()), this, SLOT(setRunning()));
}

AwProcessWidget::~AwProcessWidget()
{
	clear();
//	m_logWindow->close();
//	delete m_logWindow;
}

void AwProcessWidget::changeEvent(QEvent *e)
{
	if (e)
		if (e->type() == QEvent::LanguageChange)
			m_ui.retranslateUi(this);
}

/// 
/// clear() - close the log window and destroy it.
void AwProcessWidget::clear()
{
	if (m_timer)	{
		m_timer->stop();
		delete m_timer;
		m_timer = nullptr;
	}
	m_logWindow->close();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////:
/// SLOTS

void AwProcessWidget::removeMe()
{
	emit removeClicked(this);
}

void AwProcessWidget::setIdle()
{
	m_ui.progress->hide();
	m_ui.buttonStop->show();
	m_ui.labelMessage->show();
	m_ui.labelMessage->setText(tr("Idle"));
	m_logWindow->appendLog(tr("Running time = ") + AwUtilities::time::hmsTime(m_process->executionTime()));

	if (m_process->flags() & Aw::ProcessFlags::HasOutputUi)
		m_ui.buttonShowResults->show();
}


void AwProcessWidget::setFinished()
{
	m_ui.labelMessage->show();
	m_ui.labelMessage->setText(tr("Finished"));
	m_ui.buttonRemove->show();
	m_ui.buttonStop->hide();
	m_ui.buttonShowResults->hide();
	m_ui.progress->hide();
	QString message;
	if (m_process->wasAborted())
		message = tr("User Aborted.");
	else
		message = tr("Running time = ") + AwUtilities::time::hmsTime(m_process->executionTime());

	m_ui.labelTime->show();
	m_ui.labelTime->setText(message);
	m_logWindow->appendLog(message);

	m_isActive = false;
}

void AwProcessWidget::setRunning()
{
	m_ui.buttonStop->show();
	m_ui.progress->show();
	m_logWindow->appendLog(tr("Started"));
	m_ui.progress->setMinimum(0);
	m_ui.progress->setMaximum(0);
	m_ui.labelMessage->show();
	m_ui.labelMessage->setText(tr("Running..."));
}

void AwProcessWidget::updateNewLog()
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

void AwProcessWidget::makeShowLogBlink()
{
	static bool flag = true;

	if (flag)
		m_ui.buttonShowLog->setStyleSheet("QPushButton{color: blue}");
	else
		m_ui.buttonShowLog->setStyleSheet("QPushButton{color: black}");
	flag = !flag;
}	

void AwProcessWidget::setProgression(int value)
{
	m_ui.progress->setMinimum(0);
	m_ui.progress->setMaximum(100);
	m_ui.progress->setValue(value);
}

void AwProcessWidget::stop()
{
	if (m_process->isRunning())	{
		m_logWindow->appendLog(QString(tr("Aborting...")));
		m_process->abort();
		return;
	}
	if (m_process->isIdle()) {
		if (m_process->pdi.hasOutputWidgets()) {
			foreach(QWidget * w, m_process->pdi.output.widgets()) {
				w->close();
			}
			m_process->pdi.output.clearWidgets();
		}
		m_process->stop();
	}
	else
		setFinished();
}
void AwProcessWidget::showLog()
{
	m_logWindow->show();
	if (m_timer)
	{
		m_timer->stop();
		delete m_timer;
		m_timer = nullptr;
	}
	m_ui.buttonShowLog->setStyleSheet("QPushButton{color: black}");
	m_ui.buttonShowLog->repaint();
}

void AwProcessWidget::showResults()
{
	foreach (QWidget *w, m_process->pdi.output.widgets())
		w->show();
}