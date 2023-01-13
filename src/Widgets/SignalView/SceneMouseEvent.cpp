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
#include <widget/SignalView/AwGraphicsScene.h>
#include <QGraphicsSceneMouseEvent>
#include <utils/gui.h>
#include <widget/SignalView/AwGraphicsView.h>
#include <QMenu>
#include <widget/AwMarkingTool.h>

///
/// mousePress()
///
void AwGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
	QPointF pos = e->scenePos();
	m_positionClicked = timeAtPos(pos);
	if (e->button() != Qt::LeftButton)
		return;

	if (m_selectionIsActive && m_mouseMode == AwGraphicsScene::None) {
		m_mousePressed = true;
		if (!m_selectionRectangle) {
			m_selectionRectangle = new QGraphicsRectItem(0);
			m_selectionRectangle->setZValue(100);
			m_selectionRectangle->setPen(QPen(Qt::blue));
			m_selectionRectangle->setBrush(QBrush(Qt::blue, Qt::SolidPattern));
			m_selectionRectangle->setOpacity(0.3);
			addItem(m_selectionRectangle);
		}
		m_selectionRectangle->setRect(QRectF(e->scenePos(), QSize(0, 0)));
		m_mousePressedPos = e->scenePos();
		update();
		return;
	}
	else if (m_mouseMode == AwGraphicsScene::QTS) {
		if (!m_selectionRectangle) {
			m_selectionRectangle = new QGraphicsRectItem(0);
			m_selectionRectangle->setZValue(100);
			m_selectionRectangle->setPen(QPen(Qt::blue));
			m_selectionRectangle->setBrush(QBrush(Qt::blue, Qt::SolidPattern));
			m_selectionRectangle->setOpacity(0.3);
			addItem(m_selectionRectangle);
		}
		m_selectionRectangle->setRect(QRectF(e->scenePos(), QSize(0, 0)));
		m_mousePressed = true;
		m_mousePressedPos = e->scenePos();
		update();
		return;
	}
	else if (m_mouseMode == AwGraphicsScene::AddingMarker) {
		handleMousePressedAddingMarker(e);
	}
	else if (m_mouseMode == AwGraphicsScene::None) {
		QGraphicsItem* item = itemAt(e->scenePos(), QTransform());
		if (item) {
			QGraphicsScene::mousePressEvent(e);
			if (item->type() == QGraphicsItem::UserType + AW_GRAPHICS_ITEM_CURSOR_TYPE) {
				m_draggedCursor = static_cast<AwCursorItem*>(item);
				m_mouseMode = AwGraphicsScene::DraggingCursor;
			}
		}
	}
	else if (m_mouseMode == AwGraphicsScene::Mapping) {
		if (!m_selectionRectangle) {
			m_selectionRectangle = new QGraphicsRectItem(0);
			QColor mappingColor = QColor(AwUtilities::gui::mappingCursorColor());
			m_selectionRectangle->setZValue(100);
			m_selectionRectangle->setPen(QPen(mappingColor));
			m_selectionRectangle->setBrush(QBrush(mappingColor, Qt::SolidPattern));
			m_selectionRectangle->setOpacity(0.3);
			addItem(m_selectionRectangle);
		}
		m_selectionRectangle->setRect(QRectF(e->scenePos(), QSize(0, 0)));
		m_mousePressedPos = e->scenePos();
		m_mousePressed = true;
		m_mappingSelectionDuration = 0.;
		update();
	}
	else
		m_mousePressed = true;
	update();
}

