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
#include "AwSignalItemSettings.h"
#include <AwAmplitudeManager.h>
#include <qgraphicsscene.h>

AwSignalItemSettings::AwSignalItemSettings(AwGraphicsSignalItem *item, QWidget *parent)
	: AwGraphicsSignalDialog(item, parent)
{
	setupUi(this);	
	m_copiedChannel = new AwChannel(item->channel());
	m_sitem = item;
}

AwSignalItemSettings::~AwSignalItemSettings()
{
	delete m_copiedChannel;
}


int AwSignalItemSettings::exec()
{
	AwChannel *chan = m_item->channel();
	if (!chan)
		QDialog::done(QDialog::Rejected);

	// init channel name
	labelChannel->setText(chan->fullName());
	labelType->setText(AwChannel::typeToString(chan->type()));

	// init combobox
	int i = 0;
	for (auto color : QColor::colorNames()) {
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
	checkHighPass->setChecked(chan->highFilter() > 0);
	checkLowPass->setChecked(chan->lowFilter() > 0);
	checkNotch->setChecked(chan->notch() > 0);
	sbHigh->setValue(chan->highFilter());
	sbLow->setValue(chan->lowFilter());
	sbNotch->setValue(chan->notch());

	AwAmplitudeManager *am = AwAmplitudeManager::instance();
	// fill combo box for levels
	m_scale = am->getScale(chan->type());
	for (auto item : m_scale) {
		m_levels << QString("%1 %2/cm").arg(item).arg(chan->unit());
	}
	comboLevels->addItems(m_levels);
	index = m_scale.indexOf(chan->gain());
	if (index != -1)
		comboLevels->setCurrentIndex(index);

	connect(buttonDown, &QPushButton::clicked, this, &AwSignalItemSettings::upLevel);
	connect(buttonUp, &QPushButton::clicked, this, &AwSignalItemSettings::downLevel);
	// instant update for color and gain level.
	connect(comboLevels, SIGNAL(currentIndexChanged(int)), this, SLOT(changeChannelSettings()));
	connect(cb_Couleur, SIGNAL(currentIndexChanged(int)), this, SLOT(changeChannelSettings()));
	return QDialog::exec();
}

void AwSignalItemSettings::reject()
{
	auto channel = m_item->channel();
	channel->setColor(m_copiedChannel->color());
	channel->setGain(m_copiedChannel->gain());
	QDialog::reject();
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
	chan->setGain(m_scale.at(comboLevels->currentIndex()));
	QDialog::accept();
}


void AwSignalItemSettings::changeChannelSettings()
{
	auto chan = m_item->channel();
	chan->setColor(QColor::colorNames().at(cb_Couleur->currentIndex()));
	chan->setGain(m_scale.at(comboLevels->currentIndex()));
	m_sitem->repaint();
	m_sitem->scene()->update();
}


void AwSignalItemSettings::downLevel()
{
	int index = comboLevels->currentIndex();
	if (index > 0) 
		comboLevels->setCurrentIndex(index - 1);
}

void AwSignalItemSettings::upLevel()
{
	int index = comboLevels->currentIndex();
	if (index < comboLevels->count())
		comboLevels->setCurrentIndex(index + 1);
}
