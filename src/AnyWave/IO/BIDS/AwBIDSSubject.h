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
#include <QObject>
#include "AwBIDSSession.h"
#include "AwFileItem.h"

class AwBIDSSubject : public QObject
{
	Q_OBJECT
public:
	AwBIDSSubject(const QString& root, const QString& ID);
	~AwBIDSSubject();

	QString fullPath();
	inline QString& ID() { return m_ID; }
	inline bool hasSessions() { return !m_sessions.isEmpty(); }
	AwBIDSSession *addSession(const QString& label);
	AwFileItem *addItem(AwFileItem *item);
	AwBIDSSessionList& sessions() { return m_sessions; }
	AwFileItemList& fileItems() { return m_items; }
	QStringList findFile(const QString& filePath);
	AwFileItemList findFileItems(const QString& filePath);
	QString getDerivativesPath(int softwareKind);

protected:
	QString m_ID;
	//QString m_fullPath;
	QString m_rootDir;	// BIDS root folder path.
	AwBIDSSessionList m_sessions;
	QList<AwFileItem *> m_items;
};

using AwBIDSSubjectList = QList<AwBIDSSubject *>;
using AwBIDSSubjectDict = QHash<QString, AwBIDSSubject *>;