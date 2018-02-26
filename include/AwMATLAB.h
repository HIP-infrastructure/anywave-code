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
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <AwGlobal.h>
#include <aw_armadillo.h>
#include "matio.h"
#include <AwChannel.h>


namespace AwMATLAB {
	/** MATLAB file output **/
	int AW_MATLAB_EXPORT saveToMatlab(const QString& fileName, const AwChannelList& channels);
}

class AW_MATLAB_EXPORT AwMATLABFile
{
public:
	AwMATLABFile();
	~AwMATLABFile();

	void close();
	int create(const QString& file);
	int open(const QString& file);

	QString& error() { return m_error; }
	mat_t *filePtr() { return m_fileptr; }
	QString& fileName() { return m_fileName; }
	// WRITE
	int writeScalar(const QString& name, double value);
	int writeScalar(const QString& name, float value);
	int writeScalar(const QString& name, qint32 value);
	int writeScalar(const QString& name, qint16 value);
	int writeScalar(const QString& name, qint64 value);
	int writeString(const QString& name, const QString& value);
	int writeStringCellArray(const QString& name, const QStringList& values);
	int writeMatrix(const QString& name, fmat& matrix);
	int writeMatrix(const QString& name, mat& matrix);
	int writeMatrix(const QString& name, cube& matrix);
	int writeMatrix(const QString& name, fcube& matrix);
	int writeVec(const QString& name, fvec& vec);
	int writeVec(const QString& name, vec& vec);
	int writeVec(const QString& name, rowvec& vec);
	int writeVec(const QString& name, QVector<double>& vector);
	int writeVec(const QString& name, QVector<float>& vector);
	int writeVec(const QString& name, QVector<qint32>& vector);
	int writeVec(const QString& name, QVector<qint16>& vector);
	// Structures 
	matvar_t *createStruct(const QString& name, const char **fields, int nFields);
	int setStructField(matvar_t *structrure, const char *fieldName, mat& matrix);
	int setStructField(matvar_t *structrure, const char *fieldName, double scalar);
	int setStructField(matvar_t *structrure, const char *fieldName, const QStringList& strings);
	// READ
	int readScalar(const QString& name, double *value);
	int readScalar(const QString& name, float *value);
	int readScalar(const QString& name, qint32 *value);
	int readScalar(const QString& name, qint16 *value);
	int readMatrix(const QString& name, mat& matrix);
	int readMatrix(const QString& name, fmat& matrix);
	int readMatrix(const QString& name, cube& cube);
	int readMatrix(const QString& name, fcube& cube);
	int readStrings(const QString& name, QStringList& strings);
	int readString(const QString& name, QString& string);
	int readVec(const QString& name, vec& vector);
	int readVec(const QString& name, fvec& vector);
	int readVec(const QString& name, QVector<float>& vector);
	int readVec(const QString& name, QVector<double>& vector);
	int readVec(const QString& name, QVector<qint32>& vector);
	int readVec(const QString& name, QVector<qint16>& vector);
protected:
	mat_t *m_fileptr;
	QString m_error, m_fileName;
	bool m_isOpen;
	size_t m_scalarDims[2];
};