///
/// mouseMove()
///
void AwGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* e)
{
	m_mousePos = e->scenePos();
	QGraphicsView* v = views().at(0);
	switch (m_mouseMode) {
	case AwGraphicsScene::DraggingCursor:
		m_draggedCursor->setPosition(m_settings->posInFile, timeAtPos(m_mousePos));
		update();
		emit draggedCursorPositionChanged(m_draggedCursor->currentPos());
		break;
	case AwGraphicsScene::Mapping:
		if (!m_mousePressed) {
			m_mappingCursor->setPos(v->mapToScene(m_mousePos.x(), 0));
			m_mappingCursor->setPositionInFile(m_settings->posInFile);
			update();
			emit mappingPositionChanged(m_mappingCursor->currentPos());
		}
		else { // mouse button is pressed 
			QPoint currentPos = e->scenePos().toPoint();
			QPoint previousPos = m_mousePressedPos.toPoint();
			previousPos.setY(0);

			int w = currentPos.x() - previousPos.x();
			//int h = currentPos.y() - previousPos.y();
			int h = (int)floor(this->sceneRect().height());
			QRectF rect(previousPos, QSize(w, h));
			m_selectionRectangle->setRect(rect);
			update();
		}

		break;
	case AwGraphicsScene::Cursor:
		if (m_mousePressed) {  // mouse button is pressed => do not move cursor but enlarge cursor selection instead
			m_cursor->setOtherPos(v->mapToScene(m_mousePos.x(), 0));
			update();
		}
		else {
			m_cursor->setPos(v->mapToScene(m_mousePos.x(), 0));
			m_cursor->setWidth(1.0);
			update();
			// get position of the cursor in seconds
			emit cursorPositionChanged(m_cursor->currentPos());
		}
		break;
	case AwGraphicsScene::AddingMarker:
	{
		float endPos = std::min(timeAtPos(m_mousePos), m_settings->fileDuration);
		if (m_mousePressed) {
			if (std::abs(endPos - m_currentMarkerItem->marker()->start()) > 0.01)
				m_isTimeSelectionStarted = true;
			if (m_isTimeSelectionStarted) {
				if (m_selectionIsActive) {
					if (!m_selectionRectangle) {
						m_selectionRectangle = new QGraphicsRectItem(0);
						m_selectionRectangle->setZValue(100);
						m_selectionRectangle->setPen(QPen(Qt::blue));
						m_selectionRectangle->setBrush(QBrush(Qt::blue, Qt::SolidPattern));
						m_selectionRectangle->setOpacity(0.3);
						addItem(m_selectionRectangle);
						m_selectionRectangle->setRect(m_mousePressedPos.x(), m_mousePressedPos.y(), 0, 0);
						update();
						break;
					}
					else {
						qreal x = e->scenePos().x();
						qreal y = e->scenePos().y();
						qreal ox = m_mousePressedPos.x();
						qreal oy = m_mousePressedPos.y();
						qreal xx = std::min(x, ox);
						qreal yy = std::min(y, oy);
						qreal w = std::abs(x - ox);
						qreal h = std::abs(y - oy);
						m_selectionRectangle->setRect(QRectF(xx, yy, w, h));
						if (m_settings->posInFile > m_positionClicked) { // we moved forward pressing space bar
							m_selectionRectangle->setRect(QRectF(0., yy, x, h));

						}
						m_currentMarkerItem->marker()->setStart(std::min(m_positionClicked, endPos));
						m_currentMarkerItem->marker()->setEnd(std::max(m_positionClicked, endPos));
						auto labels = getChannelsUnderSelectionRectangle();
						if (labels.size()) {
							QString toolTip;
							int row = 0;
							for (auto const& l : labels) {
								if (row++)
									toolTip += "\n";
								toolTip += QString("%1").arg(l);
							}
							m_selectionRectangle->setToolTip(toolTip);
						}
						update();
						break;
					}
				}
				else {
					m_currentMarkerItem->marker()->setStart(std::min(m_positionClicked, endPos));
					m_currentMarkerItem->marker()->setEnd(std::max(m_positionClicked, endPos));
					m_currentMarkerItem->updatePosition();
					update();
					break;
				}
			}
			else { // no time selection started
				m_currentMarkerItem->marker()->setStart(timeAtPos(m_mousePos));
				m_currentMarkerItem->marker()->setDuration(0.);
				m_currentMarkerItem->updatePosition();
				update();
			}
		}
		else { // the mouse button is not pressed
			if (m_selectionIsActive) { // ctrl key is held
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
			m_currentMarkerItem->marker()->setStart(timeAtPos(m_mousePos));
			m_currentMarkerItem->marker()->setDuration(0.);
			m_currentMarkerItem->updatePosition();
			if (m_selectionIsActive) {
				auto labels = getChannelsUnderSelectionRectangle();
				if (labels.size()) {
					QString toolTip;
					int row = 0;
					for (auto const& l : labels) {
						if (row++)
							toolTip += "\n";
						toolTip += QString("%1").arg(l);
					}
					m_selectionRectangle->setToolTip(toolTip);
				}
			}

			update();
		}
	}
		break;
	case AwGraphicsScene::QTS:
	{
		QGraphicsScene::mouseMoveEvent(e);
		if (!m_mousePressed)
			break;
		QPoint currentPos = e->scenePos().toPoint();
		QPoint previousPos = m_mousePressedPos.toPoint();
		int w = currentPos.x() - previousPos.x();
		int h = currentPos.y() - previousPos.y();
		QRectF rect(previousPos, QSize(w, h));
		m_selectionRectangle->setRect(rect);
		update();
	}
	break;
	case AwGraphicsScene::None:
		QGraphicsScene::mouseMoveEvent(e);
		if (m_mousePressed && m_selectionIsActive) {
			QPoint currentPos = e->scenePos().toPoint();
			QPoint previousPos = m_mousePressedPos.toPoint();

			int w = currentPos.x() - previousPos.x();
			int h = currentPos.y() - previousPos.y();
			QRectF rect(previousPos, QSize(w, h));
			m_selectionRectangle->setRect(rect);
			update();
		}
		else if (m_mousePressed && m_selectionRectangle) { // user has released Ctrl key while moving the selection area.
			removeItem(m_selectionRectangle);
		}
		update();
	}
}	

void AwGraphicsScene::applyMarkingToolSettings()
{
	if (m_markingTool->mode() == AwMarkingTool::Default && m_mouseMode == AwGraphicsScene::AddingMarker) {
		auto marker = m_currentMarkerItem->marker();
		marker->setLabel(m_markingTool->defaultMarker()->label());
		m_currentMarkerItem->marker()->setLabel(m_markingTool->defaultMarker()->label());
		marker->setValue(m_markingTool->defaultMarker()->value());
		m_currentMarkerItem->marker()->setValue(m_markingTool->defaultMarker()->value());
		marker->setColor(m_markingTool->defaultMarker()->color());
		m_currentMarkerItem->marker()->setColor(m_markingTool->defaultMarker()->color());
	}
}


///
/// mouseRelease()
///
void AwGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* e)
{
	// check that the position clicked is not AFTER the end of data
	if (m_positionClicked > m_settings->fileDuration)
		m_positionClicked = m_settings->fileDuration;

	if (e->button() != Qt::LeftButton) {
		if (m_isTimeSelectionStarted) { // a selection has started => cancel it
			m_currentMarkerItem->marker()->setDuration(0);
			m_currentMarkerItem->marker()->setStart(m_positionClicked);
			m_isTimeSelectionStarted = false;
			updateMarkers();
			update();
		}
		return;
	}
	if (m_mouseMode == AwGraphicsScene::DraggingCursor)
		m_mouseMode = AwGraphicsScene::None;

	switch (m_mouseMode) {
	case AwGraphicsScene::None:
		handleMouseReleaseNone(e);
		break;
	case AwGraphicsScene::QTS:
		handleMouseReleaseQTS(e);
		break;
	case AwGraphicsScene::AddingMarker:
		handleMouseReleaseAddingMarker(e);
		break;
	case AwGraphicsScene::Cursor:
		handleMouseReleaseCursor(e);
		break;
	case AwGraphicsScene::Mapping:
		handleMouseReleaseMapping(e);
		break;
	}
	m_mousePressed = false;
}

