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
	//if (m_previousPageDuration == 0. || m_previousPageDuration != m_pageDuration) {
	//	m_previousPageDuration = m_pageDuration;
	//	emit pageDurationChanged(m_pageDuration);
	//}
	emit pageDurationChanged(m_pageDuration);
}


void AwGraphicsView::resizeEvent(QResizeEvent *event)
{
	//float duration = m_pageDuration;
	computePageDuration();
	//// update page duration
	//if (m_settings->timeScaleMode == AwViewSettings::PaperLike)
	//	computePageDuration();
	//else {
	//	//float w = (float)viewport()->rect().width();
	//	//m_physics->setPageDuration(m_pageDuration);
	//	//m_physics->setFixedPageDuration(m_pageDuration, w);
	//	computePageDuration();
	//}
	layoutItems();

	//// don't send signal if duration is the same.
	//if (duration != m_pageDuration)
	//	emit pageDurationChanged(m_pageDuration);

	AwGraphicsScene *gs = (AwGraphicsScene *)scene();
	gs->refresh();
	resetCachedContent();
	repaint();
	scene()->update();
}

void AwGraphicsView::scrollContentsBy(int dx, int dy)
{
	QGraphicsView::scrollContentsBy(dx, dy);
	update();
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
		QList<AwGraphicsSignalItem *> EEG;
		QList<AwGraphicsSignalItem *> MEG;
		QList<AwGraphicsSignalItem *> GRAD;
		QList<AwGraphicsSignalItem *> ICA;
		QList<AwGraphicsSignalItem *> Source;
		QList<AwGraphicsSignalItem *> SEEG;
		QList<AwGraphicsSignalItem *> REF;
		QList<AwGraphicsSignalItem *> Others;
		QList<AwGraphicsSignalItem *> sitems = gs->visibleSignalItems();
		foreach (AwGraphicsSignalItem *item, sitems) {
			switch (item->channel()->type()) {
			case AwChannel::SEEG:
				SEEG.append(item);
				break;
			case AwChannel::EEG:
				EEG.append(item);
				break;
			case AwChannel::MEG:
				MEG.append(item);
				break;
			case AwChannel::Reference:
				REF.append(item);
				break;
			case AwChannel::GRAD:
				GRAD.append(item);
				break;
			case AwChannel::ICA:
				ICA.append(item);
				break;
			case AwChannel::Source:
				Source.append(item);
				break;
			default:
				Others.append(item);
				break;
			}
			numberOfSignalItems++;
		}	
	
		if (numberOfSignalItems == 0)
			return;

		int nChannelsToDisplay = numberOfSignalItems;
		
		if (!EEG.isEmpty())
			nChannelsToDisplay -= (EEG.count() - 1);
		if (!MEG.isEmpty())
			nChannelsToDisplay -= (MEG.count() - 1);
		if (!SEEG.isEmpty())
			nChannelsToDisplay -= (SEEG.count() - 1);
		if (!REF.isEmpty())
			nChannelsToDisplay -= (REF.count() - 1);
		if (!GRAD.isEmpty())
			nChannelsToDisplay -= (GRAD.count() - 1);
		if (!ICA.isEmpty())
			nChannelsToDisplay -= (ICA.count() - 1);
		if (!Source.isEmpty())
			nChannelsToDisplay -= (Source.count() - 1);

		qreal h;
		if (!m_settings->limitChannels)
			offset = viewportH / (nChannelsToDisplay + 1);
		else if (m_settings->limitChannels && nChannelsToDisplay <= m_settings->maxChannels)
			offset = viewportH / (nChannelsToDisplay + 1);
		else if (m_settings->limitChannels) {
			// the number of items to display is greater than the limit fixed in viewing options.
			// Compute the offset to fit the max channel number in one page height.
			offset  = viewportH / (m_settings->maxChannels + 1);

			// compute the total height to fit all the items
			qreal totalH = numberOfSignalItems * offset + offset;
			newRect.setHeight(totalH);
		}

		// layout items
		if (!EEG.isEmpty()) {
			currentVPos += offset;
			for (qint32 i = 0; i < EEG.count(); i++)
				EEG.at(i)->setPos(0, currentVPos);
			if (currentVPos > newRect.height())
				extraSceneH = currentVPos -  newRect.height();
		}

		if (!SEEG.isEmpty()) {
			currentVPos += offset;
			for (qint32 i = 0; i < SEEG.count(); i++)
				SEEG.at(i)->setPos(0, currentVPos);
			if (currentVPos > newRect.height())
				extraSceneH = currentVPos -  newRect.height();
		}

		if (!MEG.isEmpty())	{
			currentVPos += offset;
			for (qint32 i = 0; i < MEG.count(); i++)
				MEG.at(i)->setPos(0, currentVPos);
			if (currentVPos > newRect.height())
				extraSceneH = currentVPos -  newRect.height();
		}

		if (!REF.isEmpty()) 	{
			currentVPos += offset;
			for (qint32 i = 0; i < REF.count(); i++)
				REF.at(i)->setPos(0, currentVPos);
			if (currentVPos > newRect.height())
				extraSceneH = currentVPos -  newRect.height();
		}
		if (!GRAD.isEmpty()) {
			currentVPos += offset;
			for (qint32 i = 0; i < GRAD.count(); i++)
				GRAD.at(i)->setPos(0, currentVPos);
			if (currentVPos > newRect.height())
				extraSceneH = currentVPos - newRect.height();
		}
		if (!ICA.isEmpty()) {
			currentVPos += offset;
			for (qint32 i = 0; i < ICA.count(); i++)
				ICA.at(i)->setPos(0, currentVPos);
			if (currentVPos > newRect.height())
				extraSceneH = currentVPos - newRect.height();
		}
		if (!Source.isEmpty()) {
			currentVPos += offset;
			for (qint32 i = 0; i < Source.count(); i++)
				Source.at(i)->setPos(0, currentVPos);
			if (currentVPos > newRect.height())
				extraSceneH = currentVPos - newRect.height();
		}

		if (!Others.isEmpty())		{
			foreach (AwGraphicsSignalItem *item, Others) {
				currentVPos += offset;
				item->setPos(0, currentVPos);
				if (currentVPos > newRect.height())
					extraSceneH = currentVPos -  newRect.height();
			}
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
		foreach(AwGraphicsSignalItem *item, sitems)	{
			item->updateGeometry();
			QSize itemSize = item->size();
			// restore shifted label position to zero
			item->shiftLabel(0);

			vPos = prevItemPos + offset;
			if (vPos > newRect.height()) {
				extraSceneH = vPos - newRect.height();
			}
			if (vPos - itemSize.height() / 2 < prevItemPos) {
				vPos += itemSize.height() / 2 - offset;
				item->setPos(0, vPos);
				prevItemPos = vPos + itemSize.height() / 2;
				vPos = prevItemPos + offset;
			}
			else {
				item->setPos(0, vPos);
				prevItemPos = vPos;
			}
			if (vPos > newRect.height()) {
				extraSceneH = vPos - newRect.height();
			}
			// default label height is using a font height of 12 (ARIAL)
			// if the offset between channels is less than 12 points, then reduce the font height to 8 for labels.
			if (itemSize.height() == 0)
				offset <= 12 ?	item->setLabelHeight(8) : item->setLabelHeight(12);
		}
		// check for label bounding boxes (if they are overlapping, shift them)
		int i = 1;
		while (i < sitems.size()) {
			AwGraphicsSignalItem *item1 = sitems.at(i - 1);
			AwGraphicsSignalItem *item2 = sitems.at(i);
			
			QGraphicsItem *gi1 = item1->labelItem();
			QGraphicsItem *gi2 = item2->labelItem();
			if (gi2 != NULL && gi1 != NULL) {
				if (gi1->collidesWithItem(gi2)) {
					item2->shiftLabel(gi1->boundingRect().width() + 5);
				}
			}
			i += 2;
		}

		newRect.setHeight(newRect.height() + extraSceneH);
		scene()->setSceneRect(newRect);
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
//	emit pageDurationChanged(m_pageDuration);
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
}
