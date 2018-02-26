/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
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
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef FILECONVERTER_H
#define FILECONVERTER_H

#include "fileconverter_global.h"
#include <AwProcessInterface.h>
#include <AwFileIO.h>
#include "settings.h"
#include <QtCore>

class FILECONVERTER_EXPORT FileConverter : public AwProcess
{
	Q_OBJECT
public:
	FileConverter();
	~FileConverter();

	bool showUi();
	void run();

private:
	QHash<AwFileIOPlugin *, QStringList> m_inputs;
	settings *m_ui;
};

class FILECONVERTER_EXPORT FileConverterPlugin : public AwProcessPlugin
{
	Q_OBJECT
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	Q_PLUGIN_METADATA(IID AwProcessPlugin_IID)
#endif
	Q_INTERFACES(AwProcessPlugin)
public:
	FileConverterPlugin();

	FileConverter *newInstance() { return new FileConverter; }

};

#endif // FILECONVERTER_H
