#pragma once

#include <QDialog>
#include "ui_AwBatchDialog.h"

class AwProcessPlugin;

class AwBatchDialog : public QDialog
{
	Q_OBJECT

public:
	AwBatchDialog(QWidget *parent = Q_NULLPTR);
	~AwBatchDialog();

private:
	Ui::AwBatchDialogUi m_ui;
	QMap<QString, AwProcessPlugin *> m_plugins;
};
