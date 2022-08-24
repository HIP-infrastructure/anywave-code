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
	ui->lineEditName->setText("New Marker");
	ui->spinValue->setValue(marker->value());
	// init combo color
	// default color at first index
	ui->comboBoxColor->insertItem(0, "Default", 0);
	for (int i = 0; i < QColor::colorNames().size(); i++) {
		ui->comboBoxColor->insertItem(i + 1, QColor::colorNames().at(i), i);
		ui->comboBoxColor->setItemData(i + 1, QColor(QColor::colorNames().at(i)), Qt::DecorationRole);
	}
	connect(ui->comboBoxColor, SIGNAL(currentIndexChanged(int)), this, SLOT(changeColor()));
	connect(ui->lineEditName, &QLineEdit::editingFinished, this, &AwMarkingTool::changeDefaultMarkerTag);
	connect(ui->spinValue, SIGNAL(valueChanged(double)), this, SLOT(changeDefaultValue(double)));

	// starts the gui in default marker mode => hide other options
	ui->groupCustom->hide();
	ui->groupHED->hide();

	// init custom list part of the gui
		// default color at first index
	ui->comboColorCustom->insertItem(0, "Default", 0);
	for (int i = 0; i < QColor::colorNames().size(); i++) {
		ui->comboColorCustom->insertItem(i + 1, QColor::colorNames().at(i), i);
		ui->comboColorCustom->setItemData(i + 1, QColor(QColor::colorNames().at(i)), Qt::DecorationRole);
	}
	QStringList headers = { "Name", "Value", "Color" };
	ui->tableWidgetCustom->setHorizontalHeaderLabels(headers);
	ui->tableWidgetHED->setHorizontalHeaderLabels(headers);
	ui->tableWidgetCustom->verticalHeader()->setVisible(false);
	ui->tableWidgetHED->verticalHeader()->setVisible(false);

	connect(ui->buttonAddMarker, &QPushButton::clicked, this, &AwMarkingTool::addMarkerToCustomList);
}

AwMarkingTool::~AwMarkingTool()
{
	delete ui;
}


void AwMarkingTool::loadCustomList(const QString& path)
{
	auto markers = AwMarker::load(path);
	m_customMarkers = AwMarker::toSharedPointerList(markers);
	// init the gui accordingly
	int row = 0;
	for (auto m : markers) {
		ui->tableWidgetCustom->insertRow(row);
		auto item = new QTableWidgetItem(m->label());
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
		ui->tableWidgetCustom->setItem(row, 0, item);
		item = new QTableWidgetItem(QString::number(m->value()));
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
		ui->tableWidgetCustom->setItem(row, 1, item);
		item = new QTableWidgetItem(m->color());
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
		ui->tableWidgetCustom->setItem(row++, 2, item);
	}
	if (markers.isEmpty()) {
		// init with default marker 
		ui->tableWidgetCustom->insertRow(row);
		auto item = new QTableWidgetItem(m_defaultMarker->label());
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
		ui->tableWidgetCustom->setItem(row, 0, item);
		item = new QTableWidgetItem(QString::number(m_defaultMarker->value()));
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
		ui->tableWidgetCustom->setItem(row, 1, item);
		item = new QTableWidgetItem(m_defaultMarker->color());
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
		ui->tableWidgetCustom->setItem(row, 2, item);
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

void AwMarkingTool::changeAutoIncrement(bool f)
{
	m_settings[keys::auto_increment] = f;
}

void AwMarkingTool::changeConfidenceLevel(bool f)
{
	m_settings[keys::confidence_level] = f;
}

void AwMarkingTool::changeTagWhileMarking(bool f)
{
	m_settings[keys::tag_while_marking] = f;
}

void AwMarkingTool::changeDefaultColor()
{
	int index = ui->comboBoxColor->currentIndex();
	if (index == 0) // Default color
		m_settings[keys::default_color] = QString();
	else
		m_settings[keys::default_color] = QColor(QColor::colorNames().at(index - 1)).name(QColor::HexRgb);
}

void AwMarkingTool::changeDefaultMarkerTag()
{
	m_defaultMarker->setLabel(ui->lineEditName->text());
}

void AwMarkingTool::changeDefaultValue(double v)
{
	m_defaultMarker->setValue(v);
}

void AwMarkingTool::addMarkerToCustomList()
{
	auto marker = new AwMarker;
	marker->setLabel(ui->lineEditCustomLabel->text());
	marker->setValue(ui->spinValueCustom->value());
	marker->setColor(ui->comboColorCustom->currentText());
	m_customMarkers << QSharedPointer<AwMarker>(marker);
	int row = ui->tableWidgetCustom->rowCount();
	ui->tableWidgetCustom->insertRow(row);
	auto item = new QTableWidgetItem(marker->label());
	item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
	ui->tableWidgetCustom->setItem(row, 0, item);
	item = new QTableWidgetItem(QString::number(marker->value()));
	item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
	ui->tableWidgetCustom->setItem(row, 1, item);
	item = new QTableWidgetItem(marker->color());
	item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
	ui->tableWidgetCustom->setItem(row, 2, item);
}