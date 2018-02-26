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
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - bruno.colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include <hdf5/AwHDF5.h>
#include <hdf5_hl.h>

// constructors

AwHDF5::AwHDF5()
{
	m_status = AwHDF5::Ok;
	m_group = NULL;
	m_fileMode = 0;
	m_file_id = -1;
}

AwHDF5::AwHDF5(const QString& fileName)
{
	m_status = AwHDF5::Ok;
	m_group = NULL;
	m_fileName = fileName;
	m_fileMode = 0;
	m_file_id = -1;
}

int AwHDF5::open(int mode, const char *group)
{
	m_fileMode = mode;
	m_status = AwHDF5::Ok;
	m_group = group;
	if (mode == AwHDF5::Read) {
		if (m_file_id != -1)
			H5Fclose(m_file_id);
		m_file_id = H5Fopen(m_fileName.toStdString().c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
		if (m_file_id == -1)
			m_status = AwHDF5::FileOpenError;
		else if (group) { // try to open the group
			m_group_id = H5Gopen(m_file_id, group, H5P_DEFAULT);
			if (m_group_id == -1) // Failed to open the group
				m_status = AwHDF5::UnknownGroup;
		}	
	}
	else if (mode == AwHDF5::Write) { // Write only means overwrite existing file
		if (m_file_id != -1)
			H5Fclose(m_file_id);
		m_file_id = H5Fcreate(m_fileName.toStdString().c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
		if (m_file_id == -1)
			m_status = AwHDF5::FileOpenError;
		else if (group) { // try to create the group 
			m_group_id = H5Gcreate(m_file_id, group, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
			if (m_group_id == -1)
				m_status = AwHDF5::FileOpenError;
		}
	}
	else if (mode == AwHDF5::ReadWrite)	{
		m_file_id = H5Fopen(m_fileName.toStdString().c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
		if (m_file_id == -1)
			m_status = AwHDF5::FileOpenError;
		else if (group) {
			m_group_id = H5Gopen(m_file_id, group, H5P_DEFAULT);
			if (m_group_id == -1)
				m_status = AwHDF5::UnknownGroup;
		}
	}
	return m_status;
}

void AwHDF5::setFileName(const QString& name)
{
	m_fileName = name;
}

AwHDF5::~AwHDF5()
{
	close();
}

// methods

bool AwHDF5::isGroup(const char *group)
{
	hid_t group_id = H5Gopen(m_file_id, group, H5P_DEFAULT);
	if (group_id != -1)	{
		H5Gclose(group_id);
		return true;
	}
	return false;
}

void AwHDF5::close()
{
	H5Gclose(m_group_id);
	H5Fclose(m_file_id);
	m_status = AwHDF5::Ok;
}

int AwHDF5::setGroup(const char *group)
{
	// change the group
	H5Gclose(m_group_id);
	m_group_id = H5Gopen(m_file_id, group, H5P_DEFAULT);
	m_group = group;
	if (m_group_id == -1)
		m_status =  AwHDF5::FileOpenError;

	return m_status;
}

int AwHDF5::createGroup(const char *group)
{
	// check current file mode
	if (m_fileMode == AwHDF5::Read)
		return  AwHDF5::FileOpenError;

	m_status = AwHDF5::Ok;
	H5Gclose(m_group_id);
	m_group = group;
	m_group_id = H5Gcreate(m_file_id, group, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	if (m_group_id == -1) {
		m_group = NULL;
		m_status =  AwHDF5::FileOpenError;
	}

	return m_status;
}

/**
This method will search for a dataset named "matrix" and get its rank and dimensions size.
INPUT:
matrix = the dataset name
OUTPUT:
dims = the rank
s_dims = array of hsize_t containing the size of each dimensions.
**/
int AwHDF5::getMatrixSize(const char *matrix, int *dataSize, int *row, int *col)
{

	int dims;
	hsize_t s_dims[2], maxDims[2];

	herr_t status = H5LTget_dataset_ndims(m_group_id, matrix, &dims);
	if (status)
		return AwHDF5::UnknownMatrix;

	if (dims != 2)  // only manage 2D matrices
		return AwHDF5::UnknownMatrix;

	hid_t dataset = H5Dopen(m_group_id, matrix, H5P_DEFAULT);
	hid_t dataspace = H5Dget_space(dataset);
	H5Sget_simple_extent_dims(dataspace, s_dims, maxDims);
	H5Dclose(dataset);

	// compute data size
	*dataSize = 1;
	for (int i = 0; i < dims; i++)
		*dataSize *= s_dims[i];

	*row = s_dims[0];
	*col = s_dims[1];
	return AwHDF5::Ok;
}

int AwHDF5::getStringSize(const char *string, int *size)
{
	hsize_t dims[1];
	size_t dataSize;
	herr_t status = H5LTget_dataset_info(m_group_id, string, dims, NULL, &dataSize); 

	if (status)
		return AwHDF5::UnknownMatrix;

	*size = dataSize;
	return AwHDF5::Ok;
}

int AwHDF5::getVectorSize(const char *vector, int *size)
{
	int dims;
	herr_t status = H5LTget_dataset_ndims(m_group_id, vector, &dims);
	if (status)
		return AwHDF5::UnknownMatrix;

	if (dims != 1)
		return AwHDF5::UnknownMatrix;

	hid_t dataset = H5Dopen(m_group_id, vector, H5P_DEFAULT);
	hid_t dataspace = H5Dget_space(dataset);
	hsize_t s_dims[1], maxDims[1];
	H5Sget_simple_extent_dims(dataspace, s_dims, maxDims);
	H5Dclose(dataset);

	*size = s_dims[0];
	return AwHDF5::Ok;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int *AwHDF5::readIntMatrix(const char *matrix, int *row, int *col)
{
	if (m_status != AwHDF5::Ok)
		return NULL;

	int dataSize;

	int status = getMatrixSize(matrix, &dataSize, row, col);
	if (status != AwHDF5::Ok)
		return NULL;

	int *data = new int[dataSize];
	herr_t hstatus = H5LTread_dataset_int(m_group_id, matrix, data);
	if (hstatus) {
		delete[] data;
		data = NULL;
		return NULL;
	}
	return data;
}

float *AwHDF5::readFloatMatrix(const char *matrix, int *row, int *col)
{
	if (m_status != AwHDF5::Ok)
		return NULL;

	int dataSize;

	int status = getMatrixSize(matrix, &dataSize, row, col);
	if (status != AwHDF5::Ok)
		return NULL;

	float *data = new float[dataSize];
	herr_t hstatus = H5LTread_dataset_float(m_group_id, matrix, data);
	if (hstatus) {
		delete[] data;
		data = NULL;
		return NULL;
	}
	return data;
}

double *AwHDF5::readDoubleMatrix(const char *matrix, int *row, int *col)
{
	if (m_status != AwHDF5::Ok)
		return NULL;

	int dataSize;

	int status = getMatrixSize(matrix, &dataSize, row, col);
	if (status != AwHDF5::Ok)
		return NULL;

	double *data = new double[dataSize];
	herr_t hstatus = H5LTread_dataset_double(m_group_id, matrix, data);
	if (hstatus) {
		delete[] data;
		data = NULL;
		return NULL;
	}
	return data;
}


int AwHDF5::writeIntMatrix(const char *matrix, int *data, int row, int col)
{
	if (m_status != AwHDF5::Ok)
		return m_status;

	int dims = 2;
	hsize_t sdims[2] = { hsize_t(row), hsize_t(col) };
	herr_t status;

	status = H5LTmake_dataset_int(m_group_id, matrix, dims, sdims, data);
	if (status)
		return AwHDF5::WriteError;

	return AwHDF5::Ok;
}

int AwHDF5::writeFloatMatrix(const char *matrix, float *data, int row, int col)
{
	if (m_status != AwHDF5::Ok)
		return m_status;

	int dims = 2;
	hsize_t sdims[2] = { hsize_t(row), hsize_t(col) };
	herr_t status;

	status = H5LTmake_dataset_float(m_group_id, matrix, dims, sdims, data);
	if (status)
		return AwHDF5::WriteError;
	return AwHDF5::Ok;
}

int AwHDF5::writeDoubleMatrix(const char *matrix, double *data, int row, int col)
{
	if (m_status != AwHDF5::Ok)
		return m_status;

	int dims = 2;
	hsize_t sdims[2] = { hsize_t(row), hsize_t(col) };
	herr_t status;

	status = H5LTmake_dataset_double(m_group_id, matrix, dims, sdims, data);
	if (status)
		return AwHDF5::WriteError;
	return AwHDF5::Ok;
}


int AwHDF5::writeIntVector(const char *matrix, QVector<int>& vector)
{
	if (m_status != AwHDF5::Ok)
		return m_status;

	int dims = 1;
	hsize_t sdims[1] = { hsize_t(vector.size()) };

	herr_t status = H5LTmake_dataset_int(m_group_id, matrix, dims, sdims, vector.data());

	if (status)
		return AwHDF5::WriteError;

	return AwHDF5::Ok;
}

QVector<double>  AwHDF5::readDoubleVector(const char *vector)
{
	QVector<double> res;

	if (m_status != AwHDF5::Ok)
		return res; // empty vector

	int size = 0;
	int status = getVectorSize(vector, &size);
	if (status != AwHDF5::Ok)
		return res; // empty vector
	
	double *data = new double[size];

	herr_t hstatus = H5LTread_dataset_double(m_group_id, vector, data);
	if (hstatus) {
		delete[] data;
		data = NULL;
		return res; // empty vector
	}

	res.reserve(size);
	double *dest = res.data();
	for (int i =0; i < size; i++)
		*dest++ = *data++;

	delete[] data;
	return res;
}


QVector<int>  AwHDF5::readIntVector(const char *vector)
{
	QVector<int> res;

	if (m_status != AwHDF5::Ok)
		return res; // empty vector

	int size = 0;
	int status = getVectorSize(vector, &size);
	if (status != AwHDF5::Ok)
		return res; // empty vector
	
	int *data = new int[size];

	herr_t hstatus = H5LTread_dataset_int(m_group_id, vector, data);
	if (hstatus)  {
		delete[] data;
		data = NULL;
		return res; // empty vector
	}

	res.reserve(size);
	int *dest = res.data();
	for (int i =0; i < size; i++)
		*dest++ = *data++;

	delete[] data;
	return res;
}



QVector<float>  AwHDF5::readFloatVector(const char *vector)
{
	QVector<float> res;

	if (m_status != AwHDF5::Ok)
		return res; // empty vector

	int size = 0;
	int status = getVectorSize(vector, &size);
	if (status != AwHDF5::Ok)
		return res; // empty vector
	
	float *data = new float[size];

	herr_t hstatus = H5LTread_dataset_float(m_group_id, vector, data);
	if (hstatus) {
		delete[] data;
		data = NULL;
		return res; // empty vector
	}

	res.reserve(size);
	float *dest = res.data();
	for (int i =0; i < size; i++)
		*dest++ = *data++;

	delete[] data;
	return res;
}


int AwHDF5::readFloatVector(const char *vector, float *data, int *size)
{
	if (m_status != AwHDF5::Ok)
		return m_status;

	int status = getVectorSize(vector, size);
	if (status != AwHDF5::Ok)
		return status;

	data = new float[*size];

	herr_t hstatus = H5LTread_dataset_float(m_group_id, vector, data);
	if (hstatus) {
		delete[] data;
		data = NULL;
		return AwHDF5::ReadError;
	}

	return AwHDF5::Ok;
}


int AwHDF5::readDoubleVector(const char *vector, double *data, int *size)
{
	if (m_status != AwHDF5::Ok)
		return m_status;

	int status = getVectorSize(vector, size);
	if (status != AwHDF5::Ok)
		return status;

	data = new double[*size];

	herr_t hstatus = H5LTread_dataset_double(m_group_id, vector, data);
	if (hstatus) {
		delete[] data;
		data = NULL;
		return AwHDF5::ReadError;
	}

	return AwHDF5::Ok;
}

int AwHDF5::readIntVector(const char *vector, int *data, int *size)
{
	if (m_status != AwHDF5::Ok)
		return m_status;

	int status = getVectorSize(vector, size);
	if (status != AwHDF5::Ok)
		return status;

	data = new int[*size];

	herr_t hstatus = H5LTread_dataset_int(m_group_id, vector, data);
	if (hstatus) {
		delete[] data;
		data = NULL;
		return AwHDF5::ReadError;
	}

	return AwHDF5::Ok;
}


int AwHDF5::writeFloatVector(const char *vector, float *data, int size)
{
	if (m_status != AwHDF5::Ok)
		return m_status;

	int dims = 1;
	hsize_t sdims[1] = { hsize_t(size) };

	herr_t status = H5LTmake_dataset_float(m_group_id, vector, dims, sdims, data);

	if (status)
		return AwHDF5::WriteError;

	return AwHDF5::Ok;
}

int AwHDF5::writeIntVector(const char *vector, int *data, int size)
{
	if (m_status != AwHDF5::Ok)
		return m_status;

	int dims = 1;
	hsize_t sdims[1] = { hsize_t(size) };

	herr_t status = H5LTmake_dataset_int(m_group_id, vector, dims, sdims, data);

	if (status)
		return AwHDF5::WriteError;

	return AwHDF5::Ok;
}

int AwHDF5::writeDoubleVector(const char *vector, double *data, int size)
{
	if (m_status != AwHDF5::Ok)
		return m_status;

	int dims = 1;
	hsize_t sdims[1] = { hsize_t(size) };

	herr_t status = H5LTmake_dataset_double(m_group_id, vector, dims, sdims, data);

	if (status)
		return AwHDF5::WriteError;

	return AwHDF5::Ok;
}

int AwHDF5::writeFloatVector(const char *matrix, QVector<float>& vector)
{
	if (m_status != AwHDF5::Ok)
		return m_status;

	int dims = 1;
	hsize_t sdims[1] = { hsize_t(vector.size()) };

	herr_t status = H5LTmake_dataset_float(m_group_id, matrix, dims, sdims, vector.data());

	if (status)
		return AwHDF5::WriteError;

	return AwHDF5::Ok;
}


int AwHDF5::writeDoubleVector(const char *matrix, QVector<double>& vector)
{
	if (m_status != AwHDF5::Ok)
		return m_status;

	int dims = 1;
	hsize_t sdims[1] = { hsize_t(vector.size()) };

	herr_t status = H5LTmake_dataset_double(m_group_id, matrix, dims, sdims, vector.data());

	if (status)
		return AwHDF5::WriteError;

	return AwHDF5::Ok;
}

QString AwHDF5::readString(const char *dset)
{
	QString res;
	int size;
	int status = getStringSize(dset, &size);
	if (status != AwHDF5::Ok)
		return res;

	char *data = new char[size + 1];
	
	herr_t hstatus = H5LTread_dataset_string(m_group_id, dset, data);
	data[size] = '\0';
	if (hstatus) {
		delete[] data;
		data = NULL;
		return res; // empty string
	}

	res = QString(data);
	delete[] data;
	return res;
}

int AwHDF5::writeString(const char *name, const QString& string)
{
	QByteArray ba = string.toLatin1();
	herr_t status = H5LTmake_dataset_string(m_group_id, name, ba.constData());

	if (status)
		return AwHDF5::WriteError;

	return AwHDF5::Ok;
}

int AwHDF5::writeIntAttr(const char *attribute, int value)
{
	herr_t status = H5LTset_attribute_int(m_file_id, m_group, attribute, &value, 1);
	if (status)
		return AwHDF5::WriteError;
	return AwHDF5::Ok;
}

int AwHDF5::writeFloatAttr(const char *attribute, float value)
{
	herr_t status = H5LTset_attribute_float(m_file_id, m_group, attribute, &value, 1);
	if (status)
		return AwHDF5::WriteError;
	return AwHDF5::Ok;
}

int AwHDF5::writeDoubleAttr(const char *attribute, double value)
{
	herr_t status = H5LTset_attribute_double(m_file_id, m_group, attribute, &value, 1);
	if (status)
		return AwHDF5::WriteError;
	return AwHDF5::Ok;
}

int AwHDF5::readDoubleAttr(const char *attribute, double *value)
{
	herr_t status = H5LTget_attribute_double(m_file_id, m_group, attribute, value);
	if (status)
		return AwHDF5::ReadError;

	return AwHDF5::Ok;
}

int AwHDF5::readFloatAttr(const char *attribute, float *value)
{
	herr_t status = H5LTget_attribute_float(m_file_id, m_group, attribute, value);
	if (status)
		return AwHDF5::ReadError;

	return AwHDF5::Ok;
}

int AwHDF5::readIntAttr(const char *attribute, int *value)
{
	herr_t status = H5LTget_attribute_int(m_file_id, m_group, attribute, value);
	if (status)
		return AwHDF5::ReadError;

	return AwHDF5::Ok;
}

QString AwHDF5::readStringAttr(const char *attribute)
{
	// get size of
	hid_t id = H5Aopen(m_group_id, attribute, H5P_DEFAULT);
	if (id == -1)
		return QString();

	hsize_t size = H5Aget_storage_size(id);
	QByteArray tmp;
	tmp.reserve(size);

	herr_t status;
	status = H5LTget_attribute_string(m_file_id, m_group, attribute, tmp.data());
	if (status)
		return QString();

	return QString(tmp);
}
