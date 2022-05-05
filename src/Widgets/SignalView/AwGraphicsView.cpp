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
#include <widget/SignalView/AwGraphicsView.h>
#include <widget/SignalView/AwGraphicsScene.h>
#include <QtMath>
#include <QScrollBar>
#include <QMap>


AwGraphicsView::AwGraphicsView(QGraphicsScene *scene, AwViewSettings *settings, 
			   AwDisplayPhysics *phys, QWidget *parent) : QGraphicsView(scene, parent)
{
	setOptimizationFlags(DontAdjustForAntialiasing);
	setCacheMode(QGraphicsView::CacheNone);
	setDragMode(NoDrag);
	setViewportUpdateMode(NoViewportUpdate);
	setFocusPolicy(Qt::StrongFocus);
	m_previousPageDuration = m_pageDuration = m_posInFile = m_timeOffset = 0.;
	m_physics = phys;
	applySettings(settings);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_secsPerCm = settings->secsPerCm;
	m_timeScaleMode = settings->timeScaleMode;
}	

void AwGraphicsView::setTimeShift(float shift)
{
	m_timeOffset = shift;
	resetCachedContent();
	repaint();
}

void AwGraphicsView::computePageDuration()
{
	float w = (float)viewport()->rect().width();
	if (m_timeScaleMode == AwViewSettings::PaperLike) {
		m_pageDuration = (w / m_physics->xPixPerCm()) * m_settings->secsPerCm;
		m_physics->setXPixPerSec(w / m_pageDuration);
		m_physics->setPageDuration(m_pageDuration);
	}
	else {
		m_pageDuration = m_settings->fixedPageDuration;
		//m_physics->setXPixPerSec(w / m_pageDuration);
		m_physics->setPageDuration(m_pageDuration);
		m_physics->setFixedPageDuration(m_pageDuration, w);
	}
	if (m_pageDuration == 0.)
		return;
	emit pageDurationChanged(m_pageDuration);
}


void AwGraphicsView::resizeEvent(QResizeEvent *event)
{
	computePageDuration();
	layoutItems();
	AwGraphicsScene *gs = (AwGraphicsScene *)scene();
	gs->refresh();
	resetCachedContent();
	repaint();
	gs->updateChannelsData();
	gs->update();
	QRectF rect = mapToScene(viewport()->geometry()).boundingRect();
	auto amplitudeScale = gs->amplitudeScale();
	amplitudeScale->setPos(rect.width() - amplitudeScale->boundingRect().width() - 10, rect.bottom() - amplitudeScale->boundingRect().height() - 10);
}

void AwGraphicsView::scrollContentsBy(int dx, int dy)
{
	QGraphicsView::scrollContentsBy(dx, dy);
	update();
	AwGraphicsScene* gs = (AwGraphicsScene*)scene();
	QRectF rect = mapToScene(viewport()->geometry()).boundingRect();
	auto amplitudeScale = gs->amplitudeScale();
	amplitudeScale->setPos(rect.width() - amplitudeScale->boundingRect().width() - 10, rect.bottom() - amplitudeScale->boundingRect().height() - 10);
	scene()->update();
}

void AwGraphicsView::setPositionInFile(float pos)
{
	m_posInFile = pos;
}

