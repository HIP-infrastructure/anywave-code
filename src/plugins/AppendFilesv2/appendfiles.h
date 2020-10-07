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
#ifndef APPENDFILES_H
#define APPENDFILES_H

#include "appendfiles_global.h"
#include <AwProcessInterface.h>
#include <AwFileIO.h>
#include "settings.h"

class APPENDFILES_EXPORT AppendFiles : public AwProcess
{
	Q_OBJECT
public:
	AppendFiles();
	~AppendFiles();

	bool showUi() override;
	void run() override;
private:
	settings *m_ui;
	AwFileIO *m_writer;
	QMap<QString, AwFileIOPlugin*> m_writers;  // carefull: writers here are instances of plugin objects, not AwFileIO instances
	QMap<QString, AwFileIO*> m_readers;
};

class APPENDFILES_EXPORT AppendFilesPlugin : public AwProcessPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID AwProcessPlugin_IID)
	Q_INTERFACES(AwProcessPlugin)
public:
	AppendFilesPlugin();

	AW_INSTANTIATE_PROCESS(AppendFiles)

};


#endif // APPENDFILES_H
