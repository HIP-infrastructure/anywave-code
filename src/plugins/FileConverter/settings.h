// AnyWave
// Copyright (C) 2013-2021  INS UMR 1106
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include "ui_settings.h"
#include <AwFileIO.h>
#include <filter/AwFilterSettings.h>

typedef QPair<AwFileIOPlugin *, QString> input;

class settings : public QDialog
{
	Q_OBJECT

public:
	settings(QList<AwFileIOPlugin *>& readers, QList<AwFileIOPlugin *>& writers, QWidget *parent = 0);
	~settings();

	AwFileIOPlugin *selectedWriter;
	QString outputDir;
	QList<input> inputs;
	AwFilterSettings& filterSettings() { return m_filterSettings; }
public slots:
	void accept();
	int exec();
protected slots:
	void changeCurrentWriter(int index);
	void addFiles();
	void changeOutputDir();
private:
	QList<AwFileIOPlugin *>& m_readers;
	QList<AwFileIOPlugin *>& m_writers;
	QList<AwFileIO *> m_tempReaders;
	QString m_inputFileFilters;
	QString m_dirPath;
	AwFilterSettings m_filterSettings;
	Ui::settingsUi ui;
};

#endif // SETTINGS_H
