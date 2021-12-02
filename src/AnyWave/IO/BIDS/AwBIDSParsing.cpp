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
	QDir dir(m_rootDir);

	// check for files
	// get participant tsv
	auto list = dir.entryInfoList(QDir::Files);
	for (auto l : list) {
		auto file = l.fileName();
		if (file.contains("participants.tsv"))
			m_settings[bids::participant_tsv] = l.filePath();
	}
	using mapItem = QPair<QString, AwBIDSItem*>;
	// function to be run in each thread
	std::function<AwBIDSItems(const mapItem&)> doParsing = [this](const mapItem& it) {
		return recursiveParsing2(it.first, it.second);
	};
	AwBIDSItems subItems;
	auto subjects = buildSubjectItems(m_rootDir);
	if (subjects.size()) {
		for (auto const& sourceSub : subjects)
			subItems << sourceSub.second;
		QFuture<AwBIDSItems> future = QtConcurrent::mapped(subjects, doParsing);
		future.waitForFinished();
	}
	auto dirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

	// now check for subjects in sourcedata
	if (dirs.contains("sourcedata")) {
		QString fullPath = QString("%1/sourcedata").arg(m_rootDir);
		auto sourceSubjects = buildSubjectItems(fullPath);
		if (sourceSubjects.size()) {
			AwBIDSItems subjects;
			for (auto const& sourceSub : sourceSubjects)
				subjects << sourceSub.second;
			QFuture<AwBIDSItems> future = QtConcurrent::mapped(sourceSubjects, doParsing);
			future.waitForFinished();
			// add a source data folder item
			auto sourceItem = new AwBIDSItem("sourcedata");
			sourceItem->setData(fullPath, AwBIDSItem::PathRole);
			sourceItem->setData(AwBIDSItem::Folder, AwBIDSItem::TypeRole);
			sourceItem->setData(m_fileIconProvider.icon(QFileIconProvider::Folder), Qt::DecorationRole);
			sourceItem->setData(QString("sourcedata"), AwBIDSItem::RelativePathRole);
			sourceItem->addChildren(subjects);
			m_items << sourceItem;
		}
	}
	m_items << subItems;


	//// try to implement a multi threaded parsing for each subject
	//// first: detect all subjects
	//// build a qt concurrent map to parse them all
	//auto subDirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

	//

	//QRegularExpression re("^(?<subject>sub-)(?<ID>\\w+)$");
	//QRegularExpressionMatch match;
	//

	//auto subjects = recursiveParsing2(m_rootDir, nullptr);
	//if (dirs.contains("sourcedata")) {
	//	
	//	QString fullPath = QString("%1/sourcedata").arg(m_rootDir);
	//	auto sourceSubjects = recursiveParsing2(fullPath, nullptr);
	//	// add a source data folder item
	//	auto sourceItem = new AwBIDSItem("sourcedata");
	//	sourceItem->setData(fullPath, AwBIDSItem::PathRole);
	//	sourceItem->setData(AwBIDSItem::Folder, AwBIDSItem::TypeRole);
	//	sourceItem->setData(m_fileIconProvider.icon(QFileIconProvider::Folder), Qt::DecorationRole);
	//	sourceItem->setData(QString("sourcedata"), AwBIDSItem::RelativePathRole);
	//	sourceItem->addChildren(sourceSubjects);
	//	m_items << sourceItem;
	//}
	//m_items << subjects;
	// get participants columns
	if (m_settings.contains(bids::participant_tsv))
		m_settings[bids::participant_cols] = AwUtilities::bids::getTsvColumns(m_settings.value(bids::participant_tsv).toString());
	// loading participants data
	m_participantsData = AwUtilities::bids::loadTsv(m_settings.value(bids::participant_tsv).toString());
	emit finished();
}


