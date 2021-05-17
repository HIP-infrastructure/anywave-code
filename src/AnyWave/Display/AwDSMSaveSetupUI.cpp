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
#include "AwDSMSaveSetupUI.h"
#include "Prefs/AwSettings.h"
#include <QFile>
#include <QMessageBox>

AwDSMSaveSetupUI::AwDSMSaveSetupUI(const QString& name, QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	if (!name.isEmpty())
		m_ui.lineEditName->setText(name);
	else
		m_ui.lineEditName->setText("New setup");
}

AwDSMSaveSetupUI::~AwDSMSaveSetupUI()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// EVENTS
void AwDSMSaveSetupUI::changeEvent(QEvent *e)
{
	if (e) {
		if (e->type() == QEvent::LanguageChange) 
			m_ui.retranslateUi(this);
	}
}

void AwDSMSaveSetupUI::accept()
{
	auto setupDir = AwSettings::getInstance()->value(aws::setup_dir).toString();
	if (setupDir.isEmpty()) {
		QMessageBox::critical(this, tr("Setup Folder"), tr("No Setup folder found in user directories.\nCannot save current setup."));
		QDialog::accept();
	}

	m_fullPath = setupDir + m_ui.lineEditName->text() + ".aws";
	m_name = m_ui.lineEditName->text();

	if (QFile::exists(m_fullPath))	{
		if (QMessageBox::question(this, tr("Setup name"), tr("A setup with this name already exists. Overwrite it?"),
			QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
			QDialog::accept();
	}
	else
		QDialog::accept();
		
}