/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "HDF5IO.h"
#include "structs.h"
#include <QDir>

AHDF5IOPlugin::AHDF5IOPlugin() : AwFileIOPlugin()
{
	name = QString("AnyWave HDF5 Format");
	description = QString(tr("Opens .ah5 file"));
	version = QString("1.0");
	fileExtensions << "*.ah5";
	fileExtension = ".ah5";
	m_flags = Aw::HasExtension|Aw::CanRead|Aw::CanWrite;
}

AHDF5IO::AHDF5IO(const QString& path) : AwFileIO(path)
{

}

AHDF5IO::FileStatus AHDF5IO::canRead(const QString &path)
{
	cleanUpAndClose();

	// switch to native separators
	QString fullPath = QDir::toNativeSeparators(path);
	m_file_id = H5Fopen(fullPath.toStdString().c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);

	if (m_file_id == -1)
		return AwFileIO::WrongFormat;

	hid_t attr = H5Aopen_name(m_file_id, "id");
	char id[] = { "AnyWave" };
	hid_t atype = H5Tcopy(H5T_C_S1);
	H5Tset_size(atype, 7);
	hid_t read_id = H5Aread(attr, atype, id);
	if (read_id == -1) {
		H5Fclose(m_file_id);
		return AwFileIO::WrongFormat;
	}
	if (QString(id) == "AnyWave")
		return AwFileIO::NoError;


	H5Fclose(m_file_id);
	return AwFileIO::WrongFormat;
}

AHDF5IO::FileStatus AHDF5IO::openFile(const QString &path)
{
	cleanUpAndClose();

	// switch to native separators
	QString fullPath = QDir::toNativeSeparators(path);
	m_file_id = H5Fopen(fullPath.toStdString().c_str(), H5F_ACC_RDWR, H5P_DEFAULT);

	// open channels dataset
	hid_t ch_dset = H5Dopen(m_file_id, "/channels", H5P_DEFAULT);
	if (ch_dset == -1) {
		cleanUpAndClose();
		return AwFileIO::BadHeader;
	}

	hid_t ch_space = H5Dget_space(ch_dset);
	hsize_t dims[1];
	hsize_t max_dims[1];
	H5Sget_simple_extent_dims(ch_space, dims, max_dims);

	hid_t ch_type = createChannelStructType();
	h5_channel *channels = new h5_channel[dims[0]];
	H5Dread(ch_dset, ch_type, H5S_ALL, H5S_ALL, H5P_DEFAULT, channels);
	float sr;

	// browse channels
	for (int i = 0; i < dims[0]; i++) {
		h5_channel c = channels[i];
		AwChannel chan;
		chan.setName(QString(c.label));
		QString type = QString(c.type);
		if (type == "MEG") {
			chan.setType(AwChannel::MEG);
			chan.setGain(4);
			chan.setUnit("pT");
		}
		else if (type == "REFERENCE") {
			chan.setType(AwChannel::Reference);
			chan.setGain(10);
			chan.setUnit("pT");
		}
		else if (type == "EEG") {
			chan.setType(AwChannel::EEG);
			chan.setGain(150);
			chan.setUnit(QString::fromLatin1("µV"));
		}
		else if (type == "SEEG") {
			chan.setType(AwChannel::SEEG);
			chan.setGain(300);
			chan.setUnit(QString::fromLatin1("µV"));
		}
		else if (type == "EMG") {
			chan.setType(AwChannel::EMG);
			chan.setGain(300);
			chan.setUnit(QString::fromLatin1("µV"));
		}
		else if (type == "ECG") {
			chan.setType(AwChannel::ECG);
			chan.setGain(300);
			chan.setUnit(QString::fromLatin1("µV"));
		}
		else if (type == "OTHER") {
			chan.setType(AwChannel::Other);
			chan.setGain(200);
			chan.setUnit("units");
		}
		else if (type == "ICA") {
			chan.setType(AwChannel::ICA);
			chan.setGain(5);
			chan.setUnit("units");
		}
		else if (type == "Source") {
			chan.setType(AwChannel::Source);
			chan.setGain(5);
			chan.setUnit("units");
		}
		else if (type == "GRAD") {
			chan.setType(AwChannel::GRAD);
			chan.setGain(15);
			chan.setUnit("pT/m");
		}
		chan.setSamplingRate(c.samplingRate);
		sr = c.samplingRate;
		chan.setXYZ(c.x, c.y, c.z);
		chan.setOrientationXYZ(c.ox, c.oy, c.oz);

		infos.addChannel(&chan);
	}

	// read blocks
	// get the number of blocks

	hid_t gBlocks = H5Gopen(m_file_id, "/Blocks", H5P_DEFAULT);
	hid_t attr = H5Aopen(gBlocks, "numberOfBlocks", H5P_DEFAULT);

	if (attr == -1) {
		cleanUpAndClose();
		return AwFileIO::BadHeader;
	}

	int nBlocks;
	hid_t read_att = H5Aread(attr, H5T_NATIVE_INT, &nBlocks);

	if (read_att == -1) {
		cleanUpAndClose();
		return AwFileIO::BadHeader;
	}

	hid_t group;
	//char groupName[100] = { "/Blocks/" };
	float duration;
	for (int i = 0; i < nBlocks; i++) {
		AwBlock *b = infos.newBlock();
		QString name = QString("/Blocks/%1").arg(i + 1);
		group = H5Gopen(m_file_id, name.toStdString().c_str(), H5P_DEFAULT);
		hid_t attr = H5Aopen_name(group, "duration");
		hid_t read = H5Aread(attr, H5T_NATIVE_FLOAT, &duration);
		H5Aclose(attr);
		b->setDuration(duration);

		// read markers for the block
		readMarkers(b, group);

		H5Gclose(group);
	}
	H5Gclose(gBlocks);

	return AwFileIO::NoError;
}

