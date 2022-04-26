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
#include "AwExtractTriggers.h"
#include <math.h>

AwExtractTriggers::AwExtractTriggers()
{
	_maskValue = 0;
	_parseNegative = false;
}

void AwExtractTriggers::extract()
{
	typedef QPair<qint64, float> DetectedEvent;
	int sample = 1;

	QVector<float> detectedValues;
	for (auto c : _channels) {
		float value;
		int ivalue;
		QList<DetectedEvent *> events;
		qint64 sample = 1;
		int ibaseline = (int)floor(c->data()[0]);
		float baseline = c->data()[0];
		if (_maskValue != 0) {
			ibaseline &= ~_maskValue;
			baseline = (float)ibaseline;
		}
		detectedValues.clear();
		float lastDetectedValue = 0.;
		while (sample < c->dataSize()) {
			value = c->data()[sample];
			if (_maskValue != 0) {
				ivalue = (int)floor(value);
				ivalue &= ~_maskValue;
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
			_markers << newMarker;
		}
		// clean events
		while (!events.isEmpty())
			delete events.takeFirst();
	}
}