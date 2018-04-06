#include <AwMATLAB.h>
#include <math/AwMath.h>


int AwMATLAB::saveToMatlab(const QString& fileName, const AwChannelList& channels)
{
	AwMATLABFile file;
	int status = file.create(fileName);
	if (status != 0)
		return status;

	QStringList labels = AwChannel::getLabels(channels);
	status = file.writeStringCellArray(QString("electrodes"), labels);
	if (status != 0) {
		file.close();
		return status;
	}

	// data matrix
	size_t matrixDims[2] = { size_t(channels.size()), size_t(channels.first()->dataSize()) };
	arma::fmat matrix = AwMath::channelsToFMat(channels).t();
	status = file.writeMatrix(QString("data"), matrix);
	if (status != 0) {
		file.close(); 
		return status;
	}
	file.close();
	return 0;
}
