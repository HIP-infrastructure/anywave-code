#pragma once

#include <QStringList>
class AwFileItem
{
public:
	explicit AwFileItem(int type);
	enum Types { ieeg, eeg, meg };

	void setFullPath(const QString& path) { m_fullPath = path; }
protected:
	int m_type;
	QStringList m_files;	// files present in the iEEG directory
	QString m_fullPath;
};