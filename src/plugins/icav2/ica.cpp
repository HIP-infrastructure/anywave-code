// AnyWave
// Copyright (C) 2013-2021  INS UMR 1106
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
#include <AwEvent.h>

// use layout manager to store a layout in the matlab file
#include <layout/AwLayoutManager.h>
#include <layout/AwLayout.h>
#include <utils/json.h>
#include <AwKeys.h>
#include "ICAInfomax.h"

namespace algos {
	constexpr auto ICA_infomax = 0;
	constexpr auto ICA_cca = 1;
	constexpr auto ICA_sobi = 2;
}

ICA::ICA()
{
	setInputFlags(Aw::ProcessIO::GetAsRecordedChannels | Aw::ProcessIO::DontSkipBadChannels |
		Aw::ProcessIO::GetProcessPluginNames |Aw::ProcessIO::GetDurationMarkers);
	setInputModifiers(Aw::ProcessIO::modifiers::IgnoreChannelSelection);
	pdi.addInputChannel(-1, 1, 0);
	pdi.addInputChannel(AwChannel::Source, 0, 0);
	m_isDownsamplingActive = true;
	m_hpf = m_lpf = 0.;
	m_nComp = 0;
	m_algorithms << QSharedPointer<ICAAlgorithm>(new ICAInfomax);
}

ICAPlugin::ICAPlugin() : AwProcessPlugin()
{
    type = AwProcessPlugin::Background;
    category = "ICA:ICA Extraction";
	version = "2.1.0";
    name = QString("ICA");
    description = QString("Compute ICA");
	setFlags(Aw::ProcessFlags::ProcessHasInputUi | Aw::ProcessFlags::CanRunFromCommandLine);	
	m_settings[keys::json_batch] = AwUtilities::json::fromJsonFileToString(":/ica/json/batch.json");
	m_helpUrl = "ICA::https://gitlab-dynamap.timone.univ-amu.fr/anywave/anywave/-/wikis/plugin_ica";
}

ICA::~ICA()
{
}

bool ICA::showUi()
{
	ICASettings ui(this);

	if (ui.exec() == QDialog::Accepted)	{
		auto args = pdi.input.settings;
		QString dir = pdi.input.settings.value(keys::output_dir).toString();
		// just testing to write a file in the output_dir 
		// m_filename is wrong here
		m_fileName = QString("%1/MEG_1Hz_120Hz_50c_ica.mat").arg(dir);
		QFile test(m_fileName);
		if (!test.open(QIODevice::WriteOnly)) {
			QMessageBox::critical(0, "Saving results", QString("Could not create %1").arg(m_fileName));
			return false;
		}
		test.close();
		QFile::remove(m_fileName);
		//pdi.input.settings.unite(ui.args);
		AwUniteMaps(pdi.input.settings, args);
		return true;
	}
	return false;
}


bool ICA::batchParameterCheck(const QVariantMap& hash)
{
	// this is an exhaustive test as we don't have a file open at this stage.
	// just checking for some parameters: 
	return hash.value("comp").toInt() > 1;
}

