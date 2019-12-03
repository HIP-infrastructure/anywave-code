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

#include <QStringList>
class AwFileItem
{
public:
	explicit AwFileItem(int type);
	enum Types { ieeg, eeg, meg, ica, mesh, h2 };
	enum TsvFiles  { channelsTsv, eventsTsv };

	void setFullPath(const QString& path) { m_fullPath = path; }
	void setFiles(const QStringList& files) { m_files = files; }
	inline int type() { return m_type; }
	inline QStringList& files() { return m_files; }
	inline QString& fullPath() { return m_fullPath; }
	/** Returns the possibly file name for the given BIDS file for the current item **/
	QString getTsvFileFor(const QString& fileName, int tsvType);
protected:
	int m_type;
	QStringList m_files;	// files present in the iEEG directory
	QString m_fullPath;
};

typedef QList<AwFileItem *> AwFileItemList;