#pragma once
#include <qobject.h>
#include "AwBIDSSubject.h"
#include <AwFileIO.h>


class AwBIDSManager : public QObject
{
	Q_OBJECT
public:
	AwBIDSManager(const QString& rootDir);
	enum itemTypes { iEEG, MEG };
	static AwBIDSManager *instance(const QString& rootDir = QString());

	// utilities static methods

	// generate a JSON for a file (iEEG, MEG)
	int seegToBIDS(const QString& file, const QString& subject, const QString& task, const QString &session = QString());
	int converToEDF(const QString& file, AwFileIO *reader);
protected:
	static AwBIDSManager *m_instance;

	QString m_rootDir;
	AwBIDSSubjectList m_subjects;
};