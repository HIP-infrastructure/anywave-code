#ifndef AW_MARKINGSETTINGS_H
#define AW_MARKINGSETTINGS_H
#include <AwGlobal.h>
#include <AwMarker.h>
#include <QObject>
#include <QVector>

class AW_WIDGETS_EXPORT AwMarkingSettings : public QObject
{
	Q_OBJECT
public:	
	AwMarkingSettings(QObject *parent = 0) : QObject(parent) 
	{
		type = AwMarker::Single;
		label = "New Marker";
		index = 1;
		usingSpacebar = false;
		isAutoInc = false;
		isTargettingChannels = false;
		value = -1.0;
		autoTargetChannel = false;
		m_spaceBarIndex = 0;
	}
	void setPredefinedMarkers(const AwMarkerList& markers);
	AwMarkerList getSelectedPredefinedMarkers();

	int type;						// current marker type
	bool isUsingList;				// true if we are using a predefined list of markers
	QString label;					// marker label
	float value;					// associated value 
	bool isAutoInc;					// true if auto incrementing label is on
	bool isTargettingChannels;		// true if marking is targeting predefined channels
	QStringList targets;			// labels of predefined targets
	int index;						// used while auto incrementing
	AwMarkerList predefinedMarkers;	// The predefined list of markers
	QVector<int> selectedPredefinedMarkers;
	bool autoTargetChannel;			// true if the channel under the mouse will be used as target
	bool usingSpacebar;				// true if using spacebar binding
	QString color;					// color to set for the marker
//	AwMarker *spaceBarNext();
//	void setBoundMarkers(AwMarkerList& markers);
protected:
	int m_spaceBarIndex;				// current index in boundMarkers 
	AwMarkerList m_boundMarkers;		// List of markers used with spacebar 
};

#endif