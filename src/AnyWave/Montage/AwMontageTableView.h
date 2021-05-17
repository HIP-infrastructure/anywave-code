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
#ifndef AWMONTAGETABLEVIEW_H
#define AWMONTAGETABLEVIEW_H

#include <QTableView>
#include <QDropEvent>
#include "AwChannelListModel.h"

class AwMontageTableView : public QTableView
{
	Q_OBJECT

public:
	AwMontageTableView(QWidget *parent);
	~AwMontageTableView();

//	void setModel(AwChannelListModel *model);
//	inline void setChannelsHashTable(QHash<QString, AwChannel *> *table) { m_channelHashTable = table; }
signals:
	void clickedOnIndex(const QModelIndex& index);	// Sent everytime the left mouse button is release on a row.
protected:
	void dragEnterEvent(QDragEnterEvent *e);
	void dragMoveEvent(QDragMoveEvent *);
	void mouseReleaseEvent(QMouseEvent *event);
private:
//	QList<AwChannel *> m_channels;
//	QList<AwChannel *> m_draggedChannels;
//	AwChannelListModel *m_model;
//	QHash<QString, AwChannel*> *m_channelHashTable;
	
};

#endif // AWMONTAGETABLEVIEW_H
