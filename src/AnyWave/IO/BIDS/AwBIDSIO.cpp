#include "AwBIDSManager.h"
// implementation of file input/output methods
#include <QDir>
#include <QTextStream>

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

