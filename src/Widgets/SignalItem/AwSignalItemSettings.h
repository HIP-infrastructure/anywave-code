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
#ifndef AW_SIGNAL_SETTINGS_H
#define AW_SIGNAL_SETTINGS_H
#include <AwGlobal.h>
#include <QDialog>
#include "ui_AwSignalItemSettings.h"
#include <widget/AwGraphicsObjects.h>
class QSlider;
class QLineEdit;
class QLabel;
class AwGainLevel;
class AwChannel;

using namespace Ui;

class AW_WIDGETS_EXPORT AwSignalItemSettings : public AwGraphicsSignalDialog, public SignalSettingsClass
{
	Q_OBJECT

public:
	AwSignalItemSettings(AwGraphicsSignalItem *item = nullptr, QWidget *parent = 0);
	~AwSignalItemSettings();
public slots:
	int exec() override;
	void accept() override;
	void reject() override;
protected slots:
	//void downLevel();
	//void upLevel();
	void changeChannelSettings();
	void getSliderValue(int value);
	void getEditValue();
protected:
	QStringList m_levels;
	QVector<float> m_scale; // values of amplitude in the scale.
	AwChannel *m_copiedChannel;
	AwGraphicsSignalItem *m_sitem;
	QSlider* m_gainSlider;
	QLineEdit* m_gainEdit;
	QLabel* m_labelUnit;
	AwGainLevel* m_gainLevel;
	AwChannel* m_channel;
};

#endif // SIGNAL_SETTINGS_H
