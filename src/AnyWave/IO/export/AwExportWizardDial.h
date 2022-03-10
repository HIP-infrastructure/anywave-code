#pragma once
#include <qwizard.h>
#include "AwOutputFileWizardPage.h"
#include "AwChannelsExportWizardPage.h"
#include "AwMarkersExportWizardPage.h"
#include "AwFilterExportWizardPage.h"

class AwExportWizardDial : public QWizard
{
	Q_OBJECT
public:
	AwExportWizardDial(QWidget* parent = nullptr);

	AwChannelList channels;
	QStringList useMarkers, skipMarkers;
	AwFilterSettings& filterSettings() { return m_filterPage->filterSettings; }
	QString writerPluginName;
	QString filePath;
public slots:
	void accept() override;
protected:
	AwChannelsExportWizardPage* m_channelsPage;
	AwMarkersExportWizardPage* m_markersPage;
	AwFilterExportWizardPage* m_filterPage;
	AwOutputFileWizardPage* m_outputPage;
};