#pragma once

#include <QDialog>
#include "ui_AwAddEditBatchDialog.h"
class AwBatchItem;

class AwAddEditBatchDialog : public QDialog
{
	Q_OBJECT

public:
	AwAddEditBatchDialog(AwBatchItem *item, QWidget *parent = Q_NULLPTR);
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
	AwBatchItem *m_item, *m_itemCopy;
	QMap<QString, QWidget *> m_widgets;	// store widgets based on keys defined in json ui
	QString m_errorString, m_homeDir;

	void setupParamsUi();
	void fetchParams();
	void setParams();
	void fetchFiles();
};