// extract markers from markers dataset in a specific block.
// if no markers data set is present, do nothing.
void AHDF5IO::readMarkers(AwBlock *block, hid_t group)
{

	hid_t markers_dset = H5Dopen(group, "markers", H5P_DEFAULT);
	if (markers_dset == -1)  // no markers data set
		return;

	hid_t marker_t = createMarkerStructType();
	// get real number of markers
	hid_t attr = H5Aopen_name(group, "markers");
	int nMarkers;
	hid_t read = H5Aread(attr, H5T_NATIVE_INT, &nMarkers);
	H5Aclose(attr);

	// get number of markers
	hsize_t dims[1];
	hsize_t max_dims[1];

	hid_t m_space = H5Dget_space(markers_dset);
	H5Sget_simple_extent_dims(m_space, dims, max_dims);
	h5_marker *markers = new h5_marker[dims[0]];
	H5Dread(markers_dset, marker_t, H5S_ALL, H5S_ALL, H5P_DEFAULT, markers);

	for (int i = 0; i < dims[0]; i++) {
		AwMarker m;
		m.setLabel(QString(markers[i].label));
		m.setStart(markers[i].position);
		m.setDuration(markers[i].duration);
		m.setValue(markers[i].value);

		// are there channels as targets?
		QString targets(markers[i].targets[0]);
		if (!targets.isEmpty()) {
			QStringList tokens = targets.split(",");
			m.setTargetChannels(tokens);
		}
		block->addMarker(m);
	}

	H5Dclose(markers_dset);
}

qint64 AHDF5IO::readDataFromChannels(float start, float duration, AwChannelList &channelList)
{
	if (channelList.isEmpty())
		return 0;

	if (duration <= 0)
		return 0;

	foreach(AwChannel *c, channelList) {
		// now each channel has its own dataset
		// check that the channel in list matches a dataset in the file
		QString datasetName = QString("/Blocks/1/%1").arg(c->name());
		hid_t dataset = H5Dopen(m_file_id, datasetName.toStdString().c_str(), H5P_DEFAULT);
		if (dataset == -1)  //no dataset found
			continue;
		// number of samples to read
		hsize_t nSamples = (hsize_t)floor(duration * c->samplingRate());
		// starting sample in channel.
		hsize_t nStart = (hsize_t)floor(start * c->samplingRate());
		hsize_t offset[1] = { nStart };
		hsize_t block[1] = { nSamples };
		hid_t mem_space = H5Screate_simple(1, block, NULL);
		hid_t data_space = H5Dget_space(dataset);
		herr_t status = H5Sselect_hyperslab(data_space, H5S_SELECT_SET, offset, NULL, block, NULL);
		if (status) {
			H5Dclose(dataset);
			return 0;
		}
		c->newData((qint64)block[0]);
		status = H5Dread(dataset, H5T_NATIVE_FLOAT, mem_space, data_space, H5P_DEFAULT, c->data());
		H5Dclose(dataset);
		if (status) {
			c->clearData();
			return 0;
		}
	}
	return 1;
}


