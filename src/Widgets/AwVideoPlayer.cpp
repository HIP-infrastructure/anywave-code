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
	QVideoWidget *videoWidget = new QVideoWidget;
	m_ui->verticalLayout->replaceWidget(m_ui->widget, videoWidget);
	delete m_ui->widget;
	m_ui->widget = videoWidget;
	connect(m_ui->buttonOpen, &QAbstractButton::clicked, this, &AwVideoPlayer::openFile);
	connect(m_ui->buttonClose, &QAbstractButton::clicked, this, &AwVideoPlayer::closeFile);
	m_ui->buttonClose->setEnabled(false);
	m_ui->buttonPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
	m_ui->buttonPlay->setEnabled(false);
	connect(m_ui->buttonPlay, &QAbstractButton::clicked, this, &AwVideoPlayer::play);
	m_ui->slider->setRange(0, 0);

	connect(m_ui->slider, &QAbstractSlider::sliderMoved, this, &AwVideoPlayer::setPosition);

	m_mediaPlayer->setVideoOutput(videoWidget);
	connect(m_mediaPlayer, &QMediaPlayer::stateChanged,	this, &AwVideoPlayer::mediaStateChanged);
	connect(m_mediaPlayer, &QMediaPlayer::positionChanged, this, &AwVideoPlayer::positionChanged);
	connect(m_mediaPlayer, &QMediaPlayer::durationChanged, this, &AwVideoPlayer::durationChanged);
	connect(m_mediaPlayer, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error),
		this, &AwVideoPlayer::handleError);

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
	pos -= m_synchSettings.shift / 1000.;
	float duration = m_mediaPlayer->duration() / 1000.;
	float drift = (duration * (m_synchSettings.drift / 1000.)) / 3600.;
	pos += drift;
	if (pos < 0.)
		pos = 0.;
	emit videoPositionChanged(pos);
}

void AwVideoPlayer::durationChanged(qint64 duration)
{
	m_ui->slider->setRange(0, duration);
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
