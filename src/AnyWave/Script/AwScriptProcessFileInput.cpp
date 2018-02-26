/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "AwScriptProcessFileInput.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDirIterator>


/////////////////////////////////////////////////////////////////////////////////////////////
/// AwProcessFileInputElement

AwProcessFileInputElement::AwProcessFileInputElement(const QString& dataPath)
{
	m_dataPath = dataPath;
}


/////////////////////////////////////////////////////////////////////////////////////////////
/// AwScriptProcessFileInput

AwScriptProcessFileInput::AwScriptProcessFileInput(QObject *parent)
	: QObject(parent)
{
	m_useMarkerFile = true;
	m_useMontageFile = true;
	m_downSampleRate = 0.;
}

AwScriptProcessFileInput::~AwScriptProcessFileInput()
{
	clear();
}

void AwScriptProcessFileInput::clear()
{
	while (!m_inputElements.isEmpty())
		delete m_inputElements.takeFirst();
}

void AwScriptProcessFileInput::setGlobalMontageFile(QString path)
{
	m_globalMontage = path;
	m_useMontageFile = true;
}

void AwScriptProcessFileInput::setMarkerFile(QString file)
{
	m_markerFile = file;
	m_useMarkerFile = true;
}

void AwScriptProcessFileInput::parseFiles()
{
	clear();
	for (int i = 0; i < m_files.size(); i++) {
		QString f = m_files.at(i);
		QFileInfo fi(f);
		// get .mrk and .mrk files in the same directory
		QStringList filters;
		filters << "*.mtg" << "*.mrk";
		QStringList files = fi.absoluteDir().entryList(filters, QDir::Files | QDir::Readable);

		AwPFIElement *element = new AwPFIElement(f);
		// handle marker files
		// an attached marker file will overide a global one
		if (!m_markerFiles.at(i).isEmpty()) {
			element->setMarkerPath(m_markerFiles.at(i));
		}
		else if (m_useMarkerFile) {	// if the use marker file is set then, search for marker file options
			if (!m_markerFile.isEmpty()) { // a global marker file is set
				for (auto s : files) { // looking for the marker file within the data directory
					if (s == m_markerFile)
						element->setMarkerPath(fi.absoluteDir().absoluteFilePath(files.at(files.indexOf(s))));
				}
			}
			else { // no global marker file neither attached marker file
				// takes the first .mrk found in the data directory 
				for (auto s : files)
					if (s.contains(".mrk", Qt::CaseInsensitive)) {
						element->setMarkerPath(fi.absoluteDir().absoluteFilePath(files.at(files.indexOf(s))));
						break;
					}
			}
		}
		// end of marker handling

		// handling montages
		// an attached montage file will overide a global one
		if (!m_montageFiles.at(i).isEmpty()) {
			element->setMontagePath(m_montageFiles.at(i));
		}
		else if (m_useMontageFile) {
			if (!m_globalMontage.isEmpty()) 
				element->setMontagePath(m_globalMontage);
			else { // no global montage neither attached montage is specified => take the first .mtg file in data directory
				for (auto s : files) {
					if (s.contains(".mtg", Qt::CaseInsensitive)) {
						element->setMontagePath(fi.absoluteDir().absoluteFilePath(files.at(files.indexOf(s))));
						break;
					}
				}
			}
		}
		// end of montage handling
		m_inputElements << element;
	}
}

void AwScriptProcessFileInput::addFiles(QString dataPath, QString markerPath, QString montagePath)
{
	m_files.append(dataPath);
	m_markerFiles.append(markerPath);
	m_montageFiles.append(montagePath);
}

void AwScriptProcessFileInput::setRootDir(QString root)
{
	QDir dir(root);
	QString path = QDir::toNativeSeparators(QString::fromLatin1(root.toLatin1()));
	addDirPath(root);
	QDirIterator directories(path, QDir::Dirs|QDir::NoSymLinks|QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
	while (directories.hasNext()) {
		directories.next();
		addDirPath(directories.filePath());
	}
}

void AwScriptProcessFileInput::addDirPath(QString dirPath)
{
	QString path = QDir::toNativeSeparators(QString::fromLatin1(dirPath.toLatin1()));
	QDir dir(path);
	QStringList flist;

	if (!m_fileExtensions.isEmpty())
		flist = dir.entryList(m_fileExtensions, QDir::Files|QDir::Readable);
	else
		flist = dir.entryList(QDir::Files|QDir::Readable);
	
	foreach (QString f, flist)
		addFiles(dir.absoluteFilePath(f));
}

void AwScriptProcessFileInput::readInputFromFile(QString path)
{
	QFile file(path);
	QTextStream stream(&file);

	if (file.open(QIODevice::ReadOnly|QIODevice::Text))	{
		while (!stream.atEnd())
			addFiles(stream.readLine());
		file.close();
	}
}

void AwScriptProcessFileInput::downSample(float samplingRate)
{
	m_downSampleRate = samplingRate;
}

void AwScriptProcessFileInput::setFilters(QString channelType, float lp, float hp)
{
	QString type = channelType.toLower();
	if (type == "seeg" || type == "eeg") {
		m_filters.eegLP = lp; 
		m_filters.eegHP = hp;
	}
	else if (type == "meg") {
		m_filters.megLP = lp; 
		m_filters.megHP = hp;
	}
	else if (type == "ecg" || type == "emg") {
		m_filters.emgLP = lp; 
		m_filters.emgHP = hp;
	}
}
