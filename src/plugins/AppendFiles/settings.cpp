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
	int row = ui.tableWidgetIN->rowCount();
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

	// At least two input files required
	ui.buttonAppend->setEnabled(row > 1);
}

void settings::changeOutputFile()
{
   QString output = QFileDialog::getSaveFileName(this, tr("Select Directory"), m_dirPath);
   if (output.isEmpty())
	   return;
   outputFile = output;
   ui.lineEditOutputFile->setText(output);
}

void settings::accept()
{
	if (ui.lineEditOutputFile->text().isEmpty())
		return;
	outputFile = QDir::toNativeSeparators(ui.lineEditOutputFile->text());
	QDialog::accept();
}