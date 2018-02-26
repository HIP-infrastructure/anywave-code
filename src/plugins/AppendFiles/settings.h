#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include "ui_settings.h"
#include <AwFileIO.h>

typedef QPair<AwFileIOPlugin *, QString> input;

class settings : public QDialog
{
	Q_OBJECT

public:
	settings(QList<AwFileIOPlugin *>& readers, QList<AwFileIOPlugin *>& writers, QWidget *parent = 0);
	~settings();

	AwFileIOPlugin *selectedWriter;
	QString outputFile;
	QList<input> inputs;
public slots:
	void accept();
protected slots:
	void changeCurrentWriter(int index);
	void addFiles();
	void changeOutputFile();
private:
	QList<AwFileIOPlugin *>& m_readers;
	QList<AwFileIOPlugin *>& m_writers;
	QList<AwFileIO *> m_tempReaders;
	QString m_inputFileFilters;
	QString m_dirPath;
	Ui::settingsUi ui;
};

#endif // SETTINGS_H
