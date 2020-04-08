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
#ifndef MERGEFILES_H
#define MERGEFILES_H

#include "mergefiles_global.h"
#include <AwProcessInterface.h>
#include <AwFileIO.h>
#include <QtCore>
#include "settings.h"

class MERGEFILES_EXPORT MergeFile : public AwProcess
{
	Q_OBJECT
public:
	MergeFile();
	~MergeFile();

	void run();
	bool showUi();

private:
	AwFileIOPlugin *m_adesPlugin;	// ADES plugin to create result file.
	Settings *m_ui;

	void destroyFileReader(AwFileIO *reader);
};

class MERGEFILES_EXPORT MergeFilePlugin : public AwProcessPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID AwProcessPlugin_IID)
	Q_INTERFACES(AwProcessPlugin)
public:
	MergeFilePlugin();
	AW_INSTANTIATE_PROCESS(MergeFile)

};

#endif // MERGEFILES_H
