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
#include "AwFilterSettings.h"
#include <AwChannel.h>



AwFilterSettings::AwFilterSettings(QWidget *parent)
	: QWidget(parent)
{
    setupUi(this);
	// hide ICA and Source computation settings when starting
	groupICA->hide();
	groupSource->hide();
	connect(buttonSwitch, &QPushButton::toggled, this, &AwFilterSettings::switchFilters);
	m_switchStatus = Disabled;
	m_checkBoxes = { checkEegHigh, checkEmgHigh, checkMegHigh, 	checkEegLow,  checkEmgLow, checkMegLow, cbNotchEEG, cbNotchMEG,
		cbNotchEMG };
}

void AwFilterSettings::reset()
{
	//for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
	//	m_lp[i] = m_hp[i] = m_notch[i] = 0.;
	//}
	groupICA->hide();
	spICAEEGHP->setValue(0.);
	spICAEEGHP->setValue(0.);
	spICAMEGHP->setValue(0.);
	spICAMEGLP->setValue(0.);
	spICAEMGHP->setValue(0.);
	spICAEMGLP->setValue(0.);
	groupSource->hide();
	spSourceEEGHP->setValue(0.);
	spSourceEEGHP->setValue(0.);
	spSourceMEGHP->setValue(0.);
	spSourceMEGLP->setValue(0.);

	spEmgHP->setValue(0);
	spEmgLP->setValue(0);
	spEegHP->setValue(0);
	spEegLP->setValue(0);
	spMegHP->setValue(0);
	spMegLP->setValue(0);
	spNotchEMG->setValue(0);
	spNotchEEG->setValue(0);
	spNotchMEG->setValue(0);

	for (auto w : m_checkBoxes)
		w->setChecked(false);

	m_switchStatus = Disabled;
	buttonSwitch->setDisabled(true);	
}

void AwFilterSettings::closeFile()
{
	reset();
}

///
/// setSourceSettings()
/// Set filters used when computing the source channels.
/// type is the type of channels on which beamformer had been computed.
void AwFilterSettings::setSourceSettings(int type, float lp, float hp) 
{
	// show group
	labelSourceMEGLP->setVisible(type == AwChannel::MEG);
	labelSourceMEGHP->setVisible(type == AwChannel::MEG);
	spSourceMEGLP->setVisible(type == AwChannel::MEG);
	spSourceMEGHP->setVisible(type == AwChannel::MEG);
	labelSourceEEGLP->setVisible(type == AwChannel::EEG);
	labelSourceEEGHP->setVisible(type == AwChannel::EEG);
	spSourceEEGLP->setVisible(type == AwChannel::EEG);
	spSourceEEGHP->setVisible(type == AwChannel::EEG);

	if (type == AwChannel::MEG) {
		spSourceMEGLP->setValue(lp);
		spSourceMEGHP->setValue(hp);
	}
	else if (type == AwChannel::EEG) {
		spSourceEEGLP->setValue(lp);
		spSourceEEGHP->setValue(hp);
	}
	groupSource->show();
}


///
/// setICASettings()
/// Set filters used when computing ICA.
/// type is the type of channels on which ICA had been computed.
void AwFilterSettings::setICASettings(int type, float hp, float lp) 
{
	// show group
	labelMEGLP->setVisible(type == AwChannel::MEG);
	labelMEGHP->setVisible(type == AwChannel::MEG);
	spICAMEGLP->setVisible(type == AwChannel::MEG);
	spICAMEGHP->setVisible(type == AwChannel::MEG);
	labelEEGLP->setVisible(type == AwChannel::EEG);
	labelEEGHP->setVisible(type == AwChannel::EEG);
	spICAEEGLP->setVisible(type == AwChannel::EEG);
	spICAEEGHP->setVisible(type == AwChannel::EEG);
	labelEMGLP->setVisible(type == AwChannel::EMG);
	labelEMGHP->setVisible(type == AwChannel::EMG);
	spICAEMGLP->setVisible(type == AwChannel::EMG);
	spICAEMGHP->setVisible(type == AwChannel::EMG);

	if (type == AwChannel::MEG) {
		spICAMEGLP->setValue(lp);
		spICAMEGHP->setValue(hp);
	}
	else if (type == AwChannel::EEG) {
		spICAEEGLP->setValue(lp);
		spICAEEGHP->setValue(hp);
	}
	else if (type == AwChannel::EMG) {
		spICAEMGLP->setValue(lp);
		spICAEMGHP->setValue(hp);
	}
	groupICA->show();
}

