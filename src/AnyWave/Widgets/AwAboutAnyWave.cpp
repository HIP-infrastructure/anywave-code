/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "AwAboutAnyWave.h"
#include <QSettings>
#include <QDesktopServices>
#include <QUrl>
#include <vtkVersion.h>
#include "Prefs/AwSettings.h"

AwAboutAnyWave::AwAboutAnyWave(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
	
	QSettings settings;
	QString BuildDate = settings.value("general/buildDate", "No build info").toString();
	labelQtVersion->setText(QT_VERSION_STR);
	labelBuildDate->setText(QString("Build: %1").arg(BuildDate));
	labelVtkVersion->setText(QString(VTK_VERSION));
	buttonGetLastVersion->hide();
	labelUpdateAvailable->setText("No update available.");
	auto updateUrl = AwSettings::getInstance()->getString("updateUrl");
	if (!updateUrl.isEmpty()) {
		buttonGetLastVersion->show();
		labelUpdateAvailable->setText("A new version is available");
	}
	connect(buttonGetLastVersion, SIGNAL(clicked()), this, SLOT(getLatestUpdate()));
}

void AwAboutAnyWave::openLicense()
{
	QString path = QApplication::applicationDirPath();
	QString locale = QLocale::system().name();
	if (locale == "fr")
		path += "/Licence_fr.html";
	else
		path += "/License_en.html";
	bool res = QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void AwAboutAnyWave::getLatestUpdate()
{
	QDesktopServices::openUrl(QUrl(AwSettings::getInstance()->getString("updateUrl")));
}


void AwAboutAnyWave::changeEvent(QEvent *e)
{
	if (e)
		if (e->type() == QEvent::LanguageChange)
			retranslateUi(this);
}
