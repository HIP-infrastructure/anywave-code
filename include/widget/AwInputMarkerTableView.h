#pragma once
#include <qtableview.h>
#include <AwMarker.h>
#include <AwGlobal.h>

class AW_WIDGETS_EXPORT AwInputMarkerTableView : public QTableView
{
	Q_OBJECT
public:
	AwInputMarkerTableView(QWidget *parent = Q_NULLPTR);
	void setFlags(int flags);

	enum Flags { MultiSelection = 1, ShowLabel = 2, ShowDuration = 4, ShowPosition = 8, 
	ShowColor = 16, ShowTargets = 32, ShowValue = 64, ShowAll = 126};
public slots:
	void setMarkers(const AwMarkerList& markers);
protected:
	int m_flags;
};