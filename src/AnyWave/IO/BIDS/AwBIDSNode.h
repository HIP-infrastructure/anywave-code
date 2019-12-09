
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
#include <QList>

class AwBIDSNode
{
public:
	explicit AwBIDSNode(const QString& parentDir, const QString& relativePath, int type, const QString& ID = QString());
	~AwBIDSNode();
	// items aht could be found in a BIDS structure:
	// Folder means a simple folder (for example MEG 4DNI data files are stored in folders)
	enum Types { subject, session, ieeg, meg, eeg, Folder};

	inline QString& fullPath() { return m_fullPath; }
	inline int type() { return m_type; }
	inline QString& ID() { return m_ID; }
	inline QList<AwBIDSNode *>& children() { return m_children; }
	inline QStringList& files() { return m_files; }


	AwBIDSNode * addChild(AwBIDSNode *node) { m_children.append(node); return node; }
	void addFiles(const QStringList& files) { m_files.append(files); }
protected:
	QString m_parentDir, m_ID, m_fullPath;
	// a node may contain files
	QStringList m_files;
	// a node may contain child nodes
	QList<AwBIDSNode *> m_children;
	int m_type;
};

using AwBIDSNodes = QList<AwBIDSNode *>;