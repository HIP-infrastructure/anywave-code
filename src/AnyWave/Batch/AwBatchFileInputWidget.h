#pragma once

#include <QWidget>
#include "ui_AwBatchFileInputWidget.h"

class AwBatchFileInputWidget : public QWidget
{
	Q_OBJECT

public:
	AwBatchFileInputWidget(bool checkAnyWaveCanOpenFiles = false, QWidget *parent = Q_NULLPTR);
	~AwBatchFileInputWidget();

	void checkFilesAndFillList(const QStringList& files, bool checkReaderPlugins = true);
	QStringList getFiles();
private slots:
	void addFiles();
	void addFilesFromDir();
	void removeFiles();
private:
	Ui::AwBatchFileInputWidgetUi m_ui;
	QString m_homeDir;
	bool m_checkForAnyWaveCompatibility;
	void recursiveFill(const QString& dirPath);
};
