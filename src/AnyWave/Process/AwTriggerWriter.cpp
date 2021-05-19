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
#include "AwTriggerWriter.h"
#include "Widgets/AwSelectTriggerChannelDialog.h"

AwTriggerWriterPlugin::AwTriggerWriterPlugin()
{
	name = QString("Trigger Writer");
	description = QString(tr("Writes marker values to a trigger channel"));
	version = "1.0";
	type = AwProcessPlugin::Background;
	setFlags(Aw::ProcessFlags::PluginIsHidden);
	setFlags(Aw::ProcessFlags::ProcessHasInputUi);
}

AwTriggerWriter::AwTriggerWriter() : AwProcess()
{
	//setFlags(Aw::ProcessFlags::ProcessHasInputUi);
}

bool AwTriggerWriter::showUi()
{
	AwSelectTriggerChannelDialog dlg(pdi.input.reader()->triggerChannels());
	if (dlg.exec() == QDialog::Accepted) {
		m_triggerChannel = dlg.selectedChannel();
		return true;
	}
	return false;
}

void AwTriggerWriter::run()
{
	sendMessage("Writing values...");
	int count = pdi.input.reader()->writeTriggerChannel(m_triggerChannel, pdi.input.markers());
	sendMessage(QString("%1 values written.").arg(count));
}