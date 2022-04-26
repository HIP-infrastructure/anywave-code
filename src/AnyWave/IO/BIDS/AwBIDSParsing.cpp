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
#include "AwBIDSManager.h"
#include <QDir>
#include <utils/bids.h>
#include <QtConcurrent>
#include "Plugin/AwPluginManager.h"

/// <summary>
/// parse() 
/// detects all subject in sourcedata and BIDS folder.
/// detects all data modalities inside a subject
/// detects all  derivatives of interest like GARDEL, freesurfer, ica, h2, etc
/// </summary>
void AwBIDSManager::parse()
{
	// change of strategy
	// do not launch a multi threaded recursive parsing, just launch a parsing when the user open a subject folder.
	//
	QDir dir(m_rootDir);
	// check for files
	// get participant tsv
	auto list = dir.entryInfoList(QDir::Files);
	for (auto l : list) {
		auto file = l.fileName();
		if (file.contains("participants.tsv"))
			m_settings[bids::participant_tsv] = l.filePath();
	}
	auto subjects = getSubjectItems(m_rootDir);
	auto dirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

	// now check for subjects in sourcedata
	if (dirs.contains("sourcedata")) {
		m_sourcedataItems << getSourceDataSubjectItems(QString("%1/sourcedata").arg(m_rootDir));
	}
	m_items << getSubjectItems(m_rootDir);
	// build the map to find a subject using it's name
	m_mapSubjects.clear();
	for (auto item : m_items)
		m_mapSubjects.insert(item->text(), item);

	// get participants columns
	if (m_settings.contains(bids::participant_tsv))
		m_settings[bids::participant_cols] = AwUtilities::bids::getTsvColumns(m_settings.value(bids::participant_tsv).toString());
	// loading participants data
	m_participantsData = AwUtilities::bids::loadTsv(m_settings.value(bids::participant_tsv).toString());
	emit finished();
}

AwBIDSItems AwBIDSManager::getSourceDataSubjectItems(const QString& rootDir)
{
	QRegularExpression re("^(?<subject>sub-)(?<ID>\\w+)$");
	QRegularExpressionMatch match;
	QDir dir(rootDir);
	AwBIDSItems items;
	auto dirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
	for (auto const& d : dirs) {
		auto name = d.fileName();
		match = re.match(name);
		auto fullPath = d.absoluteFilePath();
		if (match.hasMatch()) {
			// found a subject
			// auto item = new AwBIDSItem(name);
			auto item = new AwBIDSItem(match.captured("ID"));
			item->setData(fullPath, AwBIDSItem::PathRole);
			item->setData(AwBIDSItem::SourceDataSubject, AwBIDSItem::TypeRole);
			item->setData(m_fileIconProvider.icon(QFileIconProvider::Folder), Qt::DecorationRole);
			// set the relative path role
			item->setData(name, AwBIDSItem::RelativePathRole);
			// set the possible derivatives mask
	//		item->setData(AwBIDSItem::gardel | AwBIDSItem::freesurfer, AwBIDSItem::DerivativesRole);
			item->setData(false, AwBIDSItem::ParsedItem);
			items << item;
		}
	}
	return items;
}

AwBIDSItems AwBIDSManager::getSubjectItems(const QString& rootDir)
{
	QRegularExpression re("^(?<subject>sub-)(?<ID>\\w+)$");
	QRegularExpressionMatch match;
	QDir dir(rootDir);
	AwBIDSItems items;
	auto dirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
	for (auto const& d : dirs) {
		auto name = d.fileName();
		match = re.match(name);
		auto fullPath = d.absoluteFilePath();
		if (match.hasMatch()) {
			// found a subject
			// auto item = new AwBIDSItem(name);
			auto item = new AwBIDSItem(match.captured("ID"));
			item->setData(fullPath, AwBIDSItem::PathRole);
			item->setData(AwBIDSItem::Subject, AwBIDSItem::TypeRole);
			item->setData(m_fileIconProvider.icon(QFileIconProvider::Folder), Qt::DecorationRole);
			// set the relative path role
			item->setData(name, AwBIDSItem::RelativePathRole);
			// set the possible derivatives mask
			item->setData(AwBIDSItem::gardel | AwBIDSItem::freesurfer, AwBIDSItem::DerivativesRole);
			item->setData(false, AwBIDSItem::ParsedItem);
			items << item;
		}
	}
	return items; 
}


void AwBIDSManager::parseSubject(AwBIDSItem* parentItem)
{
	recursiveParsing(parentItem->data(AwBIDSItem::PathRole).toString(), parentItem);
}

