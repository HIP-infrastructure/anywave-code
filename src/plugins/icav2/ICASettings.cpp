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

	m_labels = AwMarker::getUniqueLabels(process->pdi.input.markers());
	if (m_labels.isEmpty()) {
		m_ui.checkIgnoreMarker->setEnabled(false);
		m_ui.comboIgnoredMarkers->setEnabled(false);
	    m_ui.checkIgnoreMarker->setEnabled(false);
		m_ui.checkUseMarkers->setEnabled(false);
	}
	else {
		m_ui.comboIgnoredMarkers->addItems(m_labels);
		m_ui.comboUseMarkers->addItems(m_labels);
	}
	connect(m_ui.cbAll, SIGNAL(toggled(bool)), this, SLOT(updateMaxNumOfIC()));
	connect(m_ui.comboModality, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMaxNumOfIC()));
	connect(m_ui.ignoreBads, SIGNAL(toggled(bool)),  this, SLOT(updateMaxNumOfIC()));
	m_ui.labelTotalIC->hide();
	//downSampling = true;
	//infomaxExtended = false;
	m_process = process;
}

ICASettings::~ICASettings()
{

}


void ICASettings::accept()
{
	args["skip_bad"] = m_ui.ignoreBads->isChecked();
	bool ignore = m_ui.checkIgnoreMarker->isChecked() && m_ui.checkIgnoreMarker->isEnabled();
	args["is_ignoring_markers"] = ignore;
	bool use = m_ui.checkUseMarkers->isChecked() && m_ui.checkUseMarkers->isEnabled();
	args["is_using_markers"] = use;
	
	args["modality"] = m_modes.at(m_ui.comboModality->currentIndex());
	if (args["is_ignoring_markers"].toBool()) 
		args[cl::skip_markers] = QStringList(m_labels.at(m_ui.comboIgnoredMarkers->currentIndex()));
	if (args["is_using_markers"].toBool())
		args[cl::use_markers] = QStringList(m_labels.at(m_ui.comboUseMarkers->currentIndex()));

	args[cl::lp] = m_ui.spinLPF->value();
	args[cl::hp] = m_ui.spinHPF->value();
	args["comp"] = m_ui.spinNC->value();

	// get channels matching modality
	auto channels = AwChannel::getChannelsOfType(m_channels, AwChannel::stringToType(args["modality"].toString()));
	channels = AwChannel::removeDoublons(channels);
	if (m_ui.ignoreBads->isChecked()) { // ignoring bad channels
		auto badLabels = m_process->pdi.input.settings[processio::bad_labels].toStringList();
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

	float dataDuration = 0.;

	// if use and ignore check that markers are different
	if (use && ignore) {
		if (m_labels.at(m_ui.comboIgnoredMarkers->currentIndex()) == m_labels.at(m_ui.comboUseMarkers->currentIndex())) {
			QMessageBox::critical(this, "Markers", "Markers to use and skip must be different.");
			return;
		}
	}

	// get the file duration
	auto fd = m_process->pdi.input.settings[processio::file_duration].toDouble();
	if (use || ignore) {
		auto markers = AwMarker::duplicate(m_process->pdi.input.markers());
		QStringList skippedMarkers = args["skip_markers"].toStringList();
		QStringList usedMarkers = args["use_markers"].toStringList();
			
		auto inputMarkers = AwMarker::getInputMarkers(markers, skippedMarkers, usedMarkers, fd);
		if (inputMarkers.isEmpty()) 
			dataDuration = fd;
		else {
			for (auto m : inputMarkers) 
				dataDuration += m->duration();
 		}
		AW_DESTROY_LIST(markers);
	}
	else 
		dataDuration = fd;

	qint64 nSamples = (qint64)floor(dataDuration * m_process->pdi.input.reader()->infos.channels().first()->samplingRate());
	if (sqrt(nSamples / 30.) < m_ui.spinNC->value()) {
		int res =QMessageBox::question(this, "Components", "The number of samples for the number of components requested may be insufficient.\nContinue?",
			QMessageBox::Yes | QMessageBox::No);
		if (res == QMessageBox::No)
			return;
	}

	if (m_ui.spinLPF->value() == 0. && m_ui.spinHPF->value() == 0.)	{
		int res = QMessageBox::question(this, tr("Filters"), tr("Are you sure you want to compute ICA components on raw data (no filters)?"), 
		QMessageBox::Yes|QMessageBox::No);
		if (res == QMessageBox::No)
			return;
	}

	//algo = m_ui.comboAlgo->currentIndex();
	args["infomax_extended"] = m_ui.cbInfomaxExtended->isChecked();
	args["downsampling"] = !m_ui.checkBoxDS->isChecked() && m_ui.spinLPF->value() > 0.;
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