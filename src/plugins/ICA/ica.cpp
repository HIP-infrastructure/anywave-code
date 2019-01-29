/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Marmaduke Woodman – Laboratoire UMR INS INSERM 1106 - michael.woodman@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "ica.h"
#include "ICASettings.h"
#include <QtMath>
#include <matlab/AwMATLAB.h>
#include <QFile>
#include <QDir>
#include <filter/AwFiltering.h>
#include <math/AwMath.h>
#include <filter/AwFilterSettings.h>
#include <QMessageBox>

ICA::ICA()
{
	pdi.addInputParameter(Aw::ProcessInput::GetAsRecordedChannels, QString("1-n"));
	pdi.addInputParameter(Aw::ProcessInput::GetProcessPluginNames , QString("1-n"));
	pdi.addInputParameter(Aw::ProcessInput::GetAllMarkers, QString("0-n"));
	pdi.addInputParameter(Aw::ProcessInput::SourceChannels|Aw::ProcessInput::ProcessIgnoresChannelSelection, QString("0-n"));
	setFlags(Aw::ProcessFlags::ProcessHasInputUi | Aw::ProcessFlags::CanRunFromCommandLine);
	m_algoNames << "Infomax";
	m_isDownsamplingActive = true;
}

ICAPlugin::ICAPlugin()
{
    type = AwProcessPlugin::Background;
    category = "ICA";
    name = QString("ICA extraction");
    description = QString("extract independent components");
}

ICA::~ICA()
{
}

bool ICA::showUi()
{
	if (pdi.input.processPluginNames.contains("MultiICA_MATLAB"))
		m_algoNames << "FastICA(MATLAB)";
	ICASettings ui(pdi.input.dataPath, pdi.input.channels, pdi.input.markers, m_algoNames);

	if (ui.exec() == QDialog::Accepted)	{
		m_modality = ui.modality;
		if (ui.ignoreMarkers)
			m_ignoredMarkerLabel = ui.selectedMarker;
		m_ignoreMarkers = ui.ignoreMarkers;
		m_ignoreBadChannels = ui.ignoreBadChannels;
		m_nComp = ui.components;
		m_isDownsamplingActive = ui.downSampling;
		m_fileName = ui.filePath;
		m_lpf = ui.lpf;
		m_hpf = ui.hpf;
		m_samplingRate = ui.samplingRate;
		m_algo = ui.algo;
		auto dateTime = QDateTime::currentDateTime();

		auto hpString = QString::number(m_hpf, 'f', 1);
		auto lpString = QString::number(m_lpf, 'f', 1);

		if (ui.filePath.isEmpty())
			// generates a file name based on filtering parameters and number of components.
			m_fileName = QString("%1_%2Hz_%3Hz_%4c_ica.mat").arg(pdi.input.dataPath).arg(hpString).arg(lpString).arg(m_nComp);
		else
			m_fileName = QString("%1/%2_ica.mat").arg(pdi.input.dataFolder).arg(ui.filePath);

		emit progressChanged(m_fileName);
		QFile test(m_fileName);
		if (!test.open(QIODevice::WriteOnly)) {
			QMessageBox::critical(0, "Saving results", QString("Could not create %1").arg(m_fileName));
			return false;
		}
		test.close();
		return true;
	}
	return false;
}


