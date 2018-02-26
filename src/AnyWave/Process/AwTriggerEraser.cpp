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
#include "AwTriggerEraser.h"
#include "Widgets/AwSelectTriggerChannelDialog.h"

AwTriggerEraserPlugin::AwTriggerEraserPlugin()
{
	name = QString("Trigger Eraser");
	description = QString(tr("Erase a trigger channel"));
	version = "1.0";
	type = AwProcessPlugin::Background;
	setFlags(Aw::ProcessFlags::PluginIsHidden);
}

AwTriggerEraser::AwTriggerEraser() : AwProcess()
{
	setFlags(Aw::ProcessFlags::ProcessHasInputUi);
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
	if (status != -1) 
		sendMessage(QString("Error while erasing : %1").arg(pdi.input.reader()->errorMessage()));
}