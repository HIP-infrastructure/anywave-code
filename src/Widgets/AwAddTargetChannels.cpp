#include "AwAddTargetChannels.h"

AwAddTargetChannels::AwAddTargetChannels(const QStringList& labels, QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	m_ui.listWidgetAvailable->addItems(labels);
}

void AwAddTargetChannels::accept()
{
	QList<QListWidgetItem *> selection = m_ui.listWidgetAvailable->selectedItems();

	foreach (QListWidgetItem *item, selection)
		m_selectedChannels << item->text();
	QDialog::accept();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// EVENTS
void AwAddTargetChannels::changeEvent(QEvent *e)
{
	if (e) {
		if (e->type() == QEvent::LanguageChange) 
			m_ui.retranslateUi(this);
	}
}
