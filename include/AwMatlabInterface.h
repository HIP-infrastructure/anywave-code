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
#ifndef AW_MATLAB_INTERFACE_H_
#define AW_MATLAB_INTERFACE_H_
#include <QObject>

namespace matlab_interface {
	constexpr auto pid = "pid";
	constexpr auto port = "port";
	constexpr auto json = "json";
	constexpr auto matlab_api_dir = "matlab_api_dir";
	constexpr auto matlab_mex_dir = "matlab_mex_dir";
	constexpr auto python_exe = "python_exe";
	constexpr auto python_venv_dir = "python_venv_dir";
	constexpr auto matlab_plugin_dir = "matlab_plugin_dir";
	
}

class AwMatlabInterface : public QObject
{
public:
	virtual void run(const QVariantMap& settings) = 0;
	
signals:
	void progressChanged(const QString&);
};

#define AwMatlabInterface_IID "AnyWave.MATLABInterface"
Q_DECLARE_INTERFACE(AwMatlabInterface,  AwMatlabInterface_IID)

#endif
