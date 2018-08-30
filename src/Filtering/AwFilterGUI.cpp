#include <filter/AwFilterGUI.h>
#include "AwFilterModel.h"

AwFilterGUI::AwFilterGUI(const AwFilterSettings& settings, QWidget *parent)
	: QWidget(parent)
{
	m_ui.setupUi(this);
	m_ui.tableView->setModel(new AwFilterModel(settings, this));
	m_settings = settings;
}

AwFilterGUI::~AwFilterGUI()
{
}
