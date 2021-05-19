#pragma once

#include <QDialog>
namespace Ui { class AwInputMarkerWidgetUi; };
#include <AwMarker.h>
#include <AwGlobal.h>

class AW_WIDGETS_EXPORT AwInputMarkerWidget : public QDialog
{
	Q_OBJECT

public:
	AwInputMarkerWidget(QWidget *parent = Q_NULLPTR);
	~AwInputMarkerWidget();
	enum Flags {
		MultiSelection = 1, ShowLabel = 2, ShowDuration = 4, ShowPosition = 8,
		ShowColor = 16, ShowTargets = 32, ShowValue = 64, ShowAllProperties = 126, UniqueMarkerLabel = 128, 
		OnlySingleMarkers = 256, OnlySelectionMarkers = 512
	};

	/** Use setFlags before to define how the markers will be filtered. **/
	void setMarkers(const AwMarkerList& markers);
	/** Defines the flags to filter markers and also what properties of marker to show **/
	void setFlags(int flags);

	inline AwMarkerList& markers() { return m_selectedMarkers; }
	inline float preLatency() { return m_pre; }
	inline float postLatency() { return m_post;  }
public slots:
	void accept();

private:
	Ui::AwInputMarkerWidgetUi *m_ui;
	int m_flags;
	AwMarkerList m_allMarkers, m_displayedMarkers, m_selectedMarkers;
	float m_pre, m_post;
};
