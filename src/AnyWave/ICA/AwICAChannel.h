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
//	void setTopoValues(fvec& vec) { m_topoValues = vec; }
//	fvec& topoValues() { return m_topoValues; }
	inline void setLabels(const QStringList& labels) { m_labels = labels; }
	inline QStringList& labels() { return m_labels; }
	void setAutoScale(bool flag) { m_autoYScale = flag; }
	inline bool isAutoScaling() { return m_autoYScale; }
	AwICAChannel *duplicate();
	void update();
protected:
	int m_index;	// index in components matrix
	int m_compType;	// component type (MEG or EEG)
	AwLayout *m_layout2D, *m_layout3D;
	QVector<float> m_topoValues;
//	fvec m_topoValues;
	QStringList m_labels;	// Channel labels for topography
	bool m_autoYScale;
	bool m_isRejected;
};

typedef QList<AwICAChannel *> AwICAChannelList;

#endif // AWICACHANNEL_H


