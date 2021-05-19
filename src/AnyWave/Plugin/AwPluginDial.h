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
#ifndef AW_PLUGINDIAL_H
#define AW_PLUGINDIAL_H

#include <QDialog>
#include "ui_AwPluginDial.h"

class AwFileIOPlugin;
class AwProcessPlugin;
class AwDisplayPlugin;


using namespace Ui;

class AwPluginDial : public QDialog, public PluginDialClass
{
	Q_OBJECT

public:
	AwPluginDial(QWidget *parent = 0);
protected:
	void changeEvent(QEvent *e) {
		if (e)
			if (e->type() == QEvent::LanguageChange)
				retranslateUi(this);
	}
private:
	QIcon m_interfaceIcon;
	QIcon m_featureIcon;
};

#endif // PLUGINDIAL_H
