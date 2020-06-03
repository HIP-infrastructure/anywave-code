#include "AwDSMSaveSetupUI.h"
#include "Prefs/AwSettings.h"
#include <QFile>
#include <QMessageBox>

AwDSMSaveSetupUI::AwDSMSaveSetupUI(const QString& name, QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	if (!name.isEmpty())
		m_ui.lineEditName->setText(name);
	else
		m_ui.lineEditName->setText("New setup");
}

AwDSMSaveSetupUI::~AwDSMSaveSetupUI()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// EVENTS
void AwDSMSaveSetupUI::changeEvent(QEvent *e)
{
	if (e) {
		if (e->type() == QEvent::LanguageChange) 
			m_ui.retranslateUi(this);
	}
}

void AwDSMSaveSetupUI::accept()
{
	auto setupDir = AwSettings::getInstance()->value(aws::setup_dir).toString();
	if (setupDir.isEmpty()) {
		QMessageBox::critical(this, tr("Setup Folder"), tr("No Setup folder found in user directories.\nCannot save current setup."));
		QDialog::accept();
	}

	m_fullPath = setupDir + m_ui.lineEditName->text() + ".aws";
	m_name = m_ui.lineEditName->text();

	if (QFile::exists(m_fullPath))	{
		if (QMessageBox::question(this, tr("Setup name"), tr("A setup with this name already exists. Overwrite it?"),
			QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
			QDialog::accept();
	}
	else
		QDialog::accept();
		
}