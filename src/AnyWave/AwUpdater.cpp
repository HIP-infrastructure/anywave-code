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
#include "AwUpdater.h"
#include "Prefs/AwSettings.h"

AwUpdater::AwUpdater(QObject *parent) : QObject(parent)
{
	connect(&m_webAccess, SIGNAL(finished(QNetworkReply *)), this, SLOT(handleResult(QNetworkReply *)));
}

void AwUpdater::checkForUpdate()
{
	// now using macro that must be defines at compile time
#if !defined(AW_PLATFORM) && !defined(AW_MAJOR_VERSION) && !defined(AW_MINOR_VERSION)
	return;
#else
	QString platform = QString(AW_PLATFORM), version = QString(AW_MAJOR_VERSION), minor = QString(AW_MINOR_VERSION);

//#ifdef Q_OS_MACOS
//	platform = "Mac";
//	version = "1807";
//	minor = "0";
//#endif
//#ifdef Q_OS_WIN
//	platform = "Win";
//	version = "1807";
//	minor = "0";
//#endif
//#ifdef Q_OS_LINUX
//	platform = "Linux";
//	version = "1807";
//	minor = "0";
//#endif 

	QUrl url = QUrl(QString("http://meg.univ-amu.fr/AnyWave/AnyWave_version.php?platform=%1&version=%2&minor=%3").arg(platform).arg(version).arg(minor));
	QNetworkRequest request(url);
	m_webAccess.get(request);
#endif
}

void AwUpdater::handleResult(QNetworkReply *reply)
{
	QByteArray data = reply->readAll();
	QString response = QString(data);
	if (response.toLower() != "no update") {
		url = response;
		AwSettings::getInstance()->updateUrl = response;
		// Notify user that the process has finished normally
		QSystemTrayIcon *sysTray = AwSettings::getInstance()->sysTray();
		sysTray->show();
		sysTray->showMessage("AnyWave updates", QString("A new version is available at %1").arg(response), QSystemTrayIcon::Information, 200000);
	}
}
