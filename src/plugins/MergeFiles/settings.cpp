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
#include "ui_settings.h"
#include <QFileDialog>
#include <QMessageBox>

Settings::Settings(QList<AwFileIOPlugin *>& readers, QWidget *parent)
	: QDialog(parent)
{
	m_ui = new Ui::SettingsUi();
	m_ui->setupUi(this);
	m_readers = readers;
	// build filter for open dialog box
	foreach (AwFileIOPlugin *plugin, readers)
		foreach (QString ext, plugin->fileExtensions)
				m_inputFileFilters += ext + " "; 
	// instantiate temporary readers
	foreach(AwFileIOPlugin *p, m_readers) {
		AwFileIO *fr =  p->newInstance();
		fr->setPlugin(p);
		m_tempReaders << fr;
	}
	file1Plugin = file2Plugin = NULL;
	m_currentFolder = "/";
}

Settings::~Settings()
{
	delete m_ui;
	foreach (AwFileIO *fr, m_tempReaders)
		fr->plugin()->deleteInstance(fr);
}


QString Settings::selectFile(const QString& folder)
{
	return QFileDialog::getOpenFileName(this, tr("Select file"), folder, m_inputFileFilters);
}

void Settings::selectFile1()
{
	QString file = selectFile(m_currentFolder);
	if (!file.isEmpty()) {
		m_ui->lineEdit1->setText(file);
		file1 = file;
		m_currentFolder = QFileInfo(file1).path() + "/";
	}
}

void Settings::selectFile2()
{
	QString file = selectFile(m_currentFolder);
	if (!file.isEmpty()) {
		m_ui->lineEdit2->setText(file);
		file2 = file;
		m_currentFolder = QFileInfo(file2).path() + "/";
	}
}

void Settings::accept()
{
	if (m_ui->lineEdit1->text().isEmpty() || m_ui->lineEdit2->text().isEmpty())
		return;

	file1 = m_ui->lineEdit1->text();
	file2 = m_ui->lineEdit2->text();

	foreach (AwFileIO *fr, m_tempReaders) {
		if (fr->canRead(file1) == AwFileIO::NoError)
			file1Plugin = fr->plugin();
		if (fr->canRead(file2) == AwFileIO::NoError)
			file2Plugin = fr->plugin();
	}

	if (file1Plugin == NULL || file2Plugin == NULL) {
		QMessageBox::critical(this, tr("Failed to open file"), tr("One of the input file cannot be read by AnyWave"));
		return;
	}

	outputFile = QFileDialog::getSaveFileName(this, tr("Select output file"), "/", "(*.ades)");
	if (outputFile.isEmpty())
		return;
	return QDialog::accept();
}
