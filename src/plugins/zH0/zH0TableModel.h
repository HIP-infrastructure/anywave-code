#pragma once

#include "zH0FilterSet.h"
#include <QAbstractTableModel>
#include <QItemDelegate>

constexpr int ZH0_COLUMNS = 4;
constexpr int ZH0_HP = 0;
constexpr int ZH0_LP = 1;
constexpr int ZH0_NOTCH = 2;
constexpr int ZH0_COLOR = 3;

class zH0TableModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	zH0TableModel(QObject *parent);
	~zH0TableModel();

	QVariant data(const QModelIndex& index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	Qt::ItemFlags flags(const QModelIndex &index = QModelIndex()) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

	void clear();
	void add(zH0FilterSet *set);
	zH0FilterSets& filterSets() { return m_filterSets; }
protected:
	zH0FilterSets m_filterSets;
};

class zH0ModelDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	zH0ModelDelegate(QObject *parent = 0) : QItemDelegate(parent) {}

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
private slots:
	void commitAndCloseComboBox();
};
