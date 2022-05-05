// AnyWave
// Copyright (C) 2013-2021  INS UMR 1106
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

constexpr auto aw_derivatives_folder = "derivatives/anywave";

namespace bids {
	constexpr auto participant_tsv = "participant_tsv";		// participants.tsv file path
	constexpr auto participant_cols = "participant_cols";	// columns found in participants.tsv
	constexpr auto parsing_path = "parsing_path";
	constexpr auto gui_extra_cols = "gui_extra_cols";
	// TSV COLUMNS
	// events
	constexpr auto tsv_event_trial_type = "trial_type";
	constexpr auto tsv_event_duration = "duration";
	constexpr auto tsv_event_onset = "onset";
	// channels
	constexpr auto tsv_channel_units = "units";
	constexpr auto tsv_channel_name = "name";
	constexpr auto tsv_channel_type = "type";
	constexpr auto tsv_channel_status = "status";
	// tracking of current open file in BIDS
	constexpr auto file_derivatives_dir = "file_derivatives_dir";
	constexpr auto current_open_filename = "current_open_filename";
	constexpr auto marker_file_path = "marker_file_path";
	constexpr auto montage_file_path = "montage_file_path";
	constexpr auto bad_file_path = "bad_file_path";
	constexpr auto common_marker_file_path = "common_marker_file_path";
	constexpr auto common_montage_file_path = "common_montage_file_path";
	constexpr auto common_bad_file_path = "common_bad_file_path";
}


class AwBIDSManager : public QObject
{
	Q_OBJECT
public:
	enum Modifications { ChannelsTsv, EventsTsv };
	enum supportedMEGFormats { Bti4DNI, Elekta, CTF };
	enum derivatives { ica, h2, gardel, freesurfer };
	// destructor
	~AwBIDSManager();
	// utilities static methods
	static AwBIDSManager *instance();
	
	static bool isInstantiated() { return m_instance != nullptr; }
	static void destroy();
	/** check if a path is a BIDS directory or not. **/
	static bool isBIDS(const QString& path);
	static QString detectBIDSFolderFromPath(const QString& path);
	
	int setNewOpenFile(const QString& path);
	void setRootDir(const QString& path);
	inline QString& rootDir() { return m_rootDir; }
	inline bool isBIDSActive() { return !m_rootDir.isEmpty(); }
	inline bool mustValidateMods() { return !m_modifications.isEmpty(); }
	QString createTsvPath(AwBIDSItem * item, int tsvType);
	void closeBIDS();
	inline QString& lastError() { return m_errorString; }
	AwBIDSItems items() { return m_items; }

	AwBIDSItem* getParentSubject(AwBIDSItem* item);
	bool isSubject(AwBIDSItem *item);
	bool isSourceDataSubject(AwBIDSItem* item);
	// 
	int selectItemFromFilePath(const QString& path);

	// command line methods
	void toBIDS(const AwArguments& args);
	int SEEGtoBIDS(const AwArguments& args);
	int MEGtoBIDS(const AwArguments& args);
	int convertToEDF(const QString& file, AwFileIO *reader, const AwMarkerList& markers);
	int convertToVHDR(const QString& file, AwFileIO *reader, const AwMarkerList& markers);
	static void initCommandLineOperation(const QString& filePath);
	static void finishCommandLineOperation();


	// BIDS GUI Specific
	QWidget *ui() { return m_ui; }
	
