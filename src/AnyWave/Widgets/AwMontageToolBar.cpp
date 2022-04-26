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
#include "AwMontageToolBar.h"
#include <QFileDialog>
#include <QMessageBox>

AwMontageToolBar::AwMontageToolBar(QWidget *parent, bool withToolBar)
	: QWidget(parent)
{
	ui.setupUi(this);
	m_mm = AwMontageManager::instance();

	m_mm->buildQuickMontagesList();
	updateQuickMontages();
	connect(ui.buttonEdit, SIGNAL(clicked()), m_mm, SLOT(showInterface()));

	ui.buttonLoad->setMenu(&m_menu);

	if (withToolBar) {
		// toolbar init
		m_toolBar = new QToolBar(tr("Montage"));
		m_toolBar->setObjectName("Montage");
		m_toolBar->addWidget(this);
	}
}

AwMontageToolBar::~AwMontageToolBar()
{

}

void AwMontageToolBar::changeEvent(QEvent *e)
{
	if (e)
		if (e->type() == QEvent::LanguageChange)
			ui.retranslateUi(this);
}


void AwMontageToolBar::setLabelInfo(const QString &label)
{
	ui.label->setText(label);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS

void AwMontageToolBar::updateQuickMontages()
{
	m_menu.clear();
	QStringList montages = m_mm->quickMontages();

	for (auto const& s : montages)	{
		QAction *act = m_menu.addAction(s);
		connect(act, SIGNAL(triggered()), this, SLOT(loadQuickMontage()));
	}

	// add local montages
	QStringList localMontages = m_mm->localQuickMontages();
	for (auto const& s : localMontages)	{
		QAction *act = m_menu.addAction(s);
		connect(act, SIGNAL(triggered()), this, SLOT(loadQuickMontage()));
	}
}


void AwMontageToolBar::loadQuickMontage()
{
	QAction *act = (QAction *)sender();
	if (act)
		m_mm->loadQuickMontage(act->text());
}