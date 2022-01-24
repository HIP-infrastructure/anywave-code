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
#pragma once

#include <AwGlobal.h>
#include <aw_armadillo.h>
#include <AwChannel.h>
#include <AwException.h>


namespace AwMATLAB {
	/** MATLAB file output **/
	int AW_MATLAB_EXPORT saveToMatlab(const QString& fileName, const AwChannelList& channels);
}

class Matio;
class matvar_t;
class AwMATLABStruct;

class AW_MATLAB_EXPORT AwMATLABFile
{
public:
	AwMATLABFile();
	~AwMATLABFile();

	void close();
	/** create a new file : returns 0 on success, -1 on failure **/
	int create(const QString& file);
	/** open file : returns 0 on success, -1 on failure **/
	int open(const QString& file);

	QString& error() { return m_error; }
	QString& fileName() { return m_fileName; }

	int writeStruct(AwMATLABStruct& s);
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

	// structures
	/** check if variable is a structure **/
	bool isStruct(const QString& name);
	/** load a structure variable and return a MATLAB Structure instance. Returns NULL if failure. **/
	AwMATLABStruct *getStruct(const QString& name);
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
	// general methods
	bool variableExists(const QString& name);
protected:
    Matio *m_matio;
	QString m_error, m_fileName;
	bool m_isOpen;
	size_t m_scalarDims[2];
};
