#include <matlab/AwMATLABStruct.h>
#include "matio.h"
#include <AwException.h>

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

int AwMATLABStruct::readString(const QString& fieldName, QString& string, size_t index)
{
	QString origin = "AwMATLABStruct::readString";
	// get the field variable
	auto field_matvar = Mat_VarGetStructFieldByName(m_var, fieldName.toStdString().c_str(), index);
	if (field_matvar == NULL) {
		m_error = QString("Failed to get the field variable");
		throw AwException(m_error, origin);
		return -1;
	}
	if (field_matvar->class_type != MAT_C_CHAR) {
		m_error = QString("Variable is not a string.");
		throw AwException(m_error, origin);
		return -1;
	}
	char dummy[256];
	size_t length = std::min(size_t(255), field_matvar->dims[0] * field_matvar->dims[1]);
	memcpy(dummy, (char *)field_matvar->data, length);
	dummy[length] = '\0';
	string = QString::fromLatin1(dummy);
	return 0;
}

int AwMATLABStruct::readScalar(const QString& fieldName, double *value, size_t index)
{
	QString origin = QString("AwMATLABStruct::readScalar");
	// get the field variable
	auto field_matvar = Mat_VarGetStructFieldByName(m_var, fieldName.toStdString().c_str(), index);
	if (field_matvar == NULL) {
		m_error = QString("Failed to get the field variable");
		throw AwException(m_error, origin);
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

AwMATLABStruct *AwMATLABStruct::getChildStruct(const QString& fieldName, size_t index)
{
	QString origin = QString("AwMATLABStruct::getChildStruct");
	// get the field variable
	auto field_matvar = Mat_VarGetStructFieldByName(m_var, fieldName.toStdString().c_str(), index);
	if (field_matvar == NULL) {
		m_error = QString("Failed to get the field variable");
		throw AwException(m_error, origin);
		return NULL;
	}
	if (field_matvar->class_type != MAT_C_STRUCT) {
		m_error = QString("Variable is not a struct.");
		throw AwException(m_error, origin);
		return NULL;
	}
	return new AwMATLABStruct(field_matvar, true);
}