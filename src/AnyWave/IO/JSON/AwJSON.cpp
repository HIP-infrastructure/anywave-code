#include "AwJSON.h"
#include <QFile>
using namespace aw::json;
#include <AwException.h>

QJsonDocument loadJSONFile(const QString& path)
{
	QFile loadFile(path);
	QString origin = "loadJSONFile()";
	if (loadFile.open(QIODevice::Text | QIODevice::ReadOnly)) {
		QJsonParseError error;
		QJsonDocument doc = QJsonDocument::fromJson(loadFile.readAll(), &error);
		loadFile.close();
		if (doc.isNull() || doc.isEmpty() || error.error != QJsonParseError::NoError) {
			throw AwException(QString("Json error: %1.").arg(error.errorString()), origin);
			return QJsonDocument();
		}
		return doc;
	}
	throw AwException(QString("Failed to open %1").arg(path), origin);
	return QJsonDocument();
}