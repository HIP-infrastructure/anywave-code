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
#ifndef AWICACHANNEL_H
#define AWICACHANNEL_H

#include <AwVirtualChannel.h>
#include <layout/AwLayout.h>
#include <armadillo>
using namespace arma;

class AwICAChannel : public AwVirtualChannel
{
public:
	AwICAChannel();
	AwICAChannel(AwICAChannel *c);
	AwICAChannel(AwChannel *c) : AwVirtualChannel(c) {}
	~AwICAChannel();
	inline int index() { return m_index; }
	inline void setIndex(int index) { m_index = index; }
	inline int componentType() { return m_compType; }
	void setComponentType(int type);
	inline bool isRejected() { return m_isRejected; }
	void setRejected(bool flag = true) { m_isRejected = flag; }
	inline AwLayout *layout2D() { return m_layout2D; }
	inline void setLayout2D(AwLayout *layout) { m_layout2D = layout; }
	inline AwLayout *layout3D() { return m_layout3D; }
	inline void setLayout3D(AwLayout *layout) { m_layout3D = layout; }
	inline void setTopoValues(const QVector<float>& values) { m_topoValues = values; }
	inline QVector<float>& topoValues() { return m_topoValues;  }
	inline void setLabels(const QStringList& labels) { m_labels = labels; }
	inline QStringList& labels() { return m_labels; }
	void setAutoScale(bool flag) { m_autoYScale = flag; }
	inline bool isAutoScaling() { return m_autoYScale; }
	AwICAChannel *duplicate() override;
	void update();
protected:
	int m_index;	// index in components matrix
	int m_compType;	// component type (MEG or EEG)
	AwLayout *m_layout2D, *m_layout3D;
	QVector<float> m_topoValues;
	QStringList m_labels;	// Channel labels for topography
	bool m_autoYScale;
	bool m_isRejected;
};

typedef QList<AwICAChannel *> AwICAChannelList;

#endif // AWICACHANNEL_H


