#include "Settings.h"

Settings::Settings(const QStringList& labels, QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	m_ui.comboBox->addItems(labels);
}

Settings::~Settings()
{
}


void Settings::accept()
{
	label = m_ui.comboBox->currentText();
	QDialog::accept();
}