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
	pdi.setInputFlags(Aw::ProcessInput::GetAsRecordedChannels|Aw::ProcessInput::GetProcessPluginNames |Aw::ProcessInput::GetDurationMarkers| Aw::ProcessInput::ProcessIgnoresChannelSelection);
	pdi.addInputChannel(AwChannel::Source, 0, 0);
	setFlags(Aw::ProcessFlags::ProcessHasInputUi | Aw::ProcessFlags::CanRunFromCommandLine);
	m_algoNames << "Infomax";
	m_isDownsamplingActive = true;
}

ICAPlugin::ICAPlugin()
{
    type = AwProcessPlugin::Background;
    category = "ICA:ICA Extraction";
    name = QString("ICA");
    description = QString("extract independent components");
}

ICA::~ICA()
{
}

bool ICA::showUi()
{
	ICASettings ui(pdi.input.dataPath, pdi.input.channels(), pdi.input.markers(), m_algoNames);

	if (ui.exec() == QDialog::Accepted)	{
		m_modality = ui.modality;
		auto args = pdi.input.args();
		args["modality"] = AwChannel::typeToString(ui.modality);

		if (ui.ignoreMarkers) {
			QStringList skippedMarkers = { ui.selectedMarker };
			args["skip_markers"] = skippedMarkers;

			AwMarkerList tmp = AwMarker::getMarkersWithLabels(pdi.input.markers(), skippedMarkers);
			auto markers = AwMarker::invertMarkerSelection(tmp, "selection", pdi.input.fileDuration);
			pdi.input.setNewMarkers(markers);
		}
		else { // set whole file as input
			pdi.input.clearMarkers();
			pdi.input.addMarker(new AwMarker("global", 0., pdi.input.fileDuration));
		}
		// NOT SKIPPING BAD CHANNELS, ok clear badLabels from input.
		if (!ui.ignoreBadChannels) {
			pdi.input.badLabels.clear();
		}

		args["comp"] = ui.components;
		args["downsampling"] = ui.downSampling;
		args["hp"] = ui.hpf;
		args["lp"] = ui.lpf;
		m_algo = ui.algo;

		QString testFile = QString("%1/MEG_1Hz_120Hz_50c_ica.mat").arg(pdi.input.dataFolder);
		QFile test(testFile);
		if (!test.open(QIODevice::WriteOnly)) {
			QMessageBox::critical(0, "Saving results", QString("Could not create %1").arg(m_fileName));
			return false;
		}
		test.close();
		QFile::remove(testFile);
		pdi.input.setArguments(args);
		return true;
	}
	return false;
}

