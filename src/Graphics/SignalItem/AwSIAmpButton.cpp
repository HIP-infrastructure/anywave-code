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
#include "AwSIAmpButton.h"
#include <AwChannel.h>
#include "AwAmplitudeManager.h"
#include <graphics/AwGraphicsObjects.h>

AwSIAmpButton::AwSIAmpButton(int type, AwChannel *channel, AwGraphicsSignalItem *item, QWidget *parent)
	: QPushButton(parent)
{
	m_type = type;

	QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	setSizePolicy(sizePolicy);
	setMinimumSize(QSize(32, 32));
	setMaximumSize(QSize(32, 32));
	QIcon icon;
	if (type == AwSIAmpButton::Up)
		icon.addFile(QString::fromUtf8(":/images/plus_sign_24x24.png"), QSize(), QIcon::Normal, QIcon::Off);
	else
		icon.addFile(QString::fromUtf8(":/images/minus_sign_24x24.png"), QSize(), QIcon::Normal, QIcon::Off);

	setIcon(icon);
	setIconSize(QSize(32, 32));
	setFlat(true);
	setStyleSheet("background-color: transparent;");
	m_channel = channel;
	m_item = item;
	QString tt;
	if (type == AwSIAmpButton::Up)
		tt = tr("Raise Amplitude");
	else
		tt = tr("Lower Amplitude");
	setToolTip(tt);

}

AwSIAmpButton::~AwSIAmpButton()
{

}



void AwSIAmpButton::mouseReleaseEvent(QMouseEvent *e)
{
	AwAmplitudeManager *am = AwAmplitudeManager::instance();
//	QVector<float> amps = am->getAmplitudes(m_channel->type());
	QVector<float> amps = am->getScale(m_channel->type());
	int index = amps.indexOf(m_channel->gain());
	if (m_type == AwSIAmpButton::Up) {
		if (index <= 0)
			return;
		index--;
		m_channel->setGain(amps[index]);
	}
	else  {// down
		if (index == -1 || index == amps.size() - 1 )
			return;
		index++;
		m_channel->setGain(amps[index]);
	}
	m_item->repaint();
}