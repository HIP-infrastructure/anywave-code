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
#ifndef AW_FILEIO_H
#define AW_FILEIO_H
#include <AwReadWriteLib.h>
#include <AwPluginBase.h>

class AwFileIOPlugin;

namespace FileIO
{
	/** Flags to activate some extra features **/

	enum AwIOFlags { CanRead = 1, CanWrite = 2, HasExtension = 4, TriggerChannelIsWritable = 8, IsDirectory = 16, HasVideoSupport = 32 };


}



class AW_RW_EXPORT AwFileIO : public QObject
{
public:
	AwFileIO(const QString& fileName = QString()) { m_flags = 0;}
	virtual ~AwFileIO() {}


	inline QString& errorMessage() { return m_error; }
	enum FileStatus { NoError, WrongFormat, FileAccess, BadHeader, WrongParameter };
	AwDataInfo infos;				

	void setPlugin(AwFileIOPlugin *plugin) { m_plugin = plugin; }
	AwFileIOPlugin *plugin() { return m_plugin;  }
	/** Returns the current active features for the reader. **/
	inline int flags() { return m_flags; }

	// Input 
	/** Override this method to open the file and fill up the data structure. **/
	virtual FileStatus openFile(const QString &path);
	/** Override this method to check if the file can be read by the reader. **/
	virtual FileStatus canRead(const QString &path) { return AwFileIO::WrongFormat;	}
	/**  Override this method to read data from the file.
	start is the position in seconds from the beginning of the file.
	duration is the amount of data to load, expressed in seconds.
	channeList is a list of channels that must be loaded. **/
	virtual qint64 readDataFromChannels(float start, float duration, AwChannelList &channelList) { return 0; }
	/** Returns a list of trigger channels present in the file. The list may be empty. **/
	virtual AwChannelList triggerChannels() { return AwChannelList(); }
	/** Get current full path to file **/
	inline QString& fullPath() { return m_fullPath; }
	void setFullPath(const QString& path) { m_fullPath = path; }
	// Output
	/** Create file for output **/
	virtual FileStatus createFile(const QString &path, int flags = 0) { return AwFileIO::NoError;	}
	/** Write or rewriter markers  **/
	virtual FileStatus writeMarkers() { return AwFileIO::NoError; }
	/** Write data depending on AwChannels list **/
	virtual qint64 writeData(AwChannelList *AwChannels) { return  0; } // must be used for continous data
																	   /** Write data corresponding to an epoch **/
	virtual qint64 writeData(AwChannelList *AwChannels, int epoch) { return 0; } // must be used for epoched data
	/** Override this method to close open file(s) and release memory if needed **/
	virtual void cleanUpAndClose() { infos.clear(); }
	/** Override this method to provide the full path to the file currently open by the plugin. **/
	virtual QString realFilePath() { return QString(); }

	/** Override this method if the plugin provides video support **/
	virtual QString videoPath() { return QString(); }
	inline bool hasVideoFile() { return !videoPath().isEmpty(); }

	/** headshape feature (MEG recordings mostly) **/
	virtual QVector<double> getHeadshapeCoordinates() { return QVector<double>(); }
	virtual QString getHeadShapeFile() { return QString(); }
	virtual bool hasHeadShapeFile() { return !getHeadShapeFile().isEmpty();	}

signals:
	void triggerValuesWritten(bool status, int number);
public slots:
	/** Override this method to write data to the Trigger channel.
	The name of the trigger channel must be specified.
	A markers list must be specified.**/
    virtual int writeTriggerChannel(const QString& name, const AwMarkerList& list) { return 0; }
	/** Override this method to clear the Trigger channel. **/
	virtual int clearTriggerChannels(const QStringList& labels) { return 0; }
protected:
	int m_flags;
	QString m_error;	// used by methods returning a status after an operation.
	QString m_fullPath;	// full path to current open file.
	AwFileIOPlugin *m_plugin;
};

///*!
// * \brief
// * Base class to implement a reader plug-in.
// * 
// * A Reader plug-in must derived from this class.
// * This is the base plug-in object which is loaded by AnyWave.
// * This object can instantiate a reader object that will become the file reader.
// * 
// * \remarks
// * A virtual method could be implemented to provide default montages to AnyWave.
// * 
// * 
// */
class AW_RW_EXPORT AwFileIOPlugin: public AwPluginBase
{
public:
	AwFileIOPlugin() { m_flags = 0x00000000; }
	QStringList fileExtensions;		///< A string list with supported file extensions: for reading.
	QString manufacturer;			///< The name of the manufacturer: OPTIONAL
	QStringList layouts;			///< Layout to use for topographies
									/** Returns flags that defines the available features **/
	QString fileExtension;			///< Extension du fichier : for writing
	inline int flags() { return m_flags; }

	/** Returns true if the file has extension */
	inline bool hasExtension() { return m_flags &  FileIO::HasExtension; }
	/** Returns true if the plugin can read a file **/
	inline bool canRead() { return m_flags & FileIO::CanRead; }
	/** Returns true if the plugin can write to a file **/
	inline bool canWrite() { return m_flags & FileIO::CanWrite; }
	/** Override this method to instantiate an object derived from AwFileReader. **/
	virtual AwFileIO *newInstance(const QString& filename = QString()) = 0;

	virtual void deleteInstance(AwFileIO *fr) { delete fr; fr = NULL; }
	/** Override this method to provide a string list containing paths to montage files. **/
	virtual QStringList montages() { return QStringList(); }
protected:
	int m_flags;
};
#define AwFileIOInterfacePlugin_IID  "AnyWave.FileIOInterfacePlugin"
Q_DECLARE_INTERFACE(AwFileIOPlugin, AwFileIOInterfacePlugin_IID)
Q_DECLARE_INTERFACE(AwFileIO, "AnyWave.FileIOInterface")

#define AW_INSTANTIATE_FILEIO_PLUGIN(P) P* newInstance(const QString& filename = QString()) { auto r = new P(filename); r->setPlugin(this); return r; }



#endif