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
#include <utils/json.h>
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
		m_instance = nullptr;
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
}

void AwBIDSManager::initCommandLineOperation(const QString & filePath, AwDataManager *dm)
{
	auto rootDir = QDir::toNativeSeparators(AwBIDSManager::detectBIDSFolderFromPath(filePath));
	if (rootDir.isEmpty())
		return;  // not in a BIDS
	QRegularExpression exp("sub-.*");
	QString  path = QDir::toNativeSeparators(filePath);
	auto capture = exp.match(path);
	if (!capture.hasMatch())
		return;
	path = capture.captured();
	QFileInfo fi2(filePath);

	auto fileName = fi2.fileName();
	path = path.remove(fileName);
	auto userName = AwSettings::getInstance()->value(aws::username).toString();
	QString derivPath = QString("%1/derivatives/anywave/%2/%3").arg(rootDir).arg(userName).arg(path);
	QDir dir(derivPath);
	if (dir.exists()) {
		auto baseDerivPath = QString("%1%2").arg(derivPath).arg(fileName);
		auto badFilePath = baseDerivPath + ".bad";
		auto markerFilePath = baseDerivPath + ".mrk";
		auto montageFilePath = baseDerivPath + ".mtg";
		auto filterFilePath = baseDerivPath + ".flt";
		dm->settings()[keys::bad_file] = badFilePath;
		dm->settings()[keys::montage_file] = montageFilePath;
		dm->settings()[keys::marker_file] = markerFilePath;
		dm->settings()[keys::flt_file] = filterFilePath;
	}
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

int AwBIDSManager::setRootDir(const QString& path)
{
	// check if root dir is the same as current one. If so, do nothing.
	if (path == m_rootDir)
		return 0;
	if (path.isEmpty())
		return -1;
	// check that the folder exists
	QDir dir(path);
	if (!dir.exists()) {
		AwMessageBox::critical(nullptr, "BIDS", QString("The BIDS folder %1 does not exist.").arg(path));
		return -1;
	}
	// check if we got a real BIDS 
	// we must have a datadescription.json file
	QString dataDesc = QString("%1/dataset_description.json").arg(path);
	if (!QFile::exists(dataDesc)) {
		AwMessageBox::critical(nullptr, "BIDS", "The directory is not a BIDS");
		return -1;
	}
	auto map = AwUtilities::json::fromJsonFileToMap(dataDesc);
	if (!map.contains("BIDSVersion") && !map.contains("Name")) {
		AwMessageBox::critical(nullptr, "BIDS", "Invalid dataset_description.json");
		return -1;
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
	m_ui->init();
	// there is an issue with sourcedata subject when anywave creates derivatives/anywave/username/sourcedata : it makes BM Crash
	// so for now, don't add sourcedata subjects to the tree
	   ////  m_ui->setSourceDataSubjects(m_sourcedataItems);   // uncomment to add sourcedata subjects
	m_ui->setSubjects(m_items);
	return 0;
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
	m_sourcedataItems.clear();
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

QString AwBIDSManager::currentDerivativesDir()
{
	return m_settings.value(bids::file_derivatives_dir).toString();
}

QString AwBIDSManager::currentFileName()
{
	return m_settings.value(bids::current_open_filename).toString();
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
				container = new AwBIDSItem("cortex", item);
				container->setData(AwBIDSItem::freesurfer, AwBIDSItem::TypeRole);
				container->setData(path, AwBIDSItem::PathRole);
				container->setData(QIcon(":/images/cortex.png"), Qt::DecorationRole);

			}
			QStringList meshes;
			QStringList acceptedFiles = { "lh.pial", "rh.pial" };
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

QString AwBIDSManager::getCurrentBIDSPath()
{
	if (m_currentOpenItem == nullptr)
		return QString();
	return m_currentOpenItem->data(AwBIDSItem::PathRole).toString();
}

bool AwBIDSManager::isSourceDataSubject(AwBIDSItem* item)
{
	return item->data(AwBIDSItem::TypeRole).toInt() == AwBIDSItem::SourceDataSubject;
}


bool AwBIDSManager::isSubject(AwBIDSItem* item)
{
	return item->data(AwBIDSItem::TypeRole).toInt() == AwBIDSItem::Subject;
}

QString AwBIDSManager::getUserDerivativesFolder()
{
	auto relativePath = m_currentOpenItem->data(AwBIDSItem::RelativePathRole).toString();
	auto subItem = getParentSubject(m_currentOpenItem);
	if (!subItem)
		return QString();
	QString path;
	QFileInfo fi(relativePath);
	auto userName = AwSettings::getInstance()->value(aws::username).toString();
	if (isSubject(subItem)) {
		path = QString("%1/derivatives/anywave/%2/%3").arg(m_rootDir).arg(userName).arg(fi.path());
	}
	else if (isSourceDataSubject(subItem)) { // creating a sourcedata folder seems to make BM crash....
		path = QString("%1/derivatives/anywave/%2/sourcedata/%3").arg(m_rootDir).arg(userName).arg(fi.path());
	}
	return path;
}

QString AwBIDSManager::getCommonDerivativesFolder()
{
	auto relativePath = m_currentOpenItem->data(AwBIDSItem::RelativePathRole).toString();
	auto subItem = getParentSubject(m_currentOpenItem);
	if (!subItem)
		return QString();
	QString path;
	QFileInfo fi(relativePath);
	if (isSubject(subItem)) {
		path = QString("%1/derivatives/anywave/common/%3").arg(m_rootDir).arg(fi.path());
	}
	else if (isSourceDataSubject(subItem)) { // creating a sourcedata folder seems to make BM crash....
		path = QString("%1/derivatives/anywave/common/sourcedata/%3").arg(m_rootDir).arg(fi.path());
	}
	return path;
}

void AwBIDSManager::initAnyWaveDerivativesForFile(const QString& filePath)
{
	// build the path corresponding to the current file in derivatives
	auto relativePath = m_currentOpenItem->data(AwBIDSItem::RelativePathRole).toString();
	QString commonPath;

	// get the subject item for the currentOpenItem
	auto subItem = getParentSubject(m_currentOpenItem);
	if (!subItem)
		return;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
	QFileInfo fi(relativePath);
	QFileInfo fi2(filePath);

	auto fileName = fi2.fileName();
	auto userName = AwSettings::getInstance()->value(aws::username).toString();

	QString path;
	if (isSubject(subItem)) {
		path = QString("%1/derivatives/anywave/%2/%3").arg(m_rootDir).arg(userName).arg(fi.path());
		commonPath = QString("%1/derivatives/anywave/common/%2").arg(m_rootDir).arg(fi.path());
	}
	else if (isSourceDataSubject(subItem)) { // creating a sourcedata folder seems to make BM crash....
		path = QString("%1/derivatives/anywave/%2/sourcedata/%3").arg(m_rootDir).arg(userName).arg(fi.path());
		commonPath = QString("%1/derivatives/anywave/common/sourcedata/%2").arg(m_rootDir).arg(fi.path());
	}
	else
		return;

	QDir dir;
	dir.mkpath(path); // create user derivatives path
	dir.mkpath(commonPath); // create common path
	auto basePath = QString("%1/%2/%3").arg(m_rootDir).arg(relativePath).arg(fileName);
	// set these variables first !
	m_settings[bids::current_open_filename] = fileName;
	m_settings[bids::file_derivatives_dir] = path;
	auto baseDerivPath = QString("%1/%2").arg(path).arg(fileName);
	m_settings[bids::bad_file_path] = baseDerivPath + ".bad";
	m_settings[bids::marker_file_path] = baseDerivPath + ".mrk";
	m_settings[bids::montage_file_path] = baseDerivPath + ".mtg";
	auto baseCommonPath = QString("%1/%2").arg(commonPath).arg(fileName);
	m_settings[bids::common_bad_file_path] = baseCommonPath + ".bad";
	m_settings[bids::common_marker_file_path] = baseCommonPath + ".mrk";
	m_settings[bids::common_montage_file_path] = baseCommonPath + ".mtg";

	moveSidecarFilesToDerivatives();
	copyCommonSidecarFilesToUserDerivatives();
	AwDataManager::instance()->setNewRootDirForSideFiles();
}

AwBIDSItem* AwBIDSManager::getParentSubject(AwBIDSItem* item)
{
	if (item == nullptr)
		return nullptr;
	if (!isBIDSActive())
		return nullptr;
	auto parent = item;
	int type = item->data(AwBIDSItem::TypeRole).toInt();
	do {
		if (type == AwBIDSItem::Subject || type ==  AwBIDSItem::SourceDataSubject)
			return parent;
		parent = static_cast<AwBIDSItem *>(parent->parent());
		if (parent == nullptr)
			return nullptr;
		type = parent->data(AwBIDSItem::TypeRole).toInt();
	} 
	while (true);
}


void AwBIDSManager::findCurrentFileItem(const QString& filePath)
{
	m_currentOpenItem = nullptr;
	if (!isBIDSActive())
		return;
	if (m_hashItemFiles.contains(QDir::toNativeSeparators(filePath))) {
		m_currentOpenItem = m_hashItemFiles.value(QDir::toNativeSeparators(filePath));
		if (m_guiMode)
			m_ui->showItem(m_currentOpenItem);
		// check for user in derivatives/anywave
		initAnyWaveDerivativesForFile(filePath);
	}
}

int AwBIDSManager::setNewOpenFile(const QString& path)
{
	findCurrentFileItem(path);
	if (m_currentOpenItem == nullptr)
		return -1;
	return 0;
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

void AwBIDSManager::closeFile(QStandardItem* item)
{
	// TO DO HERE  : update channels.tsv file if bad labels have changed?
	// that is not done by default but should be done if an option is set Preferences??
	// if updateChannelsTsv option set in Preferences =>  update channels.tsv
}