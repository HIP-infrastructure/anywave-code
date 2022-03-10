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