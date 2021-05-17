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
