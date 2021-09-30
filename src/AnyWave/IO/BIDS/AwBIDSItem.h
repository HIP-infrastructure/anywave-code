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
#pragma once
#include <QStandardItem>


class AwBIDSItem : public QStandardItem
{
public:
	AwBIDSItem(const QString& text, AwBIDSItem *parent = nullptr);

	enum Types {
		Subject = 0, Session = 1,  Folder = 2, DataFile = 3, anat = 4, meg = 5, eeg = 6, ieeg = 7, SourceFolder = 8,
		// flags to use with DerivativesRole
		ica = 16, h2 = 32, gardel = 64, freesurfer = 128
	};
	enum Roles { PathRole = Qt::UserRole + 1, TypeRole = Qt::UserRole + 2, RelativePathRole = Qt::UserRole + 3,
		DataTypeRole = Qt::UserRole + 4, OutputDirRole = Qt::UserRole + 5, DerivativesRole = Qt::UserRole + 6, 
		HiddenRole = Qt::UserRole + 7, GardelElectrodePathRole = Qt::UserRole + 8, FreesurferMeshesRole = Qt::UserRole + 9,
		GardelMontagesRole = Qt::UserRole + 10, ChannelsTsvRole = Qt::UserRole + 11, EventsTsvRole = Qt::UserRole + 12 };
	inline QList<AwBIDSItem *>& children() { return m_children; }
	inline QStringList& files() { return m_files; }
	inline void addFile(const QString& filePath) { m_files.append(filePath); }
	void addChild(AwBIDSItem *child) { m_children.append(child); }
	void addChildren(const QList<AwBIDSItem*>& items) { m_children += items; }
	QList<AwBIDSItem *> getDataFileItems();
	inline AwBIDSItem* bidsParent() { return m_parent; }
	QString subjectName();

protected:
	AwBIDSItem *m_parent; // nullptr if the item IS the subject.
	QList<AwBIDSItem *> m_children;
	QStringList m_files;	// an item may contain data files 
	QList<AwBIDSItem *> getChildDataFileItems(AwBIDSItem *parent);
};
using AwBIDSItems = QList<AwBIDSItem*>;


