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
#include "AwAboutAnyWave.h"
#include <QSettings>
#include <QDesktopServices>
#include <QUrl>
#include <vtkVersion.h>
#include "Prefs/AwSettings.h"
#include <AwVersion.h>

AwAboutAnyWave::AwAboutAnyWave(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
	
	labelQtVersion->setText(QT_VERSION_STR);
	labelBuildDate->setText(QString("version %1.%2").arg(AW_MAJOR_VERSION).arg(AW_MINOR_VERSION));
	labelVtkVersion->setText(QString(VTK_VERSION));
	buttonGetLastVersion->hide();
	labelUpdateAvailable->setText("No update available.");
	auto updateUrl = AwSettings::getInstance()->value(aws::update_url).toString();
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
	QDesktopServices::openUrl(QUrl(AwSettings::getInstance()->value(aws::update_url).toString()));
}


void AwAboutAnyWave::changeEvent(QEvent *e)
{
	if (e)
		if (e->type() == QEvent::LanguageChange)
			retranslateUi(this);
}
