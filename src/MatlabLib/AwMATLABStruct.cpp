#include <matlab/AwMATLABStruct.h>
#include "matio.h"
#include <AwException.h>
#include <QFile>

AwMATLABStruct::AwMATLABStruct(matvar_t *var, bool isChild)
{
	m_var = var;
	m_isChild = isChild;
	auto nFields = Mat_VarGetNumberOfFields(var);
	auto fields = Mat_VarGetStructFieldnames(var);
	for (int i = 0; i < nFields; i++) {
		m_fieldsNames << QString::fromLatin1(fields[i]);
	}
}

AwMATLABStruct::AwMATLABStruct(const QString& name, const char **fieldsNames, int nFields, int rank, size_t dims[2])
{
	m_var = Mat_VarCreateStruct(name.toStdString().c_str(), rank, dims, fieldsNames, nFields);
	if (m_var == nullptr)
		throw(AwException("Failed when creating structure."));
	m_isChild = false;

	for (int i = 0; i < nFields; i++)
		m_fieldsNames << QString::fromLatin1(fieldsNames[i]);
}


AwMATLABStruct::~AwMATLABStruct()
{
	if (!m_isChild)
		Mat_VarFree(m_var);
}

int AwMATLABStruct::getDimSize(int dim)
{
	QString origin = "AwMATLABStruct::getDimSize";
	if (dim >= m_var->rank) {
		throw AwException("Incorrect dimension specified.", origin);
		return -1;
	}
	return m_var->dims[dim];
}

int AwMATLABStruct::rank()
{
	return m_var->rank;
}

bool AwMATLABStruct::isVariableEmpty(const QString& field, size_t index)
{
	if (!m_fieldsNames.contains(field))
		return true;
	auto field_matvar = Mat_VarGetStructFieldByName(m_var, field.toStdString().c_str(), index);
	if (field_matvar) {
		if (field_matvar->rank >= 2) {
			if (field_matvar->dims[0] > 0 && field_matvar->dims[1] > 0)
				return false;
		}
	}
	return true;
}

int AwMATLABStruct::saveMatrixToFile(const QString& fieldName, const QString& filePath, size_t index)
{
	auto field_matvar = Mat_VarGetStructFieldByName(m_var, fieldName.toStdString().c_str(), index);
	if (field_matvar == NULL) {
		return -1;
	}
	if (field_matvar->class_type != MAT_C_DOUBLE && field_matvar->class_type !=  MAT_C_SINGLE) {
		return -1;
	}
	auto nrows = field_matvar->dims[0];
	auto ncols = field_matvar->dims[1];
	if (nrows < 1 || ncols < 1)
		return -1;

	QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly))
		return -1;
	if (field_matvar->class_type == MAT_C_DOUBLE) {
		double *source = (double *)field_matvar->data;
		for (auto r = 0; r < nrows; r++) {
			for (auto c = 0; c < ncols; c++) {
				file.write((char *)&source[c * nrows + r], sizeof(double));
			}
		}
	}
	else {
		float *source = (float *)field_matvar->data;
		for (auto r = 0; r < nrows; r++) {
			for (auto c = 0; c < ncols; c++) {
				file.write((char *)&source[c * nrows + r], sizeof(float));
			}
		}
	}
	file.close();
	return 0;
}

int AwMATLABStruct::readMatrix(const QString& fieldName, mat& matrix, size_t index)
{
	// get the field variable
	auto field_matvar = Mat_VarGetStructFieldByName(m_var, fieldName.toStdString().c_str(), index);
	if (field_matvar == NULL) {
		return -1;
	}
	if (field_matvar->class_type != MAT_C_DOUBLE) {
		return -1;
	}
	auto nrows = field_matvar->dims[0];
	auto ncols = field_matvar->dims[1];
	if (nrows < 1 || ncols < 1)
		return -1;
	matrix = mat((double *)field_matvar->data, nrows, ncols);
	return 0;
}

int  AwMATLABStruct::readSingleMatrix(const QString& fieldName, fmat& matrix, size_t index)
{
	// get the field variable
	auto field_matvar = Mat_VarGetStructFieldByName(m_var, fieldName.toStdString().c_str(), index);
	if (field_matvar == NULL) {
		return -1;
	}
	if (field_matvar->class_type != MAT_C_SINGLE) {
		return -1;
	}
	auto nrows = field_matvar->dims[0];
	auto ncols = field_matvar->dims[1];
	if (nrows < 1 || ncols < 1)
		return -1;

	matrix = fmat((float *)field_matvar->data, nrows, ncols);
	return 0;
}

