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
#include "AwMappingWidget.h"
#include <AwUtilities.h>
#include "Data/AwDataServer.h"
#include <vtkTextProperty.h>
#include <vtkRenderWindow.h>
#include "Widgets/AwPleaseWaitWidget.h"
#include "Filter/AwFilteringManager.h"
#include <graphics/AwColorMap.h>
#include "Montage/AwMontageManager.h"
#include <widget/AwTopoWidget.h>
#include <layout/AwLayout.h> 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// AwMappingWidget


AwMappingWidget::AwMappingWidget(int type, AwLayout *l2D, AwLayout *l3D,  QWidget *parent)
	: QWidget(parent)
{
	m_type = type;
	ui.setupUi(this);
	ui.comboCMap->initToColorMap(AwColorMap::Parula);

	// set black background color for 2D topo widget
	ui.topoWidget2D->setBackgroundColor(QString("black"));

	ui.button3D->hide();
	ui.button2D->hide();
	ui.buttonBoth->hide();
	ui.topoWidget2D->hide();
	ui.qvtkWidget3D->hide();

	if (l3D && l2D == NULL)	{
		ui.button3D->show();
		ui.qvtkWidget3D->show();
		ui.button3D->setChecked(true);
	}
	if (l2D && l3D == NULL)	{
		ui.button2D->show();
		ui.topoWidget2D->show();
		ui.button2D->setChecked(true);
	}
	if (l2D && l3D)	{
		ui.buttonBoth->show();
		ui.buttonBoth->setChecked(true);
	}

	if (l2D) {
		ui.button2D->show();
		ui.topoWidget2D->setLayout(l2D);
		ui.topoWidget2D->show();
		ui.topoWidget2D->enableContours(false);
		ui.topoWidget2D->enableSensors(false);
		if (type == AwChannel::EEG) 
			ui.topoWidget2D->setUnit(QString::fromLatin1("µV"));
		else
			ui.topoWidget2D->setUnit(QString::fromLatin1("pT"));
		ui.topoWidget2D->setFlags(AwTopoWidget::ShowLatency|AwTopoWidget::ShowValue);
		connect(ui.checkBoxSensors, SIGNAL(clicked(bool)), ui.topoWidget2D, SLOT(enableSensors(bool)));
		connect(ui.checkBoxContours, SIGNAL(clicked(bool)), ui.topoWidget2D, SLOT(enableContours(bool)));
		connect(ui.checkBoxLabels, SIGNAL(clicked(bool)), ui.topoWidget2D, SLOT(enableLabels(bool)));
		connect(ui.topoWidget2D, SIGNAL(selectedLabelsChanged(const QStringList& )), this, SLOT(updateSelection(const QStringList&)));
	}
	if (l3D)	{
		ui.qvtkWidget3D->initWithLayout(l3D);
		ui.button3D->show();
		ui.qvtkWidget3D->show();
		connect(ui.checkBoxSensors, SIGNAL(clicked(bool)), ui.qvtkWidget3D, SLOT(showSensors(bool)));
		connect(ui.checkBoxContours, SIGNAL(clicked(bool)), ui.qvtkWidget3D, SLOT(showContours(bool)));
		connect(ui.qvtkWidget3D, SIGNAL(selectedLabelsChanged(const QStringList& )), this, SLOT(updateSelection(const QStringList&)));
	}
	m_l2D = l2D;
	m_l3D = l3D;

	connect(ui.comboCMap, SIGNAL(currentIndexChanged(int)), this, SLOT(changeColorMap(int)));
	// get current bad sensors
	updateBadSensors(AwMontageManager::instance()->badLabels());
}

AwMappingWidget::~AwMappingWidget()
{
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// EVENTS

void AwMappingWidget::changeEvent(QEvent *e)
{
	if (e) {
		if (e->type() == QEvent::LanguageChange) 
			ui.retranslateUi(this);
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS

void AwMappingWidget::changeColorMap(int index)
{
	if (m_l2D)
		ui.topoWidget2D->setColorMap(ui.comboCMap->selectedMap());
	if (m_l3D)
		ui.qvtkWidget3D->changeColorMap(ui.comboCMap->selectedMap());
}


void AwMappingWidget::updateMaps(float latency, const QVector<float>& data, const QStringList& labels)
{
	if (m_l2D)	{
		ui.topoWidget2D->updateMap(latency, data, labels);
		ui.topoWidget2D->resetComputationFlags();
	}
	if (m_l3D) {
		ui.qvtkWidget3D->setLatency(latency, AwUtilities::isTimeHMS());
		ui.qvtkWidget3D->updateMap(latency, data, labels);
	}
}

void AwMappingWidget::updateMapsPCA(float start, float end, const QVector<float>& data, const QStringList& labels) 
{
	if (m_l2D)	{
		ui.topoWidget2D->updateMap(start, end, data, labels);
		ui.topoWidget2D->setComputationFlags("PCA");
	}
	if (m_l3D) {
		ui.qvtkWidget3D->setLatency(start, end, AwUtilities::isTimeHMS());
		ui.qvtkWidget3D->updateMap(start, data, labels);
	}
}


void AwMappingWidget::to2D(bool f)
{
	if (!f)
		return;
	ui.qvtkWidget3D->hide();
	ui.topoWidget2D->show();
}

void AwMappingWidget::to3D(bool f)
{
	if (!f)
		return;
	ui.topoWidget2D->hide();
	ui.qvtkWidget3D->show();
}

void AwMappingWidget::showBoth(bool f)
{
	if (!f)
		return;
	ui.qvtkWidget3D->show();
	ui.topoWidget2D->show();
}

void AwMappingWidget::selectLabels(const QStringList& labels)
{
	m_selectedLabels = labels;
	if (m_l3D)
		ui.qvtkWidget3D->selectSensors(labels);
	if (m_l2D)
		ui.topoWidget2D->selectSensors(labels);
}

void AwMappingWidget::updateSelection(const QStringList& labels)
{
	selectLabels(labels);
	emit selectedLabelsChanged(m_selectedLabels);
}

void AwMappingWidget::updateBadSensors(const QStringList &labels)
{
	if (m_l2D)
		ui.topoWidget2D->updateBadSensors(labels);
	if (m_l3D)
		ui.qvtkWidget3D->updateBadSensors(labels);
}
