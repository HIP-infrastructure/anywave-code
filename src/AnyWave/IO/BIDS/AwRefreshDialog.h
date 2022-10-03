#pragma once

#include <QDialog>
#include "ui_AwRefreshDialog.h"
#include "AwBIDSItem.h"

class AwRefreshDialog : public QDialog
{
	Q_OBJECT

public:
	AwRefreshDialog(const AwBIDSItems& subjects, QWidget *parent = nullptr);
	~AwRefreshDialog();

	AwBIDSItems subjects;
public slots:
	void accept() override;
	int exec() override;
private:
	Ui::AwRefreshDialogUi m_ui;
	AwBIDSItems m_items;
};
