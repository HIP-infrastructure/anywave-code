#pragma once

#include <QDialog>
#include "ui_AwAddEditBatchDialog.h"
class AwBatchModelItem;

class AwAddEditBatchDialog : public QDialog
{
	Q_OBJECT

public:
	AwAddEditBatchDialog(AwBatchModelItem *item, QWidget *parent = Q_NULLPTR);
	~AwAddEditBatchDialog();
public slots:
	void accept() override;
private slots:
	void addFiles();
	void addFilesFromDir();
	void removeFiles();
	void saveProfile();
	void applyProfile();
	void browseInputDir();
private:
	Ui::AwAddEditBatchDialogUi m_ui;
	AwBatchModelItem *m_item;
	QMap<QString, QWidget *> m_widgets;	// store widgets based on keys defined in json ui
	QVariantMap m_jsonUi;
	QVariantHash m_jsonDefaults;
	QString m_errorString;
	QString m_homeDir;

	void setupParamsUi();
	void fetchParams();
	void setParams();
	void fetchFiles();
	void checkFilesAndFillList(const QStringList& files, bool warning = true, bool checkReaderPlugins = true);
	void recursiveFill(const QString& dirPath);
	bool pluginParamCheck();
};
