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
#ifndef REWRITE4D_H
#define REWRITE4D_H

#include "rewrite4d_global.h"
#include <AwProcessInterface.h>
#include <QtCore>

class REWRITE4D_EXPORT Rewrite4D : public AwProcess
{
	Q_OBJECT
public:
	Rewrite4D();
	~Rewrite4D();

	bool showUi();
	void run();

private:
	qint64 offsetFilePointer(const QFile& file);
	void alignFilePointer(QFile& file);
	QString m_newFilename;
	QStringList m_selectedMarkers;
	QStringList m_channelNames;
	QString m_badFile;
	QStringList m_badLabels;
};

class REWRITE4D_EXPORT Rewrite4DPlugin : public AwProcessPlugin
{
	Q_OBJECT
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	Q_PLUGIN_METADATA(IID AwProcessPlugin_IID)
#endif
	Q_INTERFACES(AwProcessPlugin)
public:
	Rewrite4DPlugin();

	Rewrite4D *newInstance() { return new Rewrite4D; }
};

#endif // REWRITE4D_H
