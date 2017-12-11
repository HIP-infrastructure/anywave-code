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
#include "AwDockMapping.h"
#include "AwMappingWidget.h"
#include "AwMappingClient.h"
//#include "Prefs/AwSettings.h"
#include "Montage/AwMontageManager.h"
#include <layout/AwLayout.h>

AwDockMapping::AwDockMapping(int type, const QString& title, AwLayout *l2D, AwLayout *l3D, QWidget *parent, Qt::WindowFlags flags) : QDockWidget(title, parent, flags)
{
//	AwSettings *aws = AwSettings::getInstance();
	if (type == AwChannel::MEG)
		setObjectName("Dock Mapping MEG");
	else
		setObjectName("Dock Mapping EEG");
	m_client = new AwMappingClient(type);

	m_widget = new AwMappingWidget(type, l2D, l3D);
//		connect(aws, SIGNAL(languageChanged()), m_widget, SLOT(retranslate()));
	connect(m_client, SIGNAL(dataReceived(float, const QVector<float>&, const QStringList&)), m_widget, 
		SLOT(updateMaps(float, const QVector<float>&, const QStringList&)));
	connect(m_client, SIGNAL(PCADataReceived(float, float, const QVector<float>&, const QStringList&)), m_widget, 
		SLOT(updateMapsPCA(float, float, const QVector<float>&, const QStringList&)));
	connect(AwMontageManager::instance(), SIGNAL(badChannelsSet(const QStringList&)), m_widget, SLOT(updateBadSensors(const QStringList&)));
	connect(m_widget, SIGNAL(selectedLabelsChanged(const QStringList&)), this, SIGNAL(selectedLabelsChanged(const QStringList&)));
	setWidget(m_widget);
	m_client->setParent(this);
	openConnection();
}

void AwDockMapping::closeEvent(QCloseEvent *e)
{
	closeConnection();
	emit mappingClosed();
}

void AwDockMapping::openConnection()
{
	m_client->openConnection();
	m_closed = false;  // mark the dockwidget as open
}

void AwDockMapping::closeConnection()
{
	m_client->closeConnection();
	m_closed = true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////:
// SLOTS

void AwDockMapping::newMappingAtPosition(float pos)
{
	// if the dock widget is closed (no mapping active), do not relay the mapping request to the data client
	if (m_closed)
		return;
	m_client->requestDataAtLatency(pos);
}

void AwDockMapping::newMappingSelection(float pos, float duration)
{
	// if the dock widget is closed (no mapping active), do not relay the mapping request to the data client
	if (m_closed)
		return;
	m_client->requestDataAtLatency(pos, duration);
}

// Connected to display and called each time the channel selection changed in the signal views
void AwDockMapping::setSelectedChannels(AwChannelList& channels)
{
	m_selectedChannels = channels;
	m_selectedLabels.clear();
	foreach (AwChannel *c, channels) {
		if (!m_selectedLabels.contains(c->name()))
			m_selectedLabels << c->name();
	}
	m_widget->selectLabels(m_selectedLabels);
}

void AwDockMapping::setBadLabels(QStringList& labels)
{
	m_badLabels = labels;
	m_widget->updateBadSensors(labels);
}