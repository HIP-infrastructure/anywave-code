/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2020 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
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
#include "AwBIDSGUI.h"
#include <AwFileIO.h>
#include <AwCommandLine.h>
#include "AwBIDSItem.h"
#include <QFileIconProvider>

class AwFileIO;
// command line parsing
using AwArgument = QPair<QString, QString>;
using AwTSVDict = QMap<QString, QStringList>;

namespace bids {
	constexpr auto participant_tsv = "participant_tsv";		// participants.tsv file path
	constexpr auto participant_cols = "participant_cols";	// columns found in participants.tsv
	constexpr auto parsing_path = "parsing_path";
	constexpr auto gui_extra_cols = "gui_extra_cols";
}


class AwBIDSManager : public QObject
{
	Q_OBJECT
public:
	enum Modifications { ChannelsTsv, EventsTsv };
	enum supportedMEGFormats { Bti4DNI, Elekta, CTF };
	enum Derivatives { ICA };

	// destructor
	~AwBIDSManager();
	// utilities static methods
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
	AwBIDSItems subjects() { return m_items; }
	QString buildPath(const QString& dataFilePath, int derivativesKind);
	
	void toBIDS(const AwArguments& args);
	int SEEGtoBIDS(const AwArguments& args);
	int MEGtoBIDS(const AwArguments& args);
	int convertToEDF(const QString& file, AwFileIO *reader);
	int convertToVHDR(const QString& file, AwFileIO *reader);

	// BIDS GUI Specific
	QWidget *ui() { return m_ui; }
	
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
	/** try to find the subject in which the data file is stored. **/
	AwBIDSItem *findSubject(const QString& dataFilePath);
	/** Get the companion tsv file of a data file. Returns empty string if the file does not exist **/
	QString getTSVFile(const QString& dataFilePath, int tsvType);
	/** Get the BIDS path to the current open file **/
	QString getCurrentBIDSPath() { return m_settings["BIDS_FilePath"].toString(); }
	QVariantMap& settings() { return m_settings; }
	inline QStringList participantValues(const QString& participantKey) { return m_participantsData.value(participantKey).toStringList(); }
	/** Create the output_dir fullpath when processing on item. **/
	QString buildOutputDir(const QString& pluginName, AwBIDSItem *item);
public slots:
	void parse(); // parse from m_rootDir and collect all found items as AwBIDSItems;
signals:
	void log(const QString& message);
	void BIDSClosed();
	void finished();	// compatibily with threading operations
protected:
	AwBIDSManager();

	void recursiveParsing(const QString& dir, AwBIDSItem *parent);
	static AwBIDSManager *m_instance;
	static QStringList m_dataFileSuffixes;  // list of suffix for known data file (_ieeg, _eeg, ...)

	QHash<QString, AwBIDSItem *> m_hashItemFiles;  // for each file found in a sub dir, store the subject node.
	AwBIDSItems m_items;
	QVariantMap m_settings;
	QList<int> m_dataContainers;
	QFileIconProvider m_fileIconProvider;
	QVariantHash m_participantsData; // hold the columns value for each participant in participants.tsvs
	int convertFile(AwFileIO *reader, AwFileIOPlugin *plugin, const QString& file);

	QString getParsingPath();
	void modifyUpdateJson(const QStringList& branches);

	QMap<int, QString> m_modifications;
	AwBIDSGUI *m_ui;
	QString m_rootDir;
	QMap<int, QString> m_knownDerivativesPaths;
	QStringList m_fileExtensions;	// contains all file extensions that reader plugins can handle.
	// keep the subject associated with the current open file in AnyWave
	AwBIDSItem *m_currentSubject;
	bool m_mustValidateModifications;
};