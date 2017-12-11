/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "widget/AwAmplitudeButton.h"
#include <QMenu>
#include <QSettings>
#include <QTextCodec>
#include <AwChannel.h>
#include <AwAmplitudeManager.h>

void AwAmplitudeButton::createActionGroup(QMenu *menu, int type) 
{
	m_groups[type] = new QActionGroup(this);
	QAction *act;
	foreach (float v, m_am->getScale(type)) {
		act = menu->addAction(QString::number(v) + " " + m_am->unitOfChannel(type));
		m_groups[type]->addAction(act);
		act->setCheckable(true);
		act->setChecked(v == m_am->amplitude(type));
		act->setData(QVariant(v));
		connect(act, SIGNAL(toggled(bool)), this, SLOT(changeAmplitude(bool)));
	}
}

AwAmplitudeButton::AwAmplitudeButton(QWidget *parent)
	: QPushButton(parent)
{
	m_gainLevelForChannel = -1;	// default is to up/down levels for ALL channels
	for (int i = 0; i < AW_CHANNEL_TYPES; i++)
		m_groups[i] = NULL;

	m_menu = new QMenu(this);
	QAction *act;

	m_am = AwAmplitudeManager::instance();
	
	connect(m_am, SIGNAL(amplitudesChanged()), this, SLOT(setAmplitudes()));
	// MEG
	QMenu *meg_menu = new QMenu("MEG", m_menu);
	createActionGroup(meg_menu, AwChannel::MEG);
	// Reference
	QMenu *ref_menu = new QMenu("Reference", m_menu);
	createActionGroup(ref_menu, AwChannel::Reference);
	// EEG
	QMenu *eeg_menu = new QMenu("EEG", m_menu);
	createActionGroup(eeg_menu, AwChannel::EEG);
	// SEEG
	QMenu *seeg_menu = new QMenu("SEEG", m_menu);
	createActionGroup(seeg_menu, AwChannel::SEEG);
	// EMG
	QMenu *emg_menu = new QMenu("EMG", m_menu);
	createActionGroup(emg_menu, AwChannel::EMG);
	// ICA
	QMenu *ica_menu = new QMenu("ICA", m_menu);
	createActionGroup(ica_menu, AwChannel::ICA);
	// Source
	QMenu *source_menu = new QMenu("Source", m_menu);
	createActionGroup(source_menu, AwChannel::Source);
	// Trigger
	QMenu *trigger_menu = new QMenu("Trigger", m_menu);
	createActionGroup(trigger_menu, AwChannel::Trigger);
	// Other
	QMenu *other_menu = new QMenu("Other", m_menu);
	createActionGroup(other_menu, AwChannel::Other);

	m_menu->addMenu(meg_menu);
	m_menu->addMenu(ref_menu);
	m_menu->addMenu(eeg_menu);
	m_menu->addMenu(seeg_menu);
	m_menu->addMenu(emg_menu);
	m_menu->addMenu(trigger_menu);
	m_menu->addMenu(other_menu);
	m_menu->addMenu(ica_menu);
	m_menu->addMenu(source_menu);
	this->setMenu(m_menu);
}

AwAmplitudeButton::~AwAmplitudeButton()
{

}

void AwAmplitudeButton::switchGainLevelToChannel(int type) 
{
	m_gainLevelForChannel = type;

}

void AwAmplitudeButton::upActions(QActionGroup *group)
{
	if (group == NULL)
		return;
	int index = 0;
	// found the currently checked action
	foreach(QAction *a, group->actions()) {
		if (a->isChecked())
			break;
		index++;
	}
	if (index > 0)
		group->actions().at(index - 1)->setChecked(true);
}

void AwAmplitudeButton::downActions(QActionGroup *group)
{
	if (group == NULL)
		return;
	int index = 0;
	// found the currently checked action
	foreach(QAction *a, group->actions())	{
		if (a->isChecked())
			break;
		index++;
	}
	if (index < group->actions().size() - 1)
		group->actions().at(index + 1)->setChecked(true);

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS

void AwAmplitudeButton::up()
{
	if (m_gainLevelForChannel != -1)
		upActions(m_groups[m_gainLevelForChannel]);
	else {
		for (int i = 0; i < AW_CHANNEL_TYPES; i++)
			upActions(m_groups[i]);
	}
}

void AwAmplitudeButton::down()
{
	if (m_gainLevelForChannel != -1)
		downActions(m_groups[m_gainLevelForChannel]);
	else {
		for (int i = 0; i < AW_CHANNEL_TYPES; i++) 
			downActions(m_groups[i]);
	}
}

void AwAmplitudeButton::setAmplitudes()
{
	// all amplitudes have changed
	// Get the amplitudes scales from Amplitude Manager
	for (int i = 0; i < AW_CHANNEL_TYPES; i++) 
		setAmplitude(i, m_am->amplitude(i));
	emit amplitudesChanged();
}


void AwAmplitudeButton::setAmplitude(int type, float value)
{
    if (m_groups[type] == NULL)
		return;

	foreach (QAction *a, m_groups[type]->actions()) {
		if ((float)a->data().toDouble() == value) {
			a->setChecked(true);
			break;
		}
	}
}

void AwAmplitudeButton::changeAmplitude(bool checked)
{
	if (!checked)
		return;

	QAction *act = (QAction *)sender();
	if (act == NULL)
		return;

	float v = (float)act->data().toDouble();
	for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
		// skip empty groups
		if (m_groups[i] == NULL)
			continue;
		if (m_groups[i]->actions().contains(act)) {
			m_am->setAmplitude(i, v);
			emit amplitudeChanged(i, v);
			break;
		}
	}
}



