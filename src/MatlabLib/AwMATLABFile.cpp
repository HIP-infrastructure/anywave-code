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
#include <matlab/AwMATLAB.h>
#include <matlab/AwMATLABStruct.h>
#include "matio.h"
#include <QDir>

#define CHECK_OPEN_FILE if (m_matio->fileptr == NULL) { m_error = "No file is open."; return -1;	}
#define FILEPTR m_matio->fileptr
#include <AwException.h>

class Matio
{
public:
    Matio() : fileptr(NULL), m_handles(-1) {}
	~Matio() {
		for (auto s : struct_handles.values()) {
			Mat_VarFree(s);
		}
		for (auto s : m_structNames.values()) {
			Mat_VarFree(s);
		}
	}
    mat_t *fileptr;

	int addStruct(matvar_t *s) {
		struct_handles.insert(++m_handles, s); 
		return m_handles;
	}
	matvar_t* getStruct(int handle) {
		return struct_handles.value(handle);
	}
	int insertStruct(const QString& name, matvar_t *s);
	int insertChildStruct(const QString& name, matvar_t *s);
	matvar_t *getStruct(const QString& name) { return m_structNames.value(name); }
	void setFieldNames(const QString& name, const QStringList& fields) { m_structFields[name] = fields; }
	QStringList getFieldNames(const QString& name) { return m_structFields.value(name); }
protected:
	int m_handles;
	QMap<int, matvar_t *> struct_handles;
	QMap<QString, matvar_t *> m_structNames; // loaded structs indexed by a name (must be destroyed at the end).
	QMap<QString,  matvar_t *> m_structChildren; // child structures, MUST NOT destroyed at the end.
	QMap<QString, QStringList> m_structFields;
};

int Matio::insertStruct(const QString& name, matvar_t *s)
{
	if (m_structNames.contains(name)) {
		throw AwException("structure with same name already exists.", "Matio::insertStruct");
		return  -1;
	}
	m_structNames[name] = s;
	QStringList res;
	auto nFields = Mat_VarGetNumberOfFields(s);
	auto fields = Mat_VarGetStructFieldnames(s);
	for (int i = 0; i < nFields; i++) {
		res << QString::fromLatin1(fields[i]);
	}
	setFieldNames(name, res);
	return 0;
}

int Matio::insertChildStruct(const QString& name, matvar_t *s)
{
	if (m_structChildren.contains(name)) {
		throw AwException("structure with same name already exists.", "Matio::insertChildStruct");
		return  -1;
	}
	m_structChildren[name] = s;
	QStringList res;
	auto nFields = Mat_VarGetNumberOfFields(s);
	auto fields = Mat_VarGetStructFieldnames(s);
	for (int i = 0; i < nFields; i++) {
		res << QString::fromLatin1(fields[i]);
	}
	setFieldNames(name, res);
	return 0;
}


AwMATLABFile::AwMATLABFile()
{
    //m_fileptr = NULL;
    m_matio = new Matio;
	m_scalarDims[0] = 1;
	m_scalarDims[1] = 1;
	m_isOpen = false;
}

AwMATLABFile::~AwMATLABFile()
{
	close();
    delete m_matio;
}

///
/// open a file for reading.
int AwMATLABFile::open(const QString& file)
{
	close();
	QString fileName = QDir::toNativeSeparators(file);
	std::string str = fileName.toStdString();
    FILEPTR = Mat_Open(str.data(), MAT_ACC_RDONLY);
    if (FILEPTR == nullptr) {
		m_error = "Could not open file for reading.";
		throw AwException(m_error, "AwMATLABFile::open");
		return -1;
	}
	m_isOpen = true;
	m_fileName = file;
	return 0;
}


int AwMATLABFile::create(const QString& file)
{
	close();
	QString fileName = QDir::toNativeSeparators(file);
	std::string str = fileName.toStdString();
    FILEPTR = Mat_CreateVer(str.data(), nullptr, MAT_FT_MAT5);
    if (FILEPTR == nullptr) {
		m_error = QString("Could not create file: %1").arg(file);
		throw AwException(m_error, "AwMATLABFile::create");
		return -1;
	}
	m_isOpen = true;
	m_fileName = file;
	return 0;
}
 