void AwGraphicsScene::handleMousePressedAddingMarker(QGraphicsSceneMouseEvent *e)
{
	m_mousePressed = true;
	m_positionClicked = timeAtPos(e->scenePos());
	m_mousePressedPos = e->scenePos();
}

QStringList AwGraphicsScene::getChannelsUnderSelectionRectangle()
{
	QStringList labels;
	if (qAbs(m_selectionRectangle->rect().size().width()) > 10 && qAbs(m_selectionRectangle->rect().size().height()) > 10) {
		// get item under selection rectangle
		QList<QGraphicsItem*> items = this->items(m_selectionRectangle->rect());
		if (!items.isEmpty()) {
			float pos = timeAtPos(QPointF(m_selectionRectangle->rect().x(), 0));
			float end = timeAtPos(QPointF(m_selectionRectangle->rect().x() + m_selectionRectangle->rect().width(), 0));

			for (QGraphicsItem* item : items) {
				// gets parents ot that item if any
				QGraphicsItem* parent = item->parentItem();
				while (parent) {
					item = parent;
					parent = parent->parentItem();
				}
				// cast to AwGraphicsSignalItem
				AwGraphicsSignalItem* sitem = qgraphicsitem_cast<AwGraphicsSignalItem*>(item);
				if (sitem) {
					auto label = sitem->channel()->fullName();
					if (!labels.contains(label))
						labels << label;
				}
			}
		}
	}
	return labels;
}

