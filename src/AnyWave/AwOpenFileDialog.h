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
#pragma once

#include <QFileDialog>

// this class is a QFileDialog which permits to also open a directory as a file for AnyWave.
// The directory "extension" must match one of the filters extensions extracter from the list of available reader plugins.
// When a directory name ends with a matching extension, then the Dialog will automatically accept the selection and 
// the path to the directory is sent to AnyWave plugin for opening.

class AwOpenFileDialog : public QFileDialog
{
	Q_OBJECT

public:
	AwOpenFileDialog(QWidget *parent = Q_NULLPTR, const QString& caption = QString(), const QString& directory = QString(),
		const QString& filter = QString());
private slots:
	void directoryCheck(const QString& directory);

protected:
	QStringList m_extensions;

};