#include "AwSelectTargetChannelsDial.h"
#include <QGridLayout>
#include <widget/SignalView/AwBaseSignalView.h>
#include <widget/SignalView/AwViewSettings.h>
#include "Data/AwDataManager.h"
#include "Data/AwDataServer.h"
#include <widget/AwMessageBox.h>

AwSelectTargetChannelsDial::AwSelectTargetChannelsDial(const AwSharedMarker& marker, const AwChannelList& channels, AwGraphicsScene *scene, QWidget* parent) : QDialog(parent)
{
	setWindowTitle(QString("Select channels you want to keep"));
	setWindowIcon(QIcon(":/images/AnyWave_icon.png"));
	m_marker = marker;
	m_scene = scene;
	auto grid = new QGridLayout;
	auto settings = new AwViewSettings;
	settings->showMarkers = true;
	settings->showAllChannels = true;
	settings->showFilterButton = false;
	settings->showSettingsButton = false;
	settings->markerViewOptions = AwViewSettings::HideBoth;
	m_signalView = new AwBaseSignalView(settings);
	auto dm = AwDataManager::instance();
	dm->dataServer()->openConnection(m_signalView->client());
	m_signalView->setTotalDuration(dm->totalDuration());
	m_signalView->setRecordedTime(QTime::fromString(dm->value(keys::time).toString(), Qt::TextDate));
	m_signalView->setChannels(channels);
	m_signalView->setMarkers({ marker });
	m_signalView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	grid->addWidget(m_signalView, 0, 0, 1, 3);
	auto buttonOK = new QPushButton("Validate");
	auto buttonCancel = new QPushButton("Cancel");
	grid->addWidget(buttonOK, 1, 1, Qt::AlignCenter);
	grid->addWidget(buttonCancel, 1, 2, Qt::AlignCenter);
	connect(buttonOK, &QPushButton::clicked, this, &AwSelectTargetChannelsDial::accept);
	connect(buttonCancel, &QPushButton::clicked, this, &AwSelectTargetChannelsDial::reject);
	setLayout(grid);
}

int AwSelectTargetChannelsDial::exec()
{
	m_signalView->centerViewOnPosition(m_marker->start());
	return QDialog::exec();
}

void AwSelectTargetChannelsDial::accept()
{
	selectedChannels = AwChannel::getLabels(m_signalView->selectedChannels(), true);
	if (selectedChannels.size())
		QDialog::accept();
	else
		AwMessageBox::information(this, "Channels", "Select at least one channel.");
}