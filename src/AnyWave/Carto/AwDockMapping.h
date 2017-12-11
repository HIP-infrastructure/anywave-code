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
#ifndef AWDOCKMAPPING2_H
#define AWDOCKMAPPING2_H

#include <AwChannel.h>
#include <QDockWidget>
class AwMappingClient;
class AwMappingWidget;
class AwLayout;

class AwDockMapping : public QDockWidget
{
	Q_OBJECT

public:
	AwDockMapping(int type, const QString& title, 
		AwLayout *l2D, AwLayout *l3D, QWidget *parent = 0, Qt::WindowFlags flags = 0);

	void openConnection();
	void closeConnection();
	inline bool isClosed() { return m_closed; }

public slots:
	void newMappingAtPosition(float pos);
	void newMappingSelection(float pos, float durartion);
	void setBadLabels(QStringList& labels);
	void setSelectedChannels(AwChannelList& channels);
protected:
	void closeEvent(QCloseEvent *e);

	AwMappingClient *m_client;
	AwMappingWidget *m_widget;
	bool m_closed;
	QStringList m_badLabels;			// list of bad electrodes
	QStringList m_selectedLabels;		// list of selected electrodes
	AwChannelList m_selectedChannels;	// list of currently selected channels in views
signals:
	void mappingClosed();
	void selectedLabelsChanged(const QStringList& labels);
};

#endif