void AwGraphicsView::drawBackground(QPainter *painter, const QRectF& rect)
{
	if (!m_settings->ShowTimeGrid)
		return;
	QPen pen(Qt::blue);
	pen.setStyle(Qt::DotLine);
	bool show100ms = false;
	int next100ms;
	float nextSecond;
	float startingOffset100ms;
	float startingOffsetSecond;

	QVarLengthArray<QLineF, 100> lines;
	QVarLengthArray<QLineF, 30> linesMs;
	float posInFile = m_posInFile + m_timeOffset;
	nextSecond = ceil(posInFile);
	startingOffsetSecond = (nextSecond - posInFile) * m_physics->xPixPerSec();
	float pixPer100ms = m_physics->xPixPerSec() / 10;

	if (m_settings->timeScaleMode == AwViewSettings::PaperLike) {
		if (m_settings->secsPerCm < 0.1) { // when the time scale is < 0.5s/cm display the 100ms lines
			show100ms = true;
			// compute the starting 100ms line
			quint32 timeMs = (quint32)floor(std::abs(posInFile)) * 1000;
			float diff = (posInFile * 1000) - timeMs;
			next100ms = (int)floor(diff / 100);
			next100ms *= 100;
			startingOffset100ms = ((next100ms - diff) / 1000) * m_physics->xPixPerSec();
			next100ms += 100;
		}
	}

	for (float i = startingOffsetSecond; i < rect.width(); i += m_physics->xPixPerSec())
		lines.append(QLineF(i, rect.y(), i, rect.height() + rect.y()));

	if (show100ms)
		for (float i = startingOffset100ms; i < rect.width(); i += pixPer100ms)
			linesMs.append(QLineF(i, rect.y(), i, rect.height() + rect.y()));

	if (m_settings->showSeconds) {
		QPen pen(Qt::darkBlue);
		QFont font("Times", 14, QFont::Bold);
		QFontMetrics metrics(font);

		painter->setPen(pen);
		painter->setFont(font);
		int value = (int)nextSecond;
		int step5 = 0;
		QRectF tmpRect; // hold rects for displaying timing text values

		// check if recorded time is really possible
		bool recordedTimeDisabled = m_startTime.isNull() || !m_startTime.isValid();
		if (recordedTimeDisabled)
			m_settings->timeMode = AwViewSettings::ShowRelativeTime;
		for (auto x = startingOffsetSecond; x < rect.width(); x += m_physics->xPixPerSec()) {
			QString text;
			
			if (m_settings->timeMode == AwViewSettings::ShowRelativeTime)
				text = QString("%1").arg(value);
			else {
				auto time = m_startTime.addSecs(value);
				text = QString("%1").arg(time.toString(Qt::TextDate));
			}
			int w = metrics.width(text);
			QRectF rectTimeValue(x - w / 2, 0., (qreal)w, (qreal)metrics.height());
			value++;

			// check with tmpRect if exists
			if (tmpRect.isNull()) {
				painter->drawText(rectTimeValue.x(), rect.y() + rectTimeValue.height(), text);
				painter->drawText(rectTimeValue.x(), (rect.y() + rect.height()) - rectTimeValue.height(), text);
				tmpRect = rectTimeValue;
			}
			else {
				if (!rectTimeValue.intersects(tmpRect)) {
					painter->drawText(rectTimeValue.x(), rect.y() + rectTimeValue.height(), text);
					painter->drawText(rectTimeValue.x(), (rect.y() + rect.height()) - rectTimeValue.height(), text);
					tmpRect = rectTimeValue;
				}
			}
		}

		if (show100ms) {
			QFont fontMs("Times", 8, QFont::Normal);
			QFontMetrics metricsMs(fontMs);
			painter->setFont(fontMs);

			// skip first value as it is the starting position
			for (float i = startingOffset100ms + pixPer100ms; i < rect.width(); i += pixPer100ms) {
				if (next100ms == 1000) { // skip seconds
					next100ms = 100;
					continue;
				}

				QString text = "+" + QString::number(next100ms) + "ms";
				int w = metricsMs.width(text);

				painter->drawText(i - w / 2, rect.y() + metricsMs.height(), text);
				painter->drawText(i - w / 2, (rect.y() + rect.height()) - metricsMs.height(), text);
				next100ms += 100;
			}
		}
	}
	painter->setPen(pen);
	painter->drawLines(lines.data(), lines.size());
	painter->drawLines(linesMs.data(), linesMs.size());
}


void AwGraphicsView::updateSignalChildrenPositions()
{
	AwGraphicsScene* gs = static_cast<AwGraphicsScene*>(scene());
	auto sitems = gs->visibleSignalItems();
	int i = 1;
	while (i < sitems.size()) {
		auto item1 = sitems.at(i - 1);
		auto item2 = sitems.at(i);
		item2->resolveCollisionWithUpperNeighbor(item1->childrenRegion());
		i++;
	}
}

