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
#ifndef AWPROGRESSINDICATOR_H
#define AWPROGRESSINDICATOR_H

#include <qglobal.h>
#if QT_VERSION < 5
#include <QtGui/QWidget>
#else
#include <QWidget>
#endif // QT_VERSION

///
/// the source code below came from qt-apps.org
/// thanks to orginial author : Morgan Leborgne
/// the widget has been modified to suit AnyWave and to provide additional features.

class AwProgressIndicator : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(int delay READ animationDelay WRITE setAnimationDelay)
    Q_PROPERTY(bool displayedWhenStopped READ isDisplayedWhenStopped WRITE setDisplayedWhenStopped)
    Q_PROPERTY(QColor color READ color WRITE setColor)
	
public:
	AwProgressIndicator(QWidget *parent = 0);
	~AwProgressIndicator();

	    /*! Returns the delay between animation steps.
        \return The number of milliseconds between animation steps. By default, the animation delay is set to 40 milliseconds.
        \sa setAnimationDelay
     */
    int animationDelay() const { return m_delay; }

    /*! Returns a Boolean value indicating whether the component is currently animated.
        \return Animation state.
        \sa startAnimation stopAnimation
     */
    bool isAnimated () const;

    /*! Returns a Boolean value indicating whether the receiver shows itself even when it is not animating.
        \return Return true if the progress indicator shows itself even when it is not animating. By default, it returns false.
        \sa setDisplayedWhenStopped
     */
    bool isDisplayedWhenStopped() const;

    /*! Returns the color of the component.
        \sa setColor
      */
    const QColor & color() const { return m_color; }

    virtual QSize sizeHint() const;
    int heightForWidth(int w) const;
public slots:
    /*! Starts the spin animation.
        \sa stopAnimation isAnimated
     */
    void startAnimation();

    /*! Stops the spin animation.
        \sa startAnimation isAnimated
     */
    void stopAnimation();

    /*! Sets the delay between animation steps.
        Setting the \a delay to a value larger than 40 slows the animation, while setting the \a delay to a smaller value speeds it up.
        \param delay The delay, in milliseconds. 
        \sa animationDelay 
     */
    void setAnimationDelay(int delay);

    /*! Sets whether the component hides itself when it is not animating. 
       \param state The animation state. Set false to hide the progress indicator when it is not animating; otherwise true.
       \sa isDisplayedWhenStopped
     */
    void setDisplayedWhenStopped(bool state);

    /*! Sets the color of the components to the given color.
        \sa color
     */
    void setColor(const QColor & color);

protected:
    virtual void timerEvent(QTimerEvent * event); 
    virtual void paintEvent(QPaintEvent * event);
private:
    int m_angle;
    int m_timerId;
    int m_delay;
    bool m_displayedWhenStopped;
    QColor m_color;
};

#endif // AWPROGRESSINDICATOR_H
