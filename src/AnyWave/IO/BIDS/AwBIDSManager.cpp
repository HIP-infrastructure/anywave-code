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
#include "Plugin/AwPluginManager.h"
#include <AwException.h>
#include <qtextstream.h>
#include <qregularexpression.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <QDirIterator>
#include "Plugin/AwPluginManager.h"
#include <widget/AwMessageBox.h>
#include "Montage/AwMontageManager.h"
#include "Marker/AwMarkerManager.h"
#include "Debug/AwDebugLog.h"
#include <QJsonArray>
#include <utils/json.h>
#include <AwCore.h>
#include <utils/bids.h>
#include <widget/AwWaitWidget.h>
#include <QtConcurrent>
#include <montage/AwMontage.h>
#include "Prefs/AwSettings.h"
#include "Data/AwDataManager.h"
// statics
AwBIDSManager *AwBIDSManager::m_instance = 0;

QStringList AwBIDSManager::m_dataFileSuffixes = { "_eeg", "_meg", "_ieeg" };

constexpr auto GardelElectrodeFile = "ElectrodesAllCoordinates.txt";
constexpr auto GardelMeshFile = "mesh.stl";

void AwBIDSManager::destroy()
{
	if (m_instance) {
		delete m_instance;
		m_instance = Q_NULLPTR;
	}
}

bool AwBIDSManager::isBIDS(const QString& path)
{
	return QFile::exists(QString("%1/dataset_description.json").arg(path));
}


///
/// Search if the path is in a BIDS structure.
/// Returns the BIDS folder if it exists.
/// Returns an empty QString otherwise.
QString AwBIDSManager::detectBIDSFolderFromPath(const QString& path)
{
	// search for a dataset_description.json file which MUST be present at the root level
	QFileInfo fi(path);
	if (!fi.exists()) 
		return QString();

	// look in current path
	auto dirPath = fi.absolutePath();
	// check if we are in a derivatives branch of an existing BIDS
	if (dirPath.contains("derivatives"))
		return QString();

	if (QFile::exists(QString("%1/dataset_description.json").arg(dirPath))) 
		return QString();

	// check on higher branches of the directory tree
	QDir dir = fi.absoluteDir();

	while (true) {
		if (!dir.cdUp())
			break;
		if (QFile::exists(QString("%1/dataset_description.json").arg(dir.absolutePath()))) {
			return dir.absolutePath();
		}
	} 
	return QString();
}

void AwBIDSManager::finishCommandLineOperation()
{
	if (m_instance == nullptr)
		return;
	if (m_instance->m_currentOpenItem) {
		// get .bad file
		auto badFile = QString("%1.bad").arg(m_instance->m_currentOpenItem->data(AwBIDSItem::PathRole).toString());
		if (QFile::exists(badFile)) {
			m_instance->updateChannelsTsvBadChannels(AwMontage::loadBadChannels(badFile));
		}
	}
	delete m_instance;
}

void AwBIDSManager::initCommandLineOperation(const QString & filePath)
{
	auto rootDir = AwBIDSManager::detectBIDSFolderFromPath(filePath);
	if (rootDir.isEmpty())
		return;

	QDir dir(rootDir);
	// get folders in root dir and find the one matching our subject
	auto folders = dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
	QString subjectDir;
	for (auto folder : folders) {
		if (filePath.contains(folder)) {
			subjectDir = folder;
			break;
		}
	}
	if (subjectDir.isEmpty()) {  // totally UNEXPECTED
		return;
	}
	// ok it's a file in a BIDS
	if (m_instance == nullptr)
		m_instance = new AwBIDSManager;
	m_instance->m_rootDir = rootDir;
	m_instance->m_guiMode = false;

	// build the subject item
	auto fullPath = QString("%1/%2").arg(rootDir).arg(subjectDir);
	auto item = new AwBIDSItem(subjectDir);
	m_instance->m_items << item;
	item->setData(fullPath, AwBIDSItem::PathRole);
	item->setData(AwBIDSItem::Subject, AwBIDSItem::TypeRole);
	item->setData(m_instance->m_fileIconProvider.icon(QFileIconProvider::Folder), Qt::DecorationRole);
	// set the relative path role
	item->setData(subjectDir, AwBIDSItem::RelativePathRole);
	// set the possible derivatives mask
	m_instance->recursiveParsing2(fullPath, item);
	m_instance->m_currentOpenItem = item;
}


