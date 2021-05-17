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
#ifndef AW_CHANNEL_LIST_MODEL_H
#define AW_CHANNEL_LIST_MODEL_H
#include <QSortFilterProxyModel>
#include <QAbstractTableModel>
#include <QModelIndex>
#include <QItemDelegate>
#include <QComboBox>
#include <AwChannel.h>

// Defines pour la mise en place des colonnes de données dans le display setup
#define AW_MONTAGE_NB_COLUMN	7    
#define AW_MONTAGE_COLUMN_NAME	0
#define AW_MONTAGE_COLUMN_REF	1
#define AW_MONTAGE_COLUMN_TYPE	2	
#define AW_MONTAGE_COLUMN_COLOR	3
#define AW_MONTAGE_COLUMN_LPF	4
#define AW_MONTAGE_COLUMN_HPF	5
#define AW_MONTAGE_COLUMN_NOTCH	6

// Defines pour la mise en place des colonnes de données dans le as recorded tableview
#define AW_ASRECORDED_NB_COLUMN		3   
#define AW_ASRECORDED_COLUMN_NAME   0
#define AW_ASRECORDED_COLUMN_TYPE   1
#define AW_ASRECORDED_COLUMN_BAD    2


/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Classes dérivées gérant le system model/view de QT adapté aux objets AwChannels
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// AwChannelListModel défini le modele pour la tableView Montage

class AwChannelListModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	AwChannelListModel(const AwChannelList& channels, QObject *parent = 0);

	// set a new montage
	void setMontage(const AwChannelList& channels);
	// update current montage by changing the list reference.
	void updateMontage(const AwChannelList& channels);
	// Add channels at the end of the current montage.
	void addChannels(const AwChannelList& channels);
	// Get a reference to the current montage
	AwChannelList& currentMontage() { return m_channels; }
	// Remove a channel from the current montage (destroy the object)
	void removeChannel(AwChannel *chan);
	// Remove several channels at once
	void removeChannels(const AwChannelList& channels);
	// Clear the montage, destroy all objects
	void clearMontage();

	// Methodes à implémenter pour custom model
	QVariant data(const QModelIndex& index, int role) const;
	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	bool removeRows(int position, int rows, const QModelIndex& index = QModelIndex());
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	Qt::ItemFlags flags(const QModelIndex &index = QModelIndex()) const;
	bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int colum, const QModelIndex &parent);
	Qt::DropActions supportedDropActions() const { return Qt::CopyAction | Qt::MoveAction; }
	QStringList mimeTypes() const;

	// Methodes propres au modele
	void beginUpdateChannels();
	void endUpdateChannels();

	void sortByName();
	void sortByNameAndType();
signals:
	/** Sent when the user drags and drops as recorded channels into the current montage **/
	void channelsDropped(const QStringList& labels, int beginRow);
private:
	AwChannelList m_channels;
};

// AwChannelListModelAsRecorded  définie le model utilisée par la tableview asrecorded

class AwChannelListModelAsRecorded : public QAbstractTableModel
{
	Q_OBJECT

public:
	AwChannelListModelAsRecorded(const AwChannelList& channels, QObject *parent = 0);

	// Methodes à implémenter pour custom model
	QVariant data(const QModelIndex& index, int role) const;
	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	Qt::ItemFlags flags(const QModelIndex &index = QModelIndex()) const;
	QStringList mimeTypes() const;
	QMimeData *mimeData(const QModelIndexList &indexes) const;

	AwChannelList& channels() { return m_channels; }
signals:
	void badChannelSet(const QString& label);
	void badChannelUnset(const QString& label);
private:
	AwChannelList m_channels;
};


class AsRecordedProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT
public:
	AsRecordedProxyModel(QObject *parent = 0);
	bool lessThan(const QModelIndex& left, const QModelIndex& right) const;
};

class MontageProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT
public:
	MontageProxyModel(QObject *parent = 0);
	bool lessThan(const QModelIndex& left, const QModelIndex& right) const;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Delegates
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// AwChannelListDelegate est le délégué de AwChannelListModel

class AwChannelListDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	AwChannelListDelegate(const QHash<int, QStringList>& labelsByType, QObject *parent = 0);

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:
	QHash<int, QStringList> m_labels;
private slots:
	void commitAndCloseComboBox();

};

// AwChannelListDelegateAsRecorded est le délégué de AwChannelListModelAsRecorded

class AwChannelListDelegateAsRecorded : public QItemDelegate
{
	Q_OBJECT

public:
	AwChannelListDelegateAsRecorded(QObject *parent = 0);

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
private slots:
	void commitAndCloseComboBox();
};


#endif
