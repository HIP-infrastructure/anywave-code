#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include "ui_settings.h"
#include <AwFileIO.h>

class settings : public QDialog
{
	Q_OBJECT

public:
	settings(QWidget *parent = 0);
	~settings();

	QString megFile;
	QString eegFile;
	AwFileIOPlugin *megPlugin, *eegPlugin;
public slots:
	void browseMEG();
	void browseEEG();
	void accept();
private:
	Ui::settingsUi m_ui;
	bool m_megOk, m_eegOk;
	QString m_dirPath;
};

#endif // SETTINGS_H