AwBIDSManager *AwBIDSManager::instance()
{
	if (!m_instance)
		m_instance = new AwBIDSManager;
	return m_instance;
}


AwBIDSManager::AwBIDSManager()
{
	m_ui = nullptr;
	m_currentSubject = nullptr;
	m_currentOpenItem = nullptr;
	m_guiMode = true; // default is to run in GUI mode
	m_dataContainers = { AwBIDSItem::meg, AwBIDSItem::eeg, AwBIDSItem::ieeg, AwBIDSItem::anat };
	m_fileExtensions = AwPluginManager::getInstance()->getReadableFileExtensions();
	// add anat niftfi format extension
	m_fileExtensions.append("nii");
	m_fileExtensions.append("nii.mgz");

	// derivatives folders to parse 
	m_derivativesNames.insert(AwBIDSItem::ica, "ica");
	m_derivativesNames.insert(AwBIDSItem::h2, "h2");
	m_derivativesNames.insert(AwBIDSItem::gardel, "gardel");
	m_modalities = QStringList( { "anat", "meg", "eeg", "ieeg" });
}

AwBIDSManager::~AwBIDSManager()
{
	closeBIDS();
	if (!m_guiMode) {
		// delete all items
		for (auto item : m_items)
			recursiveDelete(item);
	}
}

void AwBIDSManager::recursiveDelete(AwBIDSItem *item)
{
	for (auto child : item->children())
		recursiveDelete(child);
	delete item;
}

QString AwBIDSManager::getParsingPath()
{
	if (!isBIDSActive())
		return QString();
	return m_settings[bids::parsing_path].toString();
}

void AwBIDSManager::setRootDir(const QString& path)
{
	// check if root dir is the same as current one. If so, do nothing.
	if (path == m_rootDir)
		return;

	if (path.isEmpty())
		return;
	// check that the folder exists
	QDir dir(path);
	if (!dir.exists()) {
		AwMessageBox::information(nullptr, "BIDS", QString("The BIDS folder %1 does not exist.").arg(path));
		return;
	}
	closeBIDS();
	m_rootDir = path;
	// We will use QStandardItem to represent the nodes of the BIDS tree.
	// Those nodes will be set aftewards as childrent of the tree view in the GUI.
	// The treeview model will take ownership of the items, no DO NOT DELETE them here or in the closeBIDS() method.
	m_hashItemFiles.clear();
	
	AwWaitWidget wait("Parsing");
	wait.setText("Parsing BIDS Structure...");
	connect(this, &AwBIDSManager::finished, &wait, &QDialog::accept);
	wait.run(std::bind(&AwBIDSManager::parse, this));  // bind a void method without parameters. The method must emit finished signals when finished.

	// get participants columns
	if (m_settings.contains(bids::participant_tsv)) 
		m_settings[bids::participant_cols] = AwUtilities::bids::getTsvColumns(m_settings.value(bids::participant_tsv).toString());

	// instantiate UI if needed
	if (m_ui == nullptr)
		m_ui = new AwBIDSGUI;
	m_ui->refresh();
}

void AwBIDSManager::closeBIDS()
{
	if (!isBIDSActive())
		return;
	m_hashItemFiles.clear();
	if (m_ui)
		m_ui->closeBIDS();

	m_rootDir.clear();
	m_modifications.clear();
	m_currentSubject = nullptr;
	m_currentOpenItem = nullptr;
	m_participantsData.clear();
	m_items.clear();
	emit BIDSClosed();
}

///
/// input : file item
/// get the file prefix name without the modality : sub-id_task-rest and not sub-id_task-rest_ieeg.vhdr
/// Removes the modality suffix (_meg, _ieeg, _eeg)
/// Handle special case of meg data file located inside a run container.
QString AwBIDSManager::getPrefixName(AwBIDSItem *item, bool absolutePath)
{
	QString res = absolutePath ? item->data(AwBIDSItem::RelativePathRole).toString() : item->data(Qt::DisplayRole).toString();
	if (item->data(AwBIDSItem::TypeRole).toInt() == AwBIDSItem::DataFile) {
		for (auto suffix : m_dataFileSuffixes) {
			if (res.contains(suffix)) {
				QRegularExpression exp(QString("(%1).*").arg(suffix));
				return res.remove(exp);
			}
		}
		// no modality detected in filename => meg 4D run or other MEG file format.

		// the file name does not contain the modality (_meg, _eeg, _ieeg) => check parent container name
		auto parent = item->bidsParent();
		if (parent) {
			res = parent->data(Qt::DisplayRole).toString();
			for (auto suffix : m_dataFileSuffixes) {
				if (res.contains(suffix)) {
					QRegularExpression exp(QString("(%1).*").arg(suffix));
					return res.remove(exp);
				}
			}
		}
	}
	return res;
}

