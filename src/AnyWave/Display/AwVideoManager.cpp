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