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

class matvar_t;
#include <QStringList>
#include <AwGlobal.h>
#include <aw_armadillo.h>


class AW_MATLAB_EXPORT AwMATLABStruct
{
public:
	explicit AwMATLABStruct(matvar_t *var, bool isChild = false);
	explicit AwMATLABStruct(const QString& name, const char **fields, int nFields, int rank, size_t dims[2]);
	~AwMATLABStruct();

	int getDimSize(int dim);
	int rank();
	int readString(const QString& field, QString& value, size_t index);
	int readScalar(const QString& field, double *value, size_t index);
	int readMatrix(const QString& field, mat& matrix, size_t index);
	int readSingleMatrix(const QString& field, fmat& matrix, size_t index);
	/** Save a variable (a matrix is required) to a binary file in row order storage order. **/
	int saveMatrixToFile(const QString& field, const QString& path, size_t index);

	int insertString(const QString& field, const QString& value, size_t index);
	int insertMatrix(const QString& field,  mat& matrix, size_t index);
	int insertVector(const QString& field, vec& matrix, size_t index);


	AwMATLABStruct* getChildStruct(const QString& field, size_t index);
	inline QString& errorString() { return m_error; }
	/** check if a field variable is empty. **/
	bool isVariableEmpty(const QString& field, size_t index);
	inline const QStringList& fieldsNames() { return m_fieldsNames; }

	matvar_t* var() { return m_var; }
protected:
	bool m_isChild;
	matvar_t *m_var;
	QStringList m_fieldsNames;
	QString m_error;
};