QString AwBIDSManager::getDerivativePath(AwBIDSItem *item, int derivativeType)
{
	QString res;
	if (item) {
		// we assume item is a file item. Get it's parent container
		auto parent = item->bidsParent();
		if (parent) {
			// get the parent name which should match a modality (meg, eeg, ieeg, anat)
			auto name = parent->data(Qt::DisplayRole).toString();
			auto relativePath = parent->data(AwBIDSItem::RelativePathRole).toString();

			if (m_modalities.contains(name))
				res = QString("%1/derivatives/%2/%3").arg(m_rootDir).arg(m_derivativesNames.value(derivativeType)).arg(relativePath);
			else {
				// meg file item may be inside a run container not directly the meg container (meg/run_meg)
				auto grandParent = parent->bidsParent();
				if (grandParent) {
					name = grandParent->data(Qt::DisplayRole).toString();
					relativePath = grandParent->data(AwBIDSItem::RelativePathRole).toString();
					if (m_modalities.contains(name))
						res = QString("%1/derivatives/%2/%3").arg(m_rootDir).arg(m_derivativesNames.value(derivativeType)).arg(relativePath);
				}
			}
		}
	}
	return res;
}

QString AwBIDSManager::getDerivativePath(int derivativeType)
{
	return getDerivativePath(m_currentOpenItem, derivativeType);
}


QVariant AwBIDSManager::BIDSProperty(int property)
{
	QVariant res;
	if (m_currentOpenItem == nullptr)
		return res;
	auto parent = m_currentOpenItem->bidsParent();
	if (!parent)
		return res;
	// check if parent is ieeg
	if (parent->data(AwBIDSItem::TypeRole).toInt() != AwBIDSItem::ieeg)
		return res;
	auto grandParent = parent->bidsParent();
	if (!grandParent)
		return res;
	return  grandParent->data(property);
}

void AwBIDSManager::findTsvFilesForItem(AwBIDSItem * item)
{
	// item must be a data file
	if (item->data(AwBIDSItem::TypeRole).toInt() != AwBIDSItem::DataFile)
		return;
	auto prefixName = getPrefixName(item, true);
	// build tsv names
	// channels
	auto tmp = QString("%1_channels.tsv").arg(prefixName);
	auto path = QString("%1/%2").arg(m_rootDir).arg(tmp);
	if (QFile::exists(path))
		item->setData(path, AwBIDSItem::ChannelsTsvRole);
	// events
	tmp = QString("%1_events.tsv").arg(prefixName);
	path = QString("%1/%2").arg(m_rootDir).arg(tmp);
	if (QFile::exists(path))
		item->setData(path, AwBIDSItem::EventsTsvRole);
}

QStringList AwBIDSManager::freesurferMeshes()
{
	return BIDSProperty(AwBIDSItem::FreesurferMeshesRole).toStringList();
}

QString AwBIDSManager::getGardelElectrodes()
{
	return BIDSProperty(AwBIDSItem::GardelElectrodePathRole).toString();
}

QStringList AwBIDSManager::getGardelMontages()
{
	return BIDSProperty(AwBIDSItem::GardelMontagesRole).toStringList();
}

