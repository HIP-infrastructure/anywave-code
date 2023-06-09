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
#include <utils/json.h>
#include <QFile>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <QTextStream>
#include <AwException.h>

QString  AwUtilities::json::toJsonString(const QVariantHash& hash)
{

	auto object = QJsonObject::fromVariantHash(hash);
	QJsonDocument doc(object);
	return QString(doc.toJson(QJsonDocument::Compact));
}

QString  AwUtilities::json::toJsonString(const QVariantMap& map)
{

	auto object = QJsonObject::fromVariantMap(map);
	QJsonDocument doc(object);
	return QString(doc.toJson(QJsonDocument::Compact));
}

QVariantMap AwUtilities::json::mapFromJsonString(const QString& jsonString, QString& errorString)
{
	QJsonDocument doc;
	QVariantMap map;
	if (!jsonString.isEmpty()) {
		QJsonParseError err;
		doc = QJsonDocument::fromJson(jsonString.toUtf8(), &err);
		if (doc.isNull() || err.error != QJsonParseError::NoError) {
			errorString = err.errorString();
			return map;
		}
		return doc.object().toVariantMap();
	}
	return map;
}


QVariantHash AwUtilities::json::hashFromJsonString(const QString& jsonString, QString& errorString)
{
	QJsonDocument doc;
	QVariantHash hash;
	if (!jsonString.isEmpty()) {
		QJsonParseError err;
		doc = QJsonDocument::fromJson(jsonString.toUtf8(), &err);
		if (doc.isNull() || err.error != QJsonParseError::NoError) {
			errorString = err.errorString();
			return hash;
		}
		return doc.object().toVariantHash();
	}
	return hash;
}

QJsonDocument  AwUtilities::json::jsonStringToDocument(const QString& jsonString, QString& errorString)
{
	QJsonDocument doc;
	if (jsonString.isEmpty())
		return doc;
	QJsonParseError err;
	doc = QJsonDocument::fromJson(jsonString.toUtf8(), &err);
	if (doc.isNull() || err.error != QJsonParseError::NoError) {
		errorString = err.errorString();
		return doc;
	}
	return doc;
}

bool  AwUtilities::json::jsonStringToFile(const QString& jsonString, const QString& filePath)
{
	QFile file(filePath);
	QTextStream stream(&file);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return false;
	stream << jsonString;
	file.close();
	return true;
}


QJsonDocument AwUtilities::json::readJsonFile(const QString& filePath)
{
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return QJsonDocument();
	}
	QJsonParseError error;
	QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
	file.close();
	if (doc.isNull() || doc.isEmpty() || error.error != QJsonParseError::NoError) {
		throw AwException(error.errorString(), "AwUtilities::json::readJsonFile");
		return QJsonDocument();
	}
	return doc;
}

QVariantHash AwUtilities::json::fromJsonFileToHash(const QString& filePath)
{
	auto doc = readJsonFile(filePath);
	if (!doc.isEmpty() && !doc.isNull())
		return doc.object().toVariantHash();

	return QVariantHash();
}

QVariantMap AwUtilities::json::fromJsonFileToMap(const QString& filePath)
{
	auto doc = readJsonFile(filePath);
	if (!doc.isEmpty() && !doc.isNull())
		return doc.object().toVariantMap();

	return QVariantMap();
}


QString AwUtilities::json::fromJsonFileToString(const QString& filePath)
{
	auto doc = readJsonFile(filePath);
	if (!doc.isEmpty() && !doc.isNull())
		return QString(doc.toJson(QJsonDocument::JsonFormat::Compact));
	return QString();
}

bool AwUtilities::json::saveToJsonFile(const QJsonDocument& jsonDocument, const QString& filePath)
{
	QString jsonString = jsonDocument.toJson(QJsonDocument::JsonFormat::Indented);
	return saveToJsonFile(jsonString, filePath);
}

bool AwUtilities::json::saveToJsonFile(const QString& jsonString, const QString& filePath)
{
	QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
		return false;
	}
	QTextStream stream(&file);
	stream << jsonString;
	file.close();
	return true;
}