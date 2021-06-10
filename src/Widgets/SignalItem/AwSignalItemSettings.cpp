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
#include <qgraphicsscene.h>
#include <widget/SignalView/AwViewSettings.h>
#include <QLineEdit>
#include <AwGainLevels.h>

AwSignalItemSettings::AwSignalItemSettings(AwGraphicsSignalItem *item, QWidget *parent)
	: AwGraphicsSignalDialog(item, parent)
{
	setupUi(this);	
	m_copiedChannel = nullptr; 
	m_sitem = item;
	m_channel = nullptr;
	m_gainEdit = nullptr;
	m_gainSlider = nullptr;
	m_labelUnit = nullptr;
}

AwSignalItemSettings::~AwSignalItemSettings()
{
	if (m_copiedChannel)
		delete m_copiedChannel;
}


int AwSignalItemSettings::exec()
{
	m_channel= m_item->channel();
	if (!m_channel)
		QDialog::done(QDialog::Rejected);
	m_copiedChannel = m_channel->duplicate();

	// init channel name
	labelChannel->setText(m_channel->fullName());
	labelType->setText(AwChannel::typeToString(m_channel->type()));

	// init combobox
	int i = 0;
	for (auto color : QColor::colorNames()) {
		cb_Couleur->insertItem(i, color, i);
		cb_Couleur->setItemData(i, QColor(color), Qt::DecorationRole);
		i++;
	}

	QString color_name = m_channel->color();
	int index = QColor::colorNames().indexOf(color_name);
	if (index == -1)
		index =  QColor::colorNames().indexOf("black");
	cb_Couleur->setCurrentIndex(QColor::colorNames().indexOf(color_name));

	// Init parametres filtrage
	checkHighPass->setChecked(m_channel->highFilter() > 0);
	checkLowPass->setChecked(m_channel->lowFilter() > 0);
	checkNotch->setChecked(m_channel->notch() > 0);
	sbHigh->setValue(m_channel->highFilter());
	sbLow->setValue(m_channel->lowFilter());
	sbNotch->setValue(m_channel->notch());

	// create gain levels widget in the dedicated layout
	m_gainLevel = m_sitem->viewSettings()->gainLevels->getGainLevelFor(m_channel->type());

	// create widgets to handle gain level for the channel
	m_gainSlider = new QSlider(Qt::Horizontal);
	m_gainSlider->setMaximum(m_gainLevel->values.n_elem);
	m_gainSlider->setValue(m_gainLevel->getIndexOfValue(m_gainLevel->value));
	m_gainEdit = new QLineEdit;
	m_gainEdit->setText(QString("%1").arg(m_gainLevel->value));
	m_labelUnit = new QLabel;
	m_labelUnit->setText(m_gainLevel->unit);
	gainLevelsLayout->addWidget(m_gainSlider);
	gainLevelsLayout->addWidget(m_gainEdit);
	gainLevelsLayout->addWidget(m_labelUnit);
	gainLevelsLayout->setStretch(0, 1);
	connect(m_gainSlider, &QSlider::valueChanged, this, &AwSignalItemSettings::getSliderValue);
	connect(m_gainEdit, &QLineEdit::editingFinished, this, &AwSignalItemSettings::getEditValue);
	// end of gain level
	connect(cb_Couleur, SIGNAL(currentIndexChanged(int)), this, SLOT(changeChannelSettings()));
	return QDialog::exec();
}

void AwSignalItemSettings::getSliderValue(int value)
{
	disconnect(m_gainEdit, nullptr, this, nullptr);
	m_channel->setGain(m_gainLevel->values(value)); // here set directly the gain to the original channel and make it appears in the view
	m_gainEdit->setText(QString("%1").arg(m_gainLevel->values(value)));
	connect(m_gainEdit, &QLineEdit::editingFinished, this, &AwSignalItemSettings::getEditValue);
	m_sitem->repaint();
	m_sitem->scene()->update();
}

void AwSignalItemSettings::getEditValue()
{
	disconnect(m_gainSlider, nullptr, this, nullptr);
	float value = m_gainEdit->text().toFloat();
	int index = m_gainLevel->getIndexOfValue(value);
	if (index == -1)
		index = m_gainLevel->insertNewValue(value);
	m_channel->setGain(value);  // here set directly the gain to the original channel and make it appears in the view
	m_gainSlider->setValue(index);
	connect(m_gainSlider, &QSlider::valueChanged, this, &AwSignalItemSettings::getSliderValue);
	m_sitem->repaint();
	m_sitem->scene()->update();
}

void AwSignalItemSettings::reject()
{
	m_channel->setColor(m_copiedChannel->color());
	m_channel->setGain(m_copiedChannel->gain());
	// reset filters also
	m_channel->setLowFilter(m_copiedChannel->lowFilter());
	m_channel->setHighFilter(m_copiedChannel->highFilter());
	m_channel->setNotch(m_copiedChannel->notch());
	QDialog::reject();
}

void AwSignalItemSettings::accept()
{
	m_channel->setColor(QColor::colorNames().at(cb_Couleur->currentIndex()));
	if (checkLowPass->isChecked())
		m_channel->setLowFilter((float)sbLow->value());
	else
		m_channel->setLowFilter(-1);
	if (checkHighPass->isChecked())
		m_channel->setHighFilter((float)sbHigh->value());
	else
		m_channel->setHighFilter(-1);

	// set params to parents
	// This way, parent of the channel will also have their settings changed.
	// The first parent is the same channel in the current montage.
	AwChannel *parent = m_channel->parent();
	while (parent)	{
		parent->setColor(m_channel->color());
		parent->setLowFilter(m_channel->lowFilter());
		parent->setHighFilter(m_channel->highFilter());
		parent->setGain(m_channel->gain());
		parent->setColor(m_channel->color());
		parent = parent->parent();
	}
	QDialog::accept();
}


void AwSignalItemSettings::changeChannelSettings()
{
	m_channel->setColor(QColor::colorNames().at(cb_Couleur->currentIndex()));
	m_sitem->repaint();
	m_sitem->scene()->update();
}
