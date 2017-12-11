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
#ifndef AWMATLABPROCESS_H
#define AWMATLABPROCESS_H

#include "AwProcessInterface.h"

class QSharedMemory;
class AwMatlabMCRManager;

class AwMatlabProcess : public AwProcess
{
	Q_OBJECT
public:
	AwMatlabProcess() : AwProcess() { m_mcr = NULL; m_sm = NULL;}
	~AwMatlabProcess() {}
	inline AwMatlabMCRManager* mcr() { return m_mcr; }
	virtual bool initLib();
	virtual void closeLib() {}
protected:
	bool initMatlabCommunication();
	void endMatlabCommunication();
	QSharedMemory *m_sm;
};


///
/// AwMatlabProcessPlugin
/// Base class for matlab process plugins.
class AwMatlabProcessPlugin : public AwProcessPlugin
{
	Q_OBJECT
	Q_INTERFACES(AwProcessPlugin)
public:
	AwMatlabProcessPlugin();
	virtual ~AwMatlabProcessPlugin();
	virtual AwMatlabProcess *newInstance() { return NULL; }
	void setMCR(AwMatlabMCRManager *mcr);
	inline bool isLibInitialized() { return m_processLibInit; }
	void setLibInit()	{ m_processLibInit = true; }
protected:
	AwMatlabMCRManager *m_mcr;
	bool m_processLibInit;
	void initMCR();
};


#endif
