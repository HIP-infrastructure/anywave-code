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
#include "AwDockAddMarker.h"
#include <widget/AwMarkerInspector.h>
#include "AwMarkerManager.h"
#include "Montage/AwMontageManager.h"

AwDockAddMarker::AwDockAddMarker(const QString& title, QWidget *parent, Qt::WindowFlags flags)
	:  QDockWidget(title, parent, flags)
{
	setFeatures(QDockWidget::AllDockWidgetFeatures);
	setObjectName("Adding Markers");
	setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	setFloating(true);

	m_inspector = new AwMarkerInspector(AwMarkerManager::instance()->getMarkers(), AwChannel::getLabels(AwMontageManager::instance()->channels()), this);
	setWidget(m_inspector);
}

AwDockAddMarker::~AwDockAddMarker()
{

}


void AwDockAddMarker::setVisible(bool visible)
{
	QDockWidget::setVisible(visible);
	if (!visible)
		emit addingMarkerStopped();

}