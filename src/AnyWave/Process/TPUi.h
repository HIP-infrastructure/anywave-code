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
#ifndef TPUI_H
#define TPUI_H

#include <QDialog>
#include "ui_TPUi.h"
#include <AwChannel.h>

class TPUi : public QDialog
{
	Q_OBJECT

public:
	TPUi(const AwChannelList& triggers, QWidget *parent = 0);
	~TPUi();

	inline bool isMaskValueSet() { return m_mask; }
	inline qint16 maskValue() { return m_maskValue; }
	inline AwChannelList& triggers() { return m_triggers; }
	inline bool parseNegativeValues() { return m_parseNegative; }
protected:
	void changeEvent(QEvent *);
public slots:
	int exec();
	void accept();
private:
	Ui::TPUiClass m_ui;
	AwChannelList m_triggers;
	QList<QCheckBox *> m_checks;
	qint16 m_maskValue;
	bool m_mask;
	bool m_parseNegative;
};

#endif // TPUI_H