void AwBIDSManager::setDerivativesForItem(AwBIDSItem * item)
{
	auto derivativesMask = item->data(AwBIDSItem::DerivativesRole).toInt();
	if (derivativesMask == 0)
		return;

	QString relativePath;	// relative path to the item.
	auto parentItem = item->bidsParent();

	if (derivativesMask & AwBIDSItem::freesurfer) {
		relativePath = item->data(AwBIDSItem::RelativePathRole).toString();
		auto subjName = item->subjectName();
		auto path = QString("%1/derivatives/freesurfer/%2/surf").arg(m_rootDir).arg(subjName);
		QDir dir(path);
		if (dir.exists()) {
			AwBIDSItem* container = nullptr;
			auto files = dir.entryList(QDir::Files);
			if (!files.isEmpty()) {
				container = new AwBIDSItem("freesurfer", item);
				container->setData(path, AwBIDSItem::PathRole);
				container->setData(AwBIDSItem::freesurfer, AwBIDSItem::TypeRole);
				container->setData(m_fileIconProvider.icon(QFileIconProvider::Folder), Qt::DecorationRole);
			}
			QStringList meshes;
			QStringList acceptedFiles = { "lh.pial", "rh.pial", "lh.white", "rh.white" };
			for (auto file : files) {
				auto fullPath = QString("%1/%2").arg(path).arg(file);
				if (acceptedFiles.contains(file)) {
					meshes << fullPath;
				}
			}
			if (!meshes.isEmpty()) {
				container->setData(meshes, AwBIDSItem::FreesurferMeshesRole);
				if (parentItem)
					parentItem->setData(meshes, AwBIDSItem::FreesurferMeshesRole);
			}
		}
	}

	if (derivativesMask & AwBIDSItem::gardel) {
		relativePath = item->data(AwBIDSItem::RelativePathRole).toString();
		auto path = QString("%1/derivatives/gardel/%2").arg(m_rootDir).arg(relativePath);
		QDir dir(path);
		if (dir.exists()) {
			AwBIDSItem *container = nullptr;
			auto files = dir.entryList(QDir::Files);
			if (!files.isEmpty()) {
				container = new AwBIDSItem("gardel", item);
				container->setData(path, AwBIDSItem::PathRole);
				container->setData(AwBIDSItem::gardel, AwBIDSItem::TypeRole);
				container->setData(m_fileIconProvider.icon(QFileIconProvider::Folder), Qt::DecorationRole);
			}

			QStringList montages;
			for (auto file : files) {
				auto fullPath = QString("%1/%2").arg(path).arg(file);
				
				if (file == GardelElectrodeFile) {
					// add electrode file path to gardel container
					container->setData(fullPath, AwBIDSItem::GardelElectrodePathRole);
					// set also the same properties to the parent item. That way it will be easier for a cousin item to get GARDEL results.
					if (parentItem)
						parentItem->setData(fullPath, AwBIDSItem::GardelElectrodePathRole);
				}
				// check for .mtg
				if (file.endsWith(".mtg"))
					montages << fullPath;
			}
			if (!montages.isEmpty()) {
				container->setData(montages, AwBIDSItem::GardelMontagesRole);
				if (parentItem)
					parentItem->setData(montages, AwBIDSItem::GardelMontagesRole);
			}
		}
	}

	QString derivativePath; 
	QString derivativeName;

	// check for cases related to meg data. MEG file may be inside a meg container
	if (derivativesMask & AwBIDSItem::ica  && parentItem) {
		derivativePath = getDerivativePath(item, AwBIDSItem::ica);
		QDir dir(derivativePath);
		if (dir.exists()) {
			AwBIDSItem *container = nullptr;
			auto files = dir.entryList(QDir::Files);
			auto fileItemName = getPrefixName(item);

			for (auto file : files) {
				if (file.startsWith(fileItemName) && file.endsWith(".mat")) {
					if (container == nullptr) {
						container = new AwBIDSItem("ica", item);
						container->setData(derivativePath, AwBIDSItem::PathRole);
						container->setData(AwBIDSItem::Folder, AwBIDSItem::TypeRole);
						container->setData(AwBIDSItem::ica, AwBIDSItem::DataTypeRole);
						container->setData(m_fileIconProvider.icon(QFileIconProvider::Folder), Qt::DecorationRole);
					}
					auto fileItem = new AwBIDSItem(file, container);
					auto fullPath = QString("%1/%2").arg(derivativePath).arg(file);
					fileItem->setData(fullPath, AwBIDSItem::PathRole);
					fileItem->setData(AwBIDSItem::DataFile, AwBIDSItem::TypeRole);
					fileItem->setData(AwBIDSItem::ica, AwBIDSItem::DataTypeRole);
					fileItem->setData(m_fileIconProvider.icon(QFileIconProvider::File), Qt::DecorationRole);
				}
			}
		}
	}
	// h2 
	if (derivativesMask & AwBIDSItem::h2  && parentItem) {
		derivativePath = getDerivativePath(item, AwBIDSItem::h2);
		QDir dir(derivativePath);
		if (dir.exists()) {
			AwBIDSItem *container = nullptr;
			auto files = dir.entryList(QDir::Files);
			auto fileItemName = getPrefixName(item);
			for (auto file : files) {
				if (file.startsWith(fileItemName) && file.endsWith(".mat")) {
					if (container == nullptr) {
						container = new AwBIDSItem("h2", item);
						container->setData(derivativePath, AwBIDSItem::PathRole);
						container->setData(AwBIDSItem::Folder, AwBIDSItem::TypeRole);
						container->setData(AwBIDSItem::h2, AwBIDSItem::DataTypeRole);
						container->setData(m_fileIconProvider.icon(QFileIconProvider::Folder), Qt::DecorationRole);
					}
					auto fileItem = new AwBIDSItem(file, container);
					auto fullPath = QString("%1/%2").arg(derivativePath).arg(file);
					fileItem->setData(fullPath, AwBIDSItem::PathRole);
					fileItem->setData(AwBIDSItem::DataFile, AwBIDSItem::TypeRole);
					fileItem->setData(AwBIDSItem::h2, AwBIDSItem::DataTypeRole);
					fileItem->setData(m_fileIconProvider.icon(QFileIconProvider::File), Qt::DecorationRole);
				}
			}
		}
	}
}

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
//				item->setData(AwBIDSItem::gardel|AwBIDSItem::freesurfer, AwBIDSItem::DerivativesRole);
//				mapItems.append(mapItem(fullPath, item));
//			}
//		}
//		if (mapItems.size() < 10) {  // is less than 10 subjects found => parse using sequential methode
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
//					item->setData(AwBIDSItem::gardel|AwBIDSItem::freesurfer, AwBIDSItem::DerivativesRole);
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

