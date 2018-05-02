#include "AwBIDSTools.h"

using namespace AwBIDSTools;
#include <qregularexpression.h>

QString& getSessionLabel(const QString& data)
{
	QRegularExpression re("^(?<session>ses-)(?<label>\\w+)$");
	QRegularExpressionMatch match = re.match(data);
	if (!match.hasMatch())
		return QString();
	return match.captured("label");
}

QString& getSubjectID(const QString& data)
{
	QRegularExpression re("^(?<subject>sub-)(?<ID>\\w+)$");
	QRegularExpressionMatch match = re.match(data);
	if (!match.hasMatch())
		return QString();
	return match.captured("ID");
}