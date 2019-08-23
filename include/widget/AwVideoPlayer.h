#pragma once

#include <QMediaPlayer>
#include <QWidget>
#include <AwGlobal.h>

QT_BEGIN_NAMESPACE
class QAbstractButton;
class QSlider;
class QLabel;
class QUrl;
QT_END_NAMESPACE

class AW_WIDGETS_EXPORT AwVideoPlayer : public QWidget
{
	Q_OBJECT
public:
	AwVideoPlayer(QWidget *parent = nullptr);
	~AwVideoPlayer();

	void setUrl(const QUrl &url);
signals:
	void videoPositionChanged(float position);
	void videoReady(bool flag);
public slots:
	void openFile();
	void play();
	void closeFile();

private slots:
	void mediaStateChanged(QMediaPlayer::State state);
	void positionChanged(qint64 position);
	void durationChanged(qint64 duration);
	void setPosition(int position);
	void handleError();

private:
	QMediaPlayer* m_mediaPlayer;
	QAbstractButton *m_playButton, *m_closeButton;
	QSlider *m_positionSlider;
	QLabel *m_errorLabel;
};