void AwGraphicsScene::handleMouseReleaseQTS(QGraphicsSceneMouseEvent* e)
{
	if (qAbs(m_selectionRectangle->rect().size().width()) > 10 && qAbs(m_selectionRectangle->rect().size().height()) > 10) {
		// get item under selection rectangle
		QList<QGraphicsItem*> items = this->items(m_selectionRectangle->rect());
		if (!items.isEmpty()) {
			AwChannelList channels, temp;
			float pos = timeAtPos(QPointF(m_selectionRectangle->rect().x(), 0));
			float end = timeAtPos(QPointF(m_selectionRectangle->rect().x() + m_selectionRectangle->rect().width(), 0));

			for (QGraphicsItem* item : items) {
				// gets parents ot that item if any
				QGraphicsItem* parent = item->parentItem();
				while (parent) {
					item = parent;
					parent = parent->parentItem();
				}
				// cast to AwGraphicsSignalItem
				AwGraphicsSignalItem* sitem = qgraphicsitem_cast<AwGraphicsSignalItem*>(item);
				if (sitem) {
					if (!temp.contains(sitem->channel())) {
						channels << sitem->channel()->duplicate();
						temp << sitem->channel();
					}
				}
			}
			if (m_QTSMenu) {
				m_QTSMenu->exec(e->screenPos());
				if (!m_pluginToLaunch.isEmpty())
					emit processSelectedForLaunch(m_pluginToLaunch, channels, pos, end);
			}
			update();
		}
		removeItem(m_selectionRectangle);
		emit QTSModeEnded();
	}
	setQTSMode(false);
}

void AwGraphicsScene::handleMouseReleaseCursor(QGraphicsSceneMouseEvent* e)
{
	m_cursor->noOtherPos();
	emit clickedAtTime(m_positionClicked);
	emit cursorClickedAtTime(m_positionClicked);
	update();
}

