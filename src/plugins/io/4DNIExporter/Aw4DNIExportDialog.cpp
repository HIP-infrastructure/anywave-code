#include "Aw4DNIExportDialog.h"

Aw4DNIExportDialog::Aw4DNIExportDialog(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
}

Aw4DNIExportDialog::~Aw4DNIExportDialog()
{}


void Aw4DNIExportDialog::accept()
{
	// get selected modification
	auto text = m_ui.comboBox->currentText().simplified().toLower();

	if (!text.isEmpty())
		selectedModification = text;
	QDialog::accept();
}