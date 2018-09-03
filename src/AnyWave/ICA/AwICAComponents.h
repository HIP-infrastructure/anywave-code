#ifndef AWICACOMPONENTS_H
#define AWICACOMPONENTS_H

#include <QObject>
#include <hdf5/AwHDF5.h>
#include <AwMATLAB.h>
#include <AwChannel.h>
#include "AwICAChannel.h"
#include <QMutex>
#include <filter/AwFilterSettings.h>

#include <armadillo>
using namespace arma;


class AwPanelItem;
class AwICAPanel;
class AwICAComponents : public QObject
{
	Q_OBJECT

public:
	AwICAComponents(int type, QObject *parent = 0);
	~AwICAComponents();

	int loadComponents(AwHDF5& file);
	int loadComponents(AwMATLABFile& file);
	AwChannelList& sources() { return m_sources; }
	AwICAChannelList& components() { return m_icaChannels; }
	void computeComponents(AwICAChannelList& channels);
	void rejectComponent(int index);
	void addComponent(int index);
	inline bool isRejectionNeeded() { return !m_rejectedComponents.isEmpty(); }
	void buildChannels(const AwChannelList& channels);
	inline float lpFilter() { return m_lpFilter; }
	inline float hpFilter() { return m_hpFilter; }
	inline int numberOfComponents() { return m_icaChannels.size(); }
	QList<AwPanelItem *> createPanelItems();
	inline AwICAPanel *getPanelWidget() { return m_panel; }
	inline int type() { return m_type; }
public slots:
	void showPanel();
	void setNewFilters(const AwFilterSettings& settings);
signals:
	/** Sent when the user reject a component **/
	void componentRejected(int index);
	/** Sent when the user undo the rejection of a component **/
	void componentAdded(int index);
	void filteringChecked(bool checked);
protected slots:
	void switchFilteringOn() { emit filteringChecked(true); }
	void updateComponentState(int index, bool rejected);
private:
	int m_type;
	fmat m_unmixing;
	fmat m_mixing;
	QStringList m_labels;
	AwChannelList m_sources;
	AwICAChannelList m_icaChannels;
	QHash<QString, int> m_labelToIndex;
	// filtering applied when computing ICA 
	float m_hpFilter;   
	float m_lpFilter; 
	QList<int> m_rejectedComponents;	// indexes of all rejected ICA components

	void updateRejectedComponents();
	// Thread safe specific
	QMutex m_mutex;
	AwICAPanel *m_panel;
};

#endif // AWICACOMPONENTS_H
