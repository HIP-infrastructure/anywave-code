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
#ifndef AWHIGHLIGHTPOSITIONMARKER_H
#define AWHIGHLIGHTPOSITIONMARKER_H
#include <AwGlobal.h>
#include <widget/AwGraphicsObjects.h>
#include <QtGui>



class AW_WIDGETS_EXPORT AwHighLightPositionMarker : public AwGraphicsCursorItem
{
	Q_OBJECT
public:
	AwHighLightPositionMarker(float positionInFile, float cursorPos, AwDisplayPhysics *phys);
	~AwHighLightPositionMarker();

	void updateGeometry();
protected:
	void paint (QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
	void timerEvent(QTimerEvent *ev);

	bool m_paint;
	int m_nTicks;
	int m_timerId;
	QRectF m_rect;
};

#endif // AWHIGHLIGHTPOSITIONMARKER_H
