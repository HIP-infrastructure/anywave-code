#pragma once

#include <AwChannel.h>
#include <AwGlobal.h>

struct settings {
	settings(AwChannel *c);
	float hp, lp, notch;
};

class AW_FILTER_EXPORT AwFilterSettingsStore
{
public: 
	explicit AwFilterSettingsStore(const AwChannelList& channels);
	~AwFilterSettingsStore();

	void clear();
	AwChannelList& restore();
protected:
	AwChannelList m_channels;
	QHash<AwChannel *, settings *> m_store;
};