/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This libray is free software; you can redistribute it and/or
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
#ifndef AWPROCESSLIB_H
#define AWPROCESSLIB_H
#include <AwProcess.h>
#include <QElapsedTimer>
#include <QDate>
#include <QWaitCondition>
#include <AwChannel.h>
#include <AwMarker.h>
#include <AwFilteringOptions.h>
#include <AwProcessIO.h>
#include <AwMatlabInterface.h>

/*!
 * \brief
 * This class defines the structure of inputs/ouputs for a process.
 * AwProcess encapsulates this class to manage data exchanges between AnyWave and a process.
 * AwProcessDataInterface manages input and ouput channels, input and output markers.
 * 
 * Parameters syntax to tell AnyWave what to do with a process:
 * - Parameter is a QString associated with a QVariant. That makes a pair of object used to define a parameter.
 * - Parameter example:
 * - "Meg channels","n"
 *
 * First element is a QString containing: Meg channels.
 * That means that the process requires channels of type MEG.
 * Second element is also a QString containing "n".
 * That means that the process requires n channels. => any number of channels of type MEG.
 *
 * 
 * \remarks
 * This class is used in process objects and in AwPluginManager.
 * When building a new process, you must set at least ONE input parameter (a pair of QString,QVariant).
 * Smater place to do it is in process'es constructor of your derived class.
 * 
 * \see
 * AwProcess | AwPluginManager | AwChannel | AwMarker
 */
class AW_PROCESS_EXPORT AwProcessDataInterface
{
public:
	AwProcessDataInterface() { m_mi = NULL; }
	~AwProcessDataInterface() {}

	// Parameters
	AwProcessIO input;
	AwProcessIO output;

	// Methods
	QMap<int, QVariant>& inputParameters() { return m_inputParameters; }
	QMap<int, QVariant>& outputParameters() { return m_outputParameters; }

	/** Sets an input parameter. **/
	void addInputParameter(int parameter, QVariant value) 
		{ m_inputParameters.insert(parameter, value); }
	/** Sets an output parameter **/
	void addOutputParameter(int parameter, QVariant value)
		{ m_outputParameters.insert(parameter, value); }


	inline bool hasOutput() { return !output.isEmpty(); }
	inline bool hasOutputWidgets() { return !output.widgets.isEmpty(); }
	inline AwMatlabInterface *MATLAB() { return m_mi; }
	inline void setMI(AwMatlabInterface *mi) { m_mi = mi; }
protected:		
	AwMatlabInterface *m_mi;
	QMap<int, QVariant> m_inputParameters;
	QMap<int, QVariant> m_outputParameters;
};



#endif // AWPROCESSLIB_H
