#include <AwProcess.h>
#include <AwChannel.h>
#include <AwMarker.h>
#include <AwFileIO.h>
#include <QMutex>
#include <filter/AwFilterSettings.h>
#include <AwCommandLine.h>


/** AwProcessIO defines object used as input or output by AwProcess **/
class AW_PROCESS_EXPORT AwProcessIO
{
public:
	AwProcessIO() { m_reader = nullptr; }
	~AwProcessIO();

	inline bool containsChannels() { return !m_channels.isEmpty(); }
	inline bool containsMarkers() { return !m_markers.isEmpty(); }
	inline bool containsWidgets() { return !m_widgets.isEmpty(); }
	inline bool containsCustomData() { return !customData.isEmpty(); }
	inline bool launchedUsingQST() { return timeSelection.duration() > 0.; }
	bool isEmpty();
	inline AwFileIO *reader() { return m_reader; }
	void setReader(AwFileIO* reader);
	void clearChannels();
	void clearMarkers();
	void clearWidgets();


	inline AwChannelList& channels() { return m_channels; }
	inline AwMarkerList& markers() { return m_markers; }
	inline AwMarkerList& modifiedMarkers() { return m_modifiedMarkers; }
	inline QList<QWidget*>& widgets() { return m_widgets; }
	/** Set a new list of channels. Previous channels will be deleted!!. duplicate indicates that the list will be duplicated and then set as the new list. **/
	void setNewChannels(const AwChannelList& channels, bool duplicate = false);
	/** Set a new list of markers. Previous markers will be deleted!!  duplicate indicates that the list will be duplicated and then set as the new list. **/
	void setNewMarkers(const AwMarkerList& markers, bool duplicate = false);
	/** Set the resulting marker list after filtering input markers with use_markers or skip_markers **/
	void setModifiedMarkers(const AwMarkerList& markers);
	/** Append markers **/
	void addMarkers(const AwMarkerList& markers, bool duplicate = false);
	void addMarker(AwMarker *marker);
	/** apend widgets **/
	void addWidget(QWidget* widget);
	/** Append channels **/
	void addChannels(const AwChannelList& channels, bool duplicate = false);
	void addChannel(AwChannel *channel);
	QList<AwFileIOPlugin *> readers, writers;

	AwMarker timeSelection;		// optional marker used in Quick Time Selection mode in AnyWave (contains the timing the user has selected before launching the process).
	QVariantList customData;

	AwFilterSettings filterSettings;
	QVariantMap settings;

	void lock() { m_mutex.lock(); }
	void unlock() { m_mutex.unlock(); }
protected:
	AwChannelList m_channels;
	// markers will contain input markers for the process.
	AwMarkerList m_markers;
	AwMarkerList m_modifiedMarkers;	// this list contains markers filtered by --use_markers and --skip_markers options

	QList<QWidget*> m_widgets;
	AwFileIO *m_reader;
	QMutex m_mutex;
};