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
#include <AwMATLAB.h>
#include <QFile>
#include <QDir>
#include <filter/AwFiltering.h>
#include <math/AwMath.h>
#include <filter/AwFilterSettings.h>

ICA::ICA()
{
	pdi.addInputParameter(Aw::ProcessInput::GetAsRecordedChannels, QString("1-n"));
	pdi.addInputParameter(Aw::ProcessInput::GetProcessPluginNames , QString("1-n"));
	pdi.addInputParameter(Aw::ProcessInput::GetAllMarkers, QString("0-n"));
	pdi.addInputParameter(Aw::ProcessInput::SourceChannels|Aw::ProcessInput::ProcessIgnoresChannelSelection, QString("0-n"));
	setFlags(Aw::ProcessFlags::ProcessHasInputUi);
	m_algoNames << "Infomax";
}

ICAPlugin::ICAPlugin()
{
    type = AwProcessPlugin::Background;
    category = "ICA";
    name = QString("ICA extraction");
    description = QString("Compute independent components of selected signals");
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
		m_decimateFactor = ui.decimateFactor;
		m_fileName = ui.filePath;
		m_lpf = ui.lpf;
		m_hpf = ui.hpf;
		m_samplingRate = ui.samplingRate;
		m_algo = ui.algo;
		return true;
	}
	return false;
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

	AwMarkerList selectedMarkers;
	if (m_ignoreMarkers) 
		selectedMarkers = AwMarker::invertMarkerSelection(pdi.input.markers, m_ignoredMarkerLabel, pdi.input.fileDuration);
	
	bool skipData = !selectedMarkers.isEmpty();

	AwFilterSettings filterSettings(pdi.input.filterSettings);
	filterSettings.set(m_channels.first()->type(), m_hpf, m_lpf, 0.);
	
	// compute decimate factor based on low pass filter
	int decimate = 2;
	if (m_lpf > 0) {
		float fc = m_lpf * 3;
		
		while ( m_samplingRate / decimate > fc)
			decimate++;
		decimate--;
	}
	else 
		decimate = 1;  // no decimation

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
		sendMessage("Loading data and filtering..");
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