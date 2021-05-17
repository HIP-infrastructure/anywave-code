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
#include "AwVideoManager.h"
#include <QDebug>


// statics init and definitions
AwVideoManager *AwVideoManager::m_instance = 0;


AwVideoManager *AwVideoManager::instance()
{
	if (!m_instance)
		m_instance = new AwVideoManager;
	return m_instance;
}

bool AwVideoManager::isInstantiated()
{
	return m_instance != 0;
}

AwVideoManager::AwVideoManager()
	: QObject()
{
	m_widget = Q_NULLPTR;
}

AwVideoManager::~AwVideoManager()
{
	if (m_widget)
		delete m_widget;
}



void AwVideoManager::openVideo(const QString& path)
{
	m_player.setMedia(QUrl::fromLocalFile(path));
	if (m_widget == Q_NULLPTR) {
		m_widget = new QVideoWidget;
		m_widget->setWindowTitle("Video");
	}
	m_player.setVideoOutput(m_widget);
	m_player.play();
	m_widget->show();
	qDebug() << "mediaStatus: " << m_player.mediaStatus() << "error: " << m_player.error();
}