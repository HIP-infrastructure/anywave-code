#pragma once

#include <QDialog>
#include "ui_AwAddVenvDial.h"

class AwAddVenvDial : public QDialog
{
	Q_OBJECT

public:
	AwAddVenvDial(QWidget *parent = Q_NULLPTR);
	~AwAddVenvDial();

	QString m_directory;
	QString m_alias;
public slots:
	void accept() override;
protected slots:
	void pickVenvDir();
private:
	Ui::AwAddVenvDialUi m_ui;
};
