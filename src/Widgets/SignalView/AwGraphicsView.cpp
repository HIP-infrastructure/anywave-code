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


AwGraphicsView::AwGraphicsView(QGraphicsScene *scene, AwViewSettings *settings, QWidget *parent) : QGraphicsView(scene, parent)
{
	setOptimizationFlags(DontAdjustForAntialiasing);
	setCacheMode(QGraphicsView::CacheNone);
	setDragMode(NoDrag);
	setViewportUpdateMode(NoViewportUpdate);
	setFocusPolicy(Qt::StrongFocus);
	m_settings = settings;
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	updateSettings(aw::view_settings::time_scale);
	updateSettings(aw::view_settings::time_representation);
	updateSettings(aw::view_settings::show_seconds);
	updateSettings(aw::view_settings::show_sensors);
	updateSettings(aw::view_settings::limit_channel_per_view);
	updateSettings(aw::view_settings::max_visible_channels);
	updateSettings(aw::view_settings::stack_channels);
	updateSettings(aw::view_settings::file_duration);
	updateSettings(aw::view_settings::pos_in_file);
}	

void AwGraphicsView::computePageDuration()
{
	float w = (float)viewport()->rect().width();
	if (m_settings->timeScale == AwViewSettings::PaperLike) {
		m_settings->physics.unsetFixedPageDuration();
		m_settings->pageDuration = (w / m_settings->physics.xPixPerCm()) * m_settings->secsPerCm();
	}
	else {
		m_settings->physics.setFixedPageDuration(m_settings->fixedPageDuration, w);
		m_settings->pageDuration = m_settings->fixedPageDuration;
		resetCachedContent();
		repaint();
	}

	emit settingsChanged(aw::view_settings::page_duration, aw::view_settings::sender_view);
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

void AwGraphicsView::drawBackground(QPainter *painter, const QRectF& rect)
{
	if (!m_settings->showTimeGrid)
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
	float posInFile = m_settings->posInFile;
	nextSecond = ceil(posInFile);
	startingOffsetSecond = (nextSecond - posInFile) * m_settings->physics.xPixPerSec();
	float pixPer100ms = m_settings->physics.xPixPerSec() / 10;

	if (m_settings->timeScale == AwViewSettings::PaperLike) {
		if (m_settings->secsPerCm() < 0.1) { // when the time scale is < 0.5s/cm display the 100ms lines
			show100ms = true;
			// compute the starting 100ms line
			quint32 timeMs = (quint32)floor(std::abs(posInFile)) * 1000;
			float diff = (posInFile * 1000) - timeMs;
			next100ms = (int)floor(diff / 100);
			next100ms *= 100;
			startingOffset100ms = ((next100ms - diff) / 1000) * m_settings->physics.xPixPerSec();
			next100ms += 100;
		}
	}

	for (float i = startingOffsetSecond; i < rect.width(); i += m_settings->physics.xPixPerSec())
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
			m_settings->timeRepresentation = AwViewSettings::ShowRelativeTime;
		auto timeMode = m_settings->timeRepresentation;
		for (auto x = startingOffsetSecond; x < rect.width(); x += m_settings->physics.xPixPerSec()) {
			QString text;
			if (timeMode == AwViewSettings::ShowRelativeTime)
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
		if (!m_settings->limitChannelPerView)
			offset = viewportH / (qreal(nChannelsToDisplay) + 1);
		else if (m_settings->limitChannelPerView && nChannelsToDisplay <= m_settings->maxVisibleChannels)
		

			offset = viewportH / (qreal(nChannelsToDisplay) + 1);
		else if (m_settings->limitChannelPerView) {
			// the number of items to display is greater than the limit fixed in viewing options.
			// Compute the offset to fit the max channel number in one page height.
			offset  = viewportH / (m_settings->maxVisibleChannels + 1);
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
		if (!m_settings->limitChannelPerView)
			offset = viewportH / (numberOfSignalItems + 1);
		else if (m_settings->limitChannelPerView && numberOfSignalItems <= m_settings->maxVisibleChannels)
			offset = viewportH / (numberOfSignalItems + 1);
		else if (m_settings->limitChannelPerView) {
			// the number of items to display is greater than the limit fixed in viewing options.
			// Compute the offset to fit the max channel number in one page height.
			offset  = viewportH / (m_settings->maxVisibleChannels + 1);

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

void AwGraphicsView::updateSettings(int key)
{
	switch (key) {
	case aw::view_settings::show_time_grid:
	case aw::view_settings::show_seconds:
	case aw::view_settings::time_representation:
	case aw::view_settings::pos_in_file:
		resetCachedContent();
		repaint();
		break;
	case aw::view_settings::show_amplitude_scales:
	{
		if (m_settings->showAmplScales) {
			AwGraphicsScene* gs = (AwGraphicsScene*)scene();
			QRectF rect = mapToScene(viewport()->geometry()).boundingRect();
			auto amplitudeScale = gs->amplitudeScale();
			amplitudeScale->setPos(rect.width() - amplitudeScale->boundingRect().width() - 10, rect.bottom() - amplitudeScale->boundingRect().height() - 10);
		}
	}
		break;
	case aw::view_settings::secs_per_cm:
	case aw::view_settings::time_scale:
	case aw::view_settings::fixed_page_duration:
		m_settings->physics.setSecsPerCm(m_settings->secsPerCm());
		computePageDuration();
		break;
	case aw::view_settings::max_visible_channels:
	case aw::view_settings::limit_channel_per_view:
	case aw::view_settings::stack_channels:
		layoutItems();
		break;
	case aw::view_settings::show_sensors:
		updateSignalChildrenPositions();
		break;
	}
}