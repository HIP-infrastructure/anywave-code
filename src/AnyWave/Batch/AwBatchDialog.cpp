#include "AwBatchDialog.h"
#include "Process/AwProcessManager.h"

AwBatchDialog::AwBatchDialog(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	auto processes = AwProcessManager::instance()->getBatchableProcesses();
	for (auto p : processes) {
		m_plugins[p->name] = p;
		m_ui.comboPlugin->addItem(p->name);
	}
}

AwBatchDialog::~AwBatchDialog()
{
}
