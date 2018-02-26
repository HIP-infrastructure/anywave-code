#pragma once
#include <AwGlobal.h>
#include <AwDataBuffer.h>

class AW_EPOCH_EXPORT AwEpochDataBuffer : public AwDataBuffer
{
	Q_OBJECT
public:
	AwEpochDataBuffer(AwChannelList *channels = Q_NULLPTR, QObject *parent = Q_NULLPTR);
	~AwEpochDataBuffer();
public slots :
	void loadData(AwChannelList *, float, float, float, AwFilteringOptions *) override;
};