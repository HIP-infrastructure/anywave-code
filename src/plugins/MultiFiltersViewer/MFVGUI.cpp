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
#include "MFVGUI.h"
#include "MFVTableModel.h"
#include <AwCore.h>
#include <widget/AwMessageBox.h>

MFVGUI::MFVGUI(AwGUIProcess *process, QWidget *parent)
	 : AwProcessGUIWidget(process, parent)
{
	m_ui.setupUi(this);
	m_signalView = new AwBaseSignalView();
	m_signalView->setFlags(AwBaseSignalView::NoMarkerBar | AwBaseSignalView::ViewAllChannels);
	auto layout = m_ui.signalsLayout;
	layout->addWidget(m_signalView, 0, 1);
	m_signalView->setMinimumHeight(200);
	m_ui.tableView->setModel(new MFVTableModel(this));
	m_ui.tableView->setItemDelegate(new MFVModelDelegate(this));
	m_ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_ui.tableView->resizeColumnsToContents();
	// connections
	connect(m_ui.buttonApply, &QPushButton::clicked, this, &MFVGUI::apply);
	connect(m_ui.buttonClear, &QPushButton::clicked, this, &MFVGUI::clear);
	connect(m_ui.buttonAdd, &QPushButton::clicked, this, &MFVGUI::add);

}

MFVGUI::~MFVGUI()
{
	AW_DESTROY_LIST(m_currentChannels);
}


void MFVGUI::setChannels(const AwChannelList& channels)
{
	m_signalView->setChannels(channels);
	m_channels = channels;

	repaint();
}

void MFVGUI::add()
{
	static_cast<MFVTableModel *>(m_ui.tableView->model())->add(new MFVFilterSet);

}

void MFVGUI::clear()
{
	auto r = AwMessageBox::question(this, "Confirm", "Clear the current list?", QMessageBox::Yes | QMessageBox::No);
	if (r == QMessageBox::Yes)
		static_cast<MFVTableModel *>(m_ui.tableView->model())->clear();
}

void MFVGUI::apply()
{
	AW_DESTROY_LIST(m_currentChannels);
	auto sets = static_cast<MFVTableModel *>(m_ui.tableView->model())->filterSets();
	for (auto c : m_channels) {
		auto dup = c->duplicate();
		m_currentChannels << dup;
		for (auto s : sets) {
			dup = c->duplicate();
			dup->setLowFilter(s->lp());
			dup->setHighFilter(s->hp());
			dup->setNotch(s->notch());
			dup->setColor(s->color());
			m_currentChannels << dup;
		}
	}
	m_signalView->setChannels(m_currentChannels);
	repaint();
}