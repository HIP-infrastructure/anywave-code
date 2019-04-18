#include "CompumedicsPF.h"
#include <QDir>
#include <QDomDocument>

CompumedicsReaderPlugin::CompumedicsReaderPlugin() : AwFileIOPlugin()
{
	name = QString("Profusion Reader");
	description = QString("Read .eeg Profusion recordings");
	manufacturer = QString("Compumedics");
	version = QString("1.0");
	fileExtensions << QString("*.eeg");
	m_flags = Aw::HasExtension | Aw::CanRead | Aw::IsDirectory;
}

CompumedicsReader::CompumedicsReader(const QString& fileName) : AwFileIO(fileName)
{
}

CompumedicsReader::~CompumedicsReader()
{
	cleanUpAndClose();
}



void CompumedicsReader::getResources(const QString& path)
{
	m_resources.clear();
	m_iniFiles.clear();
	m_sdyFiles.clear();
	m_rdaFiles.clear();
	m_eventsFile.clear();
	m_EPFiles.clear();
	QDir dir(path);
	m_resources = dir.entryList(QDir::Files);
	for (auto res : m_resources) {
		if (res.endsWith(".sdy"))
			m_sdyFiles << QString("%1/%2").arg(path).arg(res);
	}
	// Get EEGData subfolders
	QString EEGDataPath = QString("%1/EEGData").arg(path);
	QDir EEGData(EEGDataPath);
	auto resources= EEGData.entryList(QDir::Files);
	for (auto res : resources) {
		if (res.endsWith(".ini"))
			m_iniFiles << QString("%1/%2").arg(path).arg(res);
		if (res.endsWith(".rda"))
			m_rdaFiles << QString("%1/%2").arg(path).arg(res);
	}
	// Get ElectrodePlacements folder
	QString EPPath = QString("%1/ElectrodePlacements").arg(path);
	QDir EPDir(EPPath);
	auto resources = EEGData.entryList(QDir::Files);
	for (auto res : resources) {
		if (res.endsWith(".xml"))
			m_EPFiles << QString("%1/%2").arg(path).arg(res);
	}

	QString EEGStudyDB = QString("%1/EEGStudyDB.mdb").arg(path);
	if (QFile::exists(EEGStudyDB))
		m_eventsFile = EEGStudyDB;

}

AwFileIO::FileStatus CompumedicsReader::openFile(const QString &path)
{
	getResources(path);
	// read sdy file
	QFile sdyFile(m_sdyFiles.first());
	if (!sdyFile.open(QIODevice::ReadOnly)) {
		m_error = QString("Could not open %1 file.").arg(m_sdyFiles.first());
		return AwFileIO::WrongFormat;
	}
	QDomDocument doc;
	QDomElement element;
	int line, col;
	QString error;
	if (!doc.setContent(&sdyFile, &error, &line, &col)) {
		sdyFile.close();
		m_error = error;
		return AwFileIO::WrongFormat;
	}
	QStringList chanNames;
	QDomElement root = doc.documentElement();
	auto elements = root.elementsByTagName("Channel");
	for (int i = 0; i < elements.size(); i++) {
		auto channel = elements.at(i).toElement();
		if (channel.hasAttribute("name")) 
			chanNames << channel.attribute("name");
	}
	elements = root.elementsByTagName("eeg_sample_rate");
	if (elements.isEmpty()) {
		m_error = "Missing eeg_sample_rate tag";
		return AwFileIO::WrongFormat;
	}
	float sr = elements.at(0).toElement().text().toDouble();
	// check for Electode Placement
	QStringList electrodePlacements;
	if (!m_EPFiles.isEmpty()) {
		QFile file(m_EPFiles.first());
		if (file.open(QIODevice::ReadOnly)) {
			QDomDocument doc;
			if (doc.setContent(&file, &error, &line, &col)) {
				QDomElement root = doc.documentElement();
				
			}
		}
	}

}

AwFileIO::FileStatus CompumedicsReader::canRead(const QString &path)
{
	// very simple test: get the EEG data files.
	getResources(path);
	if (m_resources.isEmpty() || m_rdaFiles.isEmpty() || m_iniFiles.isEmpty() || m_sdyFiles.isEmpty() 
		|| m_eventsFile.isEmpty()) {
		m_error = "Missing required files (*.sdy *.ini *.rda *.xml *.mdb)";
		return AwFileIO::WrongFormat;
	}
	return AwFileIO::NoError;
}