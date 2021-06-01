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
#include "AwMappingToolBar.h"


AwMappingToolBar::AwMappingToolBar(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);
	connect(buttonMapping, SIGNAL(clicked()), this, SIGNAL(mappingButtonClicked()));
	connect(buttonGARDEL, SIGNAL(clicked()), this, SIGNAL(GARDELClicked()));
	m_toolBar = new QToolBar(tr("3D Mapping"));
	m_toolBar->setObjectName("Mapping");
	m_toolBar->addWidget(this);
}

AwMappingToolBar::~AwMappingToolBar()
{
}


void AwMappingToolBar::changeEvent(QEvent *e)
{
	if (e)
		if (e->type() == QEvent::LanguageChange)
			retranslateUi(this);
}
