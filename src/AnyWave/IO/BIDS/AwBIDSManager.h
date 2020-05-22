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
	enum derivatives { ica, h2, gardel };
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
	QString createTsvPath(AwBIDSItem * item, int tsvType);
	void closeBIDS();
	inline QString& lastError() { return m_errorString; }
	AwBIDSItems subjects() { return m_items; }
	
	// command line methods
	void toBIDS(const AwArguments& args);
	int SEEGtoBIDS(const AwArguments& args);
	int MEGtoBIDS(const AwArguments& args);
	int convertToEDF(const QString& file, AwFileIO *reader);
	int convertToVHDR(const QString& file, AwFileIO *reader);
	static void initBIDSFromCommandLineFile(const QString& filePath);

	// BIDS GUI Specific
	QWidget *ui() { return m_ui; }
	
	// TSV files
	AwChannelList getMontageFromChannelsTsv(const QString& path);
	AwMarkerList getMarkersFromEventsTsv(const QString& path);
	AwMarkerList getMarkersTsv();
	/** montage specific **/
	/** Load a montage from channels.tsv file **/
	AwChannelList getChannelsTsvMontage();
	/** Build a channels.tsv file **/
	//int createChannelsTsv(const QString& filePath, const AwChannelList& channels);
	/** Update channels.tsv file from bad file **/
	int updateChannelsTsvBadChannels(const QStringList& badLabels);
	/** markers specific **/
	int updateEventsTsv(const AwMarkerList& markers);

	/** returns a map table: keys are the columns label **/
	AwTSVDict loadTsvFile(const QString& path);
	/** returns the columns header of a tsv file **/
	QStringList readTsvColumns(const QString& path);
//	/** Create a TSV file based on a dictionnary **/
//	void saveTsvFile(const QString& path, const AwTSVDict& dict, const QStringList& orderedColumns);

	/** Get the BIDS path to the current open file **/
	QString getCurrentBIDSPath();
	QVariantMap& settings() { return m_settings; }
	inline QStringList participantValues(const QString& participantKey) { return m_participantsData.value(participantKey).toStringList(); }
	/** Create the output_dir fullpath when processing on item. **/
	QString buildOutputDir(const QString& pluginName, AwBIDSItem *item);
	/** Create the default output filename of a file item **/
	QString buildOutputFileName(AwBIDSItem * item);
	/** Get derivatives folder for current open item **/
	QString getDerivativePath(int type);
	/** GARDEL properties **/
	/** get montages gardel did generate. **/
	QStringList getGardelMontages();
	/** Get mesh file GARDEL did generate **/
	QString getGardelMesh();
	/** Get Electrode file GARDEL did generate **/
	QString getGardelElectrodes();
	/** Get the derivative folder path for a file item and a derivative modality **/
	QString getDerivativePath(AwBIDSItem *item, int type);
	/** Get the filename prefix by removing the modality **/
	QString getPrefixName(AwBIDSItem *item, bool absolutePath = false);
public slots:
	void parse(); // parse from m_rootDir and collect all found items as AwBIDSItems;
signals:
	void log(const QString& message);
	void BIDSClosed();
	void finished();	// compatibily with threading operations
protected:
	AwBIDSManager();
	void recursiveParsing(const QString& dir, AwBIDSItem *parent);
	int convertFile(AwFileIO *reader, AwFileIOPlugin *plugin, const QString& file);
	void setDerivativesForItem(AwBIDSItem *item);
	void findItem(const QString& filePath);
	QVariant gardelProperty(int property);
	void findTsvFilesForItem(AwBIDSItem *item);
	void recursiveDelete(AwBIDSItem *item); // only used when BIDS Manger runs in non gui mode

	static AwBIDSManager *m_instance;
	static QStringList m_dataFileSuffixes;  // list of suffix for known data file (_ieeg, _eeg, ...)

	QHash<QString, AwBIDSItem *> m_hashItemFiles;  // for each file found in a sub dir, store the subject node.
	QHash<int, QString> m_derivativesNames;
	AwBIDSItems m_items;
	QVariantMap m_settings;
	QList<int> m_dataContainers;
	QFileIconProvider m_fileIconProvider;
	QVariantHash m_participantsData; // hold the columns value for each participant in participants.tsvs
	QHash<AwBIDSItem *, QVariant> m_derivativesItems;

	QString getParsingPath();
	void modifyUpdateJson(const QStringList& branches);

	QMap<int, QString> m_modifications;
	AwBIDSGUI *m_ui;
	QString m_rootDir;
	QString m_errorString;
	QStringList m_fileExtensions, m_modalities;
	// keep the subject associated with the current open file in AnyWave
	AwBIDSItem *m_currentSubject;
	AwBIDSItem *m_currentOpenItem;
	bool m_guiMode; // true if AnyWave is running in normal mode with GUI. False if running in batch mode.
};