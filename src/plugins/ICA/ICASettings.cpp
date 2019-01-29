#include "ICASettings.h"
#include <QMessageBox>

ICASettings::ICASettings(const QString& dataPath, const AwChannelList& channels, const AwMarkerList& markers, 
						 const QStringList& algos, QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	m_ui.comboAlgo->addItems(algos);

	m_dataPath = dataPath + ".ica.mat";
	m_channels = channels;
	m_modes = AwChannel::getTypesAsString(channels);
	// remove unwanted modalities
	m_modes.removeAll("Trigger");
	m_modes.removeAll("ECG");
	m_modes.removeAll("Other");
	m_modes.removeAll("ICA");
	m_modes.removeAll("Reference");
	m_modes.removeAll("GRAD");
	m_ui.comboModality->addItems(m_modes);
	m_labels = AwMarker::getUniqueLabels(markers);
	if (m_labels.isEmpty()) {
		m_ui.comboIgnoredMarkers->setEnabled(false);
	    m_ui.checkIgnoreMarker->setEnabled(false);
	}
	else
		m_ui.comboIgnoredMarkers->addItems(m_labels);
	samplingRate = m_channels.at(0)->samplingRate(); 
	connect(m_ui.cbAll, SIGNAL(toggled(bool)), this, SLOT(updateMaxNumOfIC()));
	connect(m_ui.comboModality, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMaxNumOfIC()));
	connect(m_ui.ignoreBads, SIGNAL(toggled(bool)),  this, SLOT(updateMaxNumOfIC()));
	m_ui.labelTotalIC->hide();
	downSampling = true;
}

ICASettings::~ICASettings()
{

}

void ICASettings::accept()
{
	ignoreBadChannels = m_ui.ignoreBads->isChecked();
	ignoreMarkers = m_ui.checkIgnoreMarker->isChecked();
	if (!m_labels.isEmpty())
		selectedMarker = m_labels.at(m_ui.comboIgnoredMarkers->currentIndex());
	modality = AwChannel::stringToType(m_modes.at(m_ui.comboModality->currentIndex()));
	filePath = m_ui.lineEditFilename->text();
	lpf = m_ui.spinLPF->value();
	hpf = m_ui.spinHPF->value();
	components = m_ui.spinNC->value();
	
	if (lpf == 0. && hpf == 0.)	{
		int res = QMessageBox::question(this, tr("Filters"), tr("Are you sure you want to compute ICA components on raw data (no filters)?"), 
		QMessageBox::Yes|QMessageBox::No);
		if (res == QMessageBox::No)
			return;
	}

	algo = m_ui.comboAlgo->currentIndex();
	downSampling = !m_ui.checkBoxDS->isChecked();
	return QDialog::accept();
}

void ICASettings::updateMaxNumOfIC()
{
	modality = AwChannel::stringToType(m_modes.at(m_ui.comboModality->currentIndex()));
	AwChannelList list = AwChannel::extractChannelsOfType(m_channels, modality);
	int count = list.size();
	ignoreBadChannels = m_ui.ignoreBads->isChecked();
	if (ignoreBadChannels) {
		foreach (AwChannel *c, list) {
			if (c->isBad())
				count--;
		}
	}

	m_ui.labelTotalIC->setText(QString::number(count));
	m_ui.spinNC->setValue(count);
}