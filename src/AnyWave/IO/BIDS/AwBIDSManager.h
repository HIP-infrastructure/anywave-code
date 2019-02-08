#pragma once
#include <qobject.h>
#include "AwBIDSSubject.h"
#include "AwBIDSGUI.h"
#include <AwFileIO.h>
#include <AwCommandLine.h>
class AwFileItem;

#define AWBIDS_SOURCE_DIRS 3

class AwFileIO;
// command line parsing
using AwArgument = QPair<QString, QString>;
using AwTSVDict = QMap<QString, QStringList>;

class AwBIDSManager : public QObject
{
	Q_OBJECT
public:
	enum itemTypes { iEEG, MEG, EEG };
	enum dataSources { raw = 0, source = 1, derivatives = 2 }; // indicates the type of data ordering (source data are place in a source_data folder).
	enum Derivatives { EPITOOLS, EI, ICA, BIDSUpdates};
	enum Modifications { ChannelsTsv, EventsTsv };

	// destructor
	~AwBIDSManager();
	// utilities static methods
	static AwBIDSManager *instance(const QString& rootDir = QString());
	
	static bool isInstantiated() { return m_instance != NULL; }
	static void destroy();
	/** check if a path is a BIDS directory or not. **/
	static bool isBIDS(const QString& path);
	static QString detectBIDSFolderFromPath(const QString& path);

	void newFile(AwFileIO *reader);
	void setRootDir(const QString& path);
	inline bool isBIDSActive() { return !m_rootDir.isEmpty(); }
	inline bool mustValidateMods() { return !m_modifications.isEmpty(); }
	void closeBIDS();
	inline AwBIDSSubject *getSubject() { return m_currentSubject; }
	
	int SEEGtoBIDS(const AwArguments& args);
	int convertToEDF(const QString& file, AwFileIO *reader);
	int convertToVHDR(const QString& file, AwFileIO *reader);

	/** Add a modification to the list of modification the user MUST validate before closing the current file **/
	void addModification(const QString& itemPath, int modification);
	// BIDS GUI Specific
	QWidget *ui() { return m_ui; }
	
	AwBIDSSubjectList& getSubjectsFromSourceDir(int sourceDir = raw);
	QString getDerivativesPath(int type, AwBIDSSubject *subject);

	/** guess subject from a file. Set the subject as the current subject if success. **/
	AwBIDSSubject *guessSubject(const QString& path);
	// Access to some tsv files
	AwChannelList getMontageFromChannelsTsv(const QString& path);
	AwMarkerList getMarkersFromEventsTsv(const QString& path);
	/** returns a map table: keys are the columns label **/
	AwTSVDict loadTsvFile(const QString& path);
	/** returns the columns header of a tsv file **/
	QStringList readTsvColumns(const QString& path);
	/** Create a TSV file based on a dictionnary **/
	void saveTsvFile(const QString& path, const AwTSVDict& dict, const QStringList& orderedColumns);
signals:
	void log(const QString& message);
	void BIDSClosed();
protected:
	AwBIDSManager(const QString& rootDir);
	static AwBIDSManager *m_instance;
	static QString m_parsingPath;	// path to place the json file to prevent BIDSManager of any updates.


	int convertFile(AwFileIO *reader, AwFileIOPlugin *plugin, const QString& file);
	void getSubjects(int sourceDir = raw);
	void clearSubjects(int sourceDir = raw);
	AwFileItem *parseDir(const QString& fullPath, const QString& path);
	void parseSubject(AwBIDSSubject *subject);
	AwBIDSSubject *getSubject(const QString& ID, int sourceDir = raw);
	void applyModifications();
	void updateChannelsTsv(const QString& path);
	void updateEventsTsv(const QString& path);
	QString getParsingPath();
	void modifyUpdateJson(const QStringList& branches);

	QMap<int, QString> m_modifications;
	AwBIDSGUI *m_ui;
	QString m_rootDir;
	AwBIDSSubjectList m_subjects[AWBIDS_SOURCE_DIRS];
	QMap<QString, AwBIDSSubject *> m_subjectsIDs[AWBIDS_SOURCE_DIRS];
	QStringList m_fileExtensions;	// contains all file extensions that reader plugins can handle.
	// keep the subject associated with the current open file in AnyWave
	AwBIDSSubject *m_currentSubject;
	bool m_mustValidateModifications;
};