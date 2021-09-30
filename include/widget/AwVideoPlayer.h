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
