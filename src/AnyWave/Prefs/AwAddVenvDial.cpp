#include "AwAddVenvDial.h"
#include <QFileDialog>

AwAddVenvDial::AwAddVenvDial(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	m_ui.editAlias->setText(m_alias);
	m_ui.editPath->setText(m_directory);
	connect(m_ui.buttonPickDir, &QPushButton::clicked, this, &AwAddVenvDial::pickVenvDir);
	setWindowIcon(QIcon(":/images/AnyWave_icon.png"));
}

AwAddVenvDial::~AwAddVenvDial()
{
}

void AwAddVenvDial::pickVenvDir()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Select venv Directory"),
		"/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (dir.isEmpty())
		return;

	m_directory = dir;
	m_ui.editPath->setText(dir);
}


void AwAddVenvDial::accept()
{
	m_alias = m_ui.editAlias->text();
	if (m_alias.isEmpty() || m_directory.isEmpty())
		return;
	QDialog::accept();
}