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
#include "AwMarkerLabelButton.h"

AwMarkerLabelButton::AwMarkerLabelButton(QWidget *parent)
	: QPushButton(parent)
{
	setStyleSheet("QPushButton { color: darkGreen; border-width: 1px; border-color: #339; padding:3px; border-style:solid; border-radius:7; }");
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

AwMarkerLabelButton::~AwMarkerLabelButton()
{

}


void AwMarkerLabelButton::setText(const QString &text)
{
	setToolTip(text);
	QFontMetrics fm(font());
	int width;
	if (text.size() > 10)
	{
		QString newText = text;
		newText.resize(10);
		newText += "...";
		QPushButton::setText(newText);
		width = fm.width(newText);
		setMaximumWidth(fm.width(newText) + 10);
	}
	else
	{
		QPushButton::setText(text);
		width = fm.width(text);
		setMaximumWidth(fm.width(text) + 10);
	}
}