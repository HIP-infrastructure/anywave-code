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
#include "AwFilterExportWizardPage.h"
#include "Data/AwDataManager.h"
#include <QTableView>
#include <QHeaderView>
#include "AwChannelsExportWizardPage.h"
#include <QVBoxLayout>
#include <QMessageBox>
/* **************************************************************************************************
*                                                      Model
*****************************************************************************************************/
AwPageFilterModel::AwPageFilterModel(const AwChannelList& channels, const AwFilterSettings& filterSettings, QObject* parent) : QAbstractTableModel(parent)
{
	m_settings = AwFilterSettings(true); // no init => no channel type set
	m_channels = channels;
	auto types = AwChannel::getTypes(m_channels);
	for (auto t : types) {
		FilterItem *fi = new FilterItem;
		fi->type = t;
		fi->values = filterSettings.filters(t);
		m_list.append(fi);
	}
}

AwPageFilterModel::~AwPageFilterModel()
{
	qDeleteAll(m_list);
}

AwFilterSettings AwPageFilterModel::settings()
{
	for (auto fi : m_list)
		m_settings.set(fi->type, fi->values);
	return m_settings;
}

Qt::ItemFlags AwPageFilterModel::flags(const QModelIndex& index) const
{
	if (index.isValid()) {
		if (index.column() == 0)
			return Qt::ItemIsEnabled;
		else
			return Qt::ItemIsEnabled | Qt::ItemIsEditable;
	}
	return Qt::ItemIsEnabled;
}

int AwPageFilterModel::rowCount(const QModelIndex& parent) const
{
	return m_list.size();
}

int AwPageFilterModel::columnCount(const QModelIndex& parent) const
{
	return 4;
}

QVariant AwPageFilterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	// Ce modele est prévu pour une table view, donc pas de headers verticaux
	if (orientation == Qt::Vertical)
		return QVariant();
	if (section == 0 && role == Qt::DisplayRole)
		return QString("Type");
	else if (section == 1 && role == Qt::DisplayRole)
		return QString("High Pass (Hz)");
	else if (section == 2 && role == Qt::DisplayRole)
		return QString("Low Pass (Hz)");
	else if (section == 3 && role == Qt::DisplayRole)
		return QString("Notch (Hz)");
	return QVariant();
}

bool AwPageFilterModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (!index.isValid())
		return false;
	int col = index.column();
	int row = index.row();
	FilterItem *fi = m_list.at(row);
	if (role == Qt::EditRole)	
		fi->values[col - 1] = value.toFloat();

	emit dataChanged(index, index);
	return true;
}

QVariant AwPageFilterModel::data(const QModelIndex& index, int role) const
{
	int row = index.row();
	int col = index.column();
	FilterItem *fi = m_list.at(row);
	auto values = fi->values;

	if (role == Qt::DisplayRole) {
		if (col == 0)
			return AwChannel::typeToString(fi->type);
		else {
			if (values[col - 1] <= 0)
				return QString("None");
			else
				return QString("%1Hz").arg(values[col - 1]);
		}
	}
	return QVariant();
}


AwFilterExportWizardPage::AwFilterExportWizardPage(const AwChannelList& channels, const QVariantMap& settings, QWidget* parent) : QWizardPage(parent)
{
	m_settings = settings;
	m_isBids = m_settings.contains(keys::bids_file_path);
	m_channels = channels;
	m_model = nullptr;
	m_tableView = nullptr;
	setTitle("Select how to filter data while exporting");
}


void AwFilterExportWizardPage::initializePage()
{

	m_tableView = new QTableView();
	auto filterSettings = AwDataManager::instance()->filterSettings();
	if (m_isBids) {
		filterSettings.zero();
		m_bidsFilterSettings = filterSettings;
	}
	m_model = new AwPageFilterModel(m_channels, filterSettings, this);
	m_tableView->setModel(m_model);	
	m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(m_tableView);
	setLayout(layout);
}

bool AwFilterExportWizardPage::validatePage()
{
	filterSettings = m_model->settings();
	if (m_isBids) {
		// check if filters values have been set
		auto modalities = filterSettings.modalities();
		bool ok = true;
		for (const auto& m : modalities) {
			const auto& values = filterSettings.values(m);
			for (auto const& v : values)
				if (v) {
					ok = false;
					break;
				}
			if (!ok)
				break;
		}
		if (!ok) {
			auto res = QMessageBox::question(this, "WARNING about filtering in BIDS", "Are you sure to apply filters while saving?", QMessageBox::Yes | QMessageBox::No);
			if (res == QMessageBox::No)
				return false;
		}
	}
	return true;
}