void ICA::runFromCommandLine()
{
	auto args = pdi.input.args();
	
	m_modality = AwChannel::stringToType(args["modality"].toString());
	m_channels = AwChannel::getChannelsOfType(pdi.input.channels, m_modality);
	m_channels = AwChannel::removeDoublons(m_channels);
	m_samplingRate = m_channels.first()->samplingRate();
	auto badLabels = args["bad_labels"].toStringList();
	if (!badLabels.isEmpty()) {
		foreach(AwChannel *c, m_channels)
			if (badLabels.contains(c->name()))
				m_channels.removeAll(c);
	}
	m_nComp = m_channels.size();
	if (m_nComp < 2)
		return;

	AwMarkerList selectedMarkers;
	auto skipArtefacts = args["skip"].toString();
	if (!skipArtefacts.isEmpty())
		selectedMarkers = AwMarker::invertMarkerSelection(pdi.input.markers, args["skip"].toString(), pdi.input.fileDuration);

	bool skipData = !selectedMarkers.isEmpty();

	auto lp = args["lp"].toString();
	auto hp = args["hp"].toString();
	m_lpf = m_hpf = 0.;
	if (!lp.isEmpty())
		m_lpf = lp.toDouble();
	if (!hp.isEmpty())
		m_hpf = hp.toDouble();
	AwFilterSettings filterSettings;
	filterSettings.set(m_modality, m_hpf, m_lpf, 0.);
	int decimate = 1;
	if (m_isDownsamplingActive) {
		decimate = 2;
		// compute decimate factor based on low pass filter
		if (m_lpf > 0) {
			float fc = m_lpf * 3;

			while (m_samplingRate / decimate > fc)
				decimate++;
			decimate--;
		}
	}

	if (decimate > 1) {
		sendMessage("Loading data...");
		if (skipData)
			requestData(&m_channels, &selectedMarkers, true);
		else
			requestData(&m_channels, 0.0f, -1.0f, true);
		sendMessage("Done.");
		AwChannel::clearFilters(m_channels);
		sendMessage("Decimating data...");
		AwFiltering::decimate(m_channels, decimate);
		sendMessage("Done.");
		// applying filtering options to channels
		filterSettings.apply(m_channels);
		sendMessage("Filtering...");
		AwFiltering::filter(m_channels);
		sendMessage("Done.");

	}
	else {
		AwChannel::clearFilters(m_channels);
		filterSettings.apply(m_channels);
		sendMessage("Loading data and filtering...");
		if (skipData)
			requestData(&m_channels, &selectedMarkers, true);
		else
			requestData(&m_channels, 0.0f, -1.0f, true);
		sendMessage("Done.");
	}

	// check for nan values
	if (AwMath::isNanInChannels(m_channels)) {
		sendMessage("A Nan value was detected in the data. Computation aborted.");
		return;
	}

	int nSamples = m_channels.first()->dataSize(); // getting total number of samples

	if (pow(sqrt(m_nComp * 1.0), 3.0) > nSamples) {
		sendMessage(QString("Number of samples %1 for the number of components "
			"requested %2 may be insufficient.").arg(nSamples).arg(m_nComp));
		return;
	}

	// channels have been prepared.
	// prepare data matrix
	int m = m_channels.size();
	int n = nSamples;
	int nc = m_nComp;

	auto hpString = QString::number(m_hpf, 'f', 1);
	auto lpString = QString::number(m_lpf, 'f', 1);
	m_fileName = QString("%1_%2Hz_%3Hz_%4c_ica.mat").arg(pdi.input.dataPath).arg(hpString).arg(lpString).arg(m_nComp);

	infomax(m, n, nc);
	saveToFile();
}

