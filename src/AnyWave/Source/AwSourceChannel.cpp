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
#include "AwSourceChannel.h"
#include <AwAmplitudeManager.h>


AwSourceChannel::AwSourceChannel()
: AwVirtualChannel()
{
	m_type = AwChannel::Source;
	m_index = -1;
//	m_autoYScale = false;
}

AwSourceChannel::AwSourceChannel(AwSourceChannel *c)
: AwVirtualChannel(c)
{
	m_index = c->index();
	m_type =  AwChannel::Source;
	m_subType = c->subType();
}

AwSourceChannel::~AwSourceChannel()
{
}


AwSourceChannel *AwSourceChannel::duplicate()
{
	AwSourceChannel *newc = new AwSourceChannel(this);
	newc->setSourceType(this->sourceType());
	newc->setClassName(this->className());
	newc->setID(this->ID());
	newc->setSamplingRate(this->samplingRate());
	newc->setBad(this->isBad());
	newc->setColor(this->color());
	newc->setGain(this->gain());
	newc->setType(this->type());
	newc->setName(this->name());
	newc->setUnit(this->unit());
	newc->setXYZ(this->x(), this->y(), this->z());
	newc->setOrientationXYZ(this->ox(), this->oy(), this->oz());
	newc->setNotch(this->notch());
	newc->setLowFilter(this->lowFilter());
	newc->setHighFilter(this->highFilter());
	newc->setReferenceName(this->referenceName());
	newc->setDisplayPluginName(this->displayPluginName());
	
	if (m_dataSize) {
		float *data = newc->newData(this->dataSize());
		memcpy(data, this->data(), this->dataSize() * sizeof(float));
		newc->setDataReady();
	}
	
	return newc;
}
//
//void AwSourceChannel::update()
//{
//	if (!m_autoYScale) 
//		return;
//
//	// get maximum amplitude from signal
//	float max = 0.;
//	for (qint64 i = 0; i < m_dataSize; i++) {
//		float sample = qAbs(m_data[i]);
//		if (sample > max)
//			max = sample;
//	}
//
//	m_gain = (max / 2);
//}