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
#ifndef AWGTCMENU_H
#define AWGTCMENU_H

#include <QMenu>

/** AwGTCMenu: Build a menu with sub menus depending on labels of channels.
Build a letter sub menu grouping all channels starting with the same letter. **/

class AwGTCMenu : public QMenu
{
	Q_OBJECT

public:
	AwGTCMenu(const QString &title, QWidget *parent = 0);
	~AwGTCMenu();

	void updateLabels(const QStringList& labels);
private:
	
};

#endif // AWGTCMENU_H
