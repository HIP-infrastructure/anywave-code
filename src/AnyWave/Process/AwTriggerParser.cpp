/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
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
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "AwTriggerParser.h"
#include "TPUi.h"



AwTriggerParserPlugin::AwTriggerParserPlugin()
{
	name = QString(tr("Trigger Parser"));
	description = QString(tr("Extract digital values from trigger channels and make them markers"));
	version = "1.0";
	type = AwProcessPlugin::Background;
	setFlags(Aw::ProcessFlags::ProcessHasInputUi);
}

AwTriggerParser::AwTriggerParser() : AwProcess()
{
	// INPUT PARAMS
	setInputModifiers(Aw::ProcessIO::modifiers::IgnoreChannelSelection);
	pdi.addInputChannel(AwChannel::Trigger, 1, 0);

	m_maskValue = 0;
}

AwTriggerParser::~AwTriggerParser()
{
	clearMarkers();
}

void AwTriggerParser::clearMarkers()
{
	while (!m_markers.isEmpty())
		delete m_markers.takeLast();
}

bool AwTriggerParser::showUi()
{
	TPUi dlg(pdi.input.channels());

	if (dlg.exec() == QDialog::Accepted) {
		if (dlg.isMaskValueSet())
			m_maskValue = dlg.maskValue();
		m_parseNegative = dlg.parseNegativeValues();
		m_triggers = dlg.triggers();
		return true;
	}
	return false;
}

void AwTriggerParser::run()
{
	typedef QPair<qint64, float> DetectedEvent;
	clearMarkers();

	emit progressChanged(tr("Reading data..."));
	requestData(&m_triggers, (float)0, (float)-1);

	if (endOfData()) {
		sendMessage("No data could be loaded. Aborted.");
		return;
	}
	QVector<float> detectedValues;
	for (auto c : m_triggers) {
		sendMessage(QString("Detecting on channel %1...").arg(c->name()));
		float value;
		int ivalue;
		QList<DetectedEvent *> events;
		qint64 sample = 1;
		int ibaseline = (int)floor(c->data()[0]);
		float baseline = c->data()[0];
		if (m_maskValue != 0) {
			ibaseline &= ~m_maskValue;
			baseline = (float)ibaseline;
		}
		detectedValues.clear();
		float lastDetectedValue = 0.;
		while (sample < c->dataSize()) {
			if (wasAborted()) {
				qDeleteAll(events);
				return;
			}
			value = c->data()[sample];
			if (m_maskValue != 0) {
				ivalue = (int)floor(value);
				ivalue &= ~m_maskValue;
				value = (float)ivalue;
			}
			if (value != baseline) {
				DetectedEvent *e = new DetectedEvent;
				if (detectedValues.isEmpty()) {
					detectedValues.append(baseline);
					e->first = sample;
					e->second = value;
					baseline = value;
					events.append(e);

				}
				else if (value == detectedValues.last()) {
					detectedValues.takeLast();
					baseline = value;
				}
				else {
					e->first = sample;
					e->second = value - lastDetectedValue;
					baseline = value;
					events.append(e);
				}
			}
			sample++;
		} // end while
		for (auto e : events) {
			QString name = QString("%1:%2").arg(c->name()).arg(e->second);
			AwMarker *newMarker = new AwMarker(name, (float)e->first / c->samplingRate());
			newMarker->setValue(e->second);
			m_markers << newMarker;
		}
		while (!events.isEmpty())
			delete events.takeFirst();
	}

	addMarkers(&m_markers);
	sendMessage(QString("Detection complete. Found %1 markers.").arg(m_markers.size()));
	while (!m_markers.isEmpty())
		delete m_markers.takeFirst();
}