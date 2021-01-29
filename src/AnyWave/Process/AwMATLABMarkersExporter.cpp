#include "AwMATLABMarkersExporter.h"
#include <matlab/AwMATLAB.h>
#include <math/AwMath.h>
#include <AwKeys.h>

AwMATLABMarkersExporter::AwMATLABMarkersExporter()
{
	setInputFlags(Aw::ProcessIO::GetCurrentMontage);
	setInputModifiers(Aw::ProcessIO::modifiers::IgnoreChannelSelection);
}


AwMATLABMarkersExporterPlugin::AwMATLABMarkersExporterPlugin()
{
	name = QString("MATLAB_MARKERS_EXPORTER");
	description = QString(tr("Export marked data to a MATLAB file"));
	version = "1.0";
	type = AwProcessPlugin::Background;
	m_flags = Aw::ProcessFlags::PluginIsHidden;
}


void AwMATLABMarkersExporter::run()
{
	// get output_file path
	QString path = pdi.input.settings.value(keys::output_file).toString();

	// get data
	sendMessage("Loading data...");
	requestData(&pdi.input.channels(), &pdi.input.markers());
	sendMessage("Done.");
	sendMessage("Writing to file...");
	AwMATLABFile file;
	try {
		file.create(path);
		auto channels = pdi.input.channels();
		auto labels = AwChannel::getLabels(channels, true);
		file.writeStringCellArray("channels", labels);
		float sampleRate = channels.first()->samplingRate();
		file.writeScalar("sr", sampleRate);
		fmat data = AwMath::channelsToFMat(channels);
		file.writeMatrix("data", data);

		auto markers = pdi.input.markers();
		QStringList markerLabels = AwMarker::getAllLabels(markers);
		QVector<qint32> samples(markerLabels.size());
		QVector<float> position(labels.size());
		QVector<float> duration(labels.size());
		int index = 0;
		for (auto m : markers) {
			samples[index++] = (int)floor(m->duration() * sampleRate);
		}
		file.writeStringCellArray("markers", markerLabels);
		file.writeVec("marker_samples", samples);
		file.close();
	}
	catch (const AwException& e)
	{
		sendMessage(QString("Error : %1").arg(e.errorString()));
		return;
	}
	sendMessage("Done.");
}