	// TSV files
	AwChannelList getMontageFromChannelsTsv(const QString& path);
	AwMarkerList getMarkersFromEventsTsv(const QString& path);
	AwMarkerList getMarkersTsv();
	/** montage specific **/
	/** Load a montage from channels.tsv file **/
	AwChannelList getChannelsTsvMontage();
	/** Update channels.tsv file from bad file **/
	int updateChannelsTsvBadChannels(const QStringList& badLabels);
	/** returns the columns header of a tsv file **/
	QStringList readTsvColumns(const QString& path);
	/** Get the BIDS path to the current open file **/
	QString getCurrentBIDSPath();
	QVariantMap& settings() { return m_settings; }
	inline QStringList participantValues(const QString& participantKey) { return m_participantsData.value(participantKey).toStringList(); }
	/** Create the output_dir fullpath when processing on item. **/
	QString buildOutputDir(const QString& pluginName, AwBIDSItem *item);
	/** Create the default output filename of a file item **/
	QString buildOutputFileName(AwBIDSItem * item);
	/** Get the file name from a file item build leaf **/
	QString getFileName(const QString& filePath);
	/** Get derivatives folder for current open item **/
	QString getDerivativePath(int type);
	/** Get derivative folder for a specified third party software **/
	QString getDerivativesPath(int type);
	/** GARDEL properties **/
	/** get montages gardel did generate. **/
	QStringList getGardelMontages();
	///** Get mesh file GARDEL did generate **/
	//QString getGardelMesh();
	QStringList freesurferMeshes();
	/** Get Electrode file GARDEL did generate **/
	QString getGardelElectrodes();
	/** Get the derivative folder path for a file item and a derivative modality **/
	QString getDerivativePath(AwBIDSItem *item, int type);
	/** Get the filename prefix by removing the modality **/
	QString getPrefixName(AwBIDSItem *item, bool absolutePath = false);
	/** Create derivatives path for process to store resutls **/
	QString createDerivativesPath(const QString& processName);
	/** Get the path to the user derivatives folder for the current open file item **/
	QString getUserDerivativesFolder();
	/** Get the path to the common derivatives folder for the current open file item **/
	QString getCommonDerivativesFolder();
	// dat file derivatives dir and file name
	QString currentDerivativesDir();
	QString currentFileName();
	void parseSubject(AwBIDSItem* item);
	void recursiveParsing(const QString& dirPath, AwBIDSItem* parentItem);
public slots:
	void parse(); // parse from m_rootDir and collect all found items as AwBIDSItems;
	/** Copy user .mrk sidecar files to the common folder **/
	void pushMarkerFileToCommon();
	/** Get markers from common .mrk and merge them to user .mrk file **/
	void pullFromCommonMarkerFile();
	/** Copy user .mrk sidecar files to the common folder **/
	void pushMontageFileToCommon();
	/** Get default montage from common .mtg. Replace use .mtg file **/
	void pullFromCommonMontageFile();
	/** Copy user .bad sidecar files to the common folder **/
	void pushBadFileToCommon();
	/** Get bad labels from common .bad. replace user .bad file **/
	void pullFromCommonBadFile();
signals:
	void log(const QString& message);
	void BIDSClosed();
	void finished();	// compatibily with threading operations
	void parsingProgressChanged(int progress);
protected:
	AwBIDSManager();
	AwBIDSItems getSubjectItems(const QString&);
	AwBIDSItems getSourceDataSubjectItems(const QString&);
	int convertFile(AwFileIO *reader, AwFileIOPlugin *plugin, const QString& file, const AwMarkerList& markers);
	void setDerivativesForItem(AwBIDSItem *item);
	void findCurrentFileItem(const QString& filePath);
	QVariant BIDSProperty(int property);
	void findTsvFilesForItem(AwBIDSItem *item);
	void recursiveDelete(AwBIDSItem *item); // only used when BIDS Manger runs in non gui mode
	int createEventsTsv(const QString& filePath, const AwMarkerList& markers);

	void initAnyWaveDerivativesForFile(const QString& filePath);
	void moveSidecarFilesToDerivatives();
	/** Copy .mrk .bad .mtg files from common derivatives folder to the current user derivatives folder**/
	void copyCommonSidecarFilesToUserDerivatives();

	static AwBIDSManager *m_instance;
	static QStringList m_dataFileSuffixes;  // list of suffix for known data file (_ieeg, _eeg, ...)

	QHash<QString, AwBIDSItem *> m_hashItemFiles;  // for each file found in a sub dir, store the subject node.
	QHash<int, QString> m_derivativesNames;
	AwBIDSItems m_items, m_sourcedataItems;
	// build also a map to find a subject item object by its name
	QMap<QString, AwBIDSItem*> m_mapSubjects;
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