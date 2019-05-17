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
#include "AwProcessLogWindow.h"
#include <QTime>
#include <QScrollBar>

AwProcessLogWindow::AwProcessLogWindow(QWidget *parent, Qt::WindowFlags flags)
	: QWidget(parent, flags)
{
	m_ui.setupUi(this);
}

AwProcessLogWindow::~AwProcessLogWindow()
{

}

void AwProcessLogWindow::changeEvent(QEvent *e)
{
	if (e)
		if (e->type() == QEvent::LanguageChange)
			m_ui.retranslateUi(this);
}

void AwProcessLogWindow::appendLog(const QString& text)
{
	auto message = QString("%1: %2").arg(QTime::currentTime().toString()).arg(text);
	m_ui.plainTextEdit->appendPlainText(message);
	m_ui.plainTextEdit->verticalScrollBar()->setValue(m_ui.plainTextEdit->verticalScrollBar()->maximum());
}

void AwProcessLogWindow::appendError(const QString& text)
{
	QString errorHtml = "<font color=\"red\">";
	QString endHtml = "</font><br>";
	auto message = QString("%1: %2").arg(QTime::currentTime().toString()).arg(text);

	m_ui.plainTextEdit->appendHtml(QString("%1%2%3").arg(errorHtml).arg(message).arg(endHtml));
	m_ui.plainTextEdit->verticalScrollBar()->setValue(m_ui.plainTextEdit->verticalScrollBar()->maximum());
}

void AwProcessLogWindow::appendWarning(const QString& text)
{
	auto message = QString("%1: %2").arg(QTime::currentTime().toString()).arg(text);
	m_ui.plainTextEdit->appendPlainText(message);
	m_ui.plainTextEdit->verticalScrollBar()->setValue(m_ui.plainTextEdit->verticalScrollBar()->maximum());
}