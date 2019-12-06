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
#include <qobject.h>
#include "AwBIDSSubject.h"
#include "AwBIDSNode.h"
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
	enum Modifications { ChannelsTsv, EventsTsv };
	enum supportedMEGFormats { Bti4DNI, Elekta, CTF };

	// destructor
	~AwBIDSManager();
	// utilities static methods
	//static AwBIDSManager *instance(const QString& rootDir = QString());
	static AwBIDSManager *instance();
	
	static bool isInstantiated() { return m_instance != NULL; }
	static void destroy();
	/** check if a path is a BIDS directory or not. **/
	static bool isBIDS(const QString& path);
	static QString detectBIDSFolderFromPath(const QString& path);

	void newFile(AwFileIO *reader);
	void setRootDir(const QString& path);
	inline QString& rootDir() { return m_rootDir; }
	inline bool isBIDSActive() { return !m_rootDir.isEmpty(); }
	inline bool mustValidateMods() { return !m_modifications.isEmpty(); }
	void closeBIDS();
	inline AwBIDSSubject *getSubject() { return m_currentSubject; }
	
	void toBIDS(const AwArguments& args);
	int SEEGtoBIDS(const AwArguments& args);
	int MEGtoBIDS(const AwArguments& args);
	int convertToEDF(const QString& file, AwFileIO *reader);
	int convertToVHDR(const QString& file, AwFileIO *reader);

	// BIDS GUI Specific
	QWidget *ui() { return m_ui; }
	
	AwBIDSSubjectList& getSubjectsFromSourceDir(int sourceDir = raw);

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
	void updateChannelsTsv(const QString& path);
	void updateEventsTsv(const QString& path);
signals:
	void log(const QString& message);
	void BIDSClosed();
protected:
	AwBIDSManager();
	static AwBIDSManager *m_instance;
	QHash<QString, QVariant> m_settings;

	//int convert4DNI(const AwArguments& args, AwFileIO *reader, const QString& dataFile);
	int convertFile(AwFileIO *reader, AwFileIOPlugin *plugin, const QString& file);
	void getSubjects(int sourceDir = raw);
	void clearSubjects(int sourceDir = raw);
	AwFileItem *parseDir(const QString& fullPath, const QString& path);
	void parseSubject(AwBIDSSubject *subject);
	AwBIDSSubject *getSubject(const QString& ID, int sourceDir = raw);
	QString getParsingPath();
	void modifyUpdateJson(const QStringList& branches);

	QMap<int, QString> m_modifications;
	AwBIDSGUI *m_ui;
	QString m_rootDir;
	AwBIDSSubjectList m_subjects[AWBIDS_SOURCE_DIRS];
	QMap<QString, AwBIDSSubject *> m_subjectsIDs[AWBIDS_SOURCE_DIRS];
	QMap<int, QString> m_knownDerivativesPaths;
	QStringList m_fileExtensions;	// contains all file extensions that reader plugins can handle.
	// keep the subject associated with the current open file in AnyWave
	AwBIDSSubject *m_currentSubject;
	AwBIDSNodes m_nodes;
	bool m_mustValidateModifications;
};