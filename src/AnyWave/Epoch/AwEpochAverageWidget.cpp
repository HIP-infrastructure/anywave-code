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
#include "AwEpochAverageWidget.h"


AwEpochAverageWidget::AwEpochAverageWidget(const QList<AwEpochTree *>& conditions, QWidget *parent)
	: QWidget(parent)
{
	m_ui.setupUi(this);
//	connect(m_ui.buttonEditStats, &QPushButton::clicked, this, &AwEpochAverageWidget::editStats);
	m_conditions = conditions;
	m_ui.progressBar->hide();
	m_ui.labelProgress->hide();
	m_worker = NULL;
	// Instantiate settings object to hold what channel to display and what statistics to show on it.
//	m_stats = new AwStatsSettings(conditions.first()->channels());
	createItems();
}

AwEpochAverageWidget::~AwEpochAverageWidget()
{
	while (!m_items.isEmpty()) 
		delete m_items.takeLast();
	if (m_worker)
		delete m_worker;
//	delete m_stats;
}

void AwEpochAverageWidget::computeAVG(AwEpochTree *cond)
{
	if (cond->isAvgAvailable()) {
		AwAverageItemWidget *item = new AwAverageItemWidget();
		AwEpochDataBuffer *buffer = cond->createAVGBuffer();
		item->setCondition(cond, buffer);
		m_items.append(item);
		m_ui.verticalLayout->addWidget(item);
		repaint();
	}
	else {
		m_ui.progressBar->show();
		m_ui.labelProgress->setText("Averaging...");
		m_ui.labelProgress->show();
		m_ui.progressBar->setValue(0);
		if (m_worker == NULL) {
			m_worker = new AwAvgWorker;
			connect(m_worker, &AwAvgWorker::done, this, &AwEpochAverageWidget::addItemWhenAvgDone);
			connect(m_worker, &AwAvgWorker::inProgress, this, &AwEpochAverageWidget::updateProgress);
		}
		m_worker->setCondition(cond);
		QMetaObject::invokeMethod(m_worker, "run", Qt::QueuedConnection);
	}
}

void AwEpochAverageWidget::createItems()
{
	for (auto c : m_conditions) 
		computeAVG(c);
	repaint();
}

void AwEpochAverageWidget::addItemWhenAvgDone(AwEpochTree *condition)
{
	computeAVG(condition);
	m_ui.progressBar->hide();
	m_ui.labelProgress->hide();
}

void AwEpochAverageWidget::updateProgress(int progress)
{
	m_ui.progressBar->setValue(progress);
}