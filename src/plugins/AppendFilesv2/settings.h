#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include "ui_settings.h"
#include <AwFileIO.h>

//typedef QPair<AwFileIOPlugin *, QString> input;
//using InputFile = QPair<QString, AwFileIO *>;

class settings : public QDialog
{
	Q_OBJECT

public:
	settings(const QList<AwFileIOPlugin *>& readers, const QStringList& writers, QWidget *parent = nullptr);
	~settings();

	//AwFileIOPlugin *selectedWriter;
	QString m_selectedWriter;
	QString outputFile;
	//QStringList m_files;
	//QList<input> inputs;
//	QList<InputFile> m_files;
	QStringList files;
	QList<QSharedPointer<AwFileIO>> readers;
	
	//QMap<QString, AwFileIO*> m_files;
public slots:
	void accept() override;
protected slots:
	void changeCurrentWriter(int index);
	void addFiles();
	void changeOutputFile();
private:
//	QList<AwFileIOPlugin *> m_readers;
	QList<AwFileIOPlugin*> m_readers;
//	QList<AwFileIOPlugin *> m_writers;
//	QList<AwFileIO *> m_tempReaders;
//	QMap<QString, AwFileIO*> m_readers;
	QStringList m_writers;
	QString m_inputFileFilters;
	QString m_dirPath;
	Ui::settingsUi ui;
};

#endif // SETTINGS_H
