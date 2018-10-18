/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
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