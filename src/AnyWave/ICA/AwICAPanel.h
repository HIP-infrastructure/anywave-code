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
#ifndef AWICAPANEL_H
#define AWICAPANEL_H

#include <QWidget>
#include <AwChannel.h>
namespace Ui {class AwICAPanelUI;};
class AwPanelItem;
class AwICAPanel : public QWidget
{
	Q_OBJECT

public:
	AwICAPanel(int type, QWidget *parent = 0);
	~AwICAPanel();
public slots:
	void updateRejectedComponents(const QList<int>& indexes);
	void updateItemState(int index, bool rejected);
	void showAllSelected(bool flag);
	void updateSelected(AwChannelList& channels);
	void grabMappings(const QString& filePath);
signals:
	void componentModified(int index, bool rejected);
private:
	Ui::AwICAPanelUI *ui;
	int m_type;	    // type of IC components (EEG or MEG)
	int m_columns;	// number of widgets by row
	int m_rows;		// number of rows after inserting all the items.
	QList<AwPanelItem *> m_items;
	QList<int> m_rejectedIndexes;
};

#endif // AWICAPANEL_H
