#pragma once
#include <qobject.h>
#include "AwBIDSSubject.h"
#include "AwBIDSGUI.h"
#include <AwFileIO.h>


class AwBIDSManager : public QObject
{
	Q_OBJECT
public:
	AwBIDSManager(const QString& rootDir);
	enum itemTypes { iEEG, MEG };
	// utilities static methods
	static AwBIDSManager *instance(const QString& rootDir = QString());

	void setRootDir(const QString& path);

	// generate a JSON for a file (iEEG, MEG)
	int seegToBIDS(const QString& file, const QString& subject, const QString& task, const QString &session = QString());
	int convertToEDF(const QString& file, AwFileIO *reader);
	int convertToVHDR(const QString& file, AwFileIO *reader);
	QWidget *ui() { return m_ui; }
protected:
	static AwBIDSManager *m_instance;

	int convertFile(AwFileIO *reader, AwFileIOPlugin *plugin, const QString& file);
	void getSubjects();
	void clearSubjects();
	AwBIDSGUI *m_ui;
	QString m_rootDir;
	AwBIDSSubjectList m_subjects;
};