int ICA::initParameters()
{
	auto args = pdi.input.args();

	m_isDownsamplingActive = true;
	m_modality = AwChannel::stringToType(args["modality"].toString());
	m_channels = AwChannel::getChannelsOfType(pdi.input.channels(), m_modality);
	m_channels = AwChannel::removeDoublons(m_channels);
	if (args.contains("skip_bad")) {
		if (args["skip_bas"].toBool())
			pdi.input.badLabels.clear();
	}
	m_samplingRate = m_channels.first()->samplingRate();
	if (args.contains("downsampling"))
		m_isDownsamplingActive = args["downsampling"].toBool();
	// check for bad labels 
	if (!pdi.input.badLabels.isEmpty()) {
		foreach(AwChannel *c, m_channels)
			if (pdi.input.badLabels.contains(c->name()))
				m_channels.removeAll(c);
	}
	m_nComp = m_channels.size();

	if (args.contains("comp"))
		m_nComp = args["comp"].toInt();

	if (m_nComp > m_channels.size()) {
		sendMessage("The specified number of components is greater dans the number of available channels in data. Aborted.");
		return -1;
	}

	if (m_nComp < 2) {
		sendMessage("Not enough components to compute. Aborted.");
		return -1;
	}

	sendMessage(QString("computing ica on file %1 and %2 channels...").arg(pdi.input.dataPath).arg(args["modality"].toString()));

	m_lpf = args["lp"].toDouble();
	m_hpf = args["hp"].toDouble();
	AwFilterSettings filterSettings;
	filterSettings.set(m_channels.first()->type(), m_hpf, m_lpf, 0.);
	filterSettings.apply(m_channels);

	// check if we have to use specific markers or skipped some
	bool use = args.contains("use_markers");
	bool skip = args.contains("skip_markers");
	// in the case of one option is set, the AwRunProcess method has already setup the input markers for us if we runFromCommandLine
	// If we run in GUI mode, the showUi also has already setup the input.
	// But if we run in command line mode and no use_markers or skip_markers is specified we must set the input markers to what ICA expects 
	if (!use && !skip) {
		pdi.input.clearMarkers();
		pdi.input.addMarker(new AwMarker("global", 0., pdi.input.fileDuration));
	}

	// if the skip markers were selected then input markers already represent the data to load. 
	// if not than one marker should marked the entire data in pdi.input.markers.;
	sendMessage("Loading data...");
	requestData(&m_channels, &pdi.input.markers(), true);
	sendMessage("Done");

	int decimate = 1;
	if (m_isDownsamplingActive) {
		decimate = 2;
		// compute decimate factor based on low pass filter
		if (m_lpf > 0) {
			float fc = m_lpf * 4;

			while (m_samplingRate / decimate > fc)
				decimate++;
			decimate--;
		}
		if (decimate > 1) {
			sendMessage(QString("Decimating data by a a factor of %1...").arg(decimate));
			try {
				AwFiltering::decimate(m_channels, decimate);
			}
			catch (const AwException& e)
			{
				sendMessage("Error during decimation of data. Aborted.");
				return -1;
			}
			sendMessage("Done.");
		}
	}
	else { // just filter the data
		sendMessage("Filtering...");
		AwFiltering::filter(&m_channels);
		sendMessage("Done.");
	}

	// check for nan values
	if (AwMath::isNanInChannels(m_channels)) {
		sendMessage("A Nan value was detected in the data. Computation aborted.");
		return -1;
	}

	qint64 nSamples = m_channels.first()->dataSize(); // getting total number of samples

	if (sqrt(nSamples / 30.) < m_nComp) {
		sendMessage(QString("Number of samples %1 for the number of components "
			"requested %2 may be insufficient. Aborted.").arg(nSamples).arg(m_nComp));
		return -1;
	}

	// channels have been prepared.
	// prepare data matrix
	m = m_channels.size();
	n = nSamples;


	QString dir;
	if (args.contains("output_dir"))
		dir = args["output_dir"].toString();
	else
		dir = pdi.input.dataFolder;

	QString mod = args["modality"].toString();
	m_fileName = QString("%1/%2_%3Hz_%4Hz_%5c_ica.mat").arg(dir).arg(mod).arg(m_hpf).arg(m_lpf).arg(m_nComp);
	return 0;
}


void ICA::runFromCommandLine()
{
	if (initParameters() == 0) {
		infomax(m, n, m_nComp);
		saveToFile();
	}
}

void ICA::run()
{
	if (initParameters() == -1)
		return;

	infomax(m, n, m_nComp);
	if (!isAborted()) {
		saveToFile();

		// tell AnyWave to load ICA components
		QVariantList args;
		args.append(m_fileName);
		emit sendCommand(AwProcessCommand::LoadICA, args);
	}
}


//void ICA::createInputFile()
//{
//	QString inputFile = QString("%1/%2").arg(pdi.input.dataFolder).arg("input.mat");
//
//	AwMATLABFile file;
//	if (file.create(inputFile) != 0) {
//		sendMessage("Could not create input.mat for MATLAB plugin.");
//		return;
//	}
//
//	// create the data matrix
//	mat data = AwMath::channelsToMat(m_channels);
//	if (file.writeMatrix("data", data) != 0) {
//		sendMessage("Failed to write data in input.mat.");
//		return;
//	}
//	data.clear();
//	AwChannel::clearData(m_channels);
//	// create nc (number of components)
//	file.writeScalar("nc", m_nComp);
//	file.writeString("modality", AwChannel::typeToString(m_channels.first()->type()));
//	file.writeScalar("hpf", (double)m_hpf);
//	file.writeScalar("lpf", (double)m_lpf);
//	file.writeStringCellArray("labels", AwChannel::getLabels(m_channels, true));
//	// create algo (type of algo to use)
//	switch (m_algo) {
//	case ICA::FASTICA:
//		file.writeString("algo", "FASTICA");
//		break;
//	default:
//		break;
//	}
//	//done
//}

//void ICA::launchMatlabPlugin()
//{
//	QVariantList args;
//	args << "MultiICA_MATLAB";
//	emit sendCommand(AwProcessCommand::LaunchProcess, args);
//}


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
}