qint64 AHDF5IO::writeData(QList<AwChannel*> *channels)
{
	if (channels->isEmpty() || m_file_id == -1)
		return 0;

	qint64 dataSize = channels->at(0)->dataSize();
	int nChannels = channels->size();
	herr_t status;
	hid_t group = H5Gopen(m_file_id, "/Blocks/1", H5P_DEFAULT);
	// handle data offset for each channels
	hsize_t *offsets = new hsize_t[channels->size()];
	memset(offsets, 0, sizeof(hsize_t) * channels->size());
	int index = 0;
	foreach(AwChannel *channel, *channels) {
		QString name = channel->name();
		hid_t dataset = H5Dopen(group, name.toStdString().c_str(), H5P_DEFAULT);
		hid_t dataspace = H5Dget_space(dataset);
		hsize_t channelLength[1];
		H5Sget_simple_extent_dims(dataspace, channelLength, NULL);
		hsize_t offset[1] = { offsets[index] };
		hsize_t count[1] = { hsize_t(channel->dataSize()) };

		// check if dataset has sufficient space for our data
		if (offsets[index] + channel->dataSize() > channelLength[0]) { // we are out of dataset current dimension => extend it
			hsize_t delta = (offsets[index] + (hsize_t)channel->dataSize()) - channelLength[0];
			hsize_t size[1] = { channelLength[0] + delta };
			status = H5Dset_extent(dataset, size);
			if (status) {
				H5Dclose(dataset);
				H5Sclose(dataspace);
				H5Gclose(group);
				throw(QString("Error extending dataset dimention"));
			}
		}
		// save data

		hid_t sub_space = H5Screate_simple(1, count, NULL);
		// just write data
		status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);
		if (status) {
			H5Dclose(dataset);
			H5Sclose(dataspace);
			H5Gclose(group);
			throw(QString("Error selecting hyperslab"));
		}
		status = H5Dwrite(dataset, H5T_NATIVE_FLOAT, sub_space, dataspace, H5P_DEFAULT, channel->data());
		if (status) {
			H5Dclose(dataset);
			H5Sclose(dataspace);
			H5Gclose(group);
			throw(QString("Error writing the dataset"));
		}
		H5Dclose(dataset);
		H5Sclose(dataspace);
		offsets[index] += (hsize_t)channel->dataSize();
		index++;
	}
	H5Gclose(group);

	return 1;
}


AwFileIO::FileStatus AHDF5IO::writeMarkers()
{
	hid_t group;
	hid_t marker_t = createMarkerStructType();
	herr_t status;

	for (int i = 0; i < infos.blocks().size(); i++) {
		// if block does not contain markers, skip it
		if (infos.blocks().at(i)->markers().isEmpty())
			continue;
		AwMarkerList markers = infos.blocks().at(i)->markers();
		QString location = QString("/Blocks/%1").arg(i + 1);
		group = H5Gopen(m_file_id, location.toLatin1().data(), H5P_DEFAULT);
		// open the markers dataset
		hid_t dataset = H5Dopen(group, "markers", H5P_DEFAULT);
		// check current space available
		hid_t dataspace = H5Dget_space(dataset);
		hsize_t markersLength[1];
		H5Sget_simple_extent_dims(dataspace, markersLength, NULL);

		hsize_t count[1] = { hsize_t(markers.size()) };
		if (count[0] > markersLength[0]) { // we are out of dataset current dimension => extend it
			hsize_t delta = count[0] - markersLength[0];
			hsize_t size[1] = { markersLength[0] + delta };
			status = H5Dset_extent(dataset, size);
			if (status) {
				H5Dclose(dataset);
				H5Sclose(dataspace);
				H5Gclose(group);
				throw(QString("Error extending markers dataset dimension."));
			}
		}

		// writting markers
		// generating markers struct
		int nMarkers = markers.size();
		h5_marker *h5_markers = new h5_marker[nMarkers];
		memset(h5_markers, 0, sizeof(h5_marker) *  nMarkers);
		int strlen = 0;
		int j = 0;
		char *targets;

		foreach(AwMarker *m, infos.blocks().at(i)->markers()) {
			strlen = m->label().size();
			if (strlen > 255)
				strlen = 255;
			memcpy(h5_markers[j].label, m->label().toLatin1().data(), strlen);
			h5_markers[j].label[strlen] = '\0';
			h5_markers[j].position = m->start();
			h5_markers[j].duration = m->duration();
			h5_markers[j].value = m->value();

			// Are there targeted channels?
			if (!m->targetChannels().isEmpty()) {
				QString t;
				int index = 0;
				foreach(QString s, m->targetChannels()) {
					if (index)
						t += ", ";
					t += s;
					index++;
				}
				strlen = t.size();
				targets = new char[strlen + 1];
				memcpy(targets, t.toLatin1().data(), strlen);
				targets[strlen] = '\0';

				h5_markers[j].targets[0] = targets;
			}
			else {
				h5_markers[i].targets[0] = new char[1];
				h5_markers[i].targets[0][0] = '\0';
			}
			j++;
		}
		// write data to dataset
		status = H5Dwrite(dataset, marker_t, H5S_ALL, H5S_ALL, H5P_DEFAULT, h5_markers);
		H5Sclose(dataspace);
		H5Dclose(dataset);
		H5Gclose(group);

		for (int i = 0; i < nMarkers; i++)
			delete[] h5_markers[i].targets[0];

		delete[] h5_markers;

		if (status)
			return AwFileIO::FileAccess;
	}
	return AwFileIO::NoError;
}


