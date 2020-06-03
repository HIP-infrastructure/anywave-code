/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2020 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
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
#include "AnyWave.h"
#include "Batch/AwBatchManager.h"
#include <qdockwidget.h>

void AnyWave::on_actionCreate_batch_script_triggered()
{
	auto batchM = AwBatchManager::instance(); 
	batchM->setParent(this);
	// create dock widget if needed
	auto dock = m_dockWidgets.value("Batch");
	if (dock == nullptr) {
		dock = new QDockWidget(tr("Batch Processing"), this);
		m_dockWidgets["Batch"] = dock;
		dock->setWidget(batchM->ui());
		addDockWidget(Qt::LeftDockWidgetArea, dock);
		dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
		menuView_->addAction(dock->toggleViewAction());
		//connect(AwBIDSManager::instance(), &AwBIDSManager::BIDSClosed, dock, &QDockWidget::hide);
	}
	dock->show();
}