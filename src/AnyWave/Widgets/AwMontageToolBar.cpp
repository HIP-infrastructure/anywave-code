/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
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

	foreach (QString s, montages)
	{
		QAction *act = m_menu.addAction(s);
		connect(act, SIGNAL(triggered()), this, SLOT(loadQuickMontage()));
	}

	// add local montages
	QStringList localMontages = m_mm->localQuickMontages();
	foreach (QString s, localMontages)
	{
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