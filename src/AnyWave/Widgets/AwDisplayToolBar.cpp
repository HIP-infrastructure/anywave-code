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


AwDisplayToolBar::AwDisplayToolBar(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);
	connect(checkSynchro, SIGNAL(toggled(bool)), this, SLOT(setSynchronized(bool)));
	connect(comboAlignment, SIGNAL(currentIndexChanged(int)), this, SLOT(changeAlignment(int)));
	connect(buttonAddView, SIGNAL(clicked()), this, SLOT(addView()));
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



void AwDisplayToolBar::updateGUI(AwDisplaySetup* setup)
{
	disconnect(checkSynchro, SIGNAL(toggled(bool)), this, SLOT(setSynchronized(bool)));
	disconnect(comboAlignment, SIGNAL(currentIndexChanged(int)), this, SLOT(changeAlignment(int)));
	disconnect(buttonAddView, SIGNAL(clicked()), this, SLOT(addView()));

	checkSynchro->setChecked(setup->synchronizeViews());
	if (setup->orientation() == AwDisplaySetup::Vertical)
		comboAlignment->setCurrentIndex(1);
	else
		comboAlignment->setCurrentIndex(0);

	connect(checkSynchro, SIGNAL(toggled(bool)), this, SLOT(setSynchronized(bool)));
	connect(comboAlignment, SIGNAL(currentIndexChanged(int)), this, SLOT(changeAlignment(int)));
	connect(buttonAddView, SIGNAL(clicked()), this, SLOT(addView()));

}

///////////////////////////////////////////////////////////////////////////////////////////////
// SLOTS
void AwDisplayToolBar::addView()
{
	emit addViewClicked();
}

void AwDisplayToolBar::changeAlignment(int index)
{
	switch (index) 	{
	case 0: 
		emit alignHClicked();
		break;
	case 1:
		emit alignVClicked();
		break;
	}
}

void AwDisplayToolBar::setSynchronized(bool flag)
{
	emit synchronizedClicked(flag);
}
