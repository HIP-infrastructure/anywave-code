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
#ifndef AWICAREJECTBUTTON_H
#define AWICAREJECTBUTTON_H

#include <QPushButton>
#include "AwICASignalItem.h"

class AwICARejectButton : public QPushButton
{
	Q_OBJECT

public:
	AwICARejectButton(bool rejected, AwICASignalItem *item, QWidget *parent = 0);
	~AwICARejectButton();

	void updateRejectState(bool rejected);	// do not change the item state, just the button state.
public slots:
	void reject();
private:
	AwICASignalItem *m_item;
	bool m_rejected;
};

#endif // AWICAREJECTBUTTON_H
