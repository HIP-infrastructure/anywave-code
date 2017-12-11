#pragma once

#include <QDialog>
#include "ui_Settings.h"

class Settings : public QDialog
{
	Q_OBJECT

public:
	Settings(const QStringList& labels, QWidget *parent = Q_NULLPTR);
	~Settings();

	QString label;
public slots:
	void accept();
private:
	Ui::SettingsUi m_ui;
};
