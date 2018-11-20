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