#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <AwFileIO.h>

namespace Ui {class SettingsUi;};

class Settings : public QDialog
{
	Q_OBJECT

public:
	Settings(QList<AwFileIOPlugin *>& readers, QWidget *parent = 0);
	~Settings();

	AwFileIOPlugin *file1Plugin, *file2Plugin;
	QString file1, file2;
	QString outputFile;
public slots:
	void accept();
	void selectFile1();
	void selectFile2();
private:
	Ui::SettingsUi *m_ui;
	QList<AwFileIOPlugin *> m_readers;
	QList<AwFileIO *> m_tempReaders;
	QString m_inputFileFilters, m_currentFolder;
	QString selectFile(const QString& folder = QString("/"));
};

#endif // SETTINGS_H
