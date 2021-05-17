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
#ifndef AW_UPDATER_H
#define AW_UPDATER_H
#include <QObject>
#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>

class AwDownloader : public QObject
{
	Q_OBJECT
public:
	AwDownloader(QObject *parent = 0);
	void download();
protected slots:
	void downloadFinished(QNetworkReply *reply);
protected:
	void error(const QString& error);
 
 
	QNetworkAccessManager m_manager;

};


class AwUpdater : public QObject
{
	Q_OBJECT
public:
	AwUpdater(QObject *parent = 0);
	void checkForUpdate();

	QString url;
protected slots:
	void handleResult(QNetworkReply *reply);
protected:
	 QNetworkAccessManager m_webAccess;
	 AwDownloader m_downloader;
};


#endif