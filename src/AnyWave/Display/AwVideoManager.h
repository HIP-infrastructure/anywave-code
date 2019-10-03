#pragma once

#include <QObject>
#include <QVideoWidget>
#include <QMediaPlayer>

class AwVideoManager : public QObject
{
	Q_OBJECT
public:
	static AwVideoManager *instance();
	static bool isInstantiated();
	~AwVideoManager();
public slots:
	void openVideo(const QString& path);
protected:
	AwVideoManager();

private:
	static AwVideoManager *m_instance;
	QVideoWidget *m_widget;
	QMediaPlayer m_player;

};
