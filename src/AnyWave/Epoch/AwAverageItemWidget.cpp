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
#include "AwAverageItemWidget.h"
#include "AwEditStatsDialog.h"

AwAverageItemWidget::AwAverageItemWidget(QWidget *parent)
	: QWidget(parent)
{
	m_ui.setupUi(this);
	m_buffer = NULL;
	m_signalViewAvg = new AwBaseSignalView(AwBaseSignalView::NoNavButtons | AwBaseSignalView::NoInfoLabels | AwBaseSignalView::ViewAllChannels
		| AwBaseSignalView::NoHScrollBar | AwBaseSignalView::NoMarkerBar);
	m_signalViewAvg->setSecPerCm(0.1);
	m_signalViewAvg->showElectrodesNames(false);
	m_signalViewAvg->showZeroLine(false);
	m_signalViewAvg->showMarkersLabels(false);
	m_signalViewAvg->showMarkersValues(false);
	m_signalViewAvg->showMarkers(true);
	m_signalViewAvg->stackChannels(true);
	m_ui.horizontalLayout->replaceWidget(m_ui.signalViewAvg, m_signalViewAvg->widget());
	delete m_ui.signalViewAvg;
	m_signalViewStats = new AwBaseSignalView(AwBaseSignalView::NoNavButtons | AwBaseSignalView::NoInfoLabels | AwBaseSignalView::ViewAllChannels
		| AwBaseSignalView::NoHScrollBar | AwBaseSignalView::NoMarkerBar);
	m_signalViewStats->setSecPerCm(0.1);
	m_signalViewStats->showElectrodesNames(false);
	m_signalViewStats->showZeroLine(false);
	m_signalViewStats->showMarkersLabels(false);
	m_signalViewStats->showMarkersValues(false);
	m_signalViewStats->showMarkers(true);
	m_signalViewStats->stackChannels(true);
	m_signalViewStats->addNewDisplayPlugin(&m_plugin);
	m_ui.horizontalLayout->replaceWidget(m_ui.signalStats, m_signalViewStats->widget());
	delete m_ui.signalStats;
	m_statWidget = new AwStatChartWidget;
	m_ui.horizontalLayout->replaceWidget(m_ui.chartWidget, m_statWidget);
	m_statWidget->resize(QSize(300, 200));
	delete m_ui.chartWidget;
	m_zeroMarker = NULL;
	
	connect(m_ui.buttonEditStats, &QPushButton::clicked, this, &AwAverageItemWidget::editStats);
}

AwAverageItemWidget::~AwAverageItemWidget()
{
	if (m_zeroMarker)
		delete m_zeroMarker;
	delete m_signalViewAvg;
	delete m_signalViewStats;
	delete m_statWidget;
}


void AwAverageItemWidget::setCondition(AwEpochTree *condition, AwEpochDataBuffer *buffer)
{
	m_ui.labelCondition->setText(QString("%1 - %2 epochs").arg(condition->name()).arg(condition->numberOfGoodEpochs()));

	m_buffer = buffer;
	m_buffer->openConnection(m_signalViewAvg);
	m_signalViewAvg->setTotalDuration(m_buffer->duration());
	m_signalViewAvg->setTimeShift(-condition->zeroPos());
	int type = condition->channels().first()->type();
	AwChannelList channels = AwChannel::duplicateChannels(condition->channels());
	m_signalViewAvg->setChannels(channels);
	float meanRange = AwChannel::meanRangeValue(channels);
//	if (condition->modality() == AwChannel::MEG || condition->modality() == AwChannel::GRAD)
//		meanRange *= 1E12;
	m_signalViewAvg->navigationBar()->amplitudeWidget()->changeCurrentChannelTypeAndValue(type, meanRange);

	AwMarkerList tmp;
	m_zeroMarker = new AwMarker("0", condition->zeroPos());
	tmp.append(m_zeroMarker);
	m_signalViewAvg->setMarkers(tmp);

	//m_buffer->openConnection(m_signalViewStats);
	//m_signalViewStats->setTotalDuration(m_buffer->duration());
	//m_signalViewStats->setTimeShift(-condition->zeroPos());
	//m_signalViewStats->navigationBar()->amplitudeWidget()->changeCurrentChannelTypeAndValue(type, meanRange);
	m_condition = condition;

	m_statWidget->setCondition(condition);

	AwEpochAverageChannel *channel = condition->createAVGChannel(condition->channels().at(0)->name());
	if (channel) {
		channel->setColor(QColor(Qt::blue).name());
		channel->setDisplayPluginName("Epoch AVG SignalItem");
		//m_ui.chartWidget->addChannel(channel);
		m_statChannels.append(channel);
	}
	channel = condition->createAVGChannel(condition->channels().last()->name());
	if (channel) {
		channel->setColor(QColor(Qt::green).name());
		channel->setDisplayPluginName("Epoch AVG SignalItem");
		//m_ui.chartWidget->addChannel(channel);
		m_statChannels.append(channel);
	}
	m_statWidget->setChannels(m_statChannels);

	//AwChannelList list;
	//list.append(channel);
	//channel = condition->createAVGChannel(condition->channels().at(1)->name());
	//channel->setDisplayPluginName("Epoch AVG SignalItem");
	//channel->setColor(QColor(Qt::green).name());
	//list.append(channel);
	//m_signalViewStats->setChannels(list);

	m_signalViewStats->widget()->hide();

	repaint();
}

void AwAverageItemWidget::editStats()
{
	AwEditStatsDialog dlg(m_statChannels, m_condition);
	if (dlg.exec() == QDialog::Accepted) {
		// delete previous stats channels
		while (!m_statChannels.isEmpty())
			delete m_statChannels.takeFirst();
		m_statChannels = dlg.selectedChannels;
		m_statWidget->setChannels(m_statChannels);
	}
	repaint();
}