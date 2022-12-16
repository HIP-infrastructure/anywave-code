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
#include <widget/AwMarkingTool.h>
#include <AwMarker.h>
#include "ui_AwMarkingTool.h"
#include <QHeaderView>
#include "AwEditCustomListDial.h"


AwMarkingTool* AwMarkingTool::m_instance = nullptr;

AwMarkingTool* AwMarkingTool::instance()
{
	if (m_instance == nullptr)
		m_instance = new AwMarkingTool;
	return m_instance;
}

AwMarkingTool::AwMarkingTool(QWidget *parent)
	: QWidget(parent)
{
	ui = new Ui::AwMarkingToolClassUi;
	ui->setupUi(this);
	m_mode = AwMarkingTool::Default;
	auto marker = new AwMarker("New Marker", 0, 0);
	m_defaultMarker = QSharedPointer<AwMarker>(marker);
	// init gui with default settings
	// tag while marking if on by defaut
	m_settings[keys::tag_while_marking] = true;
	m_settings[keys::auto_increment] = false;
	m_settings[keys::confidence_level] = false;
	m_settings[keys::auto_inc_index] = 0;
	ui->lineEditName->setText(marker->label());
	ui->spinValue->setValue(marker->value());

	ui->comboBoxColor->insertItem(0, "Default");
	ui->comboBoxColor->setCurrentIndex(0);

	connect(ui->comboBoxColor, SIGNAL(currentIndexChanged(int)), this, SLOT(changeDefaultColor()));
	connect(ui->lineEditName, &QLineEdit::editingFinished, this, &AwMarkingTool::changeDefaultMarkerTag);
	connect(ui->spinValue, SIGNAL(valueChanged(double)), this, SLOT(changeDefaultValue(double)));

	// connect radio button to change current mode
	connect(ui->radioDefault, &QRadioButton::clicked, this, &AwMarkingTool::changeMode);
	connect(ui->radioCustom, &QRadioButton::clicked, this, &AwMarkingTool::changeMode);
	connect(ui->radioHED, &QRadioButton::clicked, this, &AwMarkingTool::changeMode);

	// starts the gui in default marker mode => hide other options
	ui->groupCustom->hide();
	ui->groupHED->hide();
	ui->checkConfidence->hide();

	ui->tableWidgetCustom->setColumnCount(3);
	ui->tableWidgetCustom->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	QStringList headers = { "Name", "Value", "Color" };
	ui->tableWidgetCustom->setHorizontalHeaderLabels(headers);
	ui->tableWidgetHED->setColumnCount(3);
	ui->tableWidgetHED->setHorizontalHeaderLabels(headers);
	ui->tableWidgetHED->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	connect(ui->buttonEdit, &QPushButton::clicked, this, &AwMarkingTool::editCustomList);
}

AwMarkingTool::~AwMarkingTool()
{
	delete ui;
}

void AwMarkingTool::saveCustomList()
{
	if (m_customListPath.isEmpty())
		return;
	AwMarker::save(m_customListPath, m_customMarkers);
}


void AwMarkingTool::loadCustomList(const QString& path)
{
	m_customListPath = path;
	m_customMarkers = AwMarker::loadShrdFaster(path);
	updateTable();
}

void AwMarkingTool::updateTable()
{
	ui->tableWidgetCustom->clearContents();
	ui->tableWidgetCustom->setRowCount(0);
	int row = 0;
	for (auto const& m : m_customMarkers) {
		ui->tableWidgetCustom->insertRow(row);
		auto item = new QTableWidgetItem(m->label());
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
		ui->tableWidgetCustom->setItem(row, 0, item);
		item = new QTableWidgetItem(QString::number(m->value()));
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
		ui->tableWidgetCustom->setItem(row, 1, item);
		if (m->color().isEmpty())
			item = new QTableWidgetItem("Default");
		else {
			item = new QTableWidgetItem(m->color());
			item->setBackground(QBrush(QColor(m->color())));
		}
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
		ui->tableWidgetCustom->setItem(row++, 2, item);
	}
}

void AwMarkingTool::loadHEDList(const QString& path)
{
	m_hedListPath = path;
	m_hedMarkers = AwMarker::loadShrdFaster(path);
	// init the gui accordingly
	int row = 0;
	for (auto const& m : m_hedMarkers) {
		ui->tableWidgetHED->insertRow(row);
		auto item = new QTableWidgetItem(m->label());
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
		ui->tableWidgetHED->setItem(row, 0, item);
		item = new QTableWidgetItem(QString::number(m->value()));
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
		ui->tableWidgetHED->setItem(row, 1, item);
		item = new QTableWidgetItem(m->color());
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
		ui->tableWidgetHED->setItem(row++, 2, item);
	}
}

QSharedPointer<AwMarker>& AwMarkingTool::defaultMarker()
{
	return m_defaultMarker;
}

void AwMarkingTool::setToDefault()
{
	m_mode = AwMarkingTool::Default;
	ui->radioDefault->setChecked(true);
}

/////// SLOTS

void AwMarkingTool::changeMode(bool f)
{
	if (!f)
		return;
	if (sender() == ui->radioDefault)
		m_mode = AwMarkingTool::Default;
	else if (sender() == ui->radioCustom)
		m_mode = AwMarkingTool::Custom;
	else 
		m_mode = AwMarkingTool::HED;
}

void AwMarkingTool::changeAutoIncrement(bool f)
{
	m_settings[keys::auto_increment] = f;
	emit settingsChanged();
}

void AwMarkingTool::changeConfidenceLevel(bool f)
{
	m_settings[keys::confidence_level] = f;
	emit settingsChanged();
}

void AwMarkingTool::changeTagWhileMarking(bool f)
{
	m_settings[keys::tag_while_marking] = f;
	emit settingsChanged();
}

void AwMarkingTool::changeDefaultColor()
{
	int index = ui->comboBoxColor->currentIndex();
	if (index == 0) // Default color
		m_settings[keys::default_color] = QString();
	else
		m_settings[keys::default_color] = QColor(QColor::colorNames().at(index - 1)).name(QColor::HexRgb);
	m_defaultMarker->setColor(m_settings.value(keys::default_color).toString());
	emit settingsChanged();
}

void AwMarkingTool::changeDefaultMarkerTag()
{
	m_defaultMarker->setLabel(ui->lineEditName->text());
	emit settingsChanged();
}

void AwMarkingTool::changeDefaultValue(double v)
{
	m_defaultMarker->setValue(v);
	emit settingsChanged();
}

void AwMarkingTool::editCustomList()
{
	AwEditCustomListDial dlg(m_customMarkers);
	if (dlg.exec() == QDialog::Accepted) {
		if (dlg.markers().size()) {
			m_customMarkers = dlg.markers();
			updateTable();
			saveCustomList();
		}
	}
	if (m_customMarkers.isEmpty()) {
		// go back to default mode
		ui->radioDefault->setChecked(true);
	}
}