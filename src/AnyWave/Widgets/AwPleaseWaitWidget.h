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
#ifndef AW_PLEASEWAIT_WIDGET_H
#define AW_PLEASEWAIT_WIDGET_H

#include <QWidget>
#include "ui_AwPleaseWaitWidget.h"

using namespace Ui;

class AwPleaseWaitWidget : public QWidget, public PleaseWaitWidgetClass
{
	Q_OBJECT

public:
	AwPleaseWaitWidget(const QString& message, QWidget *parent = 0);
	~AwPleaseWaitWidget();
	void setMessage(const QString& message) { m_message = message; labelText->setText(message); repaint(); }
	QString& message() { return m_message; }
private:
	QString m_message;
};

#endif // PLEASEWAIT_WIDGET_H
