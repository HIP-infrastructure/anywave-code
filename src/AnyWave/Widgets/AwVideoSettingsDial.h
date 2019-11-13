#pragma once

#include <QDialog>
#include "ui_AwVideoSettingsDial.h"

class AwVideoSettingsDial : public QDialog
{
	Q_OBJECT

public:
	AwVideoSettingsDial(QWidget *parent = Q_NULLPTR);
	~AwVideoSettingsDial();
public slots:
	void accept() override;
public:
	int shift, drift;
private:
	Ui::AwVideoSettingsDialUi m_ui;
};
