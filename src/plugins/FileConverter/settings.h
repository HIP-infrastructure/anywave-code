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
