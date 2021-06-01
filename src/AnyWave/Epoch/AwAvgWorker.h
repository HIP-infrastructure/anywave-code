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

#include <qobject.h>
#include <epoch/AwEpochTree.h>
class AwAvgWorker : public QObject
{
	Q_OBJECT
public:
	AwAvgWorker(QObject* parent = Q_NULLPTR);
	~AwAvgWorker();
	void setCondition(AwEpochTree * condition);
public slots:
	void run();
	void updateProgress(int epoch);
signals:
	void done(AwEpochTree * condition);
	void inProgress(int progress);
protected:
	AwEpochTree * m_condition;
};