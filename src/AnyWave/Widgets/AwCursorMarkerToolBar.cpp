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
#include "AwCursorMarkerToolBar.h"
#include "Marker/AwMarkerManager.h"

AwCursorMarkerToolBar::AwCursorMarkerToolBar(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);
	m_toolBar = new QToolBar(tr("Cursors"));
	m_toolBar->setObjectName("Cursors");
	connect(buttonInsertMarker, SIGNAL(toggled(bool)), this, SLOT(setMarkerModeOn(bool)));
	connect(buttonCursor, SIGNAL(toggled(bool)), this, SLOT(setCursorModeOn(bool)));
	connect(buttonShowMarkers, SIGNAL(clicked()), AwMarkerManager::instance(), SLOT(showDockUI()));
	connect(buttonQTS, SIGNAL(toggled(bool)), this, SLOT(setQTSModeOn(bool)));
	m_toolBar->addWidget(this);
}

AwCursorMarkerToolBar::~AwCursorMarkerToolBar()
{

}

void AwCursorMarkerToolBar::changeEvent(QEvent *e)
{
	if (e)
		if (e->type() == QEvent::LanguageChange)
			retranslateUi(this);
}


///////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS

void AwCursorMarkerToolBar::retranslate()
{
	retranslateUi(this);
}

void AwCursorMarkerToolBar::reset()
{
	setCursorModeOn(false);
	setMarkerModeOn(false);
	setQTSModeOn(false);
}

void AwCursorMarkerToolBar::setCursorModeOn(bool flag)
{
	if (flag) // Curson on => marker is off
		buttonInsertMarker->setChecked(false);
	buttonCursor->setChecked(flag);
	emit cursorModeChanged(flag);
}

void AwCursorMarkerToolBar::setMarkerModeOn(bool flag)
{
	if (flag) // Marker mode on => cursor is off
		buttonCursor->setChecked(false);
	buttonInsertMarker->setChecked(flag);
	emit markerModeChanged(flag);
}

void AwCursorMarkerToolBar::setQTSModeOn(bool flag)
{
	buttonQTS->setChecked(flag);
	emit QTSModeChanged(flag);
}


void AwCursorMarkerToolBar::stopQTSMode()
{
	buttonQTS->setChecked(false);
}