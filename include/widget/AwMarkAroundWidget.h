#ifndef AWMARKAROUNDWIDGET_H
#define AWMARKAROUNDWIDGET_H
#include <AwGlobal.h>
#include <QWidget>
namespace Ui {class AwMarkAroundWidgetUi;};
#include <AwMarker.h>

class AW_WIDGETS_EXPORT AwMarkAroundWidget : public QWidget
{
	Q_OBJECT

public:
	AwMarkAroundWidget(QWidget *parent = 0);
	~AwMarkAroundWidget();

	void setMarkers(const AwMarkerList& markers);
	void setTotalDuration(float duration) { m_totalDuration = duration; }
public slots:
	/** Generate markers before and after the selected marker and using the settings defined by the user **/
	void generatePrePost();	
	/** Generate marker around the selected marker and using the settings defined by the user **/
	void generateAround();	
signals:
	void newMarkersCreated(const AwMarkerList& markers);
private:
	Ui::AwMarkAroundWidgetUi *m_ui;
	AwMarkerList m_markers, m_newMarkers;	// complete list of all markers and current list of new markers
	float m_totalDuration;
};

#endif // AWMARKAROUNDWIDGET_H
