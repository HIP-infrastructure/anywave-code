/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
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
