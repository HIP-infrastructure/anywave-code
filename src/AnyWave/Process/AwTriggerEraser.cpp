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
#include "AwTriggerEraser.h"
#include "Widgets/AwSelectTriggerChannelDialog.h"

AwTriggerEraserPlugin::AwTriggerEraserPlugin()
{
	name = QString("Trigger Eraser");
	description = QString(tr("Erase a trigger channel"));
	category = "Process:Trigger Channels:Erase";
	version = "1.0.1";
	type = AwProcessPlugin::Background;
	setFlags(Aw::ProcessFlags::ProcessHasInputUi);
}

AwTriggerEraser::AwTriggerEraser() : AwProcess()
{
}

bool AwTriggerEraser::showUi()
{
	AwSelectTriggerChannelDialog dlg(pdi.input.reader()->triggerChannels());
	if (dlg.exec() == QDialog::Accepted) {
		m_triggerChannel = dlg.selectedChannel();
		return true;
	}
	return false;
}

void AwTriggerEraser::run()
{
	sendMessage("Erasing...");
	QStringList list;
	list << m_triggerChannel;
	int status = pdi.input.reader()->clearTriggerChannels(list);
	if (status == -1) 
		sendMessage(QString("Error while erasing : %1").arg(pdi.input.reader()->errorMessage()));
}