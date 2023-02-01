#include <widget/SignalView/AwGraphicsScene.h>
#include <widget/AwMarkingTool.h>

///
/// keyPress()
///
void AwGraphicsScene::keyPressEvent(QKeyEvent* e)
{
	switch (e->modifiers()) {
	case Qt::ShiftModifier:
		m_draggingItems = true;
		break;
	case Qt::ControlModifier:
		m_selectionIsActive = true;
		switch (e->key()) {
		case Qt::Key_A: // CTRL + A = Select all channels
			// select all channels
			selectAllChannels();
			break;
		case Qt::Key_U: // CTRL + U = Unselect all
			// clear selection
			clearChannelSelection();
			break;
		case Qt::Key_I: // CTRL + I = Invert selection
			invertChannelSelection();
			break;
		case Qt::Key_Z:
			m_markingTool->removeLastAddedMarker();
			break;
		}
	}
	switch (m_mouseMode) {
	case AwGraphicsScene::AddingMarker:
		if (m_selectionIsActive) {
			if (!m_selectionRectangle) {
				m_selectionRectangle = new QGraphicsRectItem(0);
				m_selectionRectangle->setZValue(100);
				m_selectionRectangle->setPen(QPen(Qt::blue));
				m_selectionRectangle->setBrush(QBrush(Qt::blue, Qt::SolidPattern));
				m_selectionRectangle->setOpacity(0.3);
				addItem(m_selectionRectangle);
				// start with a rectangle around the mouse position of 20 x 20
				m_selectionRectangle->setRect(m_mousePos.x() - 10, m_mousePos.y() - 10, 20, 20);
			}
			else
				m_selectionRectangle->setRect(m_mousePos.x() - 10, m_mousePos.y() - 10, 20, 20);
		}
		update();
	}
}


void AwGraphicsScene::keyReleaseEvent(QKeyEvent* e)
{
	switch (e->key()) {
	case Qt::ShiftModifier:
		m_draggingItems = false;
		break;
	case Qt::Key_Control:
		m_selectionIsActive = false;
		if (m_selectionRectangle) {
			removeItem(m_selectionRectangle);
			delete m_selectionRectangle;
			m_selectionRectangle = nullptr;
			update();
		}
		break;
	case Qt::Key_Right:
		if (m_mouseMode == AwGraphicsScene::None)
			nextPage();
		else if (m_mouseMode == AwGraphicsScene::AddingMarker)
			next();
		else if (m_mouseMode == AwGraphicsScene::Mapping) {
			if (m_mappingFixedCursor) { // move the fixed mapping cursor
				QPointF pos = m_mappingFixedCursor->pos();
				float time = timeAtPos(pos);
				float sample_duration = 1 / m_maxSR;
				time += sample_duration;
				m_mappingFixedCursor->setPos(xPosFromTime(time), 0);
				update();
				emit clickedAtTime(time);
			}
		}
		break;
	case Qt::Key_Left:
		if (m_mouseMode == AwGraphicsScene::None)
			previousPage();
		else if (m_mouseMode == AwGraphicsScene::AddingMarker)
			previous();
		else if (m_mouseMode == AwGraphicsScene::Mapping) {
			if (m_mappingFixedCursor) { // move the fixed mapping cursor
				QPointF pos = m_mappingFixedCursor->pos();
				float time = timeAtPos(pos);
				float sample_duration = 1 / m_maxSR;
				time -= sample_duration;
				m_mappingFixedCursor->setPos(xPosFromTime(time), 0);
				update();
				emit clickedAtTime(time);
			}
		}
		break;
	case Qt::Key_Space:
		nextPage();
		break;
	}
}

