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
#include "AwExportWizardDial.h"
#include "Marker/AwMarkerManager.h"
#include <QIcon>

AwExportWizardDial::AwExportWizardDial(QWidget* parent) : QWizard(parent)
{
	setWizardStyle(QWizard::ModernStyle);
	setWindowTitle("Exporting to file...");
	setWindowIcon(QIcon(":/images/AnyWave_icon.png"));
	m_outputPage = new AwOutputFileWizardPage;
	addPage(m_outputPage);
	m_channelsPage = new AwChannelsExportWizardPage;
	addPage(m_channelsPage);
	auto markers = AwMarker::getMarkersWithDuration(AwMarkerManager::instance()->getMarkers());
	m_markersPage = nullptr;
	if (markers.size()) {
		m_markersPage = new AwMarkersExportWizardPage;
		addPage(m_markersPage);
	}
	m_filterPage = new AwFilterExportWizardPage(m_channelsPage);
	addPage(m_filterPage);
}


void AwExportWizardDial::accept()
{
	channels = m_channelsPage->channels();
	if (m_markersPage) {
		useMarkers = m_markersPage->usedMarkers;
		skipMarkers = m_markersPage->skippedMarkers;
	}
	writerPluginName = m_outputPage->writerName;
	filePath = m_outputPage->filePath;

	QWizard::accept();

}