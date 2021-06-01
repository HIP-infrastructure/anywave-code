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
#ifndef CORRELATION_H
#define CORRELATION_H

#include "correlation_global.h"
#include <AwProcessInterface.h>
#include "settings.h"
#include <QtCore>
#include <aw_armadillo.h>
using namespace arma;

struct corr_result {
	mat corr;
	QStringList xLabels, yLabels;
	float position, duration;
};

class CORRELATION_EXPORT Correlation : public AwProcess
{
	Q_OBJECT
public:
	Correlation();
	~Correlation();

	bool showUi();
	void run();
	void prepareOutputUi();
private:
	settings *m_ui;

	corr_result *computeCorrelation(float start, float duration);
	QList<corr_result *> m_results;
};

class CORRELATION_EXPORT CorrelationPlugin : public AwProcessPlugin
{
	Q_OBJECT
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	Q_PLUGIN_METADATA(IID AwProcessPlugin_IID)
#endif
	Q_INTERFACES(AwProcessPlugin)
public:
	CorrelationPlugin();

	AW_INSTANTIATE_PROCESS(Correlation)

};

#endif // CORRELATION_H
