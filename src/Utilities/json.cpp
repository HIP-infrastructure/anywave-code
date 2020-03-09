/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include <utils/json.h>
#include <QFile>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>

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


QString AwUtilities::json::fromJsonFileToString(const QString& filePath)
{
	auto doc = readJsonFile(filePath);
	if (!doc.isEmpty() && !doc.isNull())
		return QString(doc.toJson(QJsonDocument::JsonFormat::Compact));
	return QString();
}