int AwMATLABStruct::readString(const QString& fieldName, QString& string, size_t index)
{
	QString origin = "AwMATLABStruct::readString";
	// get the field variable
	auto field_matvar = Mat_VarGetStructFieldByName(m_var, fieldName.toStdString().c_str(), index);
	if (field_matvar == NULL) {
	//	m_error = QString("Failed to get the field variable");
	//	throw AwException(m_error, origin);
		return -1;
	}
	if (field_matvar->class_type != MAT_C_CHAR) {
		m_error = QString("Variable is not a string.");
		throw AwException(m_error, origin);
		return -1;
	}
	//char dummy[256];
	//size_t length = std::min(size_t(255), field_matvar->dims[0] * field_matvar->dims[1]);
	//memcpy(dummy, (char *)field_matvar->data, length);
	//dummy[length] = '\0';
	//string = QString::fromLatin1(dummy);
	string = QString(static_cast<char*>(field_matvar->data));
	return 0;
}


int AwMATLABStruct::insertString(const QString& fieldName, const QString& string, size_t index)
{
	QByteArray stringData = string.toUtf8();
	//const char* data = string.toStdString().c_str();
	std::string field = fieldName.toStdString();
	//const char* field = fieldName.toStdString().c_str();
	size_t dims[2] = { 1, (size_t)string.size() };
	matvar_t* var = Mat_VarCreate(field.data(), MAT_C_CHAR, MAT_T_UTF8, 2, dims, (void *)stringData.data(), 0);
	if (!var) {
		m_error = QString("Could not create variable for field %1").arg(fieldName);
		throw AwException(m_error);
		return -1;
	}

	if (Mat_VarSetStructFieldByName(m_var, field.data(), index, var))
		return 0;
	m_error = QString("Could not set fieldname %1 variable.").arg(fieldName);
	return -1;
}

int AwMATLABStruct::insertMatrix(const QString& fieldName,  mat& matrix, size_t index)
{
	//const char* field = fieldName.toStdString().c_str();
	std::string field = fieldName.toStdString();
	size_t matrixDims[2] = { size_t(matrix.n_rows), size_t(matrix.n_cols) };
	matvar_t* var = Mat_VarCreate(field.data(), MAT_C_DOUBLE, MAT_T_DOUBLE, 2, matrixDims, matrix.memptr(), 0);
	if (var == nullptr) {
		m_error = QString("Could not create variable for field  %1").arg(fieldName);
		throw AwException(m_error);
		return -1;
	}
	if (Mat_VarSetStructFieldByName(m_var, field.data(), index, var))
		return 0;
	m_error = QString("Could not set fieldname %1 variable.").arg(fieldName);
	return -1;
}

int AwMATLABStruct::insertVector(const QString& fieldName, vec& matrix, size_t index)
{
	std::string field = fieldName.toStdString();
	size_t matrixDims[2] = { size_t(matrix.n_rows), size_t(matrix.n_cols) };
	matvar_t* var = Mat_VarCreate(field.data(), MAT_C_DOUBLE, MAT_T_DOUBLE, 2, matrixDims, matrix.memptr(), 0);
	if (var == nullptr) {
		m_error = QString("Could not create variable for field  %1").arg(fieldName);
		throw AwException(m_error);
		return -1;
	}
	if (Mat_VarSetStructFieldByName(m_var, field.data(), index, var))
		return 0;
	m_error = QString("Could not set fieldname %1 variable.").arg(fieldName);
	return -1;
}


int AwMATLABStruct::readScalar(const QString& fieldName, double *value, size_t index)
{
	QString origin = QString("AwMATLABStruct::readScalar");
	// get the field variable
	auto field_matvar = Mat_VarGetStructFieldByName(m_var, fieldName.toStdString().c_str(), index);
	// do not raise an exception if variable is not found, just return -1 to notify the variable is missing.
	if (field_matvar == NULL) {
	//	m_error = QString("Failed to get the field variable");
	//	throw AwException(m_error, origin);
		return -1;
	}
	if (field_matvar->rank != 2 || field_matvar->dims[0] > 1 || field_matvar->dims[1] > 1) {
		m_error = QString("Variable is not a scalar.");
		throw AwException(m_error, origin);
		return -1;
	}
	if (field_matvar->class_type != MAT_C_DOUBLE) {
		m_error = QString("Variable is not a double.");
		throw AwException(m_error, origin);
		return -1;
	}
	*value = *(double *)field_matvar->data;
	return 0;
}

/// 
/// get a child struct based on the fieldname.
/// If the fiedname does not exist or is empty, returns NULL.
AwMATLABStruct *AwMATLABStruct::getChildStruct(const QString& fieldName, size_t index)
{
	// get the field variable
	auto field_matvar = Mat_VarGetStructFieldByName(m_var, fieldName.toStdString().c_str(), index);
	// update : if field_matvar is null that means the struct is empty => do not raise an exception
	if (field_matvar == NULL)
		return NULL;

	if (field_matvar->class_type != MAT_C_STRUCT)
		return NULL;

	return new AwMATLABStruct(field_matvar, true);
}