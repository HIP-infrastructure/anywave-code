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
#include "AwBIDSFileItem.h"
#include <QFileInfo>

AwBIDSFileItem::AwBIDSFileItem(AwBIDSFileItem::FileType type)
{
	m_type = type;
}


///
/// tsvFile:
/// 
QString AwBIDSFileItem::tsvFile(AwBIDSFileItem::TsvFiles tsvFile)
{
	QFileInfo fi(m_fullPath);
	auto base = fi.baseName();
	switch (m_type) {
	case  AwBIDSFileItem::FileType::eeg:
		base = base.remove("_eeg");
		break;
	case  AwBIDSFileItem::FileType::ieeg:
		base = base.remove("_ieeg");
		break;
	case  AwBIDSFileItem::FileType::meg:
		base = base.remove("_meg");
		break;
	}
	QString tsv;
	switch (tsvFile) {
	case AwBIDSFileItem::TsvFiles::channels:
		tsv = "channels.tsv";
		break;
	case AwBIDSFileItem::TsvFiles::events:
		tsv = "events.tsv";
		break;
	}
	auto res = QString("%1/%2_%3").arg(m_fullPath).arg(base).arg(tsv);
	if (QFile::exists(res))
		return res;
	return QString();
}