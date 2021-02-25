#pragma once

#include <qjsondocument.h>

namespace aw {
	namespace json {
		QJsonDocument loadJSONFile(const QString& filePath);
	}
}