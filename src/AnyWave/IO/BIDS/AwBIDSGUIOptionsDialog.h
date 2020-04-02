#pragma once

#include <QDialog>
#include <QCheckBox>
#include "ui_AwBIDSGUIOptionsDialog.h"

class AwBIDSGUIOptionsDialog : public QDialog
{
	Q_OBJECT

public:
	AwBIDSGUIOptionsDialog(QWidget *parent = Q_NULLPTR);
	~AwBIDSGUIOptionsDialog();
private slots:
	void updateSubPreview();
	void resetSubPreview();
private:
	Ui::AwBIDSGUIOptionsDialogUi m_ui;
	QVariantMap m_settings;
	QList<QCheckBox *> m_columnsWigets;
};
