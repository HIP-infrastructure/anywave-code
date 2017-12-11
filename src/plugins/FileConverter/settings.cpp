/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "settings.h"
#include <QStandardPaths>
#include <QFileDialog>

settings::settings(QList<AwFileIOPlugin *>& readers, QList<AwFileIOPlugin *>& writers, QWidget *parent)
	: m_readers(readers), m_writers(writers),  QDialog(parent)
{
	ui.setupUi(this);
	m_readers = readers;
	m_writers = writers;
	// init combo box writers
	foreach (AwFileIOPlugin *writer, writers)
		ui.comboBoxWriters->addItem(writer->name);
	selectedWriter = writers.first();
	outputDir = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first();
	ui.lineEditOutputDir->setText(outputDir);
	ui.groupBoxFilters->hide();
	m_dirPath = "/";
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
	ui.tableWidgetIN->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	filters = false;
	eegLP = eegHP = megLP = megHP = eegNotch =  megNotch = 0.;
	emgLP = emgHP = emgNotch = 0.;
}

settings::~settings()
{
	foreach (AwFileIO *fr, m_tempReaders)
		fr->plugin()->deleteInstance(fr);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////:
/// SLOTS

void settings::changeCurrentWriter(int index)
{
	selectedWriter = m_writers.at(index);
}


void settings::addFiles()
{
	QStringList files = QFileDialog::getOpenFileNames(this, tr("Select files"), m_dirPath, m_inputFileFilters);
	if (files.isEmpty())
		return;

	// check for input files
	int row = 0;
	foreach (QString file, files) {
		ui.tableWidgetIN->insertRow(row);
		bool reader_found = false;
		QTableWidgetItem *col1 = new QTableWidgetItem(file);
		ui.tableWidgetIN->setItem(row, 0, col1);
		foreach (AwFileIO *fr, m_tempReaders) {
			if (fr->canRead(file) == AwFileIO::NoError) {
				inputs.append(QPair<AwFileIOPlugin *, QString>(fr->plugin(), file));
				reader_found = true;
				QTableWidgetItem *col2 = new QTableWidgetItem("ok");
				col2->setTextAlignment(Qt::AlignCenter);
				col2->setForeground(QBrush(QColor("green")));
				ui.tableWidgetIN->setItem(row, 1, col2);
			}
			if (reader_found)
				continue;
		}
		if (!reader_found) {
			QTableWidgetItem *col2 = new QTableWidgetItem("Unknown");
			col2->setTextAlignment(Qt::AlignCenter);
			col2->setForeground(QBrush(QColor("red")));
			ui.tableWidgetIN->setItem(row, 1, col2);
		}
		row++;
	}
	ui.buttonConvert->setEnabled(true);
}

void settings::changeOutputDir()
{
   QString output = QFileDialog::getExistingDirectory(this, tr("Select Directory"), m_dirPath);
   if (output.isEmpty())
	   return;
   outputDir = output;
   ui.lineEditOutputDir->setText(output);
}

int settings::exec()
{
	if (!outputDir.isEmpty()) {
		 ui.lineEditOutputDir->setText(outputDir);
	}
	return QDialog::exec();
}

void settings::accept()
{
	filters = !ui.radioNoFiltering->isChecked();
	if (filters) {
		eegLP = ui.spEEGLP->value();
		eegHP = ui.spEEGHP->value();
		megLP = ui.spMEGLP->value();
		megHP = ui.spMEGHP->value();
		eegNotch = ui.spEEGNotch->value();
		megNotch = ui.spMEGNotch->value();
		emgLP = ui.spEMGLP->value();
		emgHP = ui.spEMGHP->value();
		emgNotch = ui.spEMGNotch->value();
	}
	QDialog::accept();
}