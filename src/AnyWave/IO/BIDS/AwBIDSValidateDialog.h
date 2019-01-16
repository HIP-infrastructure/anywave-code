#pragma once

#include <QDialog>
#include "ui_AwBIDSValidateDialog.h"

class AwBIDSValidateDialog : public QDialog
{
	Q_OBJECT

public:
	AwBIDSValidateDialog(const QMap<int, QString>& mods, QWidget *parent = Q_NULLPTR);
	~AwBIDSValidateDialog();

private:
	Ui::AwBIDSValidateDialogUi m_ui;
};
