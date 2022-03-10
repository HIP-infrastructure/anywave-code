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