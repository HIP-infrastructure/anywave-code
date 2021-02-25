#include <widget/AwVideoPlayer.h>
#include "ui_AwVideoPlayer.h"

#include <QtWidgets>
#include <QVideoWidget>
#include <QMediaMetaData>

AwVideoPlayer::AwVideoPlayer(QWidget *parent)
	: QWidget(parent)
{
	m_ui = new Ui::AwVideoPlayerUi;
	m_ui->setupUi(this);
	m_mediaPlayer = new QMediaPlayer(this, QMediaPlayer::VideoSurface);
	connect(m_ui->buttonOpen, &QAbstractButton::clicked, this, &AwVideoPlayer::openFile);
	connect(m_ui->buttonClose, &QAbstractButton::clicked, this, &AwVideoPlayer::closeFile);
	m_ui->buttonClose->setEnabled(false);
	m_ui->buttonPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
	m_ui->buttonPlay->setEnabled(false);
	connect(m_ui->buttonPlay, &QAbstractButton::clicked, this, &AwVideoPlayer::play);
	m_ui->slider->setRange(0, 0);
	connect(m_ui->buttonSettings, &QAbstractButton::clicked, this, &AwVideoPlayer::changeSyncSettings);
	connect(m_ui->slider, &QAbstractSlider::sliderMoved, this, &AwVideoPlayer::setPosition);

	m_mediaPlayer->setVideoOutput(m_ui->widget);
	connect(m_mediaPlayer, &QMediaPlayer::stateChanged,	this, &AwVideoPlayer::mediaStateChanged);
	connect(m_mediaPlayer, &QMediaPlayer::positionChanged, this, &AwVideoPlayer::positionChanged);
	connect(m_mediaPlayer, &QMediaPlayer::durationChanged, this, &AwVideoPlayer::durationChanged);
	connect(m_mediaPlayer, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error),
		this, &AwVideoPlayer::handleError);
	m_sendSignals = true;
	m_driftInSecs = m_shiftInSecs = 0.;
}

AwVideoPlayer::~AwVideoPlayer()
{
	delete m_ui;
}

void AwVideoPlayer::closeFile()
{
	m_ui->buttonPlay->setEnabled(false);
	m_mediaPlayer->stop();
	emit videoReady(false);
	m_ui->buttonClose->setEnabled(false);
}

void AwVideoPlayer::openFile()
{
	QFileDialog fileDialog(this);
	fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
	fileDialog.setWindowTitle(tr("Open Movie"));
	QStringList supportedMimeTypes = m_mediaPlayer->supportedMimeTypes();
	if (!supportedMimeTypes.isEmpty())
		fileDialog.setMimeTypeFilters(supportedMimeTypes);
	fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath()));
	if (fileDialog.exec() == QDialog::Accepted)
		setUrl(fileDialog.selectedUrls().constFirst());
}

void AwVideoPlayer::setUrl(const QUrl &url)
{
	m_ui->label->setText(QString());
	setWindowFilePath(url.isLocalFile() ? url.toLocalFile() : QString());
	m_mediaPlayer->setMedia(url);
	m_ui->buttonPlay->setEnabled(true);
	m_ui->buttonClose->setEnabled(true);
	QSize size = m_mediaPlayer->metaData(QMediaMetaData::Resolution).toSize();
	if (!size.isNull())
		m_ui->widget->resize(size);
	emit videoReady(true);
}

void AwVideoPlayer::play()
{
	switch (m_mediaPlayer->state()) {
	case QMediaPlayer::PlayingState:
		m_mediaPlayer->pause();
		break;
	default:
		m_mediaPlayer->play();
		break;
	}
}

void AwVideoPlayer::mediaStateChanged(QMediaPlayer::State state)
{
	switch (state) {
	case QMediaPlayer::PlayingState:
		m_ui->buttonPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
		break;
	default:
		m_ui->buttonPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
		break;
	}
}

void AwVideoPlayer::positionChanged(qint64 position)
{
	m_ui->slider->setValue(position);
	auto pos = (float)position / 1000.;
	pos -= m_shiftInSecs;
	pos += m_driftInSecs;
#ifndef NDEBUG
	QString debug = QString("shift:%1ms - drift:%2ms - eeg: %3s - video: %4s").arg(m_synchSettings.shift).arg(m_synchSettings.drift) \
		.arg(pos).arg(float(position) / 1000.);
	m_ui->label->setText(debug);
#endif
	if (pos < 0.)
		pos = 0.;
	if (m_sendSignals)
		emit videoPositionChanged(pos);
}

void AwVideoPlayer::durationChanged(qint64 duration)
{
	m_ui->slider->setRange(0, duration);
	float dur = (float)duration  / 1000.;
	m_driftInSecs = (dur * (m_synchSettings.drift / 1000.)) / 3600.;
	m_shiftInSecs = m_synchSettings.drift / 1000.;
}

void AwVideoPlayer::setPosition(int position)
{
	m_mediaPlayer->setPosition(position);
}

void AwVideoPlayer::handleError()
{
	m_ui->buttonPlay->setEnabled(false);
	const QString errorString = m_mediaPlayer->errorString();
	QString message = "Error: ";
	if (errorString.isEmpty())
		message += " #" + QString::number(int(m_mediaPlayer->error()));
	else
		message += errorString;
	m_ui->label->setText(message);
	emit videoReady(false);
}


void AwVideoPlayer::setPositionFromSignals(float position)
{
	disconnect(m_ui->slider, &QAbstractSlider::sliderMoved, this, &AwVideoPlayer::setPosition);
	disconnect(m_mediaPlayer, &QMediaPlayer::positionChanged, this, &AwVideoPlayer::positionChanged);
	float pos = position + m_shiftInSecs;
	pos -= m_driftInSecs;
	m_sendSignals = false;
	setPosition((int)floor(position * 1000));
	m_ui->slider->setValue((int)floor(position * 1000));
	connect(m_ui->slider, &QAbstractSlider::sliderMoved, this, &AwVideoPlayer::setPosition);
	connect(m_mediaPlayer, &QMediaPlayer::positionChanged, this, &AwVideoPlayer::positionChanged);
	m_sendSignals = true;
}