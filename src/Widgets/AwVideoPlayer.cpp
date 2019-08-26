#include <widget/AwVideoPlayer.h>

#include <QtWidgets>
#include <QVideoWidget>

AwVideoPlayer::AwVideoPlayer(QWidget *parent)
	: QWidget(parent)
{
	m_mediaPlayer = new QMediaPlayer(this, QMediaPlayer::VideoSurface);
	QVideoWidget *videoWidget = new QVideoWidget;

	videoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	QAbstractButton *openButton = new QPushButton(tr("Open..."));
	connect(openButton, &QAbstractButton::clicked, this, &AwVideoPlayer::openFile);
	m_closeButton = new QPushButton(tr("Close"));
	connect(m_closeButton, &QAbstractButton::clicked, this, &AwVideoPlayer::closeFile);
	m_closeButton->setEnabled(false);

	m_playButton = new QPushButton;
	m_playButton->setEnabled(false);
	m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

	connect(m_playButton, &QAbstractButton::clicked,
		this, &AwVideoPlayer::play);

	m_positionSlider = new QSlider(Qt::Horizontal);
	m_positionSlider->setRange(0, 0);

	connect(m_positionSlider, &QAbstractSlider::sliderMoved,
		this, &AwVideoPlayer::setPosition);

	m_errorLabel = new QLabel;
	m_errorLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

	QBoxLayout *controlLayout = new QHBoxLayout;
	controlLayout->setMargin(0);
	controlLayout->addWidget(openButton);
	controlLayout->addWidget(m_closeButton);
	controlLayout->addWidget(m_playButton);
	controlLayout->addWidget(m_positionSlider);

	QBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(videoWidget);
	layout->addLayout(controlLayout);
	layout->addWidget(m_errorLabel);

	setLayout(layout);

	m_mediaPlayer->setVideoOutput(videoWidget);
	connect(m_mediaPlayer, &QMediaPlayer::stateChanged,
		this, &AwVideoPlayer::mediaStateChanged);
	connect(m_mediaPlayer, &QMediaPlayer::positionChanged, this, &AwVideoPlayer::positionChanged);
	connect(m_mediaPlayer, &QMediaPlayer::durationChanged, this, &AwVideoPlayer::durationChanged);
	connect(m_mediaPlayer, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error),
		this, &AwVideoPlayer::handleError);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

AwVideoPlayer::~AwVideoPlayer()
{
}

void AwVideoPlayer::closeFile()
{
	m_playButton->setEnabled(false);
	m_mediaPlayer->stop();
	emit videoReady(false);
	m_closeButton->setEnabled(false);
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
	m_errorLabel->setText(QString());
	setWindowFilePath(url.isLocalFile() ? url.toLocalFile() : QString());
	m_mediaPlayer->setMedia(url);
	m_playButton->setEnabled(true);
	m_closeButton->setEnabled(true);
	auto dateTime = m_mediaPlayer->metaData("DateTimeOriginal").toDateTime();
	m_originalTime = dateTime.time();
	repaint();
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
		m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
		break;
	default:
		m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
		break;
	}
}

void AwVideoPlayer::positionChanged(qint64 position)
{
	m_positionSlider->setValue(position);
	emit videoPositionChanged((float)position / 1000.);
}

void AwVideoPlayer::durationChanged(qint64 duration)
{
	m_positionSlider->setRange(0, duration);
}

void AwVideoPlayer::setPosition(int position)
{
	m_mediaPlayer->setPosition(position);
}

void AwVideoPlayer::handleError()
{
	m_playButton->setEnabled(false);
	const QString errorString = m_mediaPlayer->errorString();
	QString message = "Error: ";
	if (errorString.isEmpty())
		message += " #" + QString::number(int(m_mediaPlayer->error()));
	else
		message += errorString;
	m_errorLabel->setText(message);
	emit videoReady(false);
}