void ICA::run()
{
	emit progressChanged("Filtering channel types...");
	m_channels = AwChannel::getChannelsOfType(pdi.input.channels, m_modality);
	// if modality is set to SOURCE that may raise an issue as sources channels may also be present as As Recorded Channels if the data was previously exported.
	// So check for doublons on sources to avoid miscalculation.
	m_channels = AwChannel::removeDoublons(m_channels);

	// check for sampling rate for  all channels
	emit progressChanged("OK");

	if (m_ignoreBadChannels) {
		emit progressChanged("Removing bad channels...");
		foreach(AwChannel *c, m_channels)
			if (c->isBad())
				m_channels.removeAll(c);
		emit progressChanged("OK.");
	}

	// Verify number of IC vs total number of chans after removing bad ones
	if (m_nComp > m_channels.size()) {
		emit progressChanged("Adjusting number of IC to " + QString::number(m_channels.size()));
		m_nComp = m_channels.size();
	}

	if (m_channels.size() < 2) {
		emit progressChanged("Insufficient channels (" + QString::number(m_channels.size()) + ")");
		return;
	}

	emit progressChanged("Checking for sampling rate through all channels...");
	for (int i = 1; i < m_channels.size(); i++)
		if (m_channels.at(i)->samplingRate() != m_samplingRate) {
			emit progressChanged("not all channels have same sampling rate, "
				"please resample data before performing ICA.");
			return;
		}
	emit progressChanged("OK.");

	auto markersToSkip = AwMarker::getMarkersWithLabel(pdi.input.markers, m_ignoredMarkerLabel);
	AwMarkerList selectedMarkers;
	if (m_ignoreMarkers) 
		selectedMarkers = AwMarker::invertMarkerSelection(markersToSkip, m_ignoredMarkerLabel, pdi.input.fileDuration);
	
	bool skipData = !selectedMarkers.isEmpty();

	AwFilterSettings filterSettings(pdi.input.filterSettings);
	filterSettings.set(m_channels.first()->type(), m_hpf, m_lpf, 0.);
	
	int decimate = 1;
	if (m_isDownsamplingActive) {
		decimate = 2;
		// compute decimate factor based on low pass filter
		if (m_lpf > 0) {
			float fc = m_lpf * 3;

			while (m_samplingRate / decimate > fc)
				decimate++;
			decimate--;
		}
	}

	if (decimate > 1) {
		sendMessage("Loading data...");
		if (skipData)
			requestData(&m_channels, &selectedMarkers, true);
		else
			requestData(&m_channels, 0.0f, -1.0f, true);
		sendMessage("Done.");
		AwChannel::clearFilters(m_channels);
		sendMessage("Decimating data...");
		AwFiltering::decimate(m_channels, decimate);
		sendMessage("Done.");
		// applying filtering options to channels
		filterSettings.apply(m_channels);
		sendMessage("Filtering...");
		AwFiltering::filter(m_channels);
		sendMessage("Done.");

	}
	else {
		AwChannel::clearFilters(m_channels);
		filterSettings.apply(m_channels);
		sendMessage("Loading data and filtering...");
		if (skipData)
			requestData(&m_channels, &selectedMarkers, true);
		else
			requestData(&m_channels, 0.0f, -1.0f, true);
		sendMessage("Done.");
	}

	// check for nan values
	if (AwMath::isNanInChannels(m_channels)) {
		sendMessage("A Nan value was detected in the data. Computation aborted.");
		return;
	}

	int nSamples = m_channels.first()->dataSize(); // getting total number of samples

	if (pow(sqrt(m_nComp * 1.0), 3.0) > nSamples) {
		sendMessage(QString("Number of samples %1 for the number of components "
			"requested %2 may be insufficient.").arg(nSamples).arg(m_nComp));
		return;
	}

	// channels have been prepared.
	// prepare data matrix
	int m = m_channels.size();
	int n = nSamples;
	int nc = m_nComp;

	switch (m_algo)
	{
	case ICA::Infomax:
		infomax(m, n, nc);
		if (!isAborted())
			saveToFile();
		break;
	case ICA::FASTICA:
		createInputFile();
		launchMatlabPlugin();
		break;
	}
}


void ICA::createInputFile()
{
	QString inputFile = QString("%1/%2").arg(pdi.input.dataFolder).arg("input.mat");

	AwMATLABFile file;
	if (file.create(inputFile) != 0) {
		sendMessage("Could not create input.mat for MATLAB plugin.");
		return;
	}

	// create the data matrix
	mat data = AwMath::channelsToMat(m_channels);
	if (file.writeMatrix("data", data) != 0) {
		sendMessage("Failed to write data in input.mat.");
		return;
	}
	data.clear();
	AwChannel::clearData(m_channels);
	// create nc (number of components)
	file.writeScalar("nc", m_nComp);
	file.writeString("modality", AwChannel::typeToString(m_channels.first()->type()));
	file.writeScalar("hpf", (double)m_hpf);
	file.writeScalar("lpf", (double)m_lpf);
	file.writeStringCellArray("labels", AwChannel::getLabels(m_channels, true));
	// create algo (type of algo to use)
	switch (m_algo) {
	case ICA::FASTICA:
		file.writeString("algo", "FASTICA");
		break;
	default:
		break;
	}
	//done
}

void ICA::launchMatlabPlugin()
{
	QVariantList args;
	args << "MultiICA_MATLAB";
	emit sendCommand(AwProcessCommand::LaunchProcess, args);
}


void ICA::saveToFile()
{
	emit progressChanged("Saving to file...");
	AwMATLABFile file;
	try {
		file.create(m_fileName);
		file.writeString("modality", AwChannel::typeToString(m_modality));
		file.writeScalar("lpf", (double)m_lpf);
		file.writeScalar("hpf", (double)m_hpf);
		file.writeScalar("sr", (double)m_samplingRate);
		file.writeMatrix("mixing", m_mixing);
		file.writeMatrix("unmixing", m_unmixing);
		file.writeStringCellArray(QString("labels"), AwChannel::getLabels(m_channels));
	}
	catch (const AwException& e) {
		sendMessage(QString("Error saving to .mat : %1").arg(e.errorString()));
		return;
	}
	sendMessage(QString("saved results to %1.").arg(m_fileName));
	// tell AnyWave to load ICA components
	QVariantList args;
	args.append(m_fileName);
	emit sendCommand(AwProcessCommand::LoadICA, args);
}
