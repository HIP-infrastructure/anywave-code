#pragma once
#include <QObject>
#include <QMutex>
#include <AwGlobal.h>
#include <AwMarker.h>


/// <summary>
/// Singleton holding a global marker list 
/// The list is guarded by locks
/// </summary>
class AW_CORE_EXPORT AwGlobalMarkers : public QObject
{
public:
	~AwGlobalMarkers();

	static AwGlobalMarkers* instance();
	AwMarkerList* displayed();
	AwMarkerList* total();
	void setDisplayed(AwMarkerList* list);
	void setTotal(AwMarkerList* list);
	void closeFile();
protected:
	explicit AwGlobalMarkers();


	static AwGlobalMarkers* m_instance;
	AwMarkerList* m_displayed, *m_total;
	QMutex m_mutex;
};