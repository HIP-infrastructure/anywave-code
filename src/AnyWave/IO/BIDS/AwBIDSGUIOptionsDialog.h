#pragma once

#include <QDialog>
#include <QCheckBox>
#include "ui_AwBIDSGUIOptionsDialog.h"

class AwBIDSGUIOptionsDialog : public QDialog
{
	Q_OBJECT

public:
	AwBIDSGUIOptionsDialog(const QStringList& extraColumns,QWidget *parent = Q_NULLPTR);
	~AwBIDSGUIOptionsDialog();

	inline QStringList& columns() { return m_selectecColumns; }
public slots:
	void accept() override;
private slots:
	void reset();
private:
	Ui::AwBIDSGUIOptionsDialogUi m_ui;
	QVariantMap m_settings;
	QList<QCheckBox *> m_columnsWigets;
	QStringList m_selectecColumns;
};
