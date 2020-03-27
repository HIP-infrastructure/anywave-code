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
	void saveProfile();
	void applyProfile();
	void browseInputDir();
	void setFileProperty();
private:
	Ui::AwAddEditBatchDialogUi m_ui;
	AwBatchModelItem *m_item;
	QMap<QString, QWidget *> m_widgets;	// store widgets based on keys defined in json ui
	QString m_errorString, m_homeDir;

	void setupParamsUi();
	void fetchParams();
	void setParams();
	void fetchFiles();
};
