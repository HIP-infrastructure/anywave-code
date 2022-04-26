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
#include "AwBIDSItem.h"

// modalities
static QStringList modalities = { "anat", "meg", "eeg", "ieeg" };

AwBIDSItem::AwBIDSItem(const QString& text, AwBIDSItem *subject) : QStandardItem(text)
{
	m_parent = subject;
	if (m_parent)
		m_parent->addChild(this);
	// set hidden false by defaukt
	setData(false, AwBIDSItem::HiddenRole);
}


///
/// gather children item that are Data files.
/// Must be called in a container item (sub, ses, meg, eeg, ieeg, etc.)
QList<AwBIDSItem *> AwBIDSItem::getDataFileItems()
{
	return getChildDataFileItems(this);
}

QString AwBIDSItem::subjectName()
{
	auto parent = m_parent;
	if (!parent)
		return this->text();
	while (parent) {
		if (parent->bidsParent() == nullptr) 
			return parent->text();
		parent = parent->bidsParent();
	}
	return QString();
}

///
/// recursive search amongst children
QList<AwBIDSItem *> AwBIDSItem::getChildDataFileItems(AwBIDSItem *parent)
{
	QList<AwBIDSItem *> res;
	for (auto child : parent->children()) {
		if (child->data(AwBIDSItem::TypeRole).toInt() == AwBIDSItem::DataFile)
			res << child;
		else
			res += getChildDataFileItems(child);
	}
	return res;
}