///// <summary>
///// parse() 
///// detects all subject in sourcedata and BIDS folder.
///// detects all data modalities inside a subject
///// detects all  derivatives of interest like GARDEL, freesurfer, ica, h2, etc
///// </summary>
//void AwBIDSManager::parse()
//{
//	QDir dir(m_rootDir);
//	// check for files
//	// get participant tsv
//	auto list = dir.entryInfoList(QDir::Files);
//	for (auto l : list) {
//		auto file = l.fileName();
//		if (file.contains("participants.tsv"))
//			m_settings[bids::participant_tsv] = l.filePath();
//	}
//	// try to implement a multi threaded parsing for each subject
//	// first: detect all subjects
//	// build a qt concurrent map to parse them all
//	auto subDirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
//
//	auto dirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
//
//	QRegularExpression re("^(?<subject>sub-)(?<ID>\\w+)$");
//	QRegularExpressionMatch match;
//	using mapItem = QPair<QString, AwBIDSItem*>;
//
//	auto subjects = recursiveParsing2(m_rootDir, nullptr);
//	if (dirs.contains("sourcedata")) {
//		QString fullPath = QString("%1/sourcedata").arg(m_rootDir);
//		auto sourceSubjects = recursiveParsing2(fullPath, nullptr);
//		// add a source data folder item
//		auto sourceItem = new AwBIDSItem("sourcedata");
//		sourceItem->setData(fullPath, AwBIDSItem::PathRole);
//		sourceItem->setData(AwBIDSItem::Folder, AwBIDSItem::TypeRole);
//		sourceItem->setData(m_fileIconProvider.icon(QFileIconProvider::Folder), Qt::DecorationRole);
//		sourceItem->setData(QString("sourcedata"), AwBIDSItem::RelativePathRole);
//		sourceItem->addChildren(sourceSubjects);
//		m_items << sourceItem;
//	}
//	m_items << subjects;
//	// get participants columns
//	if (m_settings.contains(bids::participant_tsv))
//		m_settings[bids::participant_cols] = AwUtilities::bids::getTsvColumns(m_settings.value(bids::participant_tsv).toString());
//	// loading participants data
//	m_participantsData = AwUtilities::bids::loadTsv(m_settings.value(bids::participant_tsv).toString());
//	emit finished();
//}

QList<QPair<QString, AwBIDSItem*>> AwBIDSManager::buildSubjectItems(const QString& rootDir)
{
	QRegularExpression re("^(?<subject>sub-)(?<ID>\\w+)$");
	QRegularExpressionMatch match;
	using mapItem = QPair<QString, AwBIDSItem*>;
	
	QDir dir(rootDir);
	QList<mapItem> mapItems;
	auto dirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
	for (auto const& d : dirs) {
		auto name = d.fileName();
		match = re.match(name);
		auto fullPath = d.absoluteFilePath();
		if (match.hasMatch()) {
			// found a subject
			auto item = new AwBIDSItem(name);
			item->setData(fullPath, AwBIDSItem::PathRole);
			item->setData(AwBIDSItem::Subject, AwBIDSItem::TypeRole);
			item->setData(m_fileIconProvider.icon(QFileIconProvider::Folder), Qt::DecorationRole);
			// set the relative path role
			item->setData(name, AwBIDSItem::RelativePathRole);
			// set the possible derivatives mask
			item->setData(AwBIDSItem::gardel | AwBIDSItem::freesurfer, AwBIDSItem::DerivativesRole);
			mapItems.append(mapItem(fullPath, item));
		}
	}
	return mapItems;
}

AwBIDSItems AwBIDSManager::recursiveParsing2(const QString& dirPath, AwBIDSItem* parentItem)
{
	AwBIDSItems res;
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
				reader->plugin()->deleteInstance(reader);
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
				auto item = new AwBIDSItem(name, parentItem);
				item->setData(type, AwBIDSItem::TypeRole);
				item->setData(fullPath, AwBIDSItem::PathRole);
				// build relative path using parent's one
				item->setData(type, AwBIDSItem::DataTypeRole);
				item->setData(QString("%1/%2").arg(parentRelativePath).arg(name), AwBIDSItem::RelativePathRole);
				item->setData(m_fileIconProvider.icon(QFileIconProvider::Folder), Qt::DecorationRole);
				recursiveParsing2(fullPath, item);
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
			recursiveParsing2(fullPath, item);
		}
	}

	return res;
}

