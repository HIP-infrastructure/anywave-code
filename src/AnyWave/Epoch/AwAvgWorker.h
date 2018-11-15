/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
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
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <qobject.h>
#include <epoch/AwEpochTree.h>
class AwAvgWorker : public QObject
{
	Q_OBJECT
public:
	AwAvgWorker(QObject* parent = Q_NULLPTR);
	~AwAvgWorker();
	void setConditions(const QStringList& conditions);
public slots:
	void run();
signals:
	/** Sent when the computation is about to start **/
	void readyToCompute(int numberOfConditions);
	/** Sent to show progress while epoching **/
	void epochsProcessed(int percent);
	/** Sent when the data for all the epochs are loaded. **/
	void dataLoaded(const QString& condition);
	/** Sent when a condition averaging is done. **/
	void done(const QString& condition);
	/** Sent when everything is done **/
	void finished();
protected:
	AwConditionList m_conditions;
};