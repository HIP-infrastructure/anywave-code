// AnyWave
// Copyright (C) 2013-2021  INS UMR 1106
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include <montage/AwAVGChannel.h>
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