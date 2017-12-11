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
#ifndef AWPANELITEM_H
#define AWPANELITEM_H

#include <QWidget>
namespace Ui {class AwPanelItem;};

#include "AwICAChannel.h"
class AwPanelItem : public QWidget
{
	Q_OBJECT

public:
	AwPanelItem(AwICAChannel *channel, QWidget *parent = 0);
	~AwPanelItem();

	inline int index() { return m_channel->index(); }
	void setRejected(bool flag);
	inline bool isRejected()  { return m_isRejected; }
	inline AwICAChannel *channel() { return m_channel; }
	void updateSelected(bool f);
	/** Get the current widget used to display the mapping. **/
	QWidget *currentMappingWidget();
	QPixmap capture();
signals:
	void selectedLabelsChanged(const QStringList& labels);
	void itemStateChanged(int index, bool rejected);
	void itemStateChanged(const QString& label, bool rejected);
	void showSignalClicked(const QString& label);
	void selected(const QString& label, bool selected);
private slots:
	void udpateRejectedState();
	void showSignal();
	void select(bool flag);
	void switch2D3D(bool flag);
private:
	void updateRejected();

	Ui::AwPanelItem *ui;
	AwICAChannel *m_channel;
	bool m_isRejected, m_is3DMapComputed;
};

#endif // AWPANELITEM_H