void AwGraphicsScene::handleMouseReleaseMapping(QGraphicsSceneMouseEvent* e)
{
	if (m_mappingFixedCursor == nullptr) {
		m_mappingFixedCursor = new AwMappingCursorItem(m_settings->posInFile, m_positionClicked, AwUtilities::gui::mappingCursorColor(), AwUtilities::gui::mappingCursorFont(), m_settings, AwMappingCursorItem::Fixed);
		addItem(m_mappingFixedCursor);
	}
	QPointF pos = e->scenePos();

	m_mappingFixedCursor->setPos(views().at(0)->mapToScene(pos.x(), 0));
	if (m_selectionIsActive)
		selectChannelAtPosition(e->scenePos());
	if (m_selectionRectangle) {
		float duration = timeAtPos(e->scenePos()) - m_positionClicked;
		if (duration < 0) {
			m_positionClicked -= duration;
			if (m_positionClicked < 0)
				m_positionClicked = 0.;
			duration = qAbs(duration);
		}
		// need at least a 5 pixels large selection
		if (qAbs(e->scenePos().x() - m_mousePressedPos.x()) < 5)
			emit clickedAtTime(m_positionClicked);
		else {
			m_mappingSelectionDuration = duration;
			emit mappingTimeSelectionDone(m_positionClicked, duration);
		}
	}
	else {
		emit cursorClickedAtTime(timeAtPos(pos));
		emit clickedAtTime(m_positionClicked);
	}
	// update mapping marker
	m_mappingMarker->setLabel(m_markingTool->defaultMarker()->label());
	m_mappingMarker->setStart(m_positionClicked);
	m_mappingMarker->setDuration(m_mappingSelectionDuration);
	m_mappingMarker->setValue(m_markingTool->defaultMarker()->value());
	m_mappingMarker->setColor(m_markingTool->defaultMarker()->color());
}

void AwGraphicsScene::handleMouseReleaseAddingMarker(QGraphicsSceneMouseEvent* e)
{
	auto defaultFunction = [=](const AwSharedMarker& marker) {
		m_currentMarkerItem->marker()->setColor(marker->color());
		m_currentMarkerItem->marker()->setLabel(marker->label());
		m_currentMarkerItem->marker()->setValue(marker->value());
		// if the Ctrl key is pressed => try to determine the channel under the mouse 
		if (m_selectionIsActive) {
			if (m_selectionRectangle) { // should also work for single markers.
				auto labels = getChannelsUnderSelectionRectangle();
				if (labels.size()) {
					m_currentMarkerItem->marker()->setTargetChannels(labels);
					// ask to manually select channels
					if (labels.size() > 1) 
						manuallySetChannelsTarget(labels);
				}
				update();
				removeItem(m_selectionRectangle);
			}
		}
	};

	switch (m_markingTool->mode()) {
	case AwMarkingTool::Default: {
		defaultFunction(m_markingTool->defaultMarker());
		emit markerInserted(m_currentMarkerItem->marker());
		m_currentMarkerItem->setMarker(AwSharedMarker(new AwMarker));
		break;
	}
	case AwMarkingTool::Custom:
	{
		auto customList = m_markingTool->customList();
		if (customList.isEmpty()) {  // SHOULD NEVER HAPPENS
			defaultFunction(m_markingTool->defaultMarker());
			emit markerInserted(m_currentMarkerItem->marker());
			m_currentMarkerItem->setMarker(AwSharedMarker(new AwMarker));
			// force marking tool back to Default 
			m_markingTool->setToDefault();
			break;
		}
		if (customList.size() == 1) {
			defaultFunction(customList.first());
				emit markerInserted(m_currentMarkerItem->marker());
			m_currentMarkerItem->setMarker(AwSharedMarker(new AwMarker));
			break;
		}
		// build slot as lambda
		auto insertLambda = [=]() {
			QAction* act = (QAction*)sender();
			int index = act->data().toInt();
			auto marker = customList.at(index);
			defaultFunction(marker);
			emit markerInserted(m_currentMarkerItem->marker());
			m_currentMarkerItem->setMarker(AwSharedMarker(new AwMarker));
		};
		// several item in list => build menu
		auto menu = new QMenu;
		int index = 0;
		for (auto const& marker : customList) {
			auto action = menu->addAction(marker->label());
			action->setData(index++);
			connect(action, &QAction::triggered, this, insertLambda);
		}
		if (menu->exec(e->screenPos()) == nullptr) { // no actions taken in the menu => do reset marker insertion
			m_currentMarkerItem->setMarker(AwSharedMarker(new AwMarker()));
			m_currentMarkerItem->marker()->setDuration(0);
			m_currentMarkerItem->marker()->setStart(m_positionClicked);
		}
		delete menu;
		break;
	}
	case AwMarkingTool::HED:
	{
		auto hedList = m_markingTool->hedList();
		if (hedList.isEmpty()) {  // SHOULD NEVER HAPPEN
			defaultFunction(m_markingTool->defaultMarker());
			emit markerInserted(m_currentMarkerItem->marker());
			m_currentMarkerItem->setMarker(AwSharedMarker(new AwMarker));
			// force marking tool back to Default 
			m_markingTool->setToDefault();
			break;
		}
		if (hedList.size() == 1) {
			auto marker = hedList.first();
			defaultFunction(marker);
			emit markerInserted(m_currentMarkerItem->marker());
			m_currentMarkerItem->setMarker(AwSharedMarker(new AwMarker));
			break;
		}
		// build slot as lambda
		auto insertLambda = [=]() {
			QAction* act = (QAction*)sender();
			int index = act->data().toInt();
			auto marker = hedList.at(index);
			defaultFunction(marker);
			emit markerInserted(m_currentMarkerItem->marker());
			m_currentMarkerItem->setMarker(AwSharedMarker(new AwMarker));
		};
		// several item in list => build menu
		auto menu = new QMenu;
		int index = 0;
		for (auto const& marker : hedList) {
			auto action = menu->addAction(marker->label());
			action->setData(index++);
			connect(action, &QAction::triggered, this, insertLambda);
		}
		if (menu->exec(e->screenPos()) == nullptr) { // no actions taken in the menu => do reset marker insertion
			m_currentMarkerItem->setMarker(AwSharedMarker(new AwMarker()));
			m_currentMarkerItem->marker()->setDuration(0);
			m_currentMarkerItem->marker()->setStart(m_positionClicked);
		}
		delete menu;
	}
	break;
	default:
		break;
	}
	updateMarkers();
	update();
}


