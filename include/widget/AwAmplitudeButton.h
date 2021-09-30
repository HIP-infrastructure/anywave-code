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
#ifndef AWAMPLITUDEBUTTON_H
#define AWAMPLITUDEBUTTON_H
#include <AwGlobal.h>
#include <QPushButton>
#include <QActionGroup>
#include <AwChannel.h>
class QMenu;
class AwAmplitudeManager;

class AW_WIDGETS_EXPORT AwAmplitudeButton : public QPushButton
{
	Q_OBJECT

public:
	AwAmplitudeButton(QWidget *parent);
	~AwAmplitudeButton();
signals:
	void amplitudeChanged(int, float);
	void amplitudesChanged();
private slots:
	void changeAmplitude(bool checked);
public slots:
	void setAmplitudes();
	void setAmplitude(int, float);
	void up();			// raise signal amplitudes (reduces the amplitude scale).
	void down();		// lower signal amplitudes (raises the amplitude scale).
	void switchGainLevelToChannel(int type);
private:
	void upActions(QActionGroup *group);
	void downActions(QActionGroup *group);
	void createActionGroup(QMenu *menu, int type);

	AwAmplitudeManager *m_am;
	QMenu *m_menu;
	QActionGroup *m_groups[AW_CHANNEL_TYPES];
	int m_gainLevelForChannel;	
};

#endif // AWAMPLITUDEBUTTON_H
