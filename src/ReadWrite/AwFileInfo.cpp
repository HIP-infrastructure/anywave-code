#include <AwFileInfo.h>
#include <qfileinfo.h>

constexpr auto GardelElectrodeFile = "ElectrodesAllCoordinates.txt";
constexpr auto GardelMeshFile = "mesh.stl";


AwFileInfo::AwFileInfo(AwFileIO *reader, const QString& fullPath)
{
	m_reader = reader;
	if (!fullPath.isEmpty())
		m_filePath = fullPath;
	else 
		m_filePath = reader->fullPath();

	QFileInfo info(m_filePath);
	m_dirPath = info.path();
	m_fileName = info.fileName();
}

void AwFileInfo::addExtraInfo(const QString& key, const QVariant& value)
{
	if (!m_extraInfos.contains(key))
		m_extraInfos.insert(key, value);
}

bool AwFileInfo::isExtraInfo(const QString& key)
{
	return m_extraInfos.contains(key);
}

QVariant AwFileInfo::getExtraInfo(const QString& key)
{
	return m_extraInfos.value(key);
}

QVariant AwFileInfo::getFeature(int feature)
{
	switch (feature) {
	case AwFileInfo::SEEGElectrodeFile: 
	{
		QString path = QString("%1/%2").arg(m_dirPath).arg(GardelElectrodeFile);
		if (QFile::exists(path))
			return path;
	}
		break;
	case AwFileInfo::MeshFile:
	{
		QString path = QString("%1/%2").arg(m_dirPath).arg(GardelMeshFile);
		if (QFile::exists(path))
			return path;
	}
		break;
	}
	return QVariant();
}