void AwGraphicsScene::handleMouseReleaseNone(QGraphicsSceneMouseEvent* e)
{
	// handling draging of signal items!
	if (m_itemsDragged && m_itemsHaveMoved) {
		m_itemsHaveMoved = false;
		m_itemsDragged = false;
		reorderItems();
		QGraphicsScene::mouseReleaseEvent(e);
		return;
	}
	if (m_selectionRectangle) {
		// Selection must be greater than a 10x10 rectangle, otherwise it will be considered as a simple selection of the item under the mouse.
		if (qAbs(m_selectionRectangle->rect().size().width()) > 10 && qAbs(m_selectionRectangle->rect().size().height()) > 10) {
			// get item under selection rectangle
			QList<QGraphicsItem*> items = this->items(m_selectionRectangle->rect());
			if (!items.isEmpty()) {
				foreach(QGraphicsItem * item, items) {
					// gets parents ot that item if any
					QGraphicsItem* parent = item->parentItem();
					while (parent) {
						item = parent;
						parent = parent->parentItem();
					}
					// cast to AwGraphicsSignalItem
					AwGraphicsSignalItem* sitem = qgraphicsitem_cast<AwGraphicsSignalItem*>(item);
					if (sitem) {
						sitem->setSelected(true);
						sitem->channel()->setSelected(true);
					}
				}
				updateSelection();
				update();
			}
			removeItem(m_selectionRectangle);
			return;
		}
		else
			selectChannelAtPosition(e->scenePos());
	}
	QGraphicsScene::mouseReleaseEvent(e);

	updateSelection();
	update();
	emit clickedAtTime(m_positionClicked);
}