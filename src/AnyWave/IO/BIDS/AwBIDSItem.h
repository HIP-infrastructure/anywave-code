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
#include <QStandardItem>


class AwBIDSItem : public QStandardItem
{
public:
	AwBIDSItem(const QString& text, AwBIDSItem *parent = nullptr);
	enum Types {
		Subject = 0, Session = 1,  Folder = 2, DataFile = 3, anat = 4, meg = 5, eeg = 6, ieeg = 7,
		// flags to use with DerivativesRole
		ica = 8, h2 = 16, gardel = 32
	};
	enum Roles { PathRole = Qt::UserRole + 1, TypeRole = Qt::UserRole + 2, RelativePathRole = Qt::UserRole + 3,
		DataTypeRole = Qt::UserRole + 4, OutputDirRole = Qt::UserRole + 5, DerivativesRole = Qt::UserRole + 6, 
		HiddenRole = Qt::UserRole + 7, GardelElectrodePathRole = Qt::UserRole + 8, GardelMeshPathRole = Qt::UserRole + 9,
		GardelMontagesRole = Qt::UserRole + 10 };
	inline QList<AwBIDSItem *>& children() { return m_children; }
	inline QStringList& files() { return m_files; }
	inline void addFile(const QString& filePath) { m_files.append(filePath); }
	void addChild(AwBIDSItem *child) { m_children.append(child); }
	QList<AwBIDSItem *> getDataFileItems();
	inline AwBIDSItem* bidsParent() { return m_parent; }
//	static QString modalityToString(int modality);
protected:
	AwBIDSItem *m_parent; // nullptr if the item IS the subject.
	QList<AwBIDSItem *> m_children;
	QStringList m_files;	// an item may contain data files 
	QList<AwBIDSItem *> getChildDataFileItems(AwBIDSItem *parent);
};

using AwBIDSItems = QList<AwBIDSItem *>;

