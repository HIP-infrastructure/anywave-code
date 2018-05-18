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
	//AwFilteringManager *fm = AwFilteringManager::instance();
	//connect(fm, SIGNAL(filtersChanged()), this, SLOT(updateFilters()));
	//connect(fm,  SIGNAL(filtersReset()), this, SLOT(closeFile()));
	//connect(this, SIGNAL(filtersApplied()), fm, SIGNAL(filtersChanged()));
	//connect(fm, SIGNAL(newICASettings(int, float, float)), this, SLOT(setICASettings(int, float, float)));
	//connect(fm, SIGNAL(newSourceSettings(int, float, float)), this, SLOT(setSourceSettings(int, float, float)));

	// hide ICA and Source computation settings when starting
	groupICA->hide();
	groupSource->hide();
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

	checkEegHigh->setChecked(false);
	checkEmgHigh->setChecked(false);
	checkMegHigh->setChecked(false);
	checkEegLow->setChecked(false);
	checkEmgLow->setChecked(false);
	checkMegLow->setChecked(false);
	cbNotchEEG->setChecked(false);
	cbNotchMEG->setChecked(false);
	cbNotchEMG->setChecked(false);
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
	//reset();
	//AwFilteringManager *fm = AwFilteringManager::instance();

	//// LP
	//if (fm->lowPass(AwChannel::EEG) > 0)
	//	spEegLP->setValue(fm->lowPass(AwChannel::EEG));
	//if (fm->lowPass(AwChannel::MEG) > 0)
	//	spMegLP->setValue(fm->lowPass(AwChannel::MEG));
	//if (fm->lowPass(AwChannel::EMG) > 0)
	//	spEmgLP->setValue(fm->lowPass(AwChannel::EMG));
	//// HP
	//if (fm->highPass(AwChannel::EEG) > 0)
	//	spEegHP->setValue(fm->highPass(AwChannel::EEG));
	//if (fm->highPass(AwChannel::MEG) > 0)
	//	spMegHP->setValue(fm->highPass(AwChannel::MEG));
	//if (fm->highPass(AwChannel::EMG) > 0)
	//	spEmgHP->setValue(fm->highPass(AwChannel::EMG));

	//for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
	//	m_lp[i] = fm->lowPass(i);
	//	m_hp[i] = fm->highPass(i);
	//}

	//checkEegHigh->setChecked(fm->highPass(AwChannel::EEG) > 0.);
	//checkEmgHigh->setChecked(fm->highPass(AwChannel::EMG) > 0.);
	//checkMegHigh->setChecked(fm->highPass(AwChannel::MEG) > 0.);
	//checkEegLow->setChecked(fm->lowPass(AwChannel::EEG) > 0.);
	//checkEmgLow->setChecked(fm->lowPass(AwChannel::EMG) > 0.);
	//checkMegLow->setChecked(fm->lowPass(AwChannel::MEG) > 0.);

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
}

void AwFilterSettings::show()
{
	setVisible(true);
}

