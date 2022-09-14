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
#include "AwOutputFileWizardPage.h"
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <AwFileIO.h>
#include <QGridLayout>
#include "Plugin/AwPluginManager.h"
#include <QFileDialog>
#include "Data/AwDataManager.h"
#include <QMessageBox>

AwOutputFileWizardPage::AwOutputFileWizardPage(const QVariantMap& settings, QWidget* parent) : QWizardPage(parent)
{
	setTitle("Select file format and output file name");
	m_settings = settings;
	m_isBids = false;
	if (!m_settings.isEmpty()) {
		// detect bids
		QString filePath = settings.value(keys::bids_file_path).toString();
		m_isBids = !filePath.isEmpty();
	}
	QLabel* labelWriters = nullptr;
	m_comboWriters = new QComboBox();
	if (!m_isBids) {
		labelWriters = new QLabel("&File format:");
		// init combo with eligible plugins
		for (auto writer : AwPluginManager::getInstance()->writers()) {
			m_writersMap.insert(writer->name, writer);
			m_comboWriters->addItem(writer->name);
			m_extensions += writer->fileExtensions;
		}
		labelWriters->setBuddy(m_comboWriters);
	}
	else {
		for (auto writer : AwPluginManager::getInstance()->writers()) {
			if (writer->name == "Brainvision Analyser Format") {
				m_writersMap.insert(writer->name, writer);
				m_comboWriters->addItem(writer->name);
				m_extensions += writer->fileExtensions;
				break;
			}
		}
	}

	QLabel* labelFile = nullptr;
	QPushButton* buttonFile = nullptr;
	if (!m_isBids) {
		labelFile = new QLabel("&Output file:");
		m_filePath = new QLineEdit;
		// init with current data_dir 
		m_filePath->setText(AwDataManager::instance()->dataDir() + "/new_file");
		labelFile->setBuddy(m_filePath);
		buttonFile = new QPushButton("&Browse");
		connect(buttonFile, &QPushButton::clicked, this, &AwOutputFileWizardPage::browseOutputFile);
	}
	else {
		labelFile = new QLabel("proc:");
		m_filePath = new QLineEdit;
		m_filePath->setText("Type here the bids proc applied");

	}

	QGridLayout* layout = new QGridLayout;
	layout->addWidget(labelFile, 1, 0);
	layout->addWidget(m_filePath, 1, 1);
	if (!m_isBids)
		layout->addWidget(buttonFile, 1, 2);
	layout->addWidget(labelWriters, 0, 0);
	layout->addWidget(m_comboWriters, 0, 1);
	setLayout(layout);
}


void AwOutputFileWizardPage::browseOutputFile()
{
	// get current selected plugin
	auto writer = m_writersMap.value(m_comboWriters->currentText());
	if (writer == nullptr)
		return;

	QString filePath = QFileDialog::getSaveFileName(this, "Select output file path", AwDataManager::instance()->dataDir(), writer->fileExtension);
	if (filePath.isEmpty())
		return;
	m_filePath->setText(filePath);
}

bool AwOutputFileWizardPage::validatePage()
{
	if (m_isBids) {
		if (m_writersMap.isEmpty()) {
			QMessageBox::critical(this, "output plugin", "Brainvision Analyser output plugin is required.");
			return false;
		}
		// generate the real file Path
		auto proc = m_filePath->text();
		filePath = m_settings.value(keys::bids_file_path).toString();
		// remove file extension
		filePath = filePath.remove(".vhdr");
		// if the path already has a proc_%   delete it
		filePath = filePath.remove(QRegularExpression("_proc-.*$"));
		// get the modality
		QString modality;
		if (filePath.contains("eeg")) {
			modality = "eeg";
			filePath = filePath.remove("_eeg");
		}
		else if (filePath.contains("ieeg")) {
			modality = "ieeg";
			filePath = filePath.remove("_ieeg");
		}
		else if (filePath.contains("meg")) {
			modality = "meg";
			filePath = filePath.remove("_meg");

		}
		filePath += QString("_proc-%1_%2.vhdr").arg(proc.remove(' ')).arg(modality);
		auto writer = m_writersMap.value(m_comboWriters->currentText());
		writerName = writer->name;
		return true;
	}
	else {
		filePath = m_filePath->text();
		QFileInfo fi(filePath);
		if (fi.absoluteDir().exists()) {
			auto writer = m_writersMap.value(m_comboWriters->currentText());
			writerName = writer->name;
			return true;
		}
	}
	return false;
}

void AwOutputFileWizardPage::initializePage()
{

}