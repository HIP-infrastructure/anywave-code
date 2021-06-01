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
#include "AwMappingButton.h"


AwMappingButton::AwMappingButton(AwTopoWidget *widget) : QPushButton(widget)
{
	setStyleSheet("QPushButton { color: blue; border-width: 1px; border-color: #339; padding:3px; \
				  border-style:solid; border-radius:7; min-width: 50px; font-family: Arial; font-size: 8pt;}");
	setText(tr("Map Settings"));
	connect(this, SIGNAL(clicked()), widget, SLOT(openUI()));
}