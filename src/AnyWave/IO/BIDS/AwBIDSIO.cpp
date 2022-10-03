#include "AwBIDSManager.h"
// implementation of file input/output methods
#include <QDir>
#include <QTextStream>
#include "Marker/AwMarkerManager.h"
#include "Montage/AwMontageManager.h"
#include <QMessageBox>

QString AwBIDSManager::buildOutputDir(const QString& pluginName, AwBIDSItem* item)
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

QString AwBIDSManager::getFileName(const QString& filePath)
{
	findCurrentFileItem(filePath);
	if (m_currentOpenItem == nullptr)
		return QString();
	return buildOutputFileName(m_currentOpenItem);
}

///
/// based in the item relative path, generate a derivatices file name based on the plugin  name.
///
QString AwBIDSManager::buildOutputFileName(AwBIDSItem* item)
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
	for (auto const& suffix : m_dataFileSuffixes) {
		if (fileName.contains(suffix))
			fileName = fileName.remove(suffix);
	}
	return fileName;
}

/// <summary>
/// copyCommonSidecarFilesToUserDerivatives()
/// if no sidecar files exist in the user derivatives folder for the file item => check if there are in the common folder
/// them copy them to the user deriv folder.
/// </summary>
void AwBIDSManager::copyCommonSidecarFilesToUserDerivatives()
{
	auto commonFile = m_settings.value(bids::common_marker_file_path).toString();
	auto userFile = m_settings.value(bids::marker_file_path).toString();
	auto move = [](const QString& source, const QString& dest) {
		if (QFile::exists(source)) {
			if (!QFile::exists(dest)) {
				QFile::copy(source, dest);
			}
		}
	};
	move(commonFile, userFile);
	commonFile = m_settings.value(bids::common_bad_file_path).toString();
	userFile = m_settings.value(bids::bad_file_path).toString();
	move(commonFile, userFile);
	commonFile = m_settings.value(bids::common_montage_file_path).toString();
	userFile = m_settings.value(bids::montage_file_path).toString();
	move(commonFile, userFile);
}


/// <summary>
/// moveSidecarFilesToDerivatives()
/// If there are sidecar files in the subject dir (forbidden by BIDS), move them to the user derivatives folder.
/// </summary>
/// <param name="filePath"></param>
void AwBIDSManager::moveSidecarFilesToDerivatives()
{
	auto baseDestFilePath = QString("%1/%2").arg(getUserDerivativesFolder()).arg(currentFileName());
	// bad
	auto move = [](const QString& source, const QString& dest) {
		if (QFile::exists(source)) {
			QFile::copy(source, dest);
			QFile::remove(source);
		}
	};
	
	auto baseSourceFilePath = getCurrentBIDSPath();
	Q_ASSERT(!baseSourceFilePath.isEmpty());
	auto destFilePath = baseDestFilePath + ".bad";
	auto sourceFile = baseSourceFilePath + ".bad";
	move(sourceFile, destFilePath);
	destFilePath = baseDestFilePath + ".mrk";
	sourceFile = baseSourceFilePath + ".mrk";
	move(sourceFile, destFilePath);
	destFilePath = baseDestFilePath + ".mtg";
	sourceFile = baseSourceFilePath + ".mtg";
	move(sourceFile, destFilePath);
	destFilePath = baseDestFilePath + ".display";
	sourceFile = baseSourceFilePath + ".display";
	move(sourceFile, destFilePath);
	destFilePath = baseDestFilePath + ".flt";
	sourceFile = baseSourceFilePath + ".flt";
	move(sourceFile, destFilePath);
	destFilePath = baseDestFilePath + ".lvl";
	sourceFile = baseSourceFilePath + ".lvl";
	move(sourceFile, destFilePath);
}

void AwBIDSManager::pushMontageFileToCommon()
{
	auto montageFilePath = m_settings.value(bids::montage_file_path).toString();
	// ask montage manager to save current .mtg
	if (!AwMontageManager::instance()->save(montageFilePath)) {
		QMessageBox::critical(nullptr, "Montage", "Error while saving current montage.");
		return;
	}

	if (QFile::exists(montageFilePath)) { 
		int r = QMessageBox::question(nullptr, "Push current montage", "Send current montage to the common folder?\nExisting common .mtg will be replaced.", 
			QMessageBox::Yes | QMessageBox::No);
		if (r == QMessageBox::No)
			return;
		auto commonMontageFilePath = m_settings.value(bids::common_montage_file_path).toString();
		if (QFile::exists(commonMontageFilePath)) {
			if (!QFile::remove(commonMontageFilePath)) {
				QMessageBox::critical(nullptr, "Montage", "Error: could not remove existing common .mtg file.");
				return;
			}
		}
		if (!QFile::copy(montageFilePath, commonMontageFilePath)) {
			QMessageBox::critical(nullptr, "Montage", "Error: could not write current montage to commond folder.");
			return;
		}
	}
	else
		QMessageBox::information(nullptr, "Montage", "No user .mtg file detected.");
}