void AwFilterSettings::apply()
{
	//AwFilteringManager *fm = AwFilteringManager::instance();
	float lp, hp, notch;
	bool ica_over = false, source_over = false;
	// EEG/SEEG
    hp = checkEegHigh->isChecked() ? spEegHP->value() : -1;
	lp = checkEegLow->isChecked() ? spEegLP->value() : -1;
	notch = cbNotchEEG->isChecked() ? spNotchEEG->value() : -1;
	AwFiltersManager *fm = AwFiltersManager::instance();

	//if (groupICA->isVisible()) {
	//	if (lp > 0 && spICAEEGLP->value() < lp) 
	//		ica_over = true;
	//	if (hp > 0 && spICAEEGHP->value() > hp)
	//		ica_over = true;
	//}
	//if (groupSource->isVisible()) {
	//	if (lp > 0 && spSourceEEGLP->value() < lp) 
	//		source_over = true;
	//	if (hp > 0 && spSourceEEGHP->value() > hp)
	//		source_over = true;
	//}

	fm->fo().eegHP = hp;
	fm->fo().eegLP = lp;
	fm->fo().eegNotch = notch;

	//if (lp != m_lp[AwChannel::EEG] || hp != m_hp[AwChannel::EEG]) {
	//	 m_lp[AwChannel::EEG] = lp;
	//	 m_hp[AwChannel::EEG] = hp;
	//	 fm->setFilter(AwChannel::EEG, lp, hp);
	//	 fm->setFilter(AwChannel::SEEG, lp, hp);
	//}
	//if (notch > 0) {
	//	fm->setNotch(AwChannel::EEG, notch);
	//	fm->setNotch(AwChannel::SEEG, notch);
	//}

	// EMG/ECG
    hp = checkEmgHigh->isChecked() ? spEmgHP->value() : -1;
	lp = checkEmgLow->isChecked() ? spEmgLP->value() : -1;
	notch = cbNotchEMG->isChecked() ? spNotchEMG->value() : -1;
	//if (groupICA->isVisible()) {
	//	if (lp > 0 && spICAEMGLP->value() < lp) 
	//		ica_over = true;
	//	if (hp > 0 && spICAEMGHP->value() > hp)
	//		ica_over = true;
	//}
	fm->fo().emgHP = hp;
	fm->fo().emgLP = lp;
	fm->fo().emgNotch = notch;

	//if (lp != m_lp[AwChannel::EMG] || hp != m_hp[AwChannel::EMG]) {
	//	 m_lp[AwChannel::EMG] = lp;
	//	 m_hp[AwChannel::EMG] = hp;
	//	 fm->setFilter(AwChannel::EMG, lp, hp);
	//	 fm->setFilter(AwChannel::ECG, lp, hp);
	//}
	//if (notch > 0) {
	//	fm->setNotch(AwChannel::EMG, notch);
	//	fm->setNotch(AwChannel::ECG, notch);
	//}
	//MEG
	hp = checkMegHigh->isChecked() ? spMegHP->value() : -1;
	lp = checkMegLow->isChecked() ? spMegLP->value() : -1;
	notch = cbNotchMEG->isChecked() ? spNotchMEG->value() : -1;
	//if (groupICA->isVisible()) {
	//	if (lp > 0 && spICAMEGLP->value() < lp) 
	//		ica_over = true;
	//	if (hp > 0 && spICAMEGHP->value() > hp)
	//		ica_over = true;
	//}
	//if (groupSource->isVisible()) {
	//	if (lp > 0 && spSourceMEGLP->value() < lp) 
	//		source_over = true;
	//	if (hp > 0 && spSourceMEGHP->value() > hp)
	//		source_over = true;
	//}
	fm->fo().megHP = hp;
	fm->fo().megLP = lp;
	fm->fo().megNotch = notch;

	//if (lp != m_lp[AwChannel::MEG] || hp != m_hp[AwChannel::MEG]) {
	//	 m_lp[AwChannel::MEG] = lp;
	//	 m_hp[AwChannel::MEG] = hp;
	//	 m_lp[AwChannel::GRAD] = lp;
	//	 m_hp[AwChannel::GRAD] = hp;
	//	 fm->setFilter(AwChannel::MEG, lp, hp);
	//	 fm->setFilter(AwChannel::GRAD, lp, hp);
	//}
	//if (notch > 0) {
	//	fm->setNotch(AwChannel::MEG, notch);
	//	fm->setNotch(AwChannel::GRAD, notch);
	//}

	//if (ica_over) {
	//	AwMessageBox::information(this, "ICA Warning", "The filters applied on signals differ from the settings applied when the ICA was computed.");
	//}
	//if (source_over) {
	//	AwMessageBox::information(this, "Source Warning", "The filters applied on signals differ from the settings applied when the Source channels were computed.");
	//}

//	emit filtersApplied();

	fm->update();
}