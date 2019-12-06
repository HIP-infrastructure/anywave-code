#include "AwBIDSTools.h"

#include <qregularexpression.h>

QString AwBIDS::getSessionLabel(const QString& data)
{
	QRegularExpression re("^(?<session>ses-)(?<label>\\w+)$");
	QRegularExpressionMatch match = re.match(data);
	if (!match.hasMatch())
		return QString();
	return match.captured("label");
}

QString AwBIDS::getSubjectID(const QString& data)
{
	QRegularExpression re("^(?<subject>sub-)(?<ID>\\w+)$");
	QRegularExpressionMatch match = re.match(data);
	if (!match.hasMatch())
		return QString();
	return match.captured("ID");
}

///
/// convert enum class value to string.
/// SearchFolder string will be the relative path to the BIDS structure where to search for subjects.
QString AwBIDS::searchFoldersEnumToString(const SearchFolders& sf)
{
	switch (sf) {
	case SearchFolders::AnyWave:
		return QString("derivatives/anywave");
	case SearchFolders::EpiTools:
		return QString("derivatives/epitools");
	case SearchFolders::Source:
		return QString("sourcedata");
	default:
		return QString();
	}
}