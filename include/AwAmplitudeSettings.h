#pragma once

#include <AwGlobal.h>
#include <QObject>

/// This  object is designed to hold the amplitudes settings for the current montage/data file

class AW_CORE_EXPORT AwAmplitudeSettings : public QObject
{
	Q_OBJECT

public:
	AwAmplitudeSettings(QObject *parent);
	~AwAmplitudeSettings();

	void newFile(const QString& dataFile);
	void closeFile();
	void quit();
protected:
	void load();
	void load(const QString& file);
	void save();
	void save(const QString& file);

};
