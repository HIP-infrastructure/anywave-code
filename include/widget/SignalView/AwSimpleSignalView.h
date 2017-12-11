#pragma once

#include <widget/SignalView/AwBaseSignalView.h>

/** SimpleSignalView is designed to be used without connecting to a DataServer							**/
/** The goal is to simply show the signals stored in the provided channels.								**/
/** No data request will be made, thus setChannels() must be called once with channels containing data. **/
class AW_WIDGETS_EXPORT AwSimpleSignalView : public AwBaseSignalView
{
	Q_OBJECT
public:
	AwSimpleSignalView(int flags = 0, AwViewSettings *settings = NULL, QObject *parent = 0);
	~AwSimpleSignalView();
    void setChannels(AwChannelList& channels);
public slots:
	 void reloadData();	// reload data after filtering options or settings changed
protected:
	QVector<qint64> m_channelSampleOffsets;
	QList<QPair<float *, qint64> > m_dataBuffers;
};