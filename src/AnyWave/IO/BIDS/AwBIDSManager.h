#pragma once
#include <qobject.h>
#include "AwBIDSSubject.h"
#include "AwBIDSGUI.h"
#include <AwFileIO.h>
#include <AwCommandLine.h>
class AwFileItem;

#define AWBIDS_SOURCE_DIRS 3

// command line parsing
using AwArgument = QPair<QString, QString>;

class AwBIDSManager : public QObject
{
	Q_OBJECT
public:
	enum itemTypes { iEEG, MEG, EEG };
	enum dataSources { raw = 0, source = 1, derivatives = 2 }; // indicates the type of data ordering (source data are place in a source_data folder).
	enum Derivatives { EPITOOLS, EI, ICA, BIDSUpdates};

	// destructor
	~AwBIDSManager();
	// utilities static methods
	static AwBIDSManager *instance(const QString& rootDir = QString());
	static QString detectBIDSFolderFromPath(const QString& path);
	static bool isInstantiated() { return m_instance != NULL; }
	static void destroy();
	/** check if a path is a BIDS directory or not. **/
	static bool isBIDS(const QString& path);

	void setRootDir(const QString& path);
	inline bool isBIDSActive() { return !m_rootDir.isEmpty(); }
	void closeBIDS();
	
	int SEEGtoBIDS(const AwArguments& args);
	int convertToEDF(const QString& file, AwFileIO *reader);
	int convertToVHDR(const QString& file, AwFileIO *reader);
	// BIDS GUI Specific
	QWidget *ui() { return m_ui; }
	
	AwBIDSSubjectList& getSubjectsFromSourceDir(int sourceDir = raw);
	QString getDerivativesPath(int type, AwBIDSSubject *subject);

	// guess subject from a file
	AwBIDSSubject *guessSubject(const QString& path);
	// Access to some tsv files
	AwChannelList getMontageFromChannelsTsv(const QString& path);
	/** returns a map table: keys are the columns label **/
	QMap<QString, QStringList> loadTsvFile(const QString& path);
	/** Create a TSV file based on a dictionnary **/
	void saveTsvFile(const QString& path, const QMap<QString, QStringList>& dict);
	
protected:
	AwBIDSManager(const QString& rootDir);
	static AwBIDSManager *m_instance;

	int convertFile(AwFileIO *reader, AwFileIOPlugin *plugin, const QString& file);
	void getSubjects(int sourceDir = raw);
	void clearSubjects(int sourceDir = raw);
	AwFileItem *parseDir(const QString& fullPath, const QString& path);
	void parseSubject(AwBIDSSubject *subject);
	AwBIDSSubject *getSubject(const QString& ID, int sourceDir = raw);

	AwBIDSGUI *m_ui;
	QString m_rootDir;
	AwBIDSSubjectList m_subjects[AWBIDS_SOURCE_DIRS];
	QMap<QString, AwBIDSSubject *> m_subjectsIDs[AWBIDS_SOURCE_DIRS];
	QStringList m_fileExtensions;	// contains all file extensions that reader plugins can handle.
};