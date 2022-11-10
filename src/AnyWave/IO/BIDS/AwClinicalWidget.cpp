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

#include "AwClinicalWidget.h"
#include "AwBIDSManager.h"
#include <QVBoxLayout>
#include <QTextStream>
#include <QMessageBox>

AwClinicalWidget::AwClinicalWidget(QWidget* parent) : QWidget(parent)
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	QVBoxLayout* layout = new QVBoxLayout;
	m_tableView = new QTableView;
	m_tableView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	layout->addWidget(m_tableView);
	m_model = new QStandardItemModel(this);
	m_tableView->setModel(m_model);
	m_tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
	m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	setLayout(layout);
	setMinimumSize(QSize(100, 100));
	setWindowFlags(windowFlags() | Qt::Window);
	setWindowTitle("Clinical Informations");
	setWindowIcon(QIcon(":/images/AnyWave_icon.png"));
}

void AwClinicalWidget::update()
{
	auto bm = AwBIDSManager::instance();
	// get participants.tsv
	auto participant = bm->settings().value(bids::participant_tsv).toString();
	if (QFile::exists(participant) && participant != m_participant) {
		clear();
		auto cols = bm->settings().value(bids::participant_cols).toStringList();
		m_model->setColumnCount(cols.size());
		m_model->setHorizontalHeaderLabels(cols);
		QFile file(participant);
		QTextStream stream(&file);
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			// skip columns line
			stream.readLine();
			int row = 0;
			while (!stream.atEnd()) {
				auto line = stream.readLine();
				auto values = line.split('\t');
				Q_ASSERT(values.size() == cols.size());
				auto col = 0;
				for (auto const& v : values) 
					m_model->setItem(row, col++, new QStandardItem(v));
				row++;
			}
			file.close();
			m_participant = participant;
		}
	}
	else if (!QFile::exists(participant)) {
		clear();
		QMessageBox::information(this, "Clinical Informations", "No participants.tsv file found");
		return;
	}
	show();
}

void AwClinicalWidget::clear()
{
	if (m_model) 
		m_model->clear();
	
}