void AwBIDSManager::pullFromCommonMontageFile()
{
	auto commonMontageFilePath = m_settings.value(bids::common_montage_file_path).toString();
	if (QFile::exists(commonMontageFilePath)) {
		int r = QMessageBox::question(nullptr, "Pull montage", "Get common montage?", QMessageBox::Yes | QMessageBox::No);
		if (r == QMessageBox::No)
			return;
		auto userMontageFilePath = m_settings.value(bids::montage_file_path).toString();
		if (QFile::exists(userMontageFilePath)) {
			if (!QFile::remove(userMontageFilePath)) {
				QMessageBox::critical(nullptr, "Montage", "Error: could not delete user current montage file.");
				return;
			}
		}
		if (!QFile::copy(commonMontageFilePath, userMontageFilePath)) {
			QMessageBox::critical(nullptr, "Montage", "Error: could not copy common .mtg to user folder.");
			return;
		}
		AwMontageManager::instance()->loadMontage(userMontageFilePath);
	}
	else
		QMessageBox::information(nullptr, "Montage", "There is no common montage file.");
}

void AwBIDSManager::pullFromCommonBadFile()
{
	auto commonBadFilePath = m_settings.value(bids::common_bad_file_path).toString();
	if (QFile::exists(commonBadFilePath)) {
		int r = QMessageBox::question(nullptr, "Bad channels", "Get common .bad file and apply it?", QMessageBox::Yes | QMessageBox::No);
		if (r == QMessageBox::No)
			return;
		auto userBadFilePath = m_settings.value(bids::bad_file_path).toString();
		if (QFile::exists(userBadFilePath)) {
			if (!QFile::remove(userBadFilePath)) {
				QMessageBox::critical(nullptr, "Bad channels", "Error: could not delete user current .bad file.");
				return;
			}
		}
		if (!QFile::copy(commonBadFilePath, userBadFilePath)) {
			QMessageBox::critical(nullptr, "Montage", "Error: could not copy common .mtg to user folder.");
			return;
		}
		AwMontageManager::instance()->loadBadChannels();
	}
	else
		QMessageBox::information(nullptr, "Bad channels", "There is no common .bad file.");
}

void AwBIDSManager::pushBadFileToCommon()
{
	// force montage manager to save current bad channels
	AwMontageManager::instance()->saveBadChannels();
	// do the things with the files...
	auto badFilePath = m_settings.value(bids::bad_file_path).toString();
	if (QFile::exists(badFilePath)) {
		int r = QMessageBox::question(nullptr, "Bad channels", "Send current bad channels to common folder?\nExisting common .bad will be replaced.",
			QMessageBox::Yes | QMessageBox::No);
		if (r == QMessageBox::No)
			return;
		auto commonBadFilePath = m_settings.value(bids::common_bad_file_path).toString();
		if (QFile::exists(commonBadFilePath)) {
			if (!QFile::remove(commonBadFilePath)) {
				QMessageBox::critical(nullptr, "Bad channels", "Error: could not remove existing common .bad file.");
				return;
			}
		}
		if (!QFile::copy(badFilePath, commonBadFilePath)) {
			QMessageBox::critical(nullptr, "Bad channels", "Error: could not write current .bad to common folder.");
			return;
		}
	}
	else
		QMessageBox::information(nullptr, "Bad channels", "No user .bad file detected.");
}