void AwGraphicsView::layoutItems()
{
	int savedPos = verticalScrollBar()->value();
	int numberOfSignalItems = 0;
	QRectF vpRect = rect();  //use view rect instead of viewport rect (scrollbar width is included)
	qreal viewportH = viewport()->rect().height(); // get the viewport height (not the view height : again scrollbar geometry is included)

	QRectF newRect = rect();
	newRect.setHeight(viewportH);

	qreal currentVPos = 0.;
	qreal extraSceneH = 0; // height to add to the scene depending on item layouts
	qreal offset = 0; // height offset between channels

	scene()->setSceneRect(newRect);
	AwGraphicsScene *gs = (AwGraphicsScene *)scene();

	if (m_settings->stackChannels) { // OVERLAY
		QMultiMap<int, AwGraphicsSignalItem*> map;
		QList<AwGraphicsSignalItem*> sitems = gs->visibleSignalItems();
		for (auto item : sitems)
			map.insert(item->channel()->type(), item);
		if (map.isEmpty())
			return;
		int nChannelsToDisplay = map.uniqueKeys().size();
		qreal h;
		if (!m_settings->limitChannels)
			offset = viewportH / (qreal(nChannelsToDisplay) + 1);
		else if (m_settings->limitChannels && nChannelsToDisplay <= m_settings->maxChannels)
			offset = viewportH / (qreal(nChannelsToDisplay) + 1);
		else if (m_settings->limitChannels) {
			// the number of items to display is greater than the limit fixed in viewing options.
			// Compute the offset to fit the max channel number in one page height.
			offset  = viewportH / (m_settings->maxChannels + 1);
			// compute the total height to fit all the items
			qreal totalH = numberOfSignalItems * offset + offset;
			newRect.setHeight(totalH);
		}

		// layout items
		for (auto k : map.uniqueKeys()) {
			auto values = map.values(k);
			currentVPos += offset;
			for (auto v : values) 
				v->setPos(0, currentVPos);
			if (currentVPos > newRect.height())
				extraSceneH = currentVPos - newRect.height();
		}
		newRect.setHeight(newRect.height() + extraSceneH);
		// 
		scene()->setSceneRect(newRect);
	}  // END IF OVERLAY
	else { // Normal layout without overlaying channels
		QList<AwGraphicsSignalItem *> sitems = gs->visibleSignalItems();
		numberOfSignalItems = sitems.size();
		if (numberOfSignalItems == 0)
			return;
		if (!m_settings->limitChannels)
			offset = viewportH / (numberOfSignalItems + 1);
		else if (m_settings->limitChannels && numberOfSignalItems <= m_settings->maxChannels)
			offset = viewportH / (numberOfSignalItems + 1);
		else if (m_settings->limitChannels) {
			// the number of items to display is greater than the limit fixed in viewing options.
			// Compute the offset to fit the max channel number in one page height.
			offset  = viewportH / (m_settings->maxChannels + 1);

			// compute the total height to fit all the items
			qreal totalH = numberOfSignalItems * offset + offset;
			newRect.setHeight(totalH);
		}
		// layout items
		qreal prevItemPos = 0.;
		qreal vPos = 0;
		QVector<qreal> yPositions(sitems.size());
		int i = 0;

		qreal nextPosition;
		for (AwGraphicsSignalItem* item : sitems) {
			item->updateGeometry();
			QSize itemSize = item->size();
			vPos += itemSize.height() / 4;
			nextPosition = itemSize.height() / 4;
			vPos += offset;
			if (vPos > newRect.height()) {
				extraSceneH = vPos - newRect.height();
			}
			yPositions[i++] = vPos;
			// compute next item position (zero line)
			vPos += nextPosition;
		}
		extraSceneH += sitems.last()->size().height() / 4;
		// resize scene rect BEFORE moving items on it (to avoid itemChange method of each items to constrain them)
		newRect.setHeight(newRect.height() + extraSceneH);
		scene()->setSceneRect(newRect);
		i = 0;
		sitems.first()->setUpperNeighbor(nullptr);
		for (AwGraphicsSignalItem* item : sitems) {
			item->setPos(0., yPositions.at(i));
			if (i)
				item->setUpperNeighbor(sitems.at(i - 1));
			i++;
		}
	}
	// restore view vertical position
	if (savedPos)
		verticalScrollBar()->setValue(savedPos);
}

void AwGraphicsView::applySettings(AwViewSettings *settings)
{
	if (settings == nullptr)
		return;
	m_settings = settings;
	m_secsPerCm = settings->secsPerCm;
	m_physics->setSecsPerCm(settings->secsPerCm);
	computePageDuration();
	resetCachedContent();
	repaint();
	layoutItems();
	update();
}

void AwGraphicsView::updateSettings(AwViewSettings *settings, int flags)
{
	if (flags & AwViewSettings::ShowSecondTicks || flags & AwViewSettings::ShowTimeGrid 
		|| flags & AwViewSettings::TimeRepresentation) {
		resetCachedContent();
		repaint();
	}

	if (flags & AwViewSettings::ShowAmplitudeScale) {
		AwGraphicsScene* gs = (AwGraphicsScene*)scene();
		QRectF rect = mapToScene(viewport()->geometry()).boundingRect();
		auto amplitudeScale = gs->amplitudeScale();
		amplitudeScale->setPos(rect.width() - amplitudeScale->boundingRect().width() - 10, rect.bottom() - amplitudeScale->boundingRect().height() - 10);
	}

	if (flags & AwViewSettings::SecPerCm) {
		// compute new page duration only if setting really changed
		if (m_settings->secsPerCm != m_secsPerCm) {
			m_physics->setSecsPerCm(settings->secsPerCm);
			computePageDuration();
			m_secsPerCm = m_settings->secsPerCm;
			emit pageDurationChanged(m_pageDuration);
		}
	}

	if (flags & AwViewSettings::TimeScaleMode) {
		if (m_settings->timeScaleMode != m_timeScaleMode) {
			m_timeScaleMode = settings->timeScaleMode;
			if (m_timeScaleMode == AwViewSettings::PaperLike) {
				m_physics->setSecsPerCm(settings->secsPerCm);
				computePageDuration();
				m_secsPerCm = m_settings->secsPerCm;
				emit pageDurationChanged(m_pageDuration);
			}
			else {
				computePageDuration();
				
				emit pageDurationChanged(m_pageDuration);
			}
			resetCachedContent();
			repaint();
		}
	}
	// we can have to update the page duration while already switched to this mode
	if (flags & AwViewSettings::PageDuration) {
		if (m_settings->fixedPageDuration != m_pageDuration) {
			computePageDuration(); // not really recompute but propagate page duration to Physics in order to get the good pixel to sample transform
			resetCachedContent();
			repaint();
		}
	}

	if (flags & AwViewSettings::MaxNumberOfChannels || flags & AwViewSettings::LimitNumberOfChannels || flags & AwViewSettings::Overlay)
		layoutItems();
	if (flags & AwViewSettings::ShowSensors) {
		updateSignalChildrenPositions();
	}
}
