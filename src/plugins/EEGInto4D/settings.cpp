#include "settings.h"
#include <QFileDialog>
#include <QMessageBox>

settings::settings(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	m_eegOk = m_megOk = false;
	m_dirPath = "/";
}

settings::~settings()
{

}

void settings::browseEEG()
{
	QString file = QFileDialog::getOpenFileName(this, tr("Choose EEG file"), m_dirPath, "*.ades");
	if (file.isEmpty())
		return;
	m_eegOk = true;
	eegFile = file;
	m_ui.lineEditEEG->setText(file);
	QFileInfo fi(file);
	m_dirPath = fi.absolutePath();
}

void settings::browseMEG()
{
	QString file = QFileDialog::getOpenFileName(this, tr("Choose MEG file"), m_dirPath,  "*");
	if (file.isEmpty())
		return;
	m_megOk = true;
	megFile = file;
	m_ui.lineEditMEG->setText(file);
	QFileInfo fi(file);
	m_dirPath = fi.absolutePath();
}

void settings::accept()
{
	if (m_eegOk && m_megOk) {
		AwFileIO *reader = eegPlugin->newInstance();
		if (reader->openFile(eegFile) != AwFileIO::NoError) {
			QMessageBox::critical(this, tr("ADES File"), tr("Cannot open this file."));
			eegPlugin->deleteInstance(reader);
			return;
		}
		eegPlugin->deleteInstance(reader);
		reader = megPlugin->newInstance();
		if (reader->openFile(megFile) != AwFileIO::NoError) {
			QMessageBox::critical(this, tr("MEG File"), tr("Cannot open this file."));
			megPlugin->deleteInstance(reader);
			return;
		}
		megPlugin->deleteInstance(reader);
		QDialog::accept();
	}
}