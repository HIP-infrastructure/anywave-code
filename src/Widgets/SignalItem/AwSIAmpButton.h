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
#ifndef AWSIAMPBUTTON_H
#define AWSIAMPBUTTON_H
#include <AwGlobal.h>
#include <QPushButton>
class AwChannel;
class AwGraphicsSignalItem;

class AW_WIDGETS_EXPORT AwSIAmpButton : public QPushButton
{
	Q_OBJECT

public:
	AwSIAmpButton(int type, AwChannel *channel, AwGraphicsSignalItem *item, QWidget *parent = 0);
	~AwSIAmpButton();
	enum Type { Up, Down };
protected:
	void mouseReleaseEvent(QMouseEvent *event) override;
private:
	int m_type;
	AwGraphicsSignalItem *m_item;
	AwChannel *m_channel;
};

#endif // AWSIAMPBUTTON_H
