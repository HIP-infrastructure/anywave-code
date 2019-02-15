/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <AwCommandLine.h>

class AwBaseProcess;

class AwCommandLineManager
{
public:
	static void computeICA(AwArguments& arguments);
	static void convertFile(AwArguments& arguments);
	/** Instantiante a process given the plugin name. Also initialize the process PDI with default settings. Throw an exception if failed. **/
	/** if an inputFile is specified, the reader is instantiated and the file is open. The optional filter settings are also applied. **/
	/** Arguments cand be modified to add extra parameters based on input file argument for example. **/
	static AwBaseProcess *createAndInitNewProcess(const QString& pluginName, AwArguments& args, const QString& inputFile = QString());
};