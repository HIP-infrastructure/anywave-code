#include "settings.h"
#include <QStandardPaths>
#include <QFileDialog>
#include <widget/AwMessageBox.h>

settings::settings(const QList<QSharedPointer<AwFileIO>>& readers, const QStringList& writers, QWidget *parent)
	:  QDialog(parent)
{
	ui.setupUi(this);
	m_readers = readers;
	m_writers = writers;
	// init combo box writers
	ui.comboBoxWriters->addItems(writers);
	m_selectedWriter = writers.first();
	m_dirPath = "/";
	ui.tableWidgetIN->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

settings::~settings()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////:
/// SLOTS

void settings::changeCurrentWriter(int index)
{
	m_selectedWriter = m_writers.at(index);
}


void settings::addFiles()
{
	QStringList files = QFileDialog::getOpenFileNames(this, tr("Select files"), m_dirPath, m_inputFileFilters);
	if (files.isEmpty())
		return;

	QFileInfo fi(files.first());
    m_dirPath = fi.absolutePath();

	int row = ui.tableWidgetIN->rowCount();
	for (auto file : files) {
		bool readerFound = false;
		for (auto reader : m_readers) {
			if (reader->canRead(file) == AwFileIO::NoError) {
				ui.tableWidgetIN->insertRow(row);
				auto item = new QTableWidgetItem(file);
				ui.tableWidgetIN->setItem(row++, 0, item);
				readerFound = true;
				files << file;
				readers << reader;
				reader->cleanUpAndClose();
				break;
			}
		}
		if (!readerFound) {
			AwMessageBox::critical(this, "File error", QString("No plugin can read the file %1").arg(file));

		}
	}

	// At least two input files required
	ui.buttonAppend->setEnabled(ui.tableWidgetIN->rowCount() > 1);
}

void settings::changeOutputFile()
{
   QString output = QFileDialog::getSaveFileName(this, tr("Select Directory"), m_dirPath);
   if (output.isEmpty()) {
	   return;
   }
   outputFile = output;
   ui.lineEditOutputFile->setText(output);
}

void settings::accept()
{
	if (ui.lineEditOutputFile->text().isEmpty()) {
		AwMessageBox::information(this, "Output", "Please specify the output file");
		return;
	}
	outputFile = QDir::toNativeSeparators(ui.lineEditOutputFile->text());

	if (ui.tableWidgetIN->rowCount() < 2) {
		AwMessageBox::information(this, "Input files", "At least two input files are required.");
		return;
	}

	//// create list of input files
	//for (int i = 0; i < ui.tableWidgetIN->rowCount(); i++) {
	//	auto filePath = ui.tableWidgetIN->item(i, 0)->text();
	//	// find a plugin reader that can read this file.
	//	for (auto reader : m_readers.values()) {
	//		reader->cleanUpAndClose();
	//		if (reader->openFile(filePath) == AwFileIO::NoError) {
	//			reader->cleanUpAndClose();
	//			m_files.append(InputFile(filePath, reader));
	//			break;
	//		}
	//	}
	//}

	if (m_files.size() < 2) {
		QMessageBox::information(this, "Input files", "Some files could not be open and the total number of readable files is less than 2.");
		return;
	}

	QDialog::accept();
}