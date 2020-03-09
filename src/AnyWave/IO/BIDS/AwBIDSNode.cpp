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
#include "AwBIDSNode.h"

AwBIDSNode::AwBIDSNode(const QString& parentDir, const QString& relativePath, int type, const QString& ID)
{
	m_parentDir = parentDir;
	m_type = type;
	m_ID = ID;
	m_fullPath = QString("%1/%2").arg(parentDir).arg(relativePath);
	m_parent = nullptr;
}

AwBIDSNode::~AwBIDSNode()
{
	while (!m_children.isEmpty())
		delete m_children.takeFirst();
}

///
/// findNode: 
/// given a subject node, browse though children nodes to find the data file.
/// filePath must be the filename only not the full path.
AwBIDSNode *AwBIDSNode::findNode(const QString& filePath, AwBIDSNode *node)
{
	if (node->m_files.contains(filePath))
		return node;
	for (auto child : node->m_children)
		return findNode(filePath, child);
	return nullptr;
}

QStringList AwBIDSNode::findFiles(AwBIDSNode *node)
{
	QStringList res;
	if (node->m_children.isEmpty()) {
		res += node->files();
		return res;
	}
	for (auto child : node->m_children)
		return findFiles(child);
	return res;
}


QStringList AwBIDSNode::gatherFiles()
{
	return findFiles(this);
}