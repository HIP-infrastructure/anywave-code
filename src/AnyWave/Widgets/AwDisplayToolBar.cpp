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
#include "AwDisplayToolBar.h"
#include "Prefs/AwSettings.h"
#include "Display/AwDisplaySetupManager.h"

AwDisplayToolBar::AwDisplayToolBar(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);
	AwDisplaySetupManager *dsManager = AwDisplaySetupManager::instance();
	m_setup = NULL;
	connect(checkSynchro, SIGNAL(toggled(bool)), this, SLOT(setSynchronized(bool)));
	connect(comboAlignment, SIGNAL(currentIndexChanged(int)), this, SLOT(changeAlignment(int)));
	connect(buttonAddView, SIGNAL(clicked()), this, SLOT(addView()));
	connect(dsManager, SIGNAL(newSetupSelected(AwDisplaySetup *)), this, SLOT(setNewSetup(AwDisplaySetup *)));
	connect(this, SIGNAL(setupChanged(AwDisplaySetup *, int)), dsManager, SLOT(updateSetup(AwDisplaySetup *, int)));
	connect(buttonCapture, SIGNAL(clicked()), this, SIGNAL(captureClicked())); 
	// toolbar init
	m_toolBar = new QToolBar(tr("Display"));
	m_toolBar->setObjectName("Display");
	m_toolBar->addWidget(this);
}

AwDisplayToolBar::~AwDisplayToolBar()
{
}

void AwDisplayToolBar::changeEvent(QEvent *e)
{
	if (e)
		if (e->type() == QEvent::LanguageChange)
			retranslateUi(this);
}



///////////////////////////////////////////////////////////////////////////////////////////////
// SLOTS
void AwDisplayToolBar::addView()
{
	int nViews = m_setup->viewSetups().size();
	emit addViewClicked();
	// check if a new view has been added
	if (m_setup->viewSetups().size() > nViews)
		emit setupChanged(m_setup, AwDisplaySetup::ViewNumber);
}

void AwDisplayToolBar::changeAlignment(int index)
{
	switch (index) 	{
	case 0: 
		m_setup->setOrientation(AwDisplaySetup::Horizontal);
		emit setupChanged(m_setup, AwDisplaySetup::ViewOrientation);
		emit alignHClicked();
		break;
	case 1:
		m_setup->setOrientation(AwDisplaySetup::Vertical);
		emit setupChanged(m_setup, AwDisplaySetup::ViewOrientation);
		emit alignVClicked();
		break;
	}
}

void AwDisplayToolBar::updateSetup(AwDisplaySetup *setup, int flags)
{
	if (flags & AwDisplaySetup::SynchViews)
		checkSynchro->setChecked(setup->synchronizeViews());

	if (flags & AwDisplaySetup::ViewOrientation) {
		if (setup->orientation() == AwDisplaySetup::Vertical)
			comboAlignment->setCurrentIndex(1);
		else
			comboAlignment->setCurrentIndex(0);

	}
}

void AwDisplayToolBar::setNewSetup(AwDisplaySetup *setup)
{
	m_setup = setup;
	updateSetup(m_setup, AwDisplaySetup::AllFlags);
}

void AwDisplayToolBar::setSynchronized(bool flag)
{
	m_setup->setSynchronized(flag);
	emit setupChanged(m_setup, AwDisplaySetup::SynchViews);
}
