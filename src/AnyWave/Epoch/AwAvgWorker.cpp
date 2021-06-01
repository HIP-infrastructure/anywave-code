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
#include "AwAvgWorker.h"
#include <qthread.h>

AwAvgWorker::AwAvgWorker(QObject* parent) : QObject(parent)
{
	QThread *t = new QThread(this);
	moveToThread(t);
	t->start();
}

AwAvgWorker::~AwAvgWorker()
{
	thread()->exit(0);
	thread()->wait();
}

void AwAvgWorker::setCondition(AwEpochTree * condition)
{
	m_condition = condition;
	connect(m_condition, &AwEpochTree::epochLoaded, this, &AwAvgWorker::updateProgress);
}

void AwAvgWorker::run()
{
	//m_condition->doAverage(true);
	m_condition->average();
	emit done(m_condition);
}

void AwAvgWorker::updateProgress(int epoch)
{
	emit inProgress((epoch * 100) / m_condition->numberOfGoodEpochs());
}