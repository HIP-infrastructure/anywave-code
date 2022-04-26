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
#pragma once
#include <QWizardPage>
#include <filter/AwFilterSettings.h>
#include <QAbstractTableModel>
class QTableView;
class AwChannelsExportWizardPage;

using FilterItem = struct {
	int type;
	QVector<float> values;
};

class AwPageFilterModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	AwPageFilterModel(AwChannelsExportWizardPage *page, QObject* parent = nullptr);
	~AwPageFilterModel();

	AwFilterSettings settings();

	Qt::ItemFlags flags(const QModelIndex& index) const override;
	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
protected:
	AwFilterSettings m_settings;
	QList<FilterItem *> m_list;
	AwChannelList m_channels;
};


class AwFilterExportWizardPage : public QWizardPage
{
	Q_OBJECT
public:
	AwFilterExportWizardPage(AwChannelsExportWizardPage* page, QWidget* parent = nullptr);

	void initializePage() override;
	bool validatePage() override;
	AwFilterSettings filterSettings;
protected:
	AwChannelsExportWizardPage* m_channelsPage;
	QTableView* m_tableView;
	AwPageFilterModel* m_model;
};