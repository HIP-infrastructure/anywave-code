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