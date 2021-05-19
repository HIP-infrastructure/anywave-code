// AnyWave
// Copyright (C) 2013-2021  INS UMR 1106
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include "AwProcessLogWindow.h"
#include <QTime>
#include <QScrollBar>

AwProcessLogWindow::AwProcessLogWindow(QWidget *parent, Qt::WindowFlags flags)
	: QWidget(parent, flags)
{
	setWindowFlags(Qt::Window);
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