void AwBIDSManager::pushMarkerFileToCommon()
{
	auto markerFilePath = m_settings.value(bids::marker_file_path).toString();
	// force marker manager to save current markers
	AwMarkerManager::instance()->saveMarkers(markerFilePath);
	if (QFile::exists(markerFilePath)) { // should exist unless no markers at all are present
		int r = QMessageBox::question(nullptr, "Push markers", "Send current markers to the common file?", QMessageBox::Yes | QMessageBox::No);
		if (r == QMessageBox::No)
			return;
		auto commonMarkerFilePath = m_settings.value(bids::common_marker_file_path).toString();
		AwMarkerList markers;
		if (QFile::exists(commonMarkerFilePath))
			markers = AwMarker::loadFaster(commonMarkerFilePath);
		markers += AwMarker::loadFaster(markerFilePath);
		AwMarker::removeDoublons(markers, true);
		AwMarker::save(commonMarkerFilePath, markers);
		qDeleteAll(markers);
	}
	else
		QMessageBox::information(nullptr, "Markers", "No user .mrk file detected.");

}


void AwBIDSManager::pullFromCommonMarkerFile()
{
	auto commonMarkerFilePath = m_settings.value(bids::common_marker_file_path).toString();
	if (QFile::exists(commonMarkerFilePath)) {
		int r = QMessageBox::question(nullptr, "Pull markers", "Get common markers and merge them?", QMessageBox::Yes | QMessageBox::No);
		if (r == QMessageBox::No)
			return;
		auto userMarkerFilePath = m_settings.value(bids::marker_file_path).toString();
		AwMarkerList markers;
		if (QFile::exists(userMarkerFilePath))
			markers = AwMarker::loadFaster(userMarkerFilePath);
		markers += AwMarker::loadFaster(commonMarkerFilePath);
		AwMarker::removeDoublons(markers, true);
		AwMarker::save(userMarkerFilePath, markers);
		AwMarkerManager::instance()->init();
		qDeleteAll(markers);
	}
	else
		QMessageBox::information(nullptr, "Markers", "No common marker file");
}


QString AwBIDSManager::getDerivativesPath(int type)
{
	QString basePath = QString("%1/derivatives").arg(m_rootDir);
	if (m_currentOpenItem == nullptr)
		return QString();

	switch (type) {
	case AwBIDSManager::freesurfer:
		basePath += QString("/freesurfer/%1").arg(m_currentOpenItem->subjectName());
		break;
	case AwBIDSManager::ica:
		basePath += QString("/ica");
		break;
	}
	return basePath;
}


int AwBIDSManager::selectItemFromFilePath(const QString& path)
{
	// extract subject id from file name
	QRegularExpression re("(?<subject>sub-)(?<ID>\\w+)");
	QRegularExpressionMatch match = re.match(path);
	if (!match.hasMatch())
		return -1;
	QString ID = match.captured("ID");

	// search in intems
	auto item = m_mapSubjects.value(ID);
	if (item == nullptr)
		return -1;

	m_ui->openSubject(item);
	findCurrentFileItem(path);
	if (m_currentOpenItem == nullptr)
		return -1;
	m_ui->openFileItem(m_currentOpenItem);
	return 0;
}


int AwBIDSManager::updateChannelsTsvBadChannels(const QStringList& badLabels)
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

/// <summary>
/// create a derivatives path to store results of a process plugin.
/// the output dir is based on the plugin name and also keeps the file tree list
/// </summary>
/// <param name="processName"></param>
/// <returns>The full path to store results</returns>
QString AwBIDSManager::createDerivativesPath(const QString& processName)
{
	QString res;
	auto item = m_currentOpenItem;
	if (item == nullptr)
		return res;

	if (item) {
		// we assume item is a file item. Get it's parent container
		auto parent = item->bidsParent();
		if (parent) {
			// get the parent name which should match a modality (meg, eeg, ieeg, anat)
			auto name = parent->data(Qt::DisplayRole).toString();
			auto relativePath = parent->data(AwBIDSItem::RelativePathRole).toString();

			if (m_modalities.contains(name))
				res = QString("%1/derivatives/%2/%3").arg(m_rootDir).arg(processName.toLower()).arg(relativePath);
			else {
				// meg file item may be inside a run container not directly the meg container (meg/run_meg)
				auto grandParent = parent->bidsParent();
				if (grandParent) {
					name = grandParent->data(Qt::DisplayRole).toString();
					relativePath = grandParent->data(AwBIDSItem::RelativePathRole).toString();
					if (m_modalities.contains(name))
						res = QString("%1/derivatives/%2/%3").arg(m_rootDir).arg(processName.toLower()).arg(relativePath);
				}
			}
		}
	}

	// create the path if it does not exist yet
	QDir dir;
	dir.mkpath(res);
	return res;
}

