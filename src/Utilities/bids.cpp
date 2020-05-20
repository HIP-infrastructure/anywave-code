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
