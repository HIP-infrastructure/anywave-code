#pragma once

class matvar_t;
#include <QStringList>
#include <AwGlobal.h>
#include <aw_armadillo.h>

class AW_MATLAB_EXPORT AwMATLABStruct
{
public:
	AwMATLABStruct(matvar_t *var, bool isChild = false);
	~AwMATLABStruct();

	int getDimSize(int dim);
	int rank();
	int readString(const QString& field, QString& value, size_t index);
	int readScalar(const QString& field, double *value, size_t index);
	int readMatrix(const QString& field, mat& matrix, size_t index);
	int readSingleMatrix(const QString& field, fmat& matrix, size_t index);
	/** Save a variable (a matrix is required) to a binary file in row order storage order. **/
	int saveMatrixToFile(const QString& field, const QString& path, size_t index);

	AwMATLABStruct* getChildStruct(const QString& field, size_t index);
	inline QString& errorString() { return m_error; }
	/** check if a field variable is empty. **/
	bool isVariableEmpty(const QString& field, size_t index);

protected:
	bool m_isChild;
	matvar_t *m_var;
	QStringList m_fieldsNames;
	QString m_error;
};