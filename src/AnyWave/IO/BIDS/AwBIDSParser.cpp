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
#include "AwBIDSParser.h"
#include "AwBIDSManager.h"
#include "AwBIDSTools.h"
#include <QFile>
#include <QDirIterator>
#include <QStringList>
#include "Plugin/AwPluginManager.h"
#include <QThread>

AwBIDSParser::AwBIDSParser(QObject *parent)
	: QObject(parent)
{
	m_rootDir = AwBIDSManager::instance()->rootDir();
	// Get extensions readers can handle.
	auto pm = AwPluginManager::getInstance();
	for (auto r : pm->readers())
		m_fileExtensions += r->fileExtensions;
}

AwBIDSParser::~AwBIDSParser()
{
}

void AwBIDSParser::browseSubjects()
{
	QDirIterator it(m_rootDir, QDir::Dirs);
	QRegularExpression re("^(?<subject>sub-)(?<ID>\\w+)$");
	QRegularExpressionMatch match;
	while (it.hasNext()) {
		it.next();
		QString name = it.fileName();
		match = re.match(name);
		if (match.hasMatch()) {
			auto ID = match.captured("ID");
			m_nodes.append(new AwBIDSNode(m_rootDir, name, AwBIDSNode::subject, ID));
		}
	}
}

///
/// parseNode:
/// recursive search through a subject node.
/// the first call to this method must use a subject node.
void AwBIDSParser::parseNode(AwBIDSNode *node)
{
	QDir dir(node->fullPath());
	QStringList dirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

	for (auto d : dirs) {
		if (d.startsWith("ses-")) {
			// get session label
			auto label = AwBIDS::getSessionLabel(d);
			auto child = node->addChild(new AwBIDSNode(node->fullPath(), d, AwBIDSNode::session, label));
			parseNode(child);
		}
		// not session, check for other items
		else if (d == "ieeg") {
			auto child = node->addChild(new AwBIDSNode(node->fullPath(), d, AwBIDSNode::ieeg));
			parseNode(child);
		}
		else if (d == "eeg") {
			auto child = node->addChild(new AwBIDSNode(node->fullPath(), d, AwBIDSNode::eeg));
			parseNode(child);
		}
		else if (d == "meg") {
			auto child = node->addChild(new AwBIDSNode(node->fullPath(), d, AwBIDSNode::meg));
			parseNode(child);
		}
		else { // a simple non BIDS folder 
			auto child = node->addChild(new AwBIDSNode(node->fullPath(), d, AwBIDSNode::Folder));
			parseNode(child);
		}
	}
	// get the files and put them into the hash table.
	auto files = dir.entryList(m_fileExtensions, QDir::Files);
	node->addFiles(files);
	for (auto f : files) {
		m_hash.insert(f, node);
	}
}

void AwBIDSParser::parse()
{
	browseSubjects();
	// now parse for sub folder. Extract information AnyWave can handle: ieeg, meg files, ica etc
	for (auto n : m_nodes) 
		parseNode(n);
}

