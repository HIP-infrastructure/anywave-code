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
#include "TPUi.h"
#include <widget/AwMessageBox.h>

TPUi::TPUi(const AwChannelList& triggers, QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	m_triggers = triggers;
	m_mask = true;
	m_parseNegative = false;
}

TPUi::~TPUi()
{
	
}

void TPUi::changeEvent(QEvent *e)
{
	if (e)
		if (e->type() == QEvent::LanguageChange)
			m_ui.retranslateUi(this);
}



int TPUi::exec()
{
	// hide group Channels to parse if only one trigger channel.
	if (m_triggers.size() > 1)	{
		m_ui.groupChannels->show();
		QVBoxLayout *vbox = new QVBoxLayout;
		foreach (AwChannel *c, m_triggers) {
			QCheckBox *cb = new QCheckBox(this);
			m_checks << cb;
			cb->setText(c->name());
			vbox->addWidget(cb);
		}
		vbox->addStretch(1);
		m_ui.groupChannels->setLayout(vbox);
		// check first trigger channel
		m_checks.at(0)->setChecked(true);
	}
	else
		m_ui.groupChannels->hide();
	return QDialog::exec();
}


void TPUi::accept()
{
	bool atLeastOneChecked = false;
	if (m_triggers.size() > 1)	{
		for (int i = 0; i < m_checks.size(); i++) {
			QCheckBox *cb = m_checks.at(i);
			if (cb->isChecked())
				atLeastOneChecked = true;
		}
		if (!atLeastOneChecked)	{
			AwMessageBox::information(this, tr("Incorrect paremeter"), tr("Select at least one channel to parse."));
			return;
		}
		for (int i = 0; i < m_checks.size(); i++) {
			QCheckBox *cb = m_checks.at(i);
			if (!cb->isChecked())
				m_triggers.removeAt(i);
		}
	}

	if (m_ui.checkMask->isChecked())
		m_maskValue = (qint16)m_ui.spinBoxValue->value();
	else
		m_mask = false;

	if (m_ui.checkNegative->isChecked())
		m_parseNegative = true;

	return QDialog::accept();
}