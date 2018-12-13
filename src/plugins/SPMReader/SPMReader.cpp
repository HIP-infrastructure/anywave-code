#include "SPMReader.h"
#include "matio.h"
#include "AwMATLAB.h"

SPMReaderPlugin::SPMReaderPlugin() : AwFileIOPlugin()
{
	name = QString("SPM Reader");
	description = QString(tr("Reads SPM .mat files"));
	manufacturer = "SPM";
	version = QString("1.0");
	fileExtensions << "*.mat";
	m_flags = Aw::HasExtension | Aw::CanRead;
}

SPMReader::SPMReader(const QString& fileName) : AwFileIO(fileName)
{

}


SPMReader::~SPMReader()
{
	cleanUpAndClose();
}

void SPMReader::cleanUpAndClose()
{

}

qint64 SPMReader::readDataFromChannels(float start, float duration, AwChannelList &channelList)
{
	return 0;
}

AwFileIO::FileStatus SPMReader::openFile(const QString &path)
{
	AwMATLABFile file;
	qint64 nSamples;
	float sr;
	try {
		file.open(path);
		file.readStruct("D");
		// check type fields
		QString type;
		file.readStringField("D", "type", type, 0);
		if (type != "continuous") 
			return AwFileIO::WrongFormat;
		double tmp;
		file.readScalarField("D", "Nsamples", &tmp, 0);
		nSamples = (qint64)ceil(tmp);
		file.readScalarField("D", "Fsample", &tmp, 0);
		sr = (float)tmp;
		// get the structure pointer by field channels
		file.addChildStruct("D", "channels");
		auto childStructName = file.getChildStructName("D", "channels");
		auto nChannels = file.getStructNumElements(childStructName);
		for (int i = 0; i < nChannels; i++) {
			AwChannel channel;
			QString label, units;
			double bad;
			file.readStringField(childStructName, "label", label, i);
			file.readStringField(childStructName, "units", units, i);
			file.readScalarField(childStructName, "bad", &bad, i);
			channel.setName(label);
			channel.setSamplingRate(sr);
			channel.setGain(100);
			channel.setBad(bad != 0.);
			infos.addChannel(channel);
		}
		auto block = infos.newBlock();
		block->setSamples(nSamples);
		block->setDuration(nSamples / sr);
	}
	catch (const AwException& e) {
		m_error = file.error();
		return AwFileIO::WrongFormat;
	}
	return AwFileIO::NoError;
}

AwFileIO::FileStatus SPMReader::canRead(const QString &path)
{
	AwMATLABFile file;
	try {
		file.open(path);
		// try to get a variable named D which is a structure
		file.readStruct("D");
	}

	catch (const AwException& e) {
		m_error = file.error();
		return AwFileIO::WrongFormat;
	}
	return AwFileIO::NoError;
}
