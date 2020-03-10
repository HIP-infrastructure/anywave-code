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

private:
	Ui::AwAddEditBatchDialogUi m_ui;
	AwBatchModelItem *m_item;
};
