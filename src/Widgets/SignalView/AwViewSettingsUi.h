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
#ifndef AWVIEWSETTINGSUI_H
#define AWVIEWSETTINGSUI_H
#include <AwGlobal.h>
#include <QDialog>
#include "ui_AwViewSettingsUi.h"

class AwViewSettings;

class AW_WIDGETS_EXPORT AwViewSettingsUi : public QDialog, public Ui::AwViewSettingsUiClass
{
	Q_OBJECT

public:
	AwViewSettingsUi(AwViewSettings *settings, QWidget *parent = 0);
public slots:
	void selectAllFilters();
	void unselectAllFilters();
	void updateMaxChannels(bool flag);
	void accept();
	int exec();
signals:
//	void settingsChanged(AwViewSettings *settings, int flags);
	void settingsChanged(int key, int sender);
protected:
	AwViewSettings *m_settings;
	QMap<QCheckBox *, int> m_checkBoxes;
};

#endif // AwViewSettingsUi