void AwFilterSettings::updateFilters()
{
	AwFiltersManager *fm = AwFiltersManager::instance();

	checkEegHigh->setChecked(fm->fo().eegHP > 0.);
	checkEmgHigh->setChecked(fm->fo().emgHP > 0.);
	checkMegHigh->setChecked(fm->fo().megHP > 0.);
	checkEegLow->setChecked(fm->fo().eegLP > 0.);
	checkEmgLow->setChecked(fm->fo().emgLP > 0.);
	checkMegLow->setChecked(fm->fo().megLP > 0.);
	cbNotchEEG->setChecked(fm->fo().eegNotch > 0.);
	cbNotchMEG->setChecked(fm->fo().megNotch > 0.);
	cbNotchEMG->setChecked(fm->fo().emgNotch > 0.);

	spEmgHP->setValue(fm->fo().emgHP);
	spEmgLP->setValue(fm->fo().emgLP);
	spEegHP->setValue(fm->fo().eegHP);
	spEegLP->setValue(fm->fo().eegLP);
	spMegHP->setValue(fm->fo().megHP);
	spMegLP->setValue(fm->fo().megLP);
	spNotchEMG->setValue(fm->fo().emgNotch);
	spNotchEEG->setValue(fm->fo().eegNotch);
	spNotchMEG->setValue(fm->fo().megNotch);

	bool active = false;
	for (auto w : m_checkBoxes) {
		if (w->isChecked()) {
			active = true;
			break;
		}
	}

	if (active) {
		m_switchStatus = Disabled;
		buttonSwitch->setEnabled(true);
		buttonSwitch->setChecked(true);
		m_switchStatus = On;
	}
}


void AwFilterSettings::show()
{
	setVisible(true);
}

void AwFilterSettings::switchFilters(bool toggle)
{
	if (m_switchStatus == Disabled)
		return;
	if (m_switchStatus == On) {
		m_switchStatus = Off;
		buttonSwitch->setToolTip(tr("Click to restore filters."));
		disableFilters();
		AwFiltersManager::instance()->switchFilters(false);
	}
	else {
		m_switchStatus = On;
		AwFiltersManager::instance()->switchFilters(true);
		buttonSwitch->setToolTip(tr("Click to cut off all filters."));
	}
}

void AwFilterSettings::disableFilters()
{
	for (auto w : m_checkBoxes)
		w->setChecked(false);
}

void AwFilterSettings::apply()
{
	float lp, hp, notch;
	bool ica_over = false, source_over = false;
	// EEG/SEEG
    hp = checkEegHigh->isChecked() ? spEegHP->value() : -1;
	lp = checkEegLow->isChecked() ? spEegLP->value() : -1;
	notch = cbNotchEEG->isChecked() ? spNotchEEG->value() : -1;
	AwFiltersManager *fm = AwFiltersManager::instance();

	fm->fo().eegHP = hp;
	fm->fo().eegLP = lp;
	fm->fo().eegNotch = notch;

	// EMG/ECG
    hp = checkEmgHigh->isChecked() ? spEmgHP->value() : -1;
	lp = checkEmgLow->isChecked() ? spEmgLP->value() : -1;
	notch = cbNotchEMG->isChecked() ? spNotchEMG->value() : -1;

	fm->fo().emgHP = hp;
	fm->fo().emgLP = lp;
	fm->fo().emgNotch = notch;

	//MEG
	hp = checkMegHigh->isChecked() ? spMegHP->value() : -1;
	lp = checkMegLow->isChecked() ? spMegLP->value() : -1;
	notch = cbNotchMEG->isChecked() ? spNotchMEG->value() : -1;

	fm->fo().megHP = hp;
	fm->fo().megLP = lp;
	fm->fo().megNotch = notch;

	bool active = false;
	for (auto w : m_checkBoxes) {
		if (w->isChecked()) {
			active = true;
			break;
		}
	}

	if (active) {
		m_switchStatus = Disabled;
		buttonSwitch->setEnabled(true);
		buttonSwitch->setChecked(true);
		m_switchStatus = On;
	}
	else {
		m_switchStatus = Disabled;
		buttonSwitch->setEnabled(false);
	}

	fm->update();
}