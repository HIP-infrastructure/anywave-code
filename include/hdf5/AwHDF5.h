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
//  Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - bruno.colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef AWHDF5_H
#define AWHDF5_H
#include <AwGlobal.h>
#include <hdf5.h>
#include <QVector>
#include <QString>

class AW_HDF5_EXPORT AwHDF5
{
public:
	AwHDF5();
	AwHDF5(const QString& fileName);
	~AwHDF5();

	enum FileModes { Read, Write, ReadWrite };
	enum Status { Ok, ReadError, WriteError, FileOpenError, UnknownMatrix, UnknownGroup };

	/** Open the file and set the current group. **/
	int open(int mode, const char *group);
	void close();
	void setFileName(const QString& name);
	inline QString& fileName() { return m_fileName; }

	/** Change the current group. If the group does not exist, create it. **/
	int setGroup(const char *group);
	const char *group() { return m_group; }
	/** isGroup returns true if the group exists in the file **/
	bool isGroup(const char *group);
	int createGroup(const char *group);
	
	inline int status() { return m_status; }

	// Writing datasets
	int writeIntMatrix(const char *matrix, int *data, int row, int col);
	int writeFloatMatrix(const char *matrix, float *data, int row, int col);
	int writeDoubleMatrix(const char *matrix, double *data, int row, int col);
	int writeIntVector(const char *matrix, QVector<int>& vector);
	int writeIntVector(const char *vector, int *data, int size);
	int writeFloatVector(const char *matrix, QVector<float>& vector);
	int writeFloatVector(const char *vector, float *data, int size);
	int writeDoubleVector(const char *matrix, QVector<double>& vector);
	int writeDoubleVector(const char *vector, double *data, int size);
	int writeString(const char *dset, const QString& string);

	// Writing attributes
	int writeIntAttr(const char *attribute, int value);
	int writeFloatAttr(const char *attribute, float value);
	int writeDoubleAttr(const char *attribute, double value);

	// Reading datasets
	int *readIntMatrix(const char *matrix, int *row, int *col);
	float *readFloatMatrix(const char *matrix, int *row, int *col);	
	double *readDoubleMatrix(const char *matrix, int *row, int *col);	
	int readIntVector(const char *vector, int *data, int *size);
	int readFloatVector(const char *vector, float *data, int *size);
	int readDoubleVector(const char *vector, double *data, int *size);
	QVector<float> readFloatVector(const char *vector);
	QVector<double> readDoubleVector(const char *vector);
	QVector<int> readIntVector(const char *vector);
	QString readString(const char *dset);
	int readIntAttr(const char *attribute, int *value);
	int readFloatAttr(const char *attribute, float *value);
	int readDoubleAttr(const char *attribute, double *value);
	QString readStringAttr(const char *attribute);


protected:
	int m_status;
	hid_t m_file_id;
	hid_t m_group_id;
	const char *m_group;
	QString m_fileName;
	int m_fileMode;			// File open mode

	int getMatrixSize(const char *matrix, int *size, int *row, int *col);
	int getVectorSize(const char *vector, int *size);
	int getStringSize(const char *string, int *size);
};

#endif // HDF5_H
