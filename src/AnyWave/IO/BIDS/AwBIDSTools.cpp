#include "AwBIDSTools.h"

#include <qregularexpression.h>

QString AwBIDSTools::getSessionLabel(const QString& data)
{
	QRegularExpression re("^(?<session>ses-)(?<label>\\w+)$");
	QRegularExpressionMatch match = re.match(data);
	if (!match.hasMatch())
		return QString();
	return match.captured("label");
}

QString AwBIDSTools::getSubjectID(const QString& data)
{
	QRegularExpression re("^(?<subject>sub-)(?<ID>\\w+)$");
	QRegularExpressionMatch match = re.match(data);
	if (!match.hasMatch())
		return QString();
	return match.captured("ID");
}