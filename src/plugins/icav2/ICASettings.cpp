#include "ICASettings.h"
#include <QMessageBox>
#include <AwCore.h>
#include <math.h>
#include <AwKeys.h>


ICASettings::ICASettings(AwProcess *process, QWidget *parent) : QDialog(parent)
{
	m_ui.setupUi(this);
	m_channels = process->pdi.input.channels();
	m_modes = AwChannel::getTypesAsString(m_channels);
	// remove unwanted modalities
	m_modes.removeAll("TRIGGER");
	m_modes.removeAll("ECG");
	m_modes.removeAll("OTHER");
	m_modes.removeAll("ICA");
	m_modes.removeAll("REFERENCE");
	m_modes.removeAll("GRAD");
	m_ui.comboModality->addItems(m_modes);

	if (!process->pdi.input.markers().isEmpty())
		m_ui.inputDataWidget->setMarkers(process->pdi.input.markers());
	else
		m_ui.inputDataWidget->hide();
	connect(m_ui.cbAll, SIGNAL(toggled(bool)), this, SLOT(updateMaxNumOfIC()));
	connect(m_ui.comboModality, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMaxNumOfIC()));
	connect(m_ui.ignoreBads, SIGNAL(toggled(bool)),  this, SLOT(updateMaxNumOfIC()));
	m_ui.labelTotalIC->hide();
	m_process = process;
}

ICASettings::~ICASettings()
{

}


void ICASettings::accept()
{
	args[keys::skip_bad_channels] = m_ui.ignoreBads->isChecked();
	args["modality"] = m_modes.at(m_ui.comboModality->currentIndex());
	int modality = AwChannel::stringToType(args.value("modality").toString());

	args[keys::lp] = m_ui.spinLPF->value();
	args[keys::hp] = m_ui.spinHPF->value();
	args["comp"] = m_ui.spinNC->value();

	QStringList usedMarkers = m_ui.inputDataWidget->usedMarkers();
	QStringList skippedMarkers = m_ui.inputDataWidget->skippedMarkers();
	if (!usedMarkers.isEmpty())
		args[keys::use_markers] = usedMarkers;
	if (!skippedMarkers.isEmpty())
		args[keys::skip_markers] = skippedMarkers;

	// get channels matching modality
	auto channels = AwChannel::getChannelsOfType(m_channels, modality);
	channels = AwChannel::removeDoublons(channels);
	if (m_ui.ignoreBads->isChecked()) { // ignoring bad channels
		auto badLabels = m_process->pdi.input.settings[keys::bad_labels].toStringList();
		foreach (AwChannel *c, channels) {
			if (badLabels.contains(c->name()))
				channels.removeAll(c);
		}
	}

	// check if num of components is <= number of channels
	if (m_ui.spinNC->value() > channels.size()) {
		QMessageBox::critical(this, "Components", "The number of components is greater than the number of available channels.");
		return;
	}

	if (m_ui.spinNC->value() < 2) {
		QMessageBox::critical(this, "Components", "Not enough components to compute.");
		return;
	}

	if (m_ui.spinLPF->value() == 0. && m_ui.spinHPF->value() == 0.)	{
		int res = QMessageBox::question(this, tr("Filters"), tr("Are you sure you want to compute ICA components on raw data (no filters)?"), 
		QMessageBox::Yes|QMessageBox::No);
		if (res == QMessageBox::No)
			return;
	}

	args["infomax_extended"] = m_ui.cbInfomaxExtended->isChecked();
	if (m_ui.checkBoxDS->isChecked())
		args["downsampling"] = true;

	m_process->pdi.input.settings.unite(args);
	return QDialog::accept();
}

void ICASettings::updateMaxNumOfIC()
{
	auto modality = AwChannel::stringToType(m_modes.at(m_ui.comboModality->currentIndex()));
	AwChannelList list = AwChannel::extractChannelsOfType(m_channels, modality);
	int count = list.size();
	auto ignoreBadChannels = m_ui.ignoreBads->isChecked();
	if (ignoreBadChannels) {
		foreach (AwChannel *c, list) {
			if (c->isBad())
				count--;
		}
	}

	m_ui.labelTotalIC->setText(QString::number(count));
	m_ui.spinNC->setValue(count);
}