AwFileIO::FileStatus AHDF5IO::createFile(const QString &path, int flags)
{
	QString fullPath = path + plugin()->fileExtension;
	// switch to native separators
	fullPath = QDir::toNativeSeparators(fullPath);
	herr_t status;
	hsize_t id_dims[] = { 1 };

	// create the file
	m_file_id = H5Fcreate(fullPath.toStdString().c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

	hid_t id_space = H5Screate_simple(1, id_dims, NULL);
	hid_t id_type = H5Tcopy(H5T_C_S1);
	H5Tset_size(id_type, 7);

	// Create an attribute to identify AnyWave Format
	hid_t id = H5Acreate(m_file_id, "id", id_type, id_space, H5P_DEFAULT, H5P_DEFAULT);
	char att_data[] = { "AnyWave" };
	status = H5Awrite(id, id_type, att_data);
	H5Sclose(id_space);
	H5Tclose(id_type);
	H5Aclose(id);

	if (status) {
		H5Fclose(m_file_id);
		return AwFileIO::FileAccess;
	}

	// create channels data set using chunking
	status = createChannelDataSet();
	if (status) {
		H5Fclose(m_file_id);
		return AwFileIO::FileAccess;
	}
	// create a groupe named Blocks
	hid_t groupBlocks = H5Gcreate2(m_file_id, "/Blocks", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	// add attribute numberOfBlocks
	int nBlock = infos.blocks().size();
	hid_t spaceBlocks = H5Screate(H5S_SCALAR);
	hid_t attBlocks = H5Acreate(groupBlocks, "numberOfBlocks", H5T_NATIVE_INT, spaceBlocks, H5P_DEFAULT, H5P_DEFAULT);
	status = H5Awrite(attBlocks, H5T_NATIVE_INT, &nBlock);
	if (status) {
		H5Fclose(m_file_id);
		return AwFileIO::FileAccess;
	}
	H5Aclose(attBlocks);
	// create all the sub blocks group and data sets
	createBlocks(groupBlocks);
	H5Gclose(groupBlocks);

	return writeMarkers();
}


void AHDF5IO::cleanUpAndClose()
{
	AwFileIO::cleanUpAndClose();
	H5Fclose(m_file_id);
}

void AHDF5IO::createBlocks(hid_t root)
{
	int nBlock = infos.blocks().size();
	//	int strlen = 0;
	//	char buffer[256];
	hsize_t data_dims[1] = { hsize_t(infos.channelsCount()) };
	hid_t group, dataspace, dataset, prop;
	hsize_t maxDims[1] = { H5S_UNLIMITED };
	herr_t status;
	hid_t marker_t = createMarkerStructType();

	for (int i = 0; i < nBlock; i++) {
		QString name = QString::number(i + 1);
		group = H5Gcreate2(root, name.toStdString().c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		hid_t space = H5Screate(H5S_SCALAR);
		hid_t attDuration = H5Acreate(group, "duration", H5T_NATIVE_FLOAT, space, H5P_DEFAULT, H5P_DEFAULT);
		float duration = infos.blocks().at(i)->duration();
		H5Awrite(attDuration, H5T_NATIVE_FLOAT, &duration);
		H5Sclose(space);  // close the dataspace used for attribute
		H5Aclose(attDuration);

		/** MARKERS DATASET PREPARATION **/
		// Create an extensible dataset for markers in the block
		hsize_t nMarkers = (hsize_t)infos.blocks().at(i)->markers().size();
		hsize_t dimMarkers[1];
		if (nMarkers == 0)
			dimMarkers[0] = 10;  // reserve a dataspace for 10 markers
		else
			dimMarkers[0] = nMarkers;

		// write the real number of markers in the dataset as an attribute
		space = H5Screate(H5S_SCALAR);
		hid_t attNMarkers = H5Acreate(group, "markers", H5T_NATIVE_INT, space, H5P_DEFAULT, H5P_DEFAULT);
		int n = infos.blocks().at(i)->markers().size();
		H5Awrite(attNMarkers, H5T_NATIVE_INT, &n);
		H5Sclose(space);  // close the dataspace used for attribute
		H5Aclose(attNMarkers);

		// create an extensible data set using chunks
		prop = H5Pcreate(H5P_DATASET_CREATE);
		hsize_t chunk_dims[1] = { 10 }; // chunk of 10 markers
		status = H5Pset_chunk(prop, 1, chunk_dims);
		if (status) {
			H5Gclose(group);
			throw(QString("Error setting chunk property for the markers dataset"));
		}
		dataspace = H5Screate_simple(1, dimMarkers, maxDims);
		dataset = H5Dcreate(group, "markers", marker_t, dataspace, H5P_DEFAULT, prop, H5P_DEFAULT);
		H5Sclose(dataspace);
		H5Dclose(dataset);
		/** END OF MARKERS DATASET PREPARATION **/

		// Create an extensible dataset for each channel => that way we could handle channels with different sampling rate.
		foreach(AwChannel *channel, infos.channels()) {
			hsize_t channelLength = (hsize_t)floor(channel->samplingRate() * duration);
			hsize_t dims[1] = { channelLength };
			hsize_t chunk_dims[1] = { 1000 }; // 1000 samples chunk
											  // create chunk dataset property
			prop = H5Pcreate(H5P_DATASET_CREATE);
			H5Pset_chunk(prop, 1, chunk_dims);
			// Create the dataset space and the chunked dataset
			dataspace = H5Screate_simple(1, dims, maxDims);
			// name the dataset using the channel index
			QString name = channel->name();
			dataset = H5Dcreate(group, name.toStdString().c_str(), H5T_NATIVE_FLOAT, dataspace, H5P_DEFAULT, prop, H5P_DEFAULT);
			H5Sclose(dataspace);
			H5Dclose(dataset);
		}
		H5Gclose(group);
	}
}


herr_t AHDF5IO::createChannelDataSet()
{
	int nChannels = infos.channelsCount();
	hid_t dataspace, prop;
	hsize_t chunk_dims[1] = { 10 }; // chunks of 10 channels
	hsize_t dims[1] = { hsize_t(infos.channelsCount()) }; // dim at creation (number of channels)
	hsize_t maxDims[1] = { H5S_UNLIMITED };

	// Create a dataset creation property list and set it to use chunking
	prop = H5Pcreate(H5P_DATASET_CREATE);
	H5Pset_chunk(prop, 1, chunk_dims);
	// Create the dataset space and the chunked dataset
	dataspace = H5Screate_simple(1, dims, maxDims);
	// create the channel struct
	hid_t channels_type = createChannelStructType();
	h5_channel *h5Channels = new h5_channel[nChannels];
	memset(h5Channels, 0, nChannels * sizeof(h5_channel));
	int i = 0, strlen = 0;
	foreach(AwChannel *c, infos.channels()) {
		// type
		QString stype = AwChannel::typeToString(c->type());
		strlen = stype.size();
		memcpy(h5Channels[i].type, stype.toLatin1().data(), strlen);
		// label
		strlen = c->name().size();
		if (strlen > 16)
			strlen = 16;
		memcpy(h5Channels[i].label, c->name().toLatin1().data(), strlen);

		// ref
		if (!c->referenceName().isEmpty()) {
			strlen = c->referenceName().size();
			memcpy(h5Channels[i].ref, c->referenceName().toLatin1().data(), strlen);
		}
		else
			h5Channels[i].ref[0] = '\0';

		// unit
		if (!c->unit().isEmpty()) {
			strlen = c->unit().size();
			memcpy(h5Channels[i].unit, c->unit().toLatin1().data(), strlen);
		}
		else
			h5Channels[i].unit[0] = '\0';

		// sampling rate
		h5Channels[i].samplingRate = c->samplingRate();
		// x y z
		h5Channels[i].x = c->x();
		h5Channels[i].y = c->y();
		h5Channels[i].z = c->z();
		// ox oy oz
		h5Channels[i].ox = c->ox();
		h5Channels[i].oy = c->oy();
		h5Channels[i].oz = c->oz();

		i++;
	}
	hid_t dataset = H5Dcreate(m_file_id, "channels", channels_type, dataspace, H5P_DEFAULT, prop, H5P_DEFAULT);
	herr_t status = H5Dwrite(dataset, channels_type, H5S_ALL, H5S_ALL, H5P_DEFAULT, h5Channels);
	H5Tclose(channels_type);
	H5Sclose(dataspace);
	H5Dclose(dataset);
	delete[] h5Channels;
	return status;
}