//AwBIDSItems AwBIDSManager::recursiveParsing2(const QString& dirPath, AwBIDSItem* parentItem)
//{
//	AwBIDSItems res;
//	QDir dir(dirPath);
//	auto subDirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
//	using mapItem = QPair<QString, AwBIDSItem*>;
//	QList<mapItem> mapItems;
//
//	if (parentItem == nullptr) {
//		// build regexp to find subjects
//		QRegularExpression re("^(?<subject>sub-)(?<ID>\\w+)$");
//		QRegularExpressionMatch match;
//		for (auto subDir : subDirs) {
//			auto name = subDir.fileName();
//			match = re.match(name);
//			auto fullPath = subDir.absoluteFilePath();
//			if (match.hasMatch()) {
//				// found a subject
//				auto item = new AwBIDSItem(name);
//				res << item;
//				item->setData(fullPath, AwBIDSItem::PathRole);
//				item->setData(AwBIDSItem::Subject, AwBIDSItem::TypeRole);
//				item->setData(m_fileIconProvider.icon(QFileIconProvider::Folder), Qt::DecorationRole);
//				// set the relative path role
//				item->setData(name, AwBIDSItem::RelativePathRole);
//				// set the possible derivatives mask
//				item->setData(AwBIDSItem::gardel | AwBIDSItem::freesurfer, AwBIDSItem::DerivativesRole);
//				mapItems.append(mapItem(fullPath, item));
//			}
//		}
//		if (mapItems.size() < 10) {  // is less than 10 subjects found => parse using sequential method
//			for (auto it : mapItems) {
//				recursiveParsing2(it.first, it.second);
//			}
//		}
//		else {  // use multi threaded method
//			std::function<AwBIDSItems(const mapItem&)> doParsing = [this](const mapItem& it) {
//				return recursiveParsing2(it.first, it.second);
//			};
//			QFuture<AwBIDSItems> future = QtConcurrent::mapped(mapItems, doParsing);
//			future.waitForFinished();
//		}
//	}
//	else {
//		auto type = parentItem->data(AwBIDSItem::TypeRole).toInt();
//		auto parentRelativePath = parentItem->data(AwBIDSItem::RelativePathRole).toString();
//		if (m_dataContainers.contains(type)) {  // look for files only in data containers (eeg, ieeg, meg, anat)
//			auto list = dir.entryInfoList(QDir::Files);
//			// check for derivatives at container level (GARDEL for example)
//			setDerivativesForItem(parentItem);
//			for (auto f : list) {
//				auto fileName = f.fileName();
//				auto ext = f.completeSuffix().toLower();
//				// speed up file recongnition avoiding file extensions not known by plugins.
//				// SPECIAL CASE : MEG 4DNI that has no extension. So check before that the file as an extension.
//				if (!ext.isEmpty())
//					if (!m_fileExtensions.contains(ext))
//						continue;
//
//				auto fullPath = f.absoluteFilePath();
//
//				// test for .nii anat image
//				if (fullPath.endsWith(".nii")) {
//					auto fileItem = new AwBIDSItem(fileName, parentItem);
//					fileItem->setData(fullPath, AwBIDSItem::PathRole);
//					fileItem->setData(AwBIDSItem::DataFile, AwBIDSItem::TypeRole);
//					fileItem->setData(type, AwBIDSItem::DataTypeRole);
//					fileItem->setData(QIcon(":/images/ox_eye_32.png"), Qt::DecorationRole);
//
//					// set a display role without some bids keys/values to shorten the file name
//					auto tmp = AwUtilities::bids::removeBidsKey("sub", fileName);
//					tmp = AwUtilities::bids::removeBidsKey("ses", tmp);
//					fileItem->setData(tmp, Qt::DisplayRole);
//					continue;
//				}
//
//				// optimize by setting only readers which can open edf or vhdr files
//
//				auto reader = AwPluginManager::getInstance()->getReaderToOpenFile(fullPath);
//				if (reader != nullptr) {
//					auto fileItem = new AwBIDSItem(fileName, parentItem);
//					fileItem->setData(QDir::toNativeSeparators(fullPath), AwBIDSItem::PathRole);
//					fileItem->setData(AwBIDSItem::DataFile, AwBIDSItem::TypeRole);
//					fileItem->setData(type, AwBIDSItem::DataTypeRole);
//					// set possible derivatives associated to the file item
//					fileItem->setData(AwBIDSItem::ica | AwBIDSItem::h2, AwBIDSItem::DerivativesRole);
//					// add the item to the hash table
//					// use native separators
//
//					// set a display role without some bids keys/values to shorten the file name
//					auto tmp = AwUtilities::bids::removeBidsKey("sub", fileName);
//					tmp = AwUtilities::bids::removeBidsKey("ses", tmp);
//					// remove modality from file name
//					if (tmp.contains("_eeg"))
//						tmp = tmp.remove("_eeg");
//					else if (tmp.contains("_ieeg"))
//						tmp = tmp.remove("_ieeg");
//					else if (tmp.contains("_meg"))
//						tmp = tmp.remove("_meg");
//					fileItem->setData(tmp, Qt::DisplayRole);
//
//					m_hashItemFiles.insert(QDir::toNativeSeparators(fullPath), fileItem);
//					// search for derivatives for this item 
//					setDerivativesForItem(fileItem);
//					// build relative path using parent's one
//					fileItem->setData(QString("%1/%2").arg(parentRelativePath).arg(fileName), AwBIDSItem::RelativePathRole);
//					fileItem->setData(QIcon(":/images/AnyWave_icon.png"), Qt::DecorationRole);
//					// search for events and channels tsv files
//					findTsvFilesForItem(fileItem);
//					parentItem->addFile(fullPath);
//					reader->plugin()->deleteInstance(reader);
//				}
//			}
//			// in a data container (eeg, meg, ieeg) there could be a subfolder (for MEG 4DNI data for example)
//			// check for sub dirs
//			for (auto subDir : subDirs) {
//				auto name = subDir.fileName();
//				auto fullPath = subDir.absoluteFilePath();
//				// this is a MEG special case, in which a subdir may exists but must end with _meg
//				if (name.endsWith("_meg")) { //set the type role of the sub item to be the same as the data container.
//					// That will permit the child file item will have the correct data type.
//					auto item = new AwBIDSItem(name, parentItem);
//					item->setData(type, AwBIDSItem::TypeRole);
//					item->setData(fullPath, AwBIDSItem::PathRole);
//					// build relative path using parent's one
//					item->setData(type, AwBIDSItem::DataTypeRole);
//					item->setData(QString("%1/%2").arg(parentRelativePath).arg(name), AwBIDSItem::RelativePathRole);
//					item->setData(m_fileIconProvider.icon(QFileIconProvider::Folder), Qt::DecorationRole);
//					recursiveParsing2(fullPath, item);
//				}
//			}
//		}
//		else {
//			// check for child node (sub dirs)
//			for (auto subDir : subDirs) {
//				auto name = subDir.fileName();
//				auto fullPath = subDir.absoluteFilePath();
//				auto item = new AwBIDSItem(name, parentItem);
//				item->setData(fullPath, AwBIDSItem::PathRole);
//				item->setData(QString("%1/%2").arg(parentRelativePath).arg(name), AwBIDSItem::RelativePathRole);
//				// check the type 
//				if (name.startsWith("ses-"))
//					item->setData(AwBIDSItem::Session, AwBIDSItem::TypeRole);
//				else if (name == "meg")
//					item->setData(AwBIDSItem::meg, AwBIDSItem::TypeRole);
//				else if (name == "ieeg")
//					item->setData(AwBIDSItem::ieeg, AwBIDSItem::TypeRole);
//				else if (name == "eeg")
//					item->setData(AwBIDSItem::eeg, AwBIDSItem::TypeRole);
//				else if (name == "anat") {
//					item->setData(AwBIDSItem::anat, AwBIDSItem::TypeRole);
//					item->setData(AwBIDSItem::gardel | AwBIDSItem::freesurfer, AwBIDSItem::DerivativesRole);
//				}
//				else
//					item->setData(AwBIDSItem::Folder, AwBIDSItem::TypeRole);
//				item->setData(m_fileIconProvider.icon(QFileIconProvider::Folder), Qt::DecorationRole);
//				recursiveParsing2(fullPath, item);
//			}
//		}
//	}
//	return res;
//}