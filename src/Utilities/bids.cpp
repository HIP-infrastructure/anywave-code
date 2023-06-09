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
#include <utils/bids.h>
#include <QRegularExpression>
#include <QFile>
#include <QTextStream>

QString  AwUtilities::bids::getSessionLabel(const QString& data)
{
	QRegularExpression re("^(?<session>ses-)(?<label>\\w+)$");
	QRegularExpressionMatch match = re.match(data);
	if (!match.hasMatch())
		return QString();
	return match.captured("label");
}

QString AwUtilities::bids::getSubjectID(const QString& data)
{
	QRegularExpression re("^(?<subject>sub-)(?<ID>\\w+)$");
	QRegularExpressionMatch match = re.match(data);
	if (!match.hasMatch())
		return QString();
	return match.captured("ID");
}

QStringList AwUtilities::bids::getTsvColumns(const QString& tsvFile)
{
	QStringList list;
	QFile file(tsvFile);
	QTextStream stream(&file);
	if (file.open(QIODevice::Text | QIODevice::ReadOnly)) {
		auto tmp = stream.readLine().split("\t");
		for (auto s : tmp)
			list << s.simplified();
		file.close();
	}
	return list;
}

QVariantHash AwUtilities::bids::loadTsv(const QString& tsvFile)
{
	QFile file(tsvFile);
	QTextStream stream(&file);
	QVariantHash res;
	QStringList cols;
	if (file.open(QIODevice::Text | QIODevice::ReadOnly)) {
		// get columns
		cols = stream.readLine().split("\t");
		while (!stream.atEnd()) {
			auto items = stream.readLine().split("\t");
			auto key = items.takeFirst();
			res[key] = items;
		}
		file.close();
	}
	return res;
}


///
/// get the columns label and index from the first read line of a tsv file.
/// Input is the first line read in the line.
QMap<int, QString>  AwUtilities::bids::columnsFromLine(const QString & line)
{
	QMap<int, QString> res;
	auto tokens = line.split("\t");
	int i = 0;
	for (auto token : tokens) 
		res.insert(i++, token);
	
	return res;
}


/// <summary>
/// remove a key/value bids pair from a bids filename. The goal is to reformat the filename for display role in a QStandardItemModel
/// </summary>
/// <param name="key">the key to remove (sub, ses, run, ..)</param>
/// <returns>return the string without the key/value</returns>
QString AwUtilities::bids::removeBidsKey(const QString& key, const QString& name)
{
	QString exp = QString("(%1-.+?(?=_).)").arg(key);
	QRegularExpression re(exp);
	auto res = name;
	return res.remove(re);
}