#pragma once

#include <QMediaPlayer>
#include <QWidget>
#include <AwGlobal.h>
#include <AwFileIO.h>

QT_BEGIN_NAMESPACE
class QAbstractButton;
class QSlider;
class QLabel;
class QUrl;
QT_END_NAMESPACE

namespace Ui { class AwVideoPlayerUi; };

class AW_WIDGETS_EXPORT AwVideoPlayer : public QWidget
{
	Q_OBJECT
public:
	AwVideoPlayer(QWidget *parent = nullptr);
	~AwVideoPlayer();

	void setUrl(const QUrl &url);
	void setVideoSyncSettings(const AwVideoSynch& settings) { m_synchSettings = settings; }
signals:
	void videoPositionChanged(float position);
	void videoReady(bool flag);
	void changeSyncSettings();
public slots:
	void openFile();
	void play();
	void closeFile();
	void setPositionFromSignals(float position);
private slots:
	void mediaStateChanged(QMediaPlayer::State state);
	void positionChanged(qint64 position);
	void durationChanged(qint64 duration);
	void setPosition(int position);
	void handleError();

private:
	AwVideoSynch m_synchSettings;
	QMediaPlayer* m_mediaPlayer;
	Ui::AwVideoPlayerUi *m_ui;
	bool m_sendSignals;
	float m_driftInSecs;
	float m_shiftInSecs;
};
