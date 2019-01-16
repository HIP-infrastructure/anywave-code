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
#ifndef AWMATLABSUPPORT_H
#define AWMATLABSUPPORT_H

#include "awmatlabsupport_global.h"
#include <AwMatlabInterface.h>
#include <engine.h>


class AWMATLABSUPPORT_EXPORT AwMatlabSupport : public AwMatlabInterface
{
	Q_OBJECT
	Q_INTERFACES(AwMatlabInterface)
	Q_PLUGIN_METADATA(IID AwMatlabInterface_IID)
public:
	/** run a matlab script **/
	void run(const QString& path, const QString& dep, int pid, quint16 serverPort) override;
signals:
	void progressChanged(const QString&);
private:
	Engine *m_eng;
};

#endif // AWMATLABSUPPORT_H
