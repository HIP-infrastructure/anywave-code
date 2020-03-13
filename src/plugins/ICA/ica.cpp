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
#include <AwCore.h>

// use layout manager to store a layout in the matlab file
#include <layout/AwLayoutManager.h>
#include <layout/AwLayout.h>
#include <utils/json.h>

ICA::ICA()
{
	pdi.setInputFlags(Aw::ProcessInput::GetAsRecordedChannels|Aw::ProcessInput::GetProcessPluginNames |Aw::ProcessInput::GetDurationMarkers| Aw::ProcessInput::ProcessIgnoresChannelSelection);
	pdi.addInputChannel(AwChannel::Source, 0, 0);
	m_algoNames << "Infomax";
	m_isDownsamplingActive = true;
}

ICAPlugin::ICAPlugin()
{
    type = AwProcessPlugin::Background;
    category = "ICA:ICA Extraction";
    name = QString("ICA");
    description = QString("extract independent components");
	setFlags(Aw::ProcessFlags::ProcessHasInputUi | Aw::ProcessFlags::CanRunFromCommandLine);
	m_settings[processio::json_ui] = AwUtilities::json::fromJsonFileToString(":/json/ui.json");
	m_settings[processio::json_defaults] = AwUtilities::json::fromJsonFileToString(":/json/default.json");
}

ICA::~ICA()
{
}

