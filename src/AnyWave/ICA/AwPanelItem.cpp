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
#include "AwPanelItem.h"
#include "ui_AwPanelItem.h"


AwPanelItem::AwPanelItem(AwICAChannel *channel, QWidget *parent)
	: QWidget(parent)
{
	m_channel = channel;
	ui = new Ui::AwPanelItem();
	ui->setupUi(this);
	ui->widget->setLayout(channel->layout2D());
	ui->widget->updateMap(0, channel->topoValues(), channel->labels());
	ui->widget->setFlags(AwTopoWidget::ShowOptions);
	ui->widget->setLabel(channel->name());
	ui->label->setText(channel->name());
	ui->labelRejected->setStyleSheet("color:red; font:bold;");
	m_isRejected = channel->isRejected();
	updateRejected();

	// starts in 2D Mode
	ui->widget3D->hide();
	ui->widget3D->setFlags(AwMapWidget::NoColorMap|AwMapWidget::NoLatencyDisplay);
	ui->widget3D->setBackgroundColor(palette().background().color());
	m_is3DMapComputed = false;
	// is there a 3D layout?
	if (channel->layout3D() == NULL) {  // No
		ui->radioButton2D->setEnabled(false);
		ui->radioButton3D->setEnabled(false);
	}

	connect(ui->widget, SIGNAL(selectedLabelsChanged(const QStringList&)), this, SIGNAL(selectedLabelsChanged(const QStringList&)));
	connect(ui->buttonRejectAdd, SIGNAL(clicked()), this, SLOT(udpateRejectedState()));
	connect(ui->buttonShowSignal, SIGNAL(clicked()), this, SLOT(showSignal()));
	connect(ui->checkBoxSelected, SIGNAL(toggled(bool)), this, SLOT(select(bool)));
	connect(ui->radioButton2D, SIGNAL(toggled(bool)), this, SLOT(switch2D3D(bool)));
}

AwPanelItem::~AwPanelItem()
{
	delete ui;
}

QWidget *AwPanelItem::currentMappingWidget()
{
	if (ui->widget3D->isVisible())
		return ui->widget3D;
	return ui->widget;
}

QPixmap AwPanelItem::capture()
{
	if (ui->widget3D->isVisible()) 
		return ui->widget3D->pixmap();
	return ui->widget->pixmap();
}

void AwPanelItem::switch2D3D(bool flag)
{
	if (flag) { // 2D
		ui->widget3D->hide();
		ui->widget->show();
	}
	else { // 3D
		if (!m_is3DMapComputed) {
			ui->widget3D->initWithLayout(m_channel->layout3D());
			ui->widget3D->updateMap(0, m_channel->topoValues(), m_channel->labels());
			m_is3DMapComputed = true;
		}
		ui->widget3D->show();
		ui->widget->hide();
	}
}


void AwPanelItem::setRejected(bool flag)
{
	m_channel->setRejected(flag);
	m_isRejected = flag;
	updateRejected();
}


void AwPanelItem::updateRejected()
{
	if (m_isRejected) {
		ui->buttonRejectAdd->setText(tr("Add Component"));
		ui->labelRejected->show();
	}
	else {
		ui->buttonRejectAdd->setText(tr("Reject Component"));
		ui->labelRejected->hide();
	}
}

void AwPanelItem::udpateRejectedState()
{
	m_isRejected = !m_isRejected;
	updateRejected();
	emit itemStateChanged(m_channel->index(), m_isRejected);
	emit itemStateChanged(m_channel->name(), m_isRejected);
}

void AwPanelItem::showSignal()
{
	emit showSignalClicked(m_channel->name());
}

void AwPanelItem::select(bool flag)
{
	emit selected(m_channel->name(), flag);
}

void AwPanelItem::updateSelected(bool f)
{
	disconnect(ui->checkBoxSelected, SIGNAL(toggled(bool)), this, SLOT(select(bool)));
	ui->checkBoxSelected->setChecked(f);
	connect(ui->checkBoxSelected, SIGNAL(toggled(bool)), this, SLOT(select(bool)));
}