//int AwBIDSManager::updateEventsTsv(const AwMarkerList& markers)
//{
//	m_errorString.clear();
//	if (m_currentOpenItem == nullptr || markers.isEmpty())
//		return -1;
//	auto tsvPath = m_currentOpenItem->data(AwBIDSItem::EventsTsvRole).toString();
//	QFile file;
//	QTextStream stream(&file);
//	// if the events.tsv does not exist, create it.
//	if (tsvPath.isEmpty() || !QFile::exists(tsvPath)) {
//		tsvPath = getPrefixName(m_currentOpenItem, true);
//		tsvPath = QString("%1_events.tsv").arg(tsvPath);
//		file.setFileName(tsvPath);
//		if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
//			m_errorString = QString("Could not create %1").arg(tsvPath);
//			return -1;
//		}
//		// default columns
//		stream << "onset" << "\t" << "duration" << "\t" << bids::tsv_event_trial_type << endl;
//		for (auto m : markers) 
//			stream << QString("%1").arg(m->start()) << "\t" << QString("%1").arg(m->duration()) << "\t" << m->label() << "\t" << endl;
//		file.close();
//		return 0;
//	}
//	// the file exists, update it
//   // read the first line to get columns
//	file.setFileName(tsvPath);
//	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//		m_errorString = QString("Could not open %1 for reading.").arg(tsvPath);
//		return -1;
//	}
//	auto line = stream.readLine();
//	auto cols = AwUtilities::bids::columnsFromLine(line);
//	file.close();
//	// make a backup in case of...
//	QString bak = tsvPath + ".bak";
//	QFile::copy(tsvPath, bak);
//	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
//		m_errorString = QString("Could not open %1 for writing.").arg(tsvPath);
//		return -1;
//	}
//	// rewrite first line
//	stream << line << endl;
//	for (auto m : markers) {
//		for (auto i = 0; i < cols.size(); i++) {
//			auto colLabel = cols.value(i);
//			if (colLabel == bids::tsv_event_trial_type)
//				stream << m->label();
//			else if (colLabel == bids::tsv_event_onset)
//				stream << QString("%1").arg(m->start());
//			else if (colLabel == bids::tsv_event_duration)
//				stream << QString("%1").arg(m->duration());
//			else
//				stream << "n/a";
//			if (i < cols.size())
//				stream << "\t";
//		}
//		stream << endl;
//	}
//	file.close();
//	QFile::remove(bak);
//	return 0;
//}

