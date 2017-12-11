#include "AwAVGChannel.h"
#ifndef NDEBUG
#include <QDebug>
#endif

AwAVGChannel::AwAVGChannel(int type)
: AwVirtualChannel()
{
	m_type = type;
	switch (type)
	{
	case AwChannel::EEG:
		m_name = "EEG_AVG";
		break;
	case AwChannel::SEEG:
		m_name = "SEEG_AVG";
		break;
	case AwChannel::MEG:
		m_name = "MEG_AVG";
		break;
	}
	m_className = "AwAVG";
}

AwAVGChannel::AwAVGChannel(AwAVGChannel *c)
: AwVirtualChannel(c)
{
	m_type =  c->type();
	m_name = c->name();
	m_className = "AwAVG";
}

AwAVGChannel::~AwAVGChannel()
{
}

AwAVGChannel *AwAVGChannel::duplicate()
{
	AwAVGChannel *newChan = new AwAVGChannel(this);
	AwChannelList connections;
	foreach (AwChannel *chan, m_connectedChannels)
		connections << new AwChannel(chan);

	newChan->connectChannels(connections);
	return newChan;
}


void AwAVGChannel::compute()
{
	if (m_connectedChannels.isEmpty())
		return;

	m_samplingRate = m_connectedChannels.at(0)->samplingRate();
	float *data = newData(m_connectedChannels.at(0)->dataSize());

	int m = m_connectedChannels.size();
	int n = m_connectedChannels.at(0)->dataSize();
	memset(data, 0, n * sizeof(float));
	
	float **matrix = new float*[m];

	for (int i = 0; i < m; i++)
		matrix[i] = m_connectedChannels.at(i)->data();

	for (int i = 0; i < n; i++)	{
		for (int j = 0; j < m; j++)
			data[i] += matrix[j][i];
		data[i] /= m;
	}
	delete[] matrix;
}