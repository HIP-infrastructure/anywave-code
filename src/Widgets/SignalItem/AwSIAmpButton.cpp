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
#include "AwSIAmpButton.h"
#include <AwChannel.h>
#include <widget/AwGraphicsObjects.h>
#include <widget/SignalView/AwViewSettings.h>

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
	// get gain levels for the channel
	Q_ASSERT(m_item->viewSettings() != nullptr);
	auto gl = m_item->viewSettings()->gainLevels->getGainLevelFor(m_channel->type());
	auto index = gl->getIndexOfValue(m_channel->gain());
	if (index != -1) {
		if (m_type == AwSIAmpButton::Up) {
			if (index <= 0)
				return;
			index--;
			m_channel->setGain(gl->values(index));
		}
		else {
			if (index > gl->values.size() - 1)
				return;
			index++;
			m_channel->setGain(gl->values(index));
		}
	}
	m_item->repaint();
}