int AwBIDSManager::updateChannelsTsvBadChannels(const QStringList & badLabels)
{
	m_errorString.clear();
	if (m_currentOpenItem == nullptr || badLabels.isEmpty())
		return -1;
	auto tsvPath = m_currentOpenItem->data(AwBIDSItem::ChannelsTsvRole).toString();
	if (!QFile::exists(tsvPath))
		return -1;
	// try to copy tsv file as tsv.bak
	QString bak = tsvPath + ".bak";
	QFile::copy(tsvPath, bak);
	QFile sourceFile(bak);
	QFile destFile(tsvPath);
	if (!sourceFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		m_errorString = QString("Could not open %1 for reading.").arg(tsvPath);
		return -1;
	}
	QTextStream sourceStream(&sourceFile);
	QString line = sourceStream.readLine();
	QStringList columns = line.split('\t');
	auto indexName = columns.indexOf("name");
	auto indexStatus = columns.indexOf("status");
	// check that columns contains at leats name and status
	if (indexName == -1 || indexStatus == -1) {
		sourceFile.close();
		QFile::remove(bak);
		m_errorString = QString("Channels.tsv: columns 'status' and/or 'name' are missing");
		return -1;
	}
	if (!destFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
		QFile::copy(bak, tsvPath);
		QFile::remove(bak);
		sourceFile.close();
		m_errorString = QString("Could not open %1 for writing.").arg(tsvPath);
		return -1;
	}
	QTextStream destStream(&destFile);
	destStream << line << endl;
	//auto MM = AwMontageManager::instance();
	while (!sourceStream.atEnd()) {
		line = sourceStream.readLine();
		QStringList cols = line.split('\t');
		if (cols.size() != columns.size())
			break;
		auto name = cols.value(indexName);
		auto status = badLabels.contains(name) ? "bad" : "good";
		cols.replace(indexStatus, status);
		for (auto i = 0; i < cols.size(); i++) {
			destStream << cols.value(i);
			if (i + 1 < cols.size())
				destStream << '\t';
		}
		destStream << endl;
	}
	sourceFile.close();
	destFile.close();
	QFile::copy(bak, tsvPath);
	QFile::remove(bak);
	return 0;
}

QString AwBIDSManager::getCurrentBIDSPath()
{
	if (m_currentOpenItem == nullptr)
		return QString();
	return m_currentOpenItem->data(AwBIDSItem::PathRole).toString();
}

///
/// based in the item relative path, generate a derivatices file name based on the plugin  name.
///
QString AwBIDSManager::buildOutputFileName(AwBIDSItem * item)
{
	auto relativePath = item->data(AwBIDSItem::RelativePathRole).toString();
	QFileInfo fi(relativePath);
	auto fileName = fi.fileName();
	// check special case of meg file contained in a directory
	if (item->data(AwBIDSItem::DataTypeRole).toInt() == AwBIDSItem::meg) {
		if (!fileName.contains("_meg")) {
			fileName = item->parent()->data(AwBIDSItem::RelativePathRole).toString();
			fi.setFile(fileName);
			fileName = fi.fileName();
		}
	}
	for (auto suffix : m_dataFileSuffixes) {
		if (fileName.contains(suffix))
			fileName = fileName.remove(suffix);
	}
	return fileName;
}

QString AwBIDSManager::buildOutputDir(const QString& pluginName, AwBIDSItem * item)
{
	// generate full derivatives path depending on plugin name and file item to be processed.
	auto relativePath = item->data(AwBIDSItem::RelativePathRole).toString();
	QFileInfo fi(relativePath);

	QString outputPath = QString("%1/derivatives/%2/%3").arg(m_rootDir).arg(pluginName.toLower()).arg(fi.path());
	// create directory path  if necesseray
	QDir dir;
	dir.mkpath(outputPath);
	return outputPath;
}

