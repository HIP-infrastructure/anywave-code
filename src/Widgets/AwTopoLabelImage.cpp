/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "AwTopoLabelImage.h"
#include <qwt_matrix_raster_data.h>
#include <QMouseEvent>
#include <QtMath>

AwTopoLabelImage::AwTopoLabelImage(QWidget *parent, Qt::WindowFlags flags) : QLabel(parent, flags)
{
	setStyleSheet("color: red; text-align: center; font: bold 16pt;");
	m_data = NULL;
	m_pad = m_head_size = 0.;
}

void AwTopoLabelImage::setPadSize(float pad, float size)
{
	 m_pad = pad; m_head_size = size;
}

void AwTopoLabelImage::computeValueAtPosition(const QPointF& pos, double *head_pos)
{
	// Region representing the head
	float offset = (float)width() / 2 - ( m_head_size / 2);
	m_region = QRegion((int)qCeil(offset), (int)qCeil(m_pad), (int)qFloor(m_head_size), (int)qFloor(m_head_size), QRegion::Ellipse);
	QPoint p((int)qCeil(pos.x()), (int)qCeil(pos.y()));

	if (m_region.contains(p)) {
		double x = ((pos.x() - offset) / m_head_size) * m_data->interval(Qt::XAxis).maxValue();
		double y = ((pos.y() - m_pad) / m_head_size) *  m_data->interval(Qt::YAxis).maxValue();
		head_pos[0] = pos.x() - offset; // x pos in pixel in the head
		head_pos[1] = pos.y() - m_pad;  // y pos in pixel in the head

		// revert Y position to match data matrix Y axis.
		y = m_data->interval(Qt::YAxis).maxValue() - y;

		emit positionChanged(m_data->value(x, y));
	}
}

void AwTopoLabelImage::mouseReleaseEvent(QMouseEvent *e)
{
	if (m_data == NULL)
		return;

	double head_pos[2];
	computeValueAtPosition(e->localPos(), head_pos);
	emit clickedAt(head_pos[0], head_pos[1]);
}

void AwTopoLabelImage::mouseMoveEvent(QMouseEvent *e)
{
	if (m_data == NULL)
		return;

	double head_pos[2];
	computeValueAtPosition(e->localPos(), head_pos);
}