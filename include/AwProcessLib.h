// AnyWave
// Copyright (C) 2013-2021  INS UMR 1106
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#ifndef AWPROCESSLIB_H
#define AWPROCESSLIB_H
#include <AwProcessIO.h>
#include <AwMatlabInterface.h>
#include <AwGlobal.h>

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
	AwProcessDataInterface() { m_mi = nullptr;  }
	~AwProcessDataInterface() {}
	enum inputs { AnyChannels = -1 };

	// Parameters
	AwProcessIO input;
	AwProcessIO output;

	QString outputDirectory();

	// Methods
	/** Specify the type of channels required by the process as input. **/
	void addInputChannel(int type, int min, int max);
	bool areInputChannelSet() { return !m_inputChannels.isEmpty(); }
	QList<int> getInputChannels() { return m_inputChannels.keys(); }
	inline QPair<int, int> getInputChannelMinMax(int type) { return m_inputChannels.value(type); }

	inline bool hasOutput() { return !output.isEmpty(); }
	inline bool hasOutputWidgets() { return !output.widgets().isEmpty(); }
	inline AwMatlabInterface *MATLAB() { return m_mi; }
	inline void setMI(AwMatlabInterface *mi) { m_mi = mi; }
protected:		
	AwMatlabInterface *m_mi;
	QMap<int, QPair<int, int>> m_inputChannels;  // if empty => accept any channels (1-n)
};



#endif // AWPROCESSLIB_H
