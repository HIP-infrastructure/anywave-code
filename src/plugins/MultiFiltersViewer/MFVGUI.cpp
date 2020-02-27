#include "MFVGUI.h"
#include "MFVTableModel.h"
#include <AwCore.h>
#include <widget/AwMessageBox.h>

MFVGUI::MFVGUI(QWidget *parent)
	 : AwProcessGUIWidget(parent)
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