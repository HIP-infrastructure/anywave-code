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
