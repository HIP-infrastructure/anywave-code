/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <QString>
class AwBIDSSubject;
#include "AwFileItem.h"

class AwBIDSSession
{
public:
	explicit AwBIDSSession(AwBIDSSubject *subject, const QString& label);

	inline QString& fullPath() { return m_fullPath; }
	inline QString& label() { return m_label; }
	inline AwBIDSSubject *subject() { return m_sub; }
	AwFileItem *addItem(AwFileItem *item);
	AwFileItemList& fileItems() { return m_items; }
protected:
	QString m_label;
	QString m_fullPath;
	AwBIDSSubject *m_sub;
	AwFileItemList m_items;
};

using AwBIDSSessionList = QList<AwBIDSSession *>;