void AwMATLABFile::close()
{
    if (FILEPTR && m_isOpen) {
        Mat_Close(FILEPTR);
		m_isOpen = false;
		m_fileName.clear();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Write

int AwMATLABFile::writeStruct( AwMATLABStruct& s)
{
	CHECK_OPEN_FILE
	Mat_VarWrite(FILEPTR, s.var(), MAT_COMPRESSION_NONE);
	//Mat_VarFree(s.var());   /// don't free the pointer here as it is deleted in AwMATLABStruct desctructor
	return 0;
}

int AwMATLABFile::writeScalar(const QString& name, double value)
{
	CHECK_OPEN_FILE

	double tmp = value;
	std::string str = name.toStdString();
	matvar_t *var = Mat_VarCreate(str.data(), MAT_C_DOUBLE, MAT_T_DOUBLE, 2, m_scalarDims, &tmp, 0);
	if (var == NULL) {
		m_error = QString("Could not create variable %1").arg(name);
		throw AwException(m_error, "AwMATLABFile::writeScalar");
		return -1;
	}
    Mat_VarWrite(FILEPTR, var, MAT_COMPRESSION_NONE);
	Mat_VarFree(var);
	return 0;
}

int AwMATLABFile::writeScalar(const QString& name, float value)
{
	CHECK_OPEN_FILE
	float tmp = value;
	std::string str = name.toStdString();
	matvar_t *var = Mat_VarCreate(str.data(), MAT_C_SINGLE, MAT_T_SINGLE, 2, m_scalarDims, &tmp, 0);
	if (var == NULL) {
		m_error = QString("Could not create variable %1").arg(name);
		throw AwException(m_error, "AwMATLABFile::writeScalar");
		return -1;
	}
    Mat_VarWrite(FILEPTR, var, MAT_COMPRESSION_NONE);
	Mat_VarFree(var);
	return 0;
}

int AwMATLABFile::writeScalar(const QString& name, qint32 value)
{
	CHECK_OPEN_FILE
	qint32 tmp = value;
	std::string str = name.toStdString();
	matvar_t *var = Mat_VarCreate(str.data(), MAT_C_INT32, MAT_T_INT32, 2, m_scalarDims, &tmp, 0);
	if (var == NULL) {
		m_error = QString("Could not create variable %1").arg(name);
		throw AwException(m_error, "AwMATLABFile::writeScalar");
		return -1;
	}
    Mat_VarWrite(FILEPTR, var, MAT_COMPRESSION_NONE);
	Mat_VarFree(var);
	return 0;
}

int AwMATLABFile::writeScalar(const QString& name, qint64 value)
{
	CHECK_OPEN_FILE
	qint64 tmp = value;
	std::string str = name.toStdString();
	matvar_t *var = Mat_VarCreate(str.data(), MAT_C_INT64, MAT_T_INT64, 2, m_scalarDims, &tmp, 0);
	if (var == NULL) {
		m_error = QString("Could not create variable %1").arg(name);
		throw AwException(m_error, "AwMATLABFile::writeScalar");
		return -1;
	}
    Mat_VarWrite(FILEPTR, var, MAT_COMPRESSION_NONE);
	Mat_VarFree(var);
	return 0;
}

int AwMATLABFile::writeScalar(const QString& name, qint16 value)
{
	CHECK_OPEN_FILE
	qint16 tmp = value;
	std::string str = name.toStdString();
	matvar_t *var = Mat_VarCreate(str.data(), MAT_C_INT16, MAT_T_INT16, 2, m_scalarDims, &tmp, 0);
	if (var == NULL) {
		m_error = QString("Could not create variable %1").arg(name);
		throw AwException(m_error, "AwMATLABFile::writeScalar");
		return -1;
	}
    Mat_VarWrite(FILEPTR, var, MAT_COMPRESSION_NONE);
	Mat_VarFree(var);
	return 0;
}

int AwMATLABFile::writeString(const QString& name, const QString& value)
{
	CHECK_OPEN_FILE
	size_t dims[2];
	std::string s_ = value.toStdString();
	dims[0] = 1;
	dims[1] = s_.size();
	std::string str = name.toStdString();
	matvar_t *var = Mat_VarCreate(str.data(), MAT_C_CHAR, MAT_T_UINT8, 2, dims, (void *)s_.data(), 0);
	if (var == NULL) {
		m_error = QString("Could not create variable %1").arg(name);
		throw AwException(m_error, "AwMATLABFile::writeString");
		return -1;
	}
    Mat_VarWrite(FILEPTR, var, MAT_COMPRESSION_NONE);
	Mat_VarFree(var);
	return 0;
}

int AwMATLABFile::writeStringCellArray(const QString& name, const QStringList& values)
{
	CHECK_OPEN_FILE
	size_t dims[2] = { 1, (size_t)values.size() };
	std::string str = name.toStdString();
	matvar_t *var = Mat_VarCreate(str.data(), MAT_C_CELL, MAT_T_CELL, 2, dims, NULL, 0);
	if (var == NULL) {
		m_error = QString("Could not create variable %1").arg(name);
		throw AwException(m_error, "AwMATLABFile::writeStringCellArray");
		return -1;
	}
	size_t stringDims[2];
	char dummy[256];
	for (int i = 0; i < values.size(); i++) {
		QString v = values.at(i);
		int length = std::min(255, v.size());
		memcpy(dummy, v.toLatin1().data(), length);
		dummy[length] = '\0';
		stringDims[0] = 1;
		stringDims[1] = length;
		matvar_t *string = Mat_VarCreate("string", MAT_C_CHAR, MAT_T_UINT8, 2, stringDims, dummy, 0);
		if (string == NULL) {
			m_error = QString("Error creating string for cell array %1").arg(name);
			Mat_VarFree(var);
			throw AwException(m_error, "AwMATLABFile::writeStringCellArray");
			return -1;
		}
		Mat_VarSetCell(var, i, string);
	}
    Mat_VarWrite(FILEPTR, var, MAT_COMPRESSION_NONE);
	Mat_VarFree(var);
	return 0;
}

int AwMATLABFile::writeMatrix(const QString& name, fmat& matrix)
{
	CHECK_OPEN_FILE
	size_t matrixDims[2] = { size_t(matrix.n_rows), size_t(matrix.n_cols) };
	std::string str = name.toStdString();
	matvar_t *var = Mat_VarCreate(str.data(), MAT_C_SINGLE, MAT_T_SINGLE, 2, matrixDims, matrix.memptr(), 0);
	if (var == NULL) {
		m_error = QString("Could not create variable %1").arg(name);
		throw AwException(m_error, "AwMATLABFile::writeMatrix");
		return -1;
	}
    Mat_VarWrite(FILEPTR, var, MAT_COMPRESSION_NONE);
	Mat_VarFree(var);
	return 0;
}

int AwMATLABFile::writeMatrix(const QString& name, cube& matrix)
{
	CHECK_OPEN_FILE
	size_t matrixDims[3] = { size_t(matrix.n_rows), size_t(matrix.n_cols), size_t(matrix.n_slices) };
	std::string str = name.toStdString();
	matvar_t *var = Mat_VarCreate(str.data(), MAT_C_DOUBLE, MAT_T_DOUBLE, 3, matrixDims, matrix.memptr(), 0);
	if (var == NULL) {
		m_error = QString("Could not create variable %1").arg(name);
		throw AwException(m_error, "AwMATLABFile::writeMatrix");
		return -1;
	}
    Mat_VarWrite(FILEPTR, var, MAT_COMPRESSION_NONE);
	Mat_VarFree(var);
	return 0;
}

int AwMATLABFile::writeMatrix(const QString& name, fcube& matrix)
{
	CHECK_OPEN_FILE
	size_t matrixDims[3] = { size_t(matrix.n_rows), size_t(matrix.n_cols), size_t(matrix.n_slices) };
	std::string str = name.toStdString();
	matvar_t *var = Mat_VarCreate(str.data(), MAT_C_SINGLE, MAT_T_SINGLE, 3, matrixDims, matrix.memptr(), 0);
	if (var == NULL) {
		m_error = QString("Could not create variable %1").arg(name);
		throw AwException(m_error, "AwMATLABFile::writeMatrix");
		return -1;
	}
    Mat_VarWrite(FILEPTR, var, MAT_COMPRESSION_NONE);
	Mat_VarFree(var);
	return 0;
}

int AwMATLABFile::writeMatrix(const QString& name, mat& matrix)
{
	CHECK_OPEN_FILE
	size_t matrixDims[2] = { size_t(matrix.n_rows), size_t(matrix.n_cols) };
	std::string str = name.toStdString();
	if (matrix.n_cols == 0 || matrix.n_rows == 0) {
		m_error = QString("Matrix is empty");
		throw AwException(m_error, "AwMATLABFile:writeMatrix");
		return -1;
	}
	matvar_t *var = Mat_VarCreate(str.data(), MAT_C_DOUBLE, MAT_T_DOUBLE, 2, matrixDims, matrix.memptr(), 0);
	if (var == NULL) {
		m_error = QString("Could not create variable %1").arg(name);
		throw AwException(m_error, "AwMATLABFile:writeMatrix");
		return -1;
	}
    Mat_VarWrite(FILEPTR, var, MAT_COMPRESSION_NONE);
	Mat_VarFree(var);
	return 0;
}

int AwMATLABFile::writeVec(const QString& name, QVector<float>& vec)
{
	CHECK_OPEN_FILE

	size_t matrixDims[2] = { 1, size_t(vec.size()) };
	std::string str = name.toStdString();
	matvar_t *var = Mat_VarCreate(str.data(), MAT_C_SINGLE, MAT_T_SINGLE, 2, matrixDims, vec.data(), 0);
	if (var == NULL) {
		m_error = QString("Could not create variable %1").arg(name);
		throw AwException(m_error, "AwMATLABFile:writeVec");
		return -1;
	}
    Mat_VarWrite(FILEPTR, var, MAT_COMPRESSION_NONE);
	Mat_VarFree(var);
	return 0;
}

int AwMATLABFile::writeVec(const QString& name, QVector<qint32>& vec)
{
	CHECK_OPEN_FILE

	size_t matrixDims[2] = { 1, size_t(vec.size()) };

	matvar_t *var = Mat_VarCreate(name.toStdString().c_str(), MAT_C_INT32, MAT_T_INT32, 2, matrixDims, vec.data(), 0);
	if (var == NULL) {
		m_error = QString("Could not create variable %1").arg(name);
		throw AwException(m_error, "AwMATLABFile:writeVec");
		return -1;
	}
    Mat_VarWrite(FILEPTR, var, MAT_COMPRESSION_NONE);
	Mat_VarFree(var);
	return 0;
}

int AwMATLABFile::writeVec(const QString& name, QVector<qint16>& vec)
{
	CHECK_OPEN_FILE
	std::string str = name.toStdString();
	size_t matrixDims[2] = { 1, size_t(vec.size()) };
	matvar_t *var = Mat_VarCreate(str.data(), MAT_C_INT16, MAT_T_INT16, 2, matrixDims, vec.data(), 0);
	if (var == NULL) {
		m_error = QString("Could not create variable %1").arg(name);
		throw AwException(m_error, "AwMATLABFile:writeVec");
		return -1;
	}
    Mat_VarWrite(FILEPTR, var, MAT_COMPRESSION_NONE);
	Mat_VarFree(var);
	return 0;
}

int AwMATLABFile::writeVec(const QString& name, QVector<double>& vec)
{
	CHECK_OPEN_FILE

	size_t matrixDims[2] = { 1, size_t(vec.size()) };
	std::string str = name.toStdString();
	matvar_t *var = Mat_VarCreate(str.data(), MAT_C_DOUBLE, MAT_T_DOUBLE, 2, matrixDims, vec.data(), 0);
	if (var == NULL) {
		m_error = QString("Could not create variable %1").arg(name);
		throw AwException(m_error, "AwMATLABFile:writeVec");
		return -1;
	}
    Mat_VarWrite(FILEPTR, var, MAT_COMPRESSION_NONE);
	Mat_VarFree(var);
	return 0;
}

int AwMATLABFile::writeVec(const QString& name, fvec& vec)
{
	CHECK_OPEN_FILE;

	size_t matrixDims[2];
	if (vec.is_colvec()) {
		matrixDims[0] = size_t(1);
		matrixDims[1] = size_t(vec.n_elem);
	}
	else {
		matrixDims[1] = size_t(1);
		matrixDims[0] = size_t(vec.n_elem);
	}
	std::string str = name.toStdString();
	matvar_t *var = Mat_VarCreate(str.data(), MAT_C_SINGLE, MAT_T_SINGLE, 2, matrixDims, vec.memptr(), 0);
	if (var == NULL) {
		m_error = QString("Could not create variable %1").arg(name);
		throw AwException(m_error, "AwMATLABFile:writeVec");
		return -1;
	}
    Mat_VarWrite(FILEPTR, var, MAT_COMPRESSION_NONE);
	Mat_VarFree(var);
	return 0;
}

int AwMATLABFile::writeVec(const QString& name, rowvec& vec)
{
	CHECK_OPEN_FILE;
	size_t matrixDims[2];
	if (vec.is_colvec()) {
		matrixDims[0] = size_t(1);
		matrixDims[1] = size_t(vec.n_elem);
	}
	else {
		matrixDims[1] = size_t(1);
		matrixDims[0] = size_t(vec.n_elem);
	}
	matvar_t *var = Mat_VarCreate(name.toStdString().c_str(), MAT_C_DOUBLE, MAT_T_DOUBLE, 2, matrixDims, vec.memptr(), 0);
	if (var == NULL) {
		m_error = QString("Could not create variable %1").arg(name);
		throw AwException(m_error, "AwMATLABFile:writeVec");
		return -1;
	}
    Mat_VarWrite(FILEPTR, var, MAT_COMPRESSION_NONE);
	Mat_VarFree(var);
	return 0;
}

int AwMATLABFile::writeVec(const QString& name, vec& vec)
{
	CHECK_OPEN_FILE;
	size_t matrixDims[2];
	if (vec.is_colvec()) {
		matrixDims[0] = size_t(1);
		matrixDims[1] = size_t(vec.n_elem);
	}
	else {
		matrixDims[1] = size_t(1);
		matrixDims[0] = size_t(vec.n_elem);
	}
	matvar_t *var = Mat_VarCreate(name.toStdString().c_str(), MAT_C_DOUBLE, MAT_T_DOUBLE, 2, matrixDims, vec.memptr(), 0);
	if (var == NULL) {
		m_error = QString("Could not create variable %1").arg(name);
		throw AwException(m_error, "AwMATLABFile:writeVec");
		return -1;
	}
    Mat_VarWrite(FILEPTR, var, MAT_COMPRESSION_NONE);
	Mat_VarFree(var);
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Read

int AwMATLABFile::readScalar(const QString& name, double* value)
{
	CHECK_OPEN_FILE;
    matvar_t *var = Mat_VarReadInfo(FILEPTR, name.toStdString().c_str());
	if (var == NULL) {
		m_error = QString("Variable %1 not found.").arg(name);
		throw AwException(m_error, "AwMATLABFile:readScalar");
		return -1;
	}
	if (var->isComplex) {
		m_error = QString("Variable is complex.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile:readScalar");
		return -1;
	}
	if (var->rank != 2 || var->dims[0] > 1 || var->dims[1] > 1) {
		m_error = QString("Variable is not a scalar.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile:readScalar");
		return -1;
	}
	if (var->class_type != MAT_C_DOUBLE) {
		m_error = QString("Variable is not a double.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile:readScalar");
		return -1;
	}
    Mat_VarReadDataAll(FILEPTR, var);
	*value = *(double *)var->data;
	Mat_VarFree(var);
	return 0;
}

int AwMATLABFile::readScalar(const QString& name, float* value)
{
	CHECK_OPEN_FILE
        matvar_t *var = Mat_VarReadInfo(FILEPTR, name.toStdString().c_str());
	if (var == NULL) {
		m_error = QString("Variable %1 not found.").arg(name);
		throw AwException(m_error, "AwMATLABFile:readScalar");
		return -1;
	}
	if (var->isComplex) {
		m_error = QString("Variable is complex.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile:readScalar");
		return -1;
	}
	if (var->rank != 2 || var->dims[0] > 1 || var->dims[1] > 1) {
		m_error = QString("Variable is not a scalar.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile:readScalar");
		return -1;
	}
	if (var->class_type != MAT_C_SINGLE) {
		m_error = QString("Variable is not a single.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile:readScalar");
		return -1;
	}
    Mat_VarReadDataAll(FILEPTR, var);
	*value = *(float *)var->data;
	Mat_VarFree(var);
	return 0;
}

int AwMATLABFile::readScalar(const QString& name, qint32* value)
{
	CHECK_OPEN_FILE
        matvar_t *var = Mat_VarReadInfo(FILEPTR, name.toStdString().c_str());
	if (var == NULL) {
		m_error = QString("Variable %1 not found.").arg(name);
		throw AwException(m_error, "AwMATLABFile:readScalar");
		return -1;
	}
	if (var->isComplex) {
		m_error = QString("Variable is complex.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile:readScalar");
		return -1;
	}
	if (var->rank != 2 || var->dims[0] > 1 || var->dims[1] > 1) {
		m_error = QString("Variable is not a scalar.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile:readScalar");
		return -1;
	}
	if (var->class_type != MAT_C_INT32) {
		m_error = QString("Variable is not an integer.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile:readScalar");
		return -1;
	}
    Mat_VarReadDataAll(FILEPTR, var);
	*value = *(qint32 *)var->data;
	Mat_VarFree(var);
	return 0;
}

int AwMATLABFile::readScalar(const QString& name, qint16 *value)
{
	CHECK_OPEN_FILE
        matvar_t *var = Mat_VarReadInfo(FILEPTR, name.toStdString().c_str());
	if (var == NULL) {
		m_error = QString("Variable %1 not found.").arg(name);
		throw AwException(m_error, "AwMATLABFile:readScalar");
		return -1;
	}
	if (var->isComplex) {
		m_error = QString("Variable is complex.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile:readScalar");
		return -1;
	}
	if (var->rank != 2 || var->dims[0] > 1 || var->dims[1] > 1) {
		m_error = QString("Variable is not a scalar.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile:readScalar");
		return -1;
	}
	if (var->class_type != MAT_C_INT16) {
		m_error = QString("Variable is not a short integer.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile:readScalar");
		return -1;
	}
    Mat_VarReadDataAll(FILEPTR, var);
	*value = *(qint16 *)var->data;
	Mat_VarFree(var);
	return 0;
}

int AwMATLABFile::readString(const QString& name, QString& string)
{
	CHECK_OPEN_FILE
    matvar_t *var = Mat_VarReadInfo(FILEPTR, name.toStdString().c_str());
	if (var == NULL) {
		m_error = QString("Variable %1 not found.").arg(name);
		throw AwException(m_error, "AwMATLABFile:readString");
		return -1;
	}
	if (var->class_type != MAT_C_CHAR) {
		m_error = QString("Variable is not a string.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile:readString");
		return -1;
	}
    Mat_VarReadDataAll(FILEPTR, var);
	std::string s_;
	size_t size = var->dims[0] * var->dims[1];
	s_.resize(size);
	std::memcpy((void *)s_.data(), var->data, size);
	string = QString::fromStdString(s_);
	Mat_VarFree(var);
	return 0;
}

int AwMATLABFile::readStrings(const QString& name, QStringList& strings)
{
	QStringList list;
	CHECK_OPEN_FILE
    matvar_t *var = Mat_VarReadInfo(FILEPTR, name.toStdString().c_str());
	if (var == NULL) {
		m_error = QString("Variable %1 not found.").arg(name);
		throw AwException(m_error, "AwMATLABFile:readStrings");
		return -1;
	}
	if (var->class_type != MAT_C_CELL) {
		m_error = QString("Variable is not a cell array.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile:readStrings");
		return -1;
	}
    Mat_VarReadDataAll(FILEPTR, var);
	matvar_t **array = (matvar_t **)var->data;
	for (size_t i = 0; i < var->dims[0] * var->dims[1]; i++) {
		matvar_t *item = array[i];
		if (item->class_type == MAT_C_CHAR /*&& item->data_type == MAT_T_UINT8*/) {
			std::string s_;
			size_t size = item->dims[0] * item->dims[1];
			s_.resize(size);
			std::memcpy((void*)s_.data(), item->data, size);

			list << QString::fromStdString(s_);
		}
	}
	Mat_VarFree(var);
	strings = list;
	return 0;
}

int AwMATLABFile::readMatrix(const QString& name, fmat& matrix)
{
	CHECK_OPEN_FILE
        matvar_t *var = Mat_VarReadInfo(FILEPTR, name.toStdString().c_str());
	if (var == NULL) {
		m_error = QString("Variable %1 not found.").arg(name);
		throw AwException(m_error, "AwMATLABFile::readMatrix");
		return -1;
	}
	if (var->isComplex) {
		m_error = QString("Variable is complex.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readMatrix");
		return -1;
	}
	if (var->rank != 2) {
		m_error = QString("Variable is not a 2D matrix.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readMatrix");
		return -1;
	}
	if (var->class_type != MAT_C_SINGLE) {
		m_error = QString("Variable is not a matrix of single.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readMatrix");
		return -1;
	}
    Mat_VarReadDataAll(FILEPTR, var);
	matrix = fmat((float *)var->data, var->dims[0], var->dims[1]);
	Mat_VarFree(var);
	return 0;
}

int AwMATLABFile::readMatrix(const QString& name, cube& matrix)
{
	CHECK_OPEN_FILE
    matvar_t *var = Mat_VarReadInfo(FILEPTR, name.toStdString().c_str());
	if (var == NULL) {
		m_error = QString("Variable %1 not found.").arg(name);
		throw AwException(m_error, "AwMATLABFile::readMatrix");
		return -1;
	}
	if (var->isComplex) {
		m_error = QString("Variable is complex.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readMatrix");
		return -1;
	}
	if (var->rank != 3) {
		m_error = QString("Variable is not a 3D matrix.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readMatrix");
		return -1;
	}
	if (var->class_type != MAT_C_DOUBLE) {
		m_error = QString("Variable is not a matrix of double.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readMatrix");
		return -1;
	}
    Mat_VarReadDataAll(FILEPTR, var);
	matrix = cube((double *)var->data, var->dims[0], var->dims[1], var->dims[2]);
	Mat_VarFree(var);
	return 0;
}

int AwMATLABFile::readMatrix(const QString& name, fcube& matrix)
{
	CHECK_OPEN_FILE
        matvar_t *var = Mat_VarReadInfo(FILEPTR, name.toStdString().c_str());
	if (var == NULL) {
		m_error = QString("Variable %1 not found.").arg(name);
		throw AwException(m_error, "AwMATLABFile::readMatrix");
		return -1;
	}
	if (var->isComplex) {
		m_error = QString("Variable is complex.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readMatrix");
		return -1;
	}
	if (var->rank != 3) {
		m_error = QString("Variable is not a 3D matrix.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readMatrix");
		return -1;
	}
	if (var->class_type != MAT_C_SINGLE) {
		m_error = QString("Variable is not a matrix of single.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readMatrix");
		return -1;
	}
    Mat_VarReadDataAll(FILEPTR, var);
	matrix = fcube((float *)var->data, var->dims[0], var->dims[1], var->dims[2]);
	Mat_VarFree(var);
	return 0;
}

int AwMATLABFile::readMatrix(const QString& name, mat& matrix)
{
	CHECK_OPEN_FILE
    matvar_t *var = Mat_VarReadInfo(FILEPTR, name.toStdString().c_str());
	if (var == NULL) {
		m_error = QString("Variable %1 not found.").arg(name);
		throw AwException(m_error, "AwMATLABFile::readMatrix");
		return -1;
	}
	if (var->isComplex) {
		m_error = QString("Variable is complex.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readMatrix");
		return -1;
	}
	if (var->rank != 2)  {
		m_error = QString("Variable is not a 2D matrix.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readMatrix");
		return -1;
	}
	if (var->class_type != MAT_C_DOUBLE) {
		m_error = QString("Variable is not a matrix of double.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readMatrix");
		return -1;
	}
    Mat_VarReadDataAll(FILEPTR, var);
	matrix = mat((double *)var->data, var->dims[0], var->dims[1]);
	Mat_VarFree(var);
	return 0;
}

int AwMATLABFile::readVec(const QString& name, vec& vector)
{
	CHECK_OPEN_FILE
    matvar_t *var = Mat_VarReadInfo(FILEPTR, name.toStdString().c_str());
	if (var == NULL) {
		m_error = QString("Variable %1 not found.").arg(name);
		throw AwException(m_error, "AwMATLABFile::readVec");
		return -1;
	}
	if (var->isComplex) {
		m_error = QString("Variable is complex.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readVec");
		return -1;
	}
	if (var->rank != 2) {
		m_error = QString("Variable is not a 2D matrix.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readVec");
		return -1;
	}
	if (var->class_type != MAT_C_DOUBLE) {
		m_error = QString("Variable is not a vector of double.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readVec");
		return -1;
	}
	if (var->dims[0] != 1 && var->dims[1] != 1) {
		m_error = QString("Variable is not a vector.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readVec");
		return -1;
	}
    Mat_VarReadDataAll(FILEPTR, var);
	vector = vec((double *)var->data, var->dims[0], var->dims[1]);
	Mat_VarFree(var);
	return 0;
}

int AwMATLABFile::readVec(const QString& name, QVector<float>& vector)
{
	CHECK_OPEN_FILE
    matvar_t *var = Mat_VarReadInfo(FILEPTR, name.toStdString().c_str());
	if (var == NULL) {
		m_error = QString("Variable %1 not found.").arg(name);
		throw AwException(m_error, "AwMATLABFile::readVec");
		return -1;
	}
	if (var->isComplex) {
		m_error = QString("Variable is complex.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readVec");
		return -1;
	}
	if (var->rank != 2) {
		m_error = QString("Variable is not a 2D matrix.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readVec");
		return -1;
	}
	if (var->class_type != MAT_C_SINGLE) {
		m_error = QString("Variable is not a vector of single.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readVec");
		return -1;
	}
	if (var->dims[0] != 1 && var->dims[1] != 1) {
		m_error = QString("Variable is not a vector.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readVec");
		return -1;
	}
    Mat_VarReadDataAll(FILEPTR, var);
	vector = QVector<float>(var->dims[0] * var->dims[1]);
	memcpy(vector.data(), var->data, var->dims[0] * var->dims[1] * sizeof(float));
	Mat_VarFree(var);
	return 0;
}

int AwMATLABFile::readVec(const QString& name, QVector<qint16>& vector)
{
	CHECK_OPEN_FILE
        matvar_t *var = Mat_VarReadInfo(FILEPTR, name.toStdString().c_str());
	if (var == NULL) {
		m_error = QString("Variable %1 not found.").arg(name);
		throw AwException(m_error, "AwMATLABFile::readVec");
		return -1;
	}
	if (var->isComplex) {
		m_error = QString("Variable is complex.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readVec");
		return -1;
	}
	if (var->rank != 2) {
		m_error = QString("Variable is not a 2D matrix.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readVec");
		return -1;
	}
	if (var->class_type != MAT_C_INT16) {
		m_error = QString("Variable is not a vector of short integers.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readVec");
		return -1;
	}
	if (var->dims[0] != 1 && var->dims[1] != 1) {
		m_error = QString("Variable is not a vector.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readVec");
		return -1;
	}
    Mat_VarReadDataAll(FILEPTR, var);
	vector = QVector<qint16>(var->dims[0] * var->dims[1]);
	memcpy(vector.data(), var->data, var->dims[0] * var->dims[1] * sizeof(qint16));
	Mat_VarFree(var);
	return 0;
}

bool AwMATLABFile::variableExists(const QString& name)
{
	return  Mat_VarReadInfo(FILEPTR, name.toStdString().c_str()) != nullptr;
}

int AwMATLABFile::readVec(const QString& name, QVector<qint32>& vector)
{
	CHECK_OPEN_FILE
        matvar_t *var = Mat_VarReadInfo(FILEPTR, name.toStdString().c_str());
	if (var == NULL) {
		m_error = QString("Variable %1 not found.").arg(name);
		throw AwException(m_error, "AwMATLABFile::readVec");
		return -1;
	}
	if (var->isComplex) {
		m_error = QString("Variable is complex.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readVec");
		return -1;
	}
	if (var->rank != 2) {
		m_error = QString("Variable is not a 2D matrix.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readVec");
		return -1;
	}
	if (var->class_type != MAT_C_INT32) {
		m_error = QString("Variable is not a vector of integers.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readVec");
		return -1;
	}
	if (var->dims[0] != 1 && var->dims[1] != 1) {
		m_error = QString("Variable is not a vector.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readVec");
		return -1;
	}
    Mat_VarReadDataAll(FILEPTR, var);
	vector = QVector<qint32>(var->dims[0] * var->dims[1]);
	memcpy(vector.data(), var->data, var->dims[0] * var->dims[1] * sizeof(qint32));
	Mat_VarFree(var);
	return 0;
}

int AwMATLABFile::readVec(const QString& name, QVector<double>& vector)
{
	CHECK_OPEN_FILE
    matvar_t *var = Mat_VarReadInfo(FILEPTR, name.toStdString().c_str());
	if (var == NULL) {
		m_error = QString("Variable %1 not found.").arg(name);
		return -1;
	}
	if (var->isComplex) {
		m_error = QString("Variable is complex.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readVec");
		return -1;
	}
	if (var->rank != 2) {
		m_error = QString("Variable is not a 2D matrix.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readVec");
		return -1;
	}
	if (var->class_type != MAT_C_DOUBLE) {
		m_error = QString("Variable is not a vector of single.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readVec");
		return -1;
	}
	if (var->dims[0] != 1 && var->dims[1] != 1) {
		m_error = QString("Variable is not a vector.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readVec");
		return -1;
	}
    Mat_VarReadDataAll(FILEPTR, var);
	vector = QVector<double>(var->dims[0] * var->dims[1]);
	memcpy(vector.data(), var->data, var->dims[0] * var->dims[1] * sizeof(double));
	Mat_VarFree(var);
	return 0;
}

int AwMATLABFile::readVec(const QString& name, fvec& vector)
{
	CHECK_OPEN_FILE
    matvar_t *var = Mat_VarReadInfo(FILEPTR, name.toStdString().c_str());
	if (var == NULL) {
		m_error = QString("Variable %1 not found.").arg(name);
		throw AwException(m_error, "AwMATLABFile::readVec");
		return -1;
	}
	if (var->isComplex) {
		m_error = QString("Variable is complex.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readVec");
		return -1;
	}
	if (var->rank != 2) {
		m_error = QString("Variable is not a 2D matrix.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readVec");
		return -1;
	}
	if (var->class_type != MAT_C_SINGLE) {
		m_error = QString("Variable is not a vector of single.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readVec");
		return -1;
	}
	if (var->dims[0] != 1 && var->dims[1] != 1) {
		m_error = QString("Variable is not a vector.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readVec");
		return -1;
	}
    Mat_VarReadDataAll(FILEPTR, var);
	vector = fvec((float *)var->data, var->dims[0], var->dims[1]);
	Mat_VarFree(var);
	return 0;
}


bool AwMATLABFile::isStruct(const QString& name)
{
	QString origin = "AwMATLABFile::isStruct";
	if (FILEPTR == NULL) {
		m_error = "file pointer is null.";
		throw AwException(m_error, origin);
		return NULL;
	}
	matvar_t *var = Mat_VarReadInfo(FILEPTR, name.toStdString().c_str());
	if (var == NULL) {
		m_error = "variable does not exist.";
		throw AwException(m_error, origin);
		return NULL;
	}
	if (var->class_type != MAT_C_STRUCT)
		return false;
	return true;
}

AwMATLABStruct *AwMATLABFile::getStruct(const QString& name)
{
	QString origin = "AwMATLABFile::getStruct";
	if (FILEPTR == NULL) {
		m_error = "file pointer is null.";
		throw AwException(m_error, origin);
		return NULL;
	}
	matvar_t *var = Mat_VarReadInfo(FILEPTR, name.toStdString().c_str());
	if (var == NULL) {
		m_error = "variable does not exist.";
		throw AwException(m_error, origin);
		return NULL;
	}
	if (var->class_type != MAT_C_STRUCT) {
		m_error = QString("Variable is not a struct.");
		Mat_VarFree(var);
		throw AwException(m_error, "AwMATLABFile::readStruct");
		return NULL;
	}
	Mat_VarReadDataAll(FILEPTR, var);
	return new AwMATLABStruct(var);
}