bool ICA::showUi()
{
	ICASettings ui(this);

	if (ui.exec() == QDialog::Accepted)	{
		//m_modality = ui.modality;
		auto args = pdi.input.args();
		args.unite(ui.args);

		QString testFile = QString("%1/MEG_1Hz_120Hz_50c_ica.mat").arg(pdi.input.settings[processio::data_dir].toString());
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


bool ICA::batchParameterCheck(const QVariantHash& hash)
{
	// this is an exhaustive test as we don't have a file open at this stage.
	// just checking for some parameters: 
	return hash.value("comp").toInt() > 1;
}

int ICA::initParameters()
{
	auto args = pdi.input.args();

	m_isDownsamplingActive = true;
	m_modality = AwChannel::stringToType(args["modality"].toString());
	m_channels = AwChannel::getChannelsOfType(pdi.input.channels(), m_modality);
	// BIDS specific:
	// when launching ICA in batch mode and specifying ieeg as modality:
	// check for SEEG channels if none present check for EEG.
	if (args.value("modality").toString().toLower() == "ieeg") {
		m_modality = AwChannel::SEEG;
		m_channels = AwChannel::getChannelsOfType(pdi.input.channels(), m_modality);
		if (m_channels.isEmpty()) {
			m_modality = AwChannel::EEG;
			m_channels = AwChannel::getChannelsOfType(pdi.input.channels(), m_modality);
		}
	}
	if (m_channels.isEmpty()) {
		sendMessage(QString("No channels of type %1 found in file").arg(AwChannel::typeToString(m_modality)));
		return -1;
	}

	m_channels = AwChannel::removeDoublons(m_channels);
	auto badLabels = pdi.input.settings[processio::bad_labels].toStringList();
	if (args.contains("skip_bad")) {
		if (!args.value("skip_bad").toBool())
			badLabels.clear();
	}
	m_samplingRate = m_channels.first()->samplingRate();
	if (args.contains("downsampling"))
		m_isDownsamplingActive = args.value("downsampling").toBool();
	// check for bad labels 
	if (!badLabels.isEmpty()) {
		foreach(AwChannel *c, m_channels)
			if (badLabels.contains(c->name()))
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

	sendMessage(QString("computing ica on file %1 and %2 channels...").arg(pdi.input.settings[processio::data_path].toString()).arg(args["modality"].toString()));

	m_lpf = args.value("lp").toDouble();
	m_hpf = args.value("hp").toDouble();
	AwFilterSettings filterSettings;
	filterSettings.set(m_channels.first()->type(), m_hpf, m_lpf, 0.);
	filterSettings.apply(m_channels);

	// check if we have to use specific markers or skipped some
	bool use = args.contains("use_markers");
	bool skip = args.contains("skip_markers");

	auto fd = pdi.input.settings[processio::file_duration].toDouble();
	if (use || skip) {
		auto markers = AwMarker::duplicate(pdi.input.markers());
		QStringList skippedMarkers, usedMarkers;
		if (use)
			usedMarkers = args.value("use_markers").toStringList();
		if (skip)
			skippedMarkers = args.value("skip_markers").toStringList();

		auto inputMarkers = AwMarker::getInputMarkers(markers, skippedMarkers, usedMarkers, fd);
		if (inputMarkers.isEmpty()) {
			pdi.input.clearMarkers();
			pdi.input.addMarker(new AwMarker("whole data", 0., fd));
		}
		else
		{
			pdi.input.clearMarkers();
			pdi.input.setNewMarkers(inputMarkers);
		}
		AW_DESTROY_LIST(markers);
	}
	else {  // no markers to use or skip => compute on the whole data
		pdi.input.clearMarkers();
		pdi.input.addMarker(new AwMarker("whole data", 0., fd));
	}


    // Watch for memory exception
	try {

		// if the skip markers were selected then input markers already represent the data to load. 
		// if not than one marker should marked the entire data in pdi.input.markers.;
		sendMessage("Loading data...");
		requestData(&m_channels, &pdi.input.markers(), true);
		sendMessage("Done");
	}
	catch (std::bad_alloc& ba) {
		sendMessage("MEMORY ALLOCATION ERROR. Operation canceled.");
		return -1;
	}

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
		sendMessage(QString("Warning: Number of samples %1 for the number of components "
			"requested %2 may be insufficient.").arg(nSamples).arg(m_nComp));
	//	return -1;
	}

	// channels have been prepared.
	// prepare data matrix
	m = m_channels.size();
	n = nSamples;

	QString dir;
	if (args.contains("output_dir"))
		dir = args.value("output_dir").toString();
	else
		dir = pdi.input.settings[processio::data_dir].toString();
	QFileInfo fi(pdi.input.settings[processio::data_path].toString());
	m_fileName = QString("%1/%2").arg(dir).arg(fi.fileName());
	QString mod = args.value("modality").toString();
	m_fileName += QString("_%1_%2Hz_%3Hz_%4c_ica.mat").arg(mod).arg(m_hpf).arg(m_lpf).arg(m_nComp);
	return 0;
}


void ICA::runFromCommandLine()
{
	if (initParameters() == 0) {
		try {
			infomax(m, n, m_nComp);
		}
		catch (std::bad_alloc& ba) {
			sendMessage("MEMORY ALLOCATION ERROR. Operation canceled.");
			return;
		}
		saveToFile();
	}
}

void ICA::run()
{
	if (initParameters() == -1)
		return;

	try {
		infomax(m, n, m_nComp);
	}
	catch (std::bad_alloc& ba) {
		sendMessage("MEMORY ALLOCATION ERROR. Operation canceled.");
		return;
	}
	if (!isAborted()) {
		saveToFile();

		// tell AnyWave to load ICA components
		QVariantList args;
		args.append(m_fileName);
		emit sendCommand(AwProcessCommand::LoadICA, args);
	}
}

void ICA::saveToFile()
{
	AwLayout *layout2D = nullptr, *layout3D = nullptr;
	int flags2D = 0, flags3D = 0;
	if (m_modality == AwChannel::EEG) {
		flags2D = AwLayout::EEG | AwLayout::L2D;
		flags3D = AwLayout::EEG | AwLayout::L3D;
	}
	if (m_modality == AwChannel::MEG) {
		flags2D = AwLayout::MEG | AwLayout::L2D;
		flags3D = AwLayout::MEG | AwLayout::L3D;
	}
	if (flags2D) {
		auto lm = AwLayoutManager::instance();
		layout2D = lm->guessLayout(pdi.input.reader(), flags2D);
	}
	if (flags3D) {
		auto lm = AwLayoutManager::instance();
		layout3D = lm->guessLayout(pdi.input.reader(), flags3D);
	}

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
		if (layout2D)
			file.writeString("layout2D", layout2D->name());
		else
			file.writeString("layout2D", "n/a");
		if (layout3D)
			file.writeString("layout3D", layout3D->name());
		else
			file.writeString("layout3D", "n/a");
	}
	catch (const AwException& e) {
		sendMessage(QString("Error saving to .mat : %1").arg(e.errorString()));
		return;
	}
	sendMessage(QString("results saved to %1.").arg(m_fileName));
}
