#include "AwEditCustomListDial.h"
#include <QTableWidget>
#include <QGridLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QLabel>
#include <widget/AwComboColor.h>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QBrush>


AwEditCustomListDial::AwEditCustomListDial(const AwSharedMarkerList& markers, QWidget* parent) : QDialog(parent)
{
	auto grid = new QGridLayout;
	setWindowTitle("Edit custom markers list");
	setWindowIcon(QIcon(":/images/AnyWave_icon.png"));
	m_customTableWidget = new QTableWidget;
	m_customTableWidget->setColumnCount(3);
	m_customTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_customTableWidget->setHorizontalHeaderLabels({ "Name", "Value", "Color" });
	m_customTableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_customTableWidget->setSelectionBehavior(QTableWidget::SelectRows);
	m_customTableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
	m_markers = markers;
	updateTable();
	grid->addWidget(m_customTableWidget, 0, 0, 1, 4);
	grid->setRowStretch(0, 1);

	m_add = new QPushButton("Add");
	m_remove = new QPushButton("Remove");
	m_clear = new QPushButton("Clear");
	grid->addWidget(m_add, 1, 0, Qt::AlignLeft);
	grid->addWidget(m_remove, 1, 1, Qt::AlignLeft);
	grid->addWidget(m_clear, 1, 3, Qt::AlignRight);
	connect(m_add, &QPushButton::clicked, this, &AwEditCustomListDial::addMarker);
	connect(m_remove, &QPushButton::clicked, this, &AwEditCustomListDial::removeMarkers);
	connect(m_clear, &QPushButton::clicked, this, &AwEditCustomListDial::clearMarkers);

	// add apply button
	auto apply = new QPushButton("Apply");
	grid->addWidget(apply, 2, 3, Qt::AlignRight);
	connect(apply, &QPushButton::clicked, this, &AwEditCustomListDial::accept);
	setLayout(grid);
}


void AwEditCustomListDial::accept()
{
	return QDialog::accept();
}

void AwEditCustomListDial::updateTable()
{
	m_customTableWidget->clearContents();
	m_customTableWidget->setRowCount(0);
	int row = 0;
	for (auto const& m : m_markers) {
		m_customTableWidget->insertRow(row);
		auto item = new QTableWidgetItem(m->label());
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
		m_customTableWidget->setItem(row, 0, item);
		item = new QTableWidgetItem(QString::number(m->value()));
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
		m_customTableWidget->setItem(row, 1, item);
		auto color = m->color();
		if (color.isEmpty())
			item = new QTableWidgetItem("Default");
		else {
			item = new QTableWidgetItem(color);
			item->setBackground(QBrush(QColor(color)));
		}
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
		m_customTableWidget->setItem(row++, 2, item);
	}
}

void AwEditCustomListDial::addMarker()
{
	AwAddCustomMarkerDial dlg;
	if (dlg.exec() == QDialog::Accepted) {
		m_markers << dlg.marker;
		updateTable();
	}
}

void AwEditCustomListDial::removeMarkers()
{
	auto selections = m_customTableWidget->selectionModel()->selectedRows();
	for (auto const& s : selections)
		m_markers.removeAt(s.row());
	updateTable();
}

void AwEditCustomListDial::clearMarkers()
{
	m_customTableWidget->clearContents();
	m_customTableWidget->setRowCount(0);
	m_markers.clear();
}

AwAddCustomMarkerDial::AwAddCustomMarkerDial(QWidget* parent) : QDialog(parent)
{
	setWindowTitle("Add marker to custom list");
	setWindowIcon(QIcon(":/images/AnyWave_icon.png"));
	auto grid = new QGridLayout;
	auto hGrid = new QHBoxLayout;
	hGrid->addWidget(new QLabel("Label: "), 0, Qt::AlignLeft);
	m_comboColor = new AwComboColor(this);
	m_comboColor->insertItem(0, "Default");
	m_comboColor->setCurrentIndex(0);
	m_editLabel = new QLineEdit;
	m_editLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	hGrid->addWidget(m_editLabel, 1, Qt::AlignLeft);
	hGrid->addWidget(new QLabel("Color: "));
	hGrid->addWidget(m_comboColor);
	m_spinValue = new QDoubleSpinBox;
	m_spinValue->setMaximum(1000000);
	m_spinValue->setValue(-1.);
	m_spinValue->setButtonSymbols(QDoubleSpinBox::NoButtons);
	m_spinValue->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	hGrid->addWidget(m_spinValue);
	grid->addLayout(hGrid, 0, 0);
	auto apply = new QPushButton("Apply");
	connect(apply, &QPushButton::clicked, this, &AwAddCustomMarkerDial::accept);
	grid->addWidget(apply, 1, 1, Qt::AlignRight);
	setLayout(grid);
}

void AwAddCustomMarkerDial::accept()
{
	auto label = m_editLabel->text();
	auto value = m_spinValue->value();
	auto color = m_comboColor->currentText();

	if (label.size()) {
		auto m = new AwMarker(label, 0, 0);
		if (color != "Default")
			m->setColor(QColor(color).name());
		m->setValue(value);
		marker = AwSharedMarker(m);
		return QDialog::accept();
	}
} 