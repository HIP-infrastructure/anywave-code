#pragma once

#include <widget/SignalView/AwBaseSignalView.h>
#include <epoch/AwEpoch.h>

/// A view designed to display an epoch.
/// Channels set to the view must contain the whole data corresponding to the epoch.
/// Time bounds of the epoch must be specified.

class AW_EPOCH_EXPORT AwEpochSignalView : public AwBaseSignalView
{
	Q_OBJECT
public:
	AwEpochSignalView(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags(),
		int flags = AwBaseSignalView::Default, AwViewSettings *settings = Q_NULLPTR);
	~AwEpochSignalView() {}

	void updatePageDuration(float duration) override;
	void reloadData() override;
	void setChannels(const AwChannelList& channels) override;
	void setEpoch(AwEpoch *epoch);
protected:
	AwEpoch *m_epoch;
};

