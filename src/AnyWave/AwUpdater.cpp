/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
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
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "AwUpdater.h"
#include "Prefs/AwSettings.h"
#include <widget/AwMessageBox.h>
#include <QFileInfo>
#include <QTemporaryDir>
#include <widget/AwWaitWidget.h>
#include <QProcess>
#include <QStandardPaths>
#ifdef Q_OS_WIN
#include "zip.h"
#endif
#include <AwVersion.h>
//////////////////////////////////////////////////////////////////////////////////////////
/// Downloader

AwDownloader::AwDownloader(QObject *parent) : QObject(parent)
{
	connect(&m_manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(downloadFinished(QNetworkReply *)));
}

void AwDownloader::error(const QString& message)
{
	AwMessageBox::information(0, "Update error", message);
}

void AwDownloader::download()
{
	QUrl url;
	bool isINSVersion = AwSettings::getInstance()->value(aws::ins_version).toBool();
	QString baseURL;
	if (isINSVersion)
		baseURL = QString("http://139.124.150.47/AnyWave");
	else
		baseURL = QString("https://meg.univ-amu.fr/AnyWave");

#ifdef Q_OS_WIN
	url = QUrl(QString("%1/AnyWave_win64.zip").arg(baseURL));
#endif

#ifdef Q_OS_MAC
	url = QUrl(QString("%1/AnyWave.zip").arg(baseURL));
#endif
#ifdef Q_OS_LINUX
	url = QUrl(QString("%1/install_AnyWave.run").arg(baseURL));
#endif

	QNetworkRequest request(url);
	m_manager.get(request);
	AwWaitWidget wait("Downloading update...");
	connect(&m_manager, SIGNAL(finished(QNetworkReply *)), &wait, SLOT(accept()));
	wait.exec();
}

void AwDownloader::downloadFinished(QNetworkReply *reply)
{
	QUrl url = reply->url();
	QString basename;
	if (reply->error()) {
		error(QString("Download of %1 failed: %2").arg(url.toEncoded().constData()).arg(reply->errorString()));
		return;
	} 
	QString path = url.path();
	basename = QFileInfo(path).fileName();
	if (basename.isEmpty()) {
		error(QString("no basename for file."));
		return;
	}

	// save to temp folder.
	auto downloadPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
	QTemporaryDir dir(QString("%1/AnyWave").arg(downloadPath));
	if (!dir.isValid()) {
		error(QString("Could not create a temporary folder to save the update."));
		return;
	}
	QString fullPath = QString("%1/%2").arg(dir.path()).arg(basename);
	QFile file(fullPath);
	if (!file.open(QIODevice::WriteOnly)) {
		error(QString("Failed to create the file %1").arg(fullPath));
		return;
	}
	file.write(reply->readAll());
	file.close();
	dir.setAutoRemove(false);
#ifdef Q_OS_WIN
	QString exePath = QString("%1/setup.exe").arg(dir.path());
	int err;
	struct zip_stat sb;
	auto za = zip_open(fullPath.toStdString().c_str(), ZIP_RDONLY, &err);
	if (za == NULL) {
		char buf[100];
		error(QString("can't open archive file: %1").arg(QString(zip_error_to_str(buf, sizeof(buf), err, errno))));
		return;
	}
	// get index of setup.exe in archive (should be 0)
	auto index = zip_name_locate(za, "setup.exe", ZIP_FL_NOCASE);
	if (index == -1) {
		error(QString("failed to find setup.exe file in the zip file."));
		zip_close(za);
		return;
	}
	// get stats
	if (zip_stat_index(za, index, 0, &sb) != 0) {
		error(QString("failed to get status information for setup.exe file."));
		zip_close(za);
		return;
	}
	// open setup.exe file in zip
	auto zf = zip_fopen_index(za, index, 0);
	if (!zf) {
		error(QString("Failed to open setup.exe file in archive."));
		zip_close(za);
		return;
	}

	QFile outputFile(exePath);
	if (!outputFile.open(QIODevice::WriteOnly)) { 
		error(QString("Failed to create outputfile %1").arg(exePath));
		zip_close(za);
		return;
	}

	quint64 sum = 0;
	int len;
	char *buf = new char[1000000];
	while (sum != sb.size) {
		len = zip_fread(zf, buf, 1000000);
		if (len < 0) {
			delete[] buf;
			error(QString("error reading data from archive file."));
			zip_close(za);
			return;
		}
		outputFile.write(buf, len); 
		sum += len;
	}
	delete[] buf;
	outputFile.close();
	zip_fclose(zf);
	zip_close(za);
	QProcess::startDetached(exePath);
	exit(0);
#endif
#ifdef Q_OS_MAC
	QProcess::execute("open", QStringList(fullPath));
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////
/// Updater


AwUpdater::AwUpdater(QObject *parent) : QObject(parent)
{
	connect(&m_webAccess, SIGNAL(finished(QNetworkReply *)), this, SLOT(handleResult(QNetworkReply *)));
}

void AwUpdater::checkForUpdate()
{
	QString platform; 
	auto aws = AwSettings::getInstance();

	if (!aws->value(aws::check_updates).toBool())
		return;
	auto major = AW_MAJOR_VERSION;
	auto minor = AW_MINOR_VERSION;
	bool isINSVersion = aws->value(aws::ins_version).toBool();

#ifdef Q_OS_MACOS
	platform = "Mac";
#endif
#ifdef Q_OS_WIN
	platform = "Win";
#endif
#ifdef Q_OS_LINUX
	platform = "Linux";
#endif 
	QUrl url;
	if (!isINSVersion)
		url = QUrl(QString("https://meg.univ-amu.fr/AnyWave/AnyWave_version.php?platform=%1&version=%2&minor=%3").arg(platform).arg(major).arg(minor));
	else
		url = QUrl(QString("http://139.124.150.47/AnyWave/AnyWave_version.php?platform=%1&version=%2&minor=%3").arg(platform).arg(major).arg(minor));
	QNetworkRequest request(url);
	m_webAccess.get(request);

}

void AwUpdater::handleResult(QNetworkReply *reply)
{
	auto aws = AwSettings::getInstance();
	QByteArray data = reply->readAll();
	QString response = QString(data);
	if (response.toLower() != "no update") {
		url = response;
		aws->setValue(aws::update_url, response);
		// Notify user that the process has finished normally
		QSystemTrayIcon *sysTray = AwSettings::getInstance()->sysTray();
		sysTray->show();
		sysTray->showMessage("AnyWave update", QString("A new version is available at %1").arg(response), QSystemTrayIcon::Information, 200000);

		if (QMessageBox::question(0, tr("Update"), tr("Would you like to download the newer version of AnyWave?"), QMessageBox::Yes | QMessageBox::No) ==
			QMessageBox::Yes) {
			m_downloader.download();
		}
	}
}
