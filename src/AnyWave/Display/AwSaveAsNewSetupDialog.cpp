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
#include "AwSaveAsNewSetupDialog.h"
#include "AwDisplaySetupManager.h"

AwSaveAsNewSetupDialog::AwSaveAsNewSetupDialog(const QStringList& setups, QWidget *parent)
: QDialog(parent)
{
	setupUi(this);
	labelInfo->hide();
	// setups may be empty if so, ask AwDisplaySetupManager for the list of available setups
	if (setups.isEmpty())
		m_loadedSetups = AwDisplaySetupManager::instance()->loadedSetups();
	else 
		m_loadedSetups = setups;

	QStringList list;
	foreach (QString s, m_loadedSetups)
		list << s.toLower();

	m_loadedSetups = list;
}

AwSaveAsNewSetupDialog::~AwSaveAsNewSetupDialog()
{

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// EVENTS
void AwSaveAsNewSetupDialog::changeEvent(QEvent *e)
{
	if (e) {
		if (e->type() == QEvent::LanguageChange) 
			retranslateUi(this);
	}
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SLOTS
///////////////////////////////////////////////////////////////////////////////////////////////////////////


void AwSaveAsNewSetupDialog::accept()
{
	QString newSetup = editName->text().toLower();

	if (newSetup.isEmpty())
	{
		labelInfo->setText(tr("You must enter a name for the setup"));
		labelInfo->show();
		return;
	}

	if (m_loadedSetups.contains(newSetup))
	{
		// Warning message
		labelInfo->setText(tr("A setup with that name already exists!"));
		labelInfo->show();
		return;
	}

	m_newName = editName->text();
	QDialog::accept();
}