void AwBIDSManager::recursiveParsing(const QString& dirPath, AwBIDSItem* parentItem)
{
//	AwBIDSItems res;
	QDir dir(dirPath);
	auto subDirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

	auto type = parentItem->data(AwBIDSItem::TypeRole).toInt();
	auto parentRelativePath = parentItem->data(AwBIDSItem::RelativePathRole).toString();
	if (m_dataContainers.contains(type)) {  // look for files only in data containers (eeg, ieeg, meg, anat)
		auto list = dir.entryInfoList(QDir::Files);
		// check for derivatives at container level (GARDEL for example)
		setDerivativesForItem(parentItem);

		for (auto f : list) {
			auto fileName = f.fileName();
			auto ext = f.completeSuffix().toLower();
			// speed up file recongnition avoiding file extensions not known by plugins.
			// SPECIAL CASE : MEG 4DNI that has no extension. So check before that the file as an extension.
			if (!ext.isEmpty())
				if (!m_fileExtensions.contains(ext))
					continue;

			auto fullPath = f.absoluteFilePath();

			// test for .nii anat image
			if (fullPath.endsWith(".nii")) {
				auto fileItem = new AwBIDSItem(fileName, parentItem);
				fileItem->setData(fullPath, AwBIDSItem::PathRole);
				fileItem->setData(AwBIDSItem::DataFile, AwBIDSItem::TypeRole);
				fileItem->setData(type, AwBIDSItem::DataTypeRole);
				fileItem->setData(QIcon(":/images/ox_eye_32.png"), Qt::DecorationRole);
				fileItem->setData(false, AwBIDSItem::ParsedItem);

				// set a display role without some bids keys/values to shorten the file name
				auto tmp = AwUtilities::bids::removeBidsKey("sub", fileName);
				tmp = AwUtilities::bids::removeBidsKey("ses", tmp);
				fileItem->setData(tmp, Qt::DisplayRole);
				continue;
			}

			// optimize by setting only readers which can open edf or vhdr files

			auto reader = AwPluginManager::getInstance()->getReaderToOpenFile(fullPath);
			if (reader != nullptr) {
				auto fileItem = new AwBIDSItem(fileName, parentItem);
				fileItem->setData(QDir::toNativeSeparators(fullPath), AwBIDSItem::PathRole);
				fileItem->setData(AwBIDSItem::DataFile, AwBIDSItem::TypeRole);
				fileItem->setData(type, AwBIDSItem::DataTypeRole);
				// set possible derivatives associated to the file item
				fileItem->setData(AwBIDSItem::ica | AwBIDSItem::h2, AwBIDSItem::DerivativesRole);
				fileItem->setData(false, AwBIDSItem::ParsedItem);
				// add the item to the hash table
				// use native separators

				// set a display role without some bids keys/values to shorten the file name
				auto tmp = AwUtilities::bids::removeBidsKey("sub", fileName);
				tmp = AwUtilities::bids::removeBidsKey("ses", tmp);
				// remove modality from file name
				if (tmp.contains("_eeg"))
					tmp = tmp.remove("_eeg");
				else if (tmp.contains("_ieeg"))
					tmp = tmp.remove("_ieeg");
				else if (tmp.contains("_meg"))
					tmp = tmp.remove("_meg");
				fileItem->setData(tmp, Qt::DisplayRole);

				m_hashItemFiles.insert(QDir::toNativeSeparators(fullPath), fileItem);
				// search for derivatives for this item 
				 setDerivativesForItem(fileItem);
				// build relative path using parent's one
				fileItem->setData(QString("%1/%2").arg(parentRelativePath).arg(fileName), AwBIDSItem::RelativePathRole);
				fileItem->setData(QIcon(":/images/AnyWave_icon.png"), Qt::DecorationRole);
				// search for events and channels tsv files
				findTsvFilesForItem(fileItem);
				parentItem->addFile(fullPath);
				delete reader;
			}
		}
		// in a data container (eeg, meg, ieeg) there could be a subfolder (for MEG 4DNI data for example)
		// check for sub dirs
		for (auto subDir : subDirs) {
			auto name = subDir.fileName();
			auto fullPath = subDir.absoluteFilePath();
			// this is a MEG special case, in which a subdir may exists but must end with _meg
			if (name.endsWith("_meg")) { //set the type role of the sub item to be the same as the data container.
				// That will permit the child file item will have the correct data type.
				auto tmp = AwUtilities::bids::removeBidsKey("sub", name);
				tmp = AwUtilities::bids::removeBidsKey("meg", tmp);
				tmp = AwUtilities::bids::removeBidsKey("ses", tmp);
				tmp = tmp.remove("_meg");
				auto item = new AwBIDSItem(tmp, parentItem);
				item->setData(type, AwBIDSItem::TypeRole);
				item->setData(fullPath, AwBIDSItem::PathRole);
				// build relative path using parent's one
				item->setData(type, AwBIDSItem::DataTypeRole);
				item->setData(QString("%1/%2").arg(parentRelativePath).arg(name), AwBIDSItem::RelativePathRole);
				item->setData(m_fileIconProvider.icon(QFileIconProvider::Folder), Qt::DecorationRole);
				item->setData(false, AwBIDSItem::ParsedItem);
				recursiveParsing(fullPath, item);
			}
		}
	}
	else {
		// check for child node (sub dirs)
		for (auto subDir : subDirs) {
			auto name = subDir.fileName();
			auto fullPath = subDir.absoluteFilePath();
			auto item = new AwBIDSItem(name, parentItem);
			item->setData(fullPath, AwBIDSItem::PathRole);
			item->setData(QString("%1/%2").arg(parentRelativePath).arg(name), AwBIDSItem::RelativePathRole);
			// check the type 
			if (name.startsWith("ses-"))
				item->setData(AwBIDSItem::Session, AwBIDSItem::TypeRole);
			else if (name == "meg")
				item->setData(AwBIDSItem::meg, AwBIDSItem::TypeRole);
			else if (name == "ieeg")
				item->setData(AwBIDSItem::ieeg, AwBIDSItem::TypeRole);
			else if (name == "eeg")
				item->setData(AwBIDSItem::eeg, AwBIDSItem::TypeRole);
			else if (name == "anat") {
				item->setData(AwBIDSItem::anat, AwBIDSItem::TypeRole);
				item->setData(AwBIDSItem::gardel | AwBIDSItem::freesurfer, AwBIDSItem::DerivativesRole);
			}
			else
				item->setData(AwBIDSItem::Folder, AwBIDSItem::TypeRole);
			item->setData(m_fileIconProvider.icon(QFileIconProvider::Folder), Qt::DecorationRole);
			recursiveParsing(fullPath, item);
		}
	}
//	emit finished();
//	return res;
}