void AwBIDSManager::initAnyWaveDerivativesForFile(const QString& filePath)
{
	// build the path corresponding to the current file in derivatives
	auto relativePath = m_currentOpenItem->data(AwBIDSItem::RelativePathRole).toString();
	QFileInfo fi(relativePath);
	auto userName = AwSettings::getInstance()->value(aws::username).toString();

	QString path = QString("%1/derivatives/anywave/%2/%3").arg(m_rootDir).arg(userName).arg(fi.path());
	QDir dir;
	dir.mkpath(path);
	
	// get filename
	auto dm = AwDataManager::instance();
	auto fileName = dm->value(keys::data_file).toString();
	auto basePath = QString("%1/%2/%3").arg(m_rootDir).arg(relativePath).arg(fileName);

	// get .mrk if any
	auto srcFile = dm->mrkFilePath();
	auto destFile = QString("%1/%2.mrk").arg(path).arg(fileName);
	bool fileExists = QFile::exists(srcFile);
	bool destExists = QFile::exists(destFile);
	if (fileExists && !destExists) {
		QFile::copy(srcFile, destFile);
		QFile::remove(srcFile);
	}
	if (fileExists && destExists) {
		// avoid loosing markers: load the both file in memory, remove doublon and save it.
		auto srcMarkers = AwMarker::load(srcFile);
		srcMarkers += AwMarker::load(destFile);
		AwMarker::removeDoublons(srcMarkers);
		QFile::remove(destFile);
		AwMarker::save(destFile, srcMarkers);
		QFile::remove(srcFile);
	}

	// move mtg file if any
	srcFile = dm->mtgFilePath();
	destFile = QString("%1/%2.mtg").arg(path).arg(fileName);
	moveSidecarFilesToDerivatives(srcFile, destFile);
	srcFile = dm->value(keys::disp_file).toString();
	destFile = QString("%1/%2.display").arg(path).arg(fileName);
	moveSidecarFilesToDerivatives(srcFile, destFile);
	srcFile = dm->value(keys::lvl_file).toString();
	destFile = QString("%1/%2.levels").arg(path).arg(fileName);
	moveSidecarFilesToDerivatives(srcFile, destFile);
	srcFile = dm->badFilePath();
	destFile = QString("%1/%2.bad").arg(path).arg(fileName);
	moveSidecarFilesToDerivatives(srcFile, destFile);
	srcFile = dm->value(keys::flt_file).toString();
	destFile = QString("%1/%2.flt").arg(path).arg(fileName);
	moveSidecarFilesToDerivatives(srcFile, destFile);

	AwDataManager::instance()->setNewRootDirForSideFiles(path);
}

void AwBIDSManager::moveSidecarFilesToDerivatives(const QString& src, const QString& dest)
{
	bool fileExists = QFile::exists(src);
	bool destExists = QFile::exists(dest);
	if (fileExists && !destExists) {
		QFile::copy(src, dest);
		QFile::remove(src);
	}
	if (fileExists && destExists) {
		QFile::remove(dest);
		QFile::copy(src, dest);
		QFile::remove(src);
	}
}

void AwBIDSManager::findItem(const QString& filePath)
{
	m_currentOpenItem = nullptr;
	if (!isBIDSActive())
		return;
	if (m_hashItemFiles.contains(QDir::toNativeSeparators(filePath))) {
		m_currentOpenItem = m_hashItemFiles.value(QDir::toNativeSeparators(filePath));
		m_ui->showItem(m_currentOpenItem);

		// check for user in derivatives/anywave
		initAnyWaveDerivativesForFile(filePath);
	}
}

void AwBIDSManager::newFile(AwFileIO *reader)
{
	// check if the new file is in a BIDS structure or not
	auto root = AwBIDSManager::detectBIDSFolderFromPath(reader->fullPath());

	// root is empty => the file is not located inside a BIDS
	if (root.isEmpty()) {
		closeBIDS();
		return;
	}
	// root bids is the same as the actual one, the file is located inside the current BIDS.
	if (root == m_rootDir) {
		findItem(reader->fullPath());
		return;
	}

	// if current state is no BIDS open:
	if (m_rootDir.isEmpty()) {
		setRootDir(root);
		// find the corresponding subject node
		findItem(reader->fullPath());
		return;
	}

	// root bids is different, close current BIDS and parse the new one.
	if (root != m_rootDir) {
		if (AwMessageBox::information(nullptr, "BIDS", 
			"You requested to open a file located inside another BIDS structure.\nThe current BIDS will be closed.")) {
			closeBIDS();
			setRootDir(root);
			// find the corresponding subject node
			findItem(reader->fullPath());
			return;
		}
	}
}

QStringList AwBIDSManager::readTsvColumns(const QString& path)
{
	QFile file(path);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream stream(&file);
		auto cols = stream.readLine().split('\t');
		file.close();
		return cols;
	}
	return QStringList();
}

