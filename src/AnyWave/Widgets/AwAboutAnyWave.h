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
#ifndef AW_ABOUT_ANYWAVE_H
#define AW_ABOUT_ANYWAVE_H

#include <QDialog>
#include "ui_AwAboutAnyWave.h"

class AwUpdateManager;

using namespace Ui;

class AwAboutAnyWave : public QDialog, public AboutAnyWaveClass
{
	Q_OBJECT

public:
	AwAboutAnyWave(AwUpdateManager *, QWidget *parent = 0);
protected:
	void changeEvent(QEvent*);
public slots:
	void openLicense();
	void getLatestUpdate();
private:
	AwUpdateManager* m_um;
};

#endif // ABOUT_ANYWAVE_H
