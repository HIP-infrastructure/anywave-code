#pragma once

#include <QWidget>
#include <AwMarker.h>

namespace Ui { class AwSelectInputDataWidgetUi; };

class AW_WIDGETS_EXPORT AwSelectInputDataWidget : public QWidget
{
	Q_OBJECT

public:
	AwSelectInputDataWidget(QWidget *parent = Q_NULLPTR);
	~AwSelectInputDataWidget();

	void setMarkers(const AwMarkerList& markers);
	QStringList skippedMarkers();
	QStringList usedMarkers();
protected slots:
	void addSkipLabel();
	void addUseLabel();
	void clearUse();
	void clearSkip();
private:
	QStringList m_skipLabels, m_useLabels;
	Ui::AwSelectInputDataWidgetUi *m_ui;
};
