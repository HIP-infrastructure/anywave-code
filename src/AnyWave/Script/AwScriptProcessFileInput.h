/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef AWSCRIPTPROCESSFILEINPUT_H
#define AWSCRIPTPROCESSFILEINPUT_H

#include <QObject>
#include <QStringList>
#include <filter/AwFilterSettings.h>

class AwProcessFileInputElement
{
public:
	AwProcessFileInputElement(const QString& dataPath);
	inline QString& dataPath() { return m_dataPath; }
	inline QString& montagePath() { return m_montagePath; }
	inline QString& markerPath() { return m_markerPath; }
	inline QString& markerFile() { return m_markerFile; }

	inline void setMarkerPath(const QString& path) { m_markerPath = path; }
	inline void setMontagePath(const QString& path) { m_montagePath = path; }
	inline void setMarkerFile(const QString& file) { m_markerFile = file;  m_markerPath.clear(); }
private:
	QString m_dataPath, m_montagePath, m_markerPath, m_markerFile;
};

typedef AwProcessFileInputElement AwPFIElement;
typedef QList<AwPFIElement *> AwPIElements;

class AwScriptProcessFileInput : public QObject
{
	Q_OBJECT
	Q_PROPERTY(bool useMarkerFile READ useMarkerFile WRITE setMarkerFile)
	Q_PROPERTY(bool useMontageFile READ useMontageFile WRITE setMontageFile)
	Q_PROPERTY(QStringList fileExtensions READ fileExtensions)
	Q_PROPERTY(QStringList useMarkers READ getUsedMarkers WRITE setUsedMarkers)
	Q_PROPERTY(QStringList skipMarkers READ getSkippedMarkers WRITE setSkippedMarkers)
public:
	AwScriptProcessFileInput(QObject *parent);
	~AwScriptProcessFileInput();

	bool useMarkerFile() { return m_useMarkerFile; }
	void setMarkerFile(bool f) { m_useMarkerFile = f; }

	bool useMontageFile() { return m_useMontageFile; }
	void setMontageFile(bool f) { m_useMontageFile = f; }

	QStringList fileExtensions() { return m_fileExtensions; }
	QStringList getUsedMarkers() { return m_usedMarkers; }
	QStringList getSkippedMarkers() { return m_skippedMarkers; }
	void setUsedMarkers(const QStringList& markers) { m_usedMarkers = markers; }
	void setSkippedMarkers(const QStringList& markers) { m_skippedMarkers = markers; }
	inline AwPIElements& inputElements() { return m_inputElements; }
	void parseFiles();	// build the internal data structure based on added files.
	inline QStringList& usedMarkers() { return m_usedMarkers; }
	inline QStringList& skippedMarkers() { return m_skippedMarkers; }
	inline AwFilterSettings& filterSettings() { return m_filterSettings; }
public slots:
	void addFileExtension(QString ext) { m_fileExtensions << ext; }
	void setRootDir(QString root);
	void addFiles(QString path, QString markerPath = QString(), QString montagePath = QString());
	void addDirPath(QString dirPath);
	void readInputFromFile(QString path);
	void setFilters(QString channelType, float lp, float hp);
	void downSample(float samplingRate);
	void setGlobalMontageFile(QString montagePath);
	/** Set the marker file to use. This file must be present in every subfolders containing data when used with the setRootDir feature. **/
	void setMarkerFile(QString markerFile);
	void clear();	/** clear input files **/
private:
	AwPIElements m_inputElements;
	QStringList m_fileExtensions;	// set the extensions of files to be set as input
	bool m_useMarkerFile;	// if true, marker files found in the same path will be used.
	bool m_useMontageFile;	// if true, montage file found in the same path will be used.
	AwFilterSettings m_filterSettings;
	float m_downSampleRate;	// if this value is greater then 0. then try to downsample the data before any further processing.
	QString m_globalMontage;	// path to a montage file to use for all the input data files. (If empty, other options will be used (useMontageFile)
	QString m_markerFile;	// name of the marker file to use (not a path)
	QStringList m_files, m_markerFiles, m_montageFiles;	// collection of data files set as input along with optional marker and montage files.
	QStringList m_usedMarkers, m_skippedMarkers;
};


#endif // AWSCRIPTPROCESSFILEINPUT_H
