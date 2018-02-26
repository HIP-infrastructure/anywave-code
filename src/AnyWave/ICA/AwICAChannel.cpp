/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "AwICAChannel.h"
#include <AwAmplitudeManager.h>


AwICAChannel::AwICAChannel()
: AwVirtualChannel()
{
	m_type = AwChannel::ICA;
	m_index = -1;
	m_compType = AwChannel::MEG;
	m_layout2D = m_layout3D = NULL;
	m_autoYScale = false;
	m_isRejected = false;
}

AwICAChannel::AwICAChannel(AwICAChannel *c)
: AwVirtualChannel(c)
{
	m_index = c->index();
	m_type =  AwChannel::ICA;
	m_compType = c->componentType();
	m_layout2D = c->layout2D();
	m_layout3D = c->layout3D();
	m_labels = c->labels();
	m_topoValues = c->topoValues();
	m_compType = c->componentType();
	m_autoYScale = c->isAutoScaling();
	m_isRejected = c->isRejected();
}

AwICAChannel::~AwICAChannel()
{
}


AwICAChannel *AwICAChannel::duplicate()
{
	AwICAChannel *newc = new AwICAChannel(this);
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
	newc->setLayout2D(this->layout2D());
	newc->setLayout3D(this->layout3D());
	
	if (m_dataSize) {
		float *data = newc->newData(this->dataSize());
		memcpy(data, this->data(), this->dataSize() * sizeof(float));
		newc->setDataReady();
	}
	newc->setTopoValues(this->topoValues());
	newc->setLabels(this->labels());
	return newc;
}

void AwICAChannel::update()
{
	if (!m_autoYScale) 
		return;

	// get maximum amplitude from signal
	float max = 0.;
	for (int i = 0; i < m_dataSize; i++) {
		float sample = qAbs(m_data[i]);
		if (sample > max)
			max = sample;
	}

	m_gain = (max / 2);
}

void AwICAChannel::setComponentType(int type)
{
	m_compType = type;
	AwAmplitudeManager *am = AwAmplitudeManager::instance();
	setGain(am->middleValueForScale(type));
}