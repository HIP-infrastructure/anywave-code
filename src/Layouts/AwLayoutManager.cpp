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
//    Author: Marmaduke Woodman – Laboratoire UMR INS INSERM 1106 - michael.woodman@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include <layout/AwLayoutManager.h>
#include <layout/AwLayout.h>
#include <QFile>
#include <QTextStream>
#include <QStringList>

AwLayoutManager *AwLayoutManager::m_instance = 0;
AwLayoutManager *AwLayoutManager::instance()
{
	if (!m_instance)
		m_instance = new AwLayoutManager();
	return m_instance;
}

// constructor
AwLayoutManager::AwLayoutManager()
{
	m_isInit = false;
}

void AwLayoutManager::init() {
	QFile file(":/layouts/layouts.txt");
	QTextStream stream(&file);

	if (file.open(QIODevice::ReadOnly|QIODevice::Text))	{
		while (!stream.atEnd()) {
			QString line = stream.readLine();
			QStringList tokens = line.split(",");

			if (tokens.size() != 4)
				continue;
			
			// check type
			bool is2D = tokens.at(1).trimmed().toUpper() ==  "2D";
			int type = is2D ? AwLayout::L2D : AwLayout::L3D;
			if (tokens.at(2).trimmed().toUpper() == "MEG")
				type |= AwLayout::MEG;
			else
				type |= AwLayout::EEG;

			// add new layout
			addLayout(new AwLayout(tokens.at(0).trimmed(), type, tokens.at(3).trimmed()));
		}
	}
	file.close();
	m_isInit = true;
}

AwLayoutManager::~AwLayoutManager()
{
	QList<AwLayout *> layouts = m_layouts.values();
	while (!layouts.isEmpty())
		delete layouts.takeLast();
}

void AwLayoutManager::addLayout(AwLayout *layout)
{
	if (layout->load() == 0)
		m_layouts.insert(layout->name(), layout);
	else // failed to load layout. Do not add it to the layout list.
		delete layout;
}

int AwLayoutManager::electrodesMatch(const AwChannelList& channels, AwLayout *layout)
{
	// search for matching electrode names
	int count = 0;
	int type = layout->isEEG() ? AwChannel::EEG : AwChannel::MEG;
	QStringList labels = layout->labels();
	foreach(AwChannel *c, channels) {
		if (c->type() == type)
			if (labels.contains(c->name()))
				count++;
	}
	return count;
}


AwLayout *AwLayoutManager::guessLayout(AwFileIO *reader, int flags)
{
	if (!m_isInit)
		init();

	AwLayout *layout = NULL;
	QList<QPair<AwLayout *, int> > res;
	int nChannels = 0;
	int matchingElectrodes = 0;
	// check flags
	if (flags & AwLayout::MEG) { // search for MEG layout
		nChannels = AwChannel::getChannelsOfType(reader->infos.channels(), AwChannel::MEG).size();
		if (flags & AwLayout::L2D)  { // search for 2D layout
			foreach (AwLayout *l, m_layouts.values()) {
				if (l->isMEG() && l->is2D()) {
					matchingElectrodes = electrodesMatch(reader->infos.channels(), l);
					if (matchingElectrodes)
						res << QPair<AwLayout *, int>(l, matchingElectrodes);
						//return l;
				}
			}
		}
		else if (flags & AwLayout::L3D) { // search for 3D layout
			foreach (AwLayout *l, m_layouts.values()) {
				if (l->isMEG() && l->is3D()) {
					matchingElectrodes = electrodesMatch(reader->infos.channels(), l);
					if (matchingElectrodes)
						res << QPair<AwLayout *, int>(l, matchingElectrodes);
				}
			}
		}
		else // Missing dimension information
			return NULL;
	}
	else if (flags & AwLayout::EEG) { // search for EEG layout
		nChannels = AwChannel::getChannelsOfType(reader->infos.channels(), AwChannel::EEG).size();
		if (flags & AwLayout::L2D)  { // search for 2D layout
			foreach (AwLayout *l, m_layouts.values()) {
				if (l->isEEG() && l->is2D()) {
					matchingElectrodes = electrodesMatch(reader->infos.channels(), l);
					if (matchingElectrodes)
						res << QPair<AwLayout *, int>(l, matchingElectrodes);
				}
			}
		}
		else if (flags & AwLayout::L3D) { // search for 3D layout
			foreach (AwLayout *l, m_layouts.values()) {
				if (l->isEEG() && l->is3D()) {
					matchingElectrodes = electrodesMatch(reader->infos.channels(), l);
					if (matchingElectrodes)
						res << QPair<AwLayout *, int>(l, matchingElectrodes);
				}
			}
		}
		else // Missing dimension information
			return NULL;
	}
	else // wrong flags
		return NULL;

	if (res.isEmpty())
		return NULL;

	// if several layouts found, try to find the one which has almost the same number of electrodes than then number of channels
	int delta = 1000;
	for (int i = 0; i < res.size(); i++) {
		AwLayout *l = res.at(i).first;
		int matches = res.at(i).second;
		if (qAbs(l->numberOfElectrodes() - nChannels) < delta && matches >= nChannels / 2) {
			delta = qAbs(l->numberOfElectrodes() - nChannels);
			layout = l;
		}
	}

	return layout;
}

QList<AwLayout *> AwLayoutManager::layouts()
{
	if (!m_isInit)
		init();
	return m_layouts.values();
}

//
// get layout from file reader depending on specified flags. 
// if the file reader provides one or several layouts which match flags then the corresponding layout is returned.
// Returns NULL if no layout was found.
AwLayout *AwLayoutManager::layoutFromFile(AwFileIO *reader, int flags)
{
	QStringList layouts = reader->plugin()->layouts;
	if (layouts.isEmpty())
		return NULL;

	AwLayout *res = NULL;
	foreach(QString l, layouts)	{
		res = layoutByName(l, flags);
		if (res)
			return res;
	}
	return NULL;
}

AwLayout *AwLayoutManager::layoutByName(const QString& name, int type)
{
	if (!m_isInit)
		init();

	QList<AwLayout *> res = m_layouts.values(name);
		
	if (res.isEmpty())
		return NULL;

	foreach(AwLayout *l, res)
		if (l->type() == type)
			return l;

	return NULL;
}