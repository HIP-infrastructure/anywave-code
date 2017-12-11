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
#ifndef AW_TOPOLABELIMAGE_H
#define AW_TOPOLABELIMAGE_H
#include <QLabel>
class QwtMatrixRasterData;

class AwTopoLabelImage : public QLabel
{
	Q_OBJECT
public:
	AwTopoLabelImage(QWidget *parent = 0, Qt::WindowFlags = 0);
	inline void setData(QwtMatrixRasterData *data) { m_data = data; }
	void setPadSize(float pad, float size);
signals:
	void positionChanged(double value);
	void clickedAt(double x, double y);
protected:
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void computeValueAtPosition(const QPointF& pos, double *head_pos);

	float m_pad, m_head_size;
	QwtMatrixRasterData *m_data;	// pointer to current matrix containing interpolated values.
	QRegion m_region;	// region used to map mouse position in the head display.
};

#endif