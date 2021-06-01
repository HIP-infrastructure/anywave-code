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
