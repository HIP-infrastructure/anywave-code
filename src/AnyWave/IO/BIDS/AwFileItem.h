#pragma once

#include <QStringList>
class AwFileItem
{
public:
	explicit AwFileItem(int type);
	enum Types { ieeg, eeg, meg };
	enum TsvFiles  { channelsTsv, eventsTsv };

	void setFullPath(const QString& path) { m_fullPath = path; }
	void setFiles(const QStringList& files) { m_files = files; }
	inline int type() { return m_type; }
	inline QStringList& files() { return m_files; }
	inline QString& fullPath() { return m_fullPath; }
	/** Returns the possibly file name for the given BIDS file for the current item **/
	QString getTsvFileFor(const QString& fileName, int tsvType);
protected:
	int m_type;
	QStringList m_files;	// files present in the iEEG directory
	QString m_fullPath;
};

typedef QList<AwFileItem *> AwFileItemList;