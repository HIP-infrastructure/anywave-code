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
#include "AwSignalItemSettings.h"
#include <AwAmplitudeManager.h>


AwSignalItemSettings::AwSignalItemSettings(AwGraphicsSignalItem *item, QWidget *parent)
	: AwGraphicsSignalDialog(item, parent)
{
	setupUi(this);	
}


int AwSignalItemSettings::exec()
{
	AwChannel *chan = m_item->channel();
	if (!chan)
		QDialog::done(QDialog::Rejected);

	// init channel name
	QString label = chan->name();
	if (chan->hasReferences())
		label += " - " + chan->referenceName();

	labelChannel->setText(label);
	labelType->setText(AwChannel::typeToString(chan->type()));

	// init combobox
	int i = 0;
	foreach (QString color, QColor::colorNames()) {
		cb_Couleur->insertItem(i, color, i);
		cb_Couleur->setItemData(i, QColor(color), Qt::DecorationRole);
		i++;
	}

	QString color_name = chan->color();
	int index = QColor::colorNames().indexOf(color_name);
	if (index == -1)
		index =  QColor::colorNames().indexOf("black");
	cb_Couleur->setCurrentIndex(QColor::colorNames().indexOf(color_name));

	// Init parametres filtrage
	if (chan->highFilter() != -1)	{
		checkHighPass->setChecked(true);
		sbHigh->setEnabled(true);
		sbHigh->setValue(chan->highFilter());
	}
	else {
		checkHighPass->setChecked(false);
		sbHigh->setEnabled(false);
	}
	
	if (chan->lowFilter() != -1) {
		checkLowPass->setChecked(true);
		sbLow->setEnabled(true);
		sbLow->setValue(chan->lowFilter());
	}
	else {
		checkLowPass->setChecked(false);
		sbLow->setEnabled(false);
	}
	AwAmplitudeManager *am = AwAmplitudeManager::instance();
	spGainLevel->setValue(am->amplitude(m_item->channel()->type()));
	spGainLevel->setSuffix(am->unitOfChannel(m_item->channel()->type()));
	return QDialog::exec();
}

void AwSignalItemSettings::accept()
{
	AwChannel *chan = m_item->channel();

	chan->setColor(QColor::colorNames().at(cb_Couleur->currentIndex()));
	if (checkLowPass->isChecked())
		chan->setLowFilter((float)sbLow->value());
	else
		chan->setLowFilter(-1);
	if (checkHighPass->isChecked())
		chan->setHighFilter((float)sbHigh->value());
	else
		chan->setHighFilter(-1);

	// set params to parents
	// This way, parent of the channel will also have their settings changed.
	// The first parent is the same channel in the current montage.
	AwChannel *parent = chan->parent();
	while (parent)	{
		parent->setColor(chan->color());
		parent->setLowFilter(chan->lowFilter());
		parent->setHighFilter(chan->highFilter());
		parent->setGain(chan->gain());
		parent->setColor(chan->color());
		parent = parent->parent();
	}
//	AwAmplitudeManager *am = AwAmplitudeManager::instance();
//	QVector<float> amps = am->getScale(chan->type());
//	chan->setGain(amps[comboAmp->currentIndex()]);
	chan->setGain((float)spGainLevel->value());
	
	QDialog::accept();
}