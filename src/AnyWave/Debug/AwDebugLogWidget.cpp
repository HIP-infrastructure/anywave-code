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
#include "AwDebugLogWidget.h"
#include "AwDebugLog.h"

AwDebugLogWidget::AwDebugLogWidget(QWidget *parent, Qt::WindowFlags flags)
	: QWidget(parent, flags)
{
	m_ui.setupUi(this);
	AwDebugLog *adl = AwDebugLog::instance();

	QStringList comps = adl->components();
	comps.insert(0, tr("All Components"));
	m_ui.comboLogType->addItems(comps);
	m_ui.comboLogType->setCurrentIndex(0);
	connect(adl, &AwDebugLog::newLog, this, &AwDebugLogWidget::refresh);
	setWindowFlag(Qt::Window);
}

AwDebugLogWidget::~AwDebugLogWidget()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// EVENTS
void AwDebugLogWidget::changeEvent(QEvent *e)
{
	if (e) {
		if (e->type() == QEvent::LanguageChange) 
			m_ui.retranslateUi(this);
	}
}




////////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS

void AwDebugLogWidget::updateLogs(QString type)
{
	QStringList logs;
	AwDebugLog *adl = AwDebugLog::instance();
	QStringList comps = adl->components();
	m_currentType = type;
	if (type == tr("All Components"))
		foreach(QString s, comps)
			logs += adl->logsForComponent(s);
	else
		logs = adl->logsForComponent(type);

	if (logs.isEmpty())
		m_ui.plainTextEditLogs->setPlainText(tr("No Debug Logs"));
	else
	{
		QString text;
		foreach (QString s, logs)
			text += s + "\n";
		m_ui.plainTextEditLogs->setPlainText(text);
	}
}

void AwDebugLogWidget::refresh()
{
	updateLogs(m_currentType);
}