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
