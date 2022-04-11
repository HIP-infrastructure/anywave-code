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
#include "AwMarkersExportWizardPage.h"
#include "Marker/AwMarkerManager.h"
#include <widget/AwSelectInputDataWidget.h>
#include <QVBoxLayout>

AwMarkersExportWizardPage::AwMarkersExportWizardPage(QWidget* parent) : QWizardPage(parent)
{
	setTitle("Pick markers to export and/or to avoid");
	setSubTitle("this is optional. If you pick marker to export, only marked data by those markers will be saved");

	m_inputWidget = new AwSelectInputDataWidget;
	m_inputWidget->setPalette(this->palette());
	m_inputWidget->setMarkers(AwMarker::getMarkersWithDuration(AwMarkerManager::instance()->getMarkers()));

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(m_inputWidget);
	setLayout(layout);
}


bool AwMarkersExportWizardPage::validatePage()
{
	usedMarkers = m_inputWidget->usedMarkers();
	skippedMarkers = m_inputWidget->skippedMarkers();
	return true;
}