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
	AwSharedMarkerList* displayed();  // raw pointers
	AwSharedMarkerList* total();	// raw pointers
	void setDisplayed(AwSharedMarkerList* list);
	void setTotal(AwSharedMarkerList* list);
	void closeFile();
protected:
	explicit AwGlobalMarkers();


	static AwGlobalMarkers* m_instance;
	AwSharedMarkerList *m_displayed, *m_total;
	QMutex m_mutex;
};