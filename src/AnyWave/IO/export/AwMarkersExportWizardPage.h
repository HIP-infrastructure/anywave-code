#pragma once
#include <QWizardPage>
class AwSelectInputDataWidget;

class AwMarkersExportWizardPage : public QWizardPage
{
	Q_OBJECT
public:
	AwMarkersExportWizardPage(QWidget* parent = nullptr);
	bool validatePage() override;

	QStringList usedMarkers, skippedMarkers;
protected:
	AwSelectInputDataWidget* m_inputWidget;
}; 
