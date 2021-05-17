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
	QString file = QFileDialog::getOpenFileName(this, tr("Choose EEG file"), m_dirPath, "*.*");
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
		//AwFileIO *reader = eegPlugin->newInstance();
		//if (reader->openFile(eegFile) != AwFileIO::NoError) {
		//	QMessageBox::critical(this, tr("ADES File"), tr("Cannot open this file."));
		//	eegPlugin->deleteInstance(reader);
		//	return;
		//}
		//eegPlugin->deleteInstance(reader);
		//reader = megPlugin->newInstance();
		//if (reader->openFile(megFile) != AwFileIO::NoError) {
		//	QMessageBox::critical(this, tr("MEG File"), tr("Cannot open this file."));
		//	megPlugin->deleteInstance(reader);
		//	return;
		//}
		//megPlugin->deleteInstance(reader);
		QDialog::accept();
	}
}