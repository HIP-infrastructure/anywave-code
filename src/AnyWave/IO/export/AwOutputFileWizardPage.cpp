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

AwOutputFileWizardPage::AwOutputFileWizardPage(QWidget* parent) : QWizardPage(parent)
{
	setTitle("Select file format and output file name");
	auto labelWriters = new QLabel("&File format:");
	m_comboWriters = new QComboBox();
	// init combo with eligible plugins
	for (auto writer : AwPluginManager::getInstance()->writers()) {
		m_writersMap.insert(writer->name, writer);
		m_comboWriters->addItem(writer->name);
		m_extensions += writer->fileExtensions;
	}
	labelWriters->setBuddy(m_comboWriters);

	auto labelFile = new QLabel("&Output file:");
	m_filePath = new QLineEdit;
	// init with current data_dir 
	m_filePath->setText(AwDataManager::instance()->dataDir() + "/new_file");
	labelFile->setBuddy(m_filePath);
	auto buttonFile = new QPushButton("&Browse");
	connect(buttonFile, &QPushButton::clicked, this, &AwOutputFileWizardPage::browseOutputFile);

	QGridLayout* layout = new QGridLayout;
	layout->addWidget(labelFile, 1, 0);
	layout->addWidget(m_filePath, 1, 1);
	layout->addWidget(buttonFile, 1, 2);
	layout->addWidget(labelWriters, 0, 0);
	layout->addWidget(m_comboWriters, 0, 1);
	setLayout(layout);
//	registerField("filePath*", m_filePath);
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
	filePath = m_filePath->text();
	QFileInfo fi(filePath);
	if (fi.absoluteDir().exists()) {
		auto writer = m_writersMap.value(m_comboWriters->currentText());
		writerName = writer->name;
		return true;
	}
	return false;
}