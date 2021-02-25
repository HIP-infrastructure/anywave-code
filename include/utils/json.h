/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <AwGlobal.h>
#include <QVariant>

namespace AwUtilities // put utility functions inside a namespace
{
	namespace json {
		// from a json string returns a variant hash : empty if error. errorString contains the error description
		QVariantHash AW_UTILITIES_EXPORT hashFromJsonString(const QString& jsonString, QString& errorString);
		QString AW_UTILITIES_EXPORT toJsonString(const QVariantHash& hash);
		QString AW_UTILITIES_EXPORT toJsonString(const QVariantMap& hash);
		QVariantMap AW_UTILITIES_EXPORT mapFromJsonString(const QString& jsonString, QString& errorString);
		QJsonDocument AW_UTILITIES_EXPORT readJsonFile(const QString& file);
		QJsonDocument AW_UTILITIES_EXPORT jsonStringToDocument(const QString& jsonString, QString& errorString);
		bool AW_UTILITIES_EXPORT jsonStringToFile(const QString& jsonString, const QString& filePath);
		QVariantHash AW_UTILITIES_EXPORT fromJsonFileToHash(const QString& file);
		QVariantMap AW_UTILITIES_EXPORT fromJsonFileToMap(const QString& file);
		QString AW_UTILITIES_EXPORT fromJsonFileToString(const QString& file);
		bool AW_UTILITIES_EXPORT saveToJsonFile(const QString& jsonString, const QString& file);
		bool AW_UTILITIES_EXPORT saveToJsonFile(const QJsonDocument& jsonDocument, const QString& file);
	}
}