AwMarkerList AwBIDSManager::getMarkersFromEventsTsv(const QString& path)
{
	AwMarkerList res;
	QFile file(path);
	QTextStream stream(&file);
	m_errorString.clear();
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		m_errorString = QString("Could not open %1 for reading.").arg(path);
		return res;
	}
	auto line = stream.readLine();
	auto cols = AwUtilities::bids::columnsFromLine(line);
	auto labels = cols.values();
	if (!labels.contains(bids::tsv_event_duration) || !labels.contains(bids::tsv_event_onset) || !labels.contains(bids::tsv_event_trial_type)) {
		m_errorString = QString("required columns are missing in file %1").arg(path);
		file.close();
		return res;
	}
	while (!stream.atEnd()) {
		line = stream.readLine();
		auto tokens = line.split("\t");
		// we assume line contains all the columns otherwise skip it
		if (tokens.size() != cols.size())
			continue;
		auto marker = new AwMarker();
		for (auto i = 0; i < cols.size(); i++) {
			auto label = cols.value(i);
			if (label == bids::tsv_event_trial_type)
				marker->setLabel(tokens.at(i));
			else if (label == bids::tsv_event_onset)
				marker->setStart(tokens.at(i).toDouble());
			else if (label == bids::tsv_event_duration)
				marker->setDuration(tokens.at(i).toDouble());
		}
		res << marker;
	}
	file.close();
	return res;
}

AwMarkerList AwBIDSManager::getMarkersTsv()
{
	AwMarkerList res;
	if (m_currentOpenItem == nullptr)
		return res;
	auto tsvPath = m_currentOpenItem->data(AwBIDSItem::EventsTsvRole).toString();
	if (QFile::exists(tsvPath))
		return getMarkersFromEventsTsv(tsvPath);
	return res;
}

AwChannelList AwBIDSManager::getChannelsTsvMontage()
{
	AwChannelList res;
	if (m_currentOpenItem == nullptr)
		return res;

	auto channelTsvFile = m_currentOpenItem->data(AwBIDSItem::ChannelsTsvRole).toString();
	if (channelTsvFile.isEmpty())
		return res;
	try {
		res = getMontageFromChannelsTsv(channelTsvFile);
	}
	catch (const AwException& e) {
		return res;
	}
	return res;
}

AwChannelList AwBIDSManager::getMontageFromChannelsTsv(const QString& path)
{
	AwChannelList res;
	QFile file(path);
	QTextStream stream(&file);
	m_errorString.clear();
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		m_errorString = QString("Could not open %1 for reading.").arg(path);
		return res;
	}
	auto line = stream.readLine();
	auto cols = AwUtilities::bids::columnsFromLine(line);
	auto labels = cols.values();
	if (!labels.contains(bids::tsv_channel_name) || !labels.contains(bids::tsv_channel_units) || !labels.contains(bids::tsv_channel_type)) {
		m_errorString = QString("required columns are missing in file %1").arg(path);
		file.close();
		return res;
	}
	while (!stream.atEnd()) {
		line = stream.readLine();
		auto tokens = line.split("\t");
		// we assume line contains all the columns otherwise skip it
		if (tokens.size() != cols.size())
			continue;
		auto channel = new AwChannel();
		for (auto i = 0; i < cols.size(); i++) {
			auto label = cols.value(i);
			if (label == bids::tsv_channel_name)
				channel->setName(tokens.at(i));
			//else if (label == bids::tsv_channel_units)
			//	channel->setUnit(tokens.at(i));
			else if (label == bids::tsv_channel_type) {
				auto type = tokens.at(i);
				if (type == "MEGMAG")
					channel->setType(AwChannel::MEG);
				else if (type == "MEGREFMAG" || type == "MEGREFGRADAXIAL" || type == "MEGREFGRADPLANAR")
					channel->setType(AwChannel::Reference);
				else if (type == "MEGGRADAXIAL" || type == "MEGGRADPLANAR")
					channel->setType(AwChannel::GRAD);
				else if (type == "EEG")
					channel->setType(AwChannel::EEG);
				else if (type == "SEEG")
					channel->setType(AwChannel::SEEG);
				else if (type == "ECG")
					channel->setType(AwChannel::ECG);
				else if (type == "EMG")
					channel->setType(AwChannel::EMG);
				else if (type == "TRIG")
					channel->setType(AwChannel::Trigger);
				else
					channel->setType(AwChannel::Other);
			}
			else if (label == bids::tsv_channel_status) {
				channel->setBad(tokens.at(i) == "bad");
			}
		}
		res << channel;
	}
	file.close();
	return res;
}