int ICA::initParameters()
{
	auto args = pdi.input.settings;
	QMap<QString, int> algos;
	algos.insert("infomax", algos::ICA_infomax);
	algos.insert("cca", algos::ICA_cca);
	algos.insert("sobi", algos::ICA_sobi);

	m_isDownsamplingActive = false;
	m_modality = AwChannel::stringToType(args.value("modality").toString());
	if (m_modality == -1) {
		sendMessage(QString("modality: %1 invalid parameter").arg(m_modality));
		return -1;
	}
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
	auto badLabels = pdi.input.settings[keys::bad_labels].toStringList();
	if (args.contains(keys::skip_bad_channels)) {
		if (!args.value(keys::skip_bad_channels).toBool())
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
		m_nComp = args.value("comp").toInt();

	// check algo
	// some algos have a fixed number of components (no PCA)
	m_algo = 0; // default to infomax
	QString algo = "infomax";
	if (args.contains("algorithm")) {
		auto algoName = args.value("algorithm").toString().toLower().simplified();
		m_algo = algos.value(algoName);
		if (m_algo)
			algo = algoName;
	}
	if (m_algo == algos::ICA_cca || m_algo == algos::ICA_sobi) 
		m_nComp = m_channels.size();

	if (m_nComp > m_channels.size()) {
		sendMessage("The specified number of components is greater dans the number of available channels in data. Aborted.");
		return -1;
	}

	if (m_nComp < 2) {
		sendMessage("Not enough components to compute. Aborted.");
		return -1;
	}

	sendMessage(QString("computing ica on file %1 and %2 channels...").arg(pdi.input.settings[keys::data_path].toString()).arg(args["modality"].toString()));

	m_lpf = args.value(keys::lp).toDouble();
	m_hpf = args.value(keys::hp).toDouble();
	AwFilterSettings filterSettings;
	filterSettings.set(m_channels.first()->type(), m_hpf, m_lpf, 0.);
	filterSettings.apply(m_channels);

	if (!(modifiersFlags() & Aw::ProcessIO::modifiers::UseOrSkipMarkersApplied)) {
		pdi.input.clearMarkers();
		pdi.input.addMarker(new AwMarker("global", 0., args.value(keys::file_duration).toFloat()));
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

	// if output_file exists => use it as the full path to result file
	if (args.contains(keys::output_file))
		m_fileName = args.value(keys::output_file).toString();
	else {
		// default file name is the basename of the data file.
		QFileInfo fi(pdi.input.settings[keys::data_path].toString());
		m_fileName = fi.fileName();
	}

	QString dir;
	// should always contain output_dir execpt if launched from command line with no output_dir option
	if (args.contains(keys::output_dir))
		dir = args.value(keys::output_dir).toString();
	else
		dir = pdi.input.settings[keys::data_dir].toString();

	// output_prefix will override this
	if (args.contains(keys::output_prefix))
		m_fileName = QString("%1%2").arg(args.value(keys::output_prefix).toString()).arg(m_fileName);

	QString mod = args.value("modality").toString();
	if (args.contains(keys::output_suffix))
		m_fileName += QString("_algo-%1_mod-%2_hp-%3_lp-%4_comp-%5%6.mat").arg(algo).arg(mod).arg(m_hpf).arg(m_lpf).arg(m_nComp).arg(args.value(keys::output_suffix).toString());
	else // default suffix is _ica
	    m_fileName += QString("_algo-%1_mod-%2_hp-%3_lp-%4_comp-%5_ica.mat").arg(algo).arg(mod).arg(m_hpf).arg(m_lpf).arg(m_nComp);
	// generate full path
	m_fileName = QString("%1/%2").arg(dir).arg(m_fileName);
	return 0;
}


void ICA::runFromCommandLine()
{
	if (initParameters() == 0) {
		try {
			switch (m_algo) {
			case algos::ICA_infomax:
				infomax(m, n, m_nComp);
				break;
			case algos::ICA_cca: // cca
				run_cca(m, n);
				break;
			case algos::ICA_sobi:
				run_sobi(m, n, m_nComp);
				break;
			}
			
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
		switch (m_algo) {
		case algos::ICA_infomax:
			infomax(m, n, m_nComp);
			break;
		case algos::ICA_cca: // cca
			run_cca(m, n);
			break;
		case algos::ICA_sobi:
			run_sobi(m, n, m_nComp);
			break;
		}
	}
	catch (std::bad_alloc& ba) {
		sendMessage("MEMORY ALLOCATION ERROR. Operation canceled.");
		return;
	}
	if (!isAborted()) {
		saveToFile();

		//// tell AnyWave to load ICA components
		//QVariantList args;
		//args.append(m_fileName);
		//emit sendCommand(AwProcessCommand::LoadICA, args);
		QSharedPointer<AwEvent> e = QSharedPointer<AwEvent>(new AwEvent());
		e->setId(AwEvent::LoadICAMatFile);
		QStringList args = { m_fileName };
		e->addValue("args", args);
		emit sendEvent(e);
		// also send an event to open a signal view to visualise ICA components
		QSharedPointer<AwEvent> e2 = QSharedPointer<AwEvent>(new AwEvent());
		e2->setId(AwEvent::AddNewView);
		QStringList filters = { AwChannel::typeToString(AwChannel::ICA) };
		e2->addValue("filters", filters);
		emit sendEvent(e2);
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
