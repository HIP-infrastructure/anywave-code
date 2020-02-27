#pragma once

#include "MFVFilterSet.h"
#include <QAbstractTableModel>
#include <QItemDelegate>

constexpr int MFV_COLUMNS = 4;
constexpr int MFV_HP = 0;
constexpr int MFV_LP = 1;
constexpr int MFV_NOTCH = 2;
constexpr int MFV_COLOR = 3;

class MFVTableModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	MFVTableModel(QObject *parent);
	~MFVTableModel();

	QVariant data(const QModelIndex& index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	Qt::ItemFlags flags(const QModelIndex &index = QModelIndex()) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

	void clear();
	void add(MFVFilterSet *set);
	MFVFilterSets& filterSets() { return m_filterSets; }
protected:
	MFVFilterSets m_filterSets;
};

class MFVModelDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	MFVModelDelegate(QObject *parent = 0) : QItemDelegate(parent) {}

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
private slots:
	void commitAndCloseComboBox();
};
