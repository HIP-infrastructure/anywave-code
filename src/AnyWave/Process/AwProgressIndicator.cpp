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
#include "AwProgressIndicator.h"
#include <QPainter>

AwProgressIndicator::AwProgressIndicator(QWidget *parent)
	: QWidget(parent),
	m_angle(0),
	m_timerId(-1),
	m_delay(40),
	m_displayedWhenStopped(false),
	m_color(Qt::black)
{
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFocusPolicy(Qt::NoFocus);
}

AwProgressIndicator::~AwProgressIndicator()
{

}

bool AwProgressIndicator::isAnimated () const
{
    return (m_timerId != -1);
}

void AwProgressIndicator::setDisplayedWhenStopped(bool state)
{
    m_displayedWhenStopped = state;

    update();
}

bool AwProgressIndicator::isDisplayedWhenStopped() const
{
    return m_displayedWhenStopped;
}

void AwProgressIndicator::startAnimation()
{
    m_angle = 0;

    if (m_timerId == -1)
        m_timerId = startTimer(m_delay);
}

void AwProgressIndicator::stopAnimation()
{
    if (m_timerId != -1)
        killTimer(m_timerId);

    m_timerId = -1;

    update();
}

void AwProgressIndicator::setAnimationDelay(int delay)
{
    if (m_timerId != -1)
        killTimer(m_timerId);

    m_delay = delay;

    if (m_timerId != -1)
        m_timerId = startTimer(m_delay);
}

void AwProgressIndicator::setColor(const QColor & color)
{
    m_color = color;

    update();
}

QSize AwProgressIndicator::sizeHint() const
{
    return QSize(20,20);
}

int AwProgressIndicator::heightForWidth(int w) const
{
    return w;
}

void AwProgressIndicator::timerEvent(QTimerEvent *e)
{
	Q_UNUSED(e)
    m_angle = (m_angle+30)%360;

    update();
}

void AwProgressIndicator::paintEvent(QPaintEvent *e)
{
	Q_UNUSED(e)
    if (!m_displayedWhenStopped && !isAnimated())
        return;

    int width = qMin(this->width(), this->height());
    
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    
    int outerRadius = (width-1)*0.5;
    int innerRadius = (width-1)*0.5*0.38;

    int capsuleHeight = outerRadius - innerRadius;
    int capsuleWidth  = (width > 32 ) ? capsuleHeight *.23 : capsuleHeight *.35;
    int capsuleRadius = capsuleWidth/2;

    for (int i=0; i<12; i++)
    {
        QColor color = m_color;
        color.setAlphaF(1.0f - (i/12.0f));
        p.setPen(Qt::NoPen);
        p.setBrush(color);       
        p.save();
        p.translate(rect().center());
        p.rotate(m_angle - i*30.0f);
        p.drawRoundedRect(-capsuleWidth*0.5, -(innerRadius+capsuleHeight), capsuleWidth, capsuleHeight, capsuleRadius, capsuleRadius);
        p.restore();
    }
}