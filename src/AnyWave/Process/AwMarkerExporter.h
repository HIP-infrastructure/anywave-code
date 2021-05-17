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
#pragma once

#include <AwProcessInterface.h>
#include <math/AwMath.h>
#include <matlab/AwMATLAB.h>
#include "IO/ADES/ADESIO.h"
using namespace arma;


// this object is designed to act as a process but with no plugin and no input settings checking.
// this is a way to launch internal processes in AnyWave without using plugins.
class AwMarkerExporter : public AwProcess
{
	Q_OBJECT
		Q_INTERFACES(AwProcess)
public:
	AwMarkerExporter();
	~AwMarkerExporter();

	void run();

	QString outputFile;
	bool concatenate, matlabFormat;
protected:
	int saveToMatlab(fmat& matrix);

	QString m_error;
	AwMATLABFile m_matlabFile;
	ADESIO m_adesFile;
	ADESIOPlugin m_adesPlugin;
};

// The plugin object is mandatory in the process mechanic of AnyWave.
// Here we don't use it to instantiate the process but just as a wrapper.
// The process itself will instantiate a plugin object and destroy it when dying.
class AwMarkerExporterPlugin : public AwProcessPlugin
{
	Q_OBJECT
	Q_INTERFACES(AwProcessPlugin)
public:
	AwMarkerExporterPlugin();
	AwMarkerExporter *newInstance() { return NULL; }
};
