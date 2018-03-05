#include "maingui.h"
#include <qfiledialog.h>
#include "matio.h"
#include <widget/AwMessageBox.h>


maingui::maingui(const QList<corr_result *>& results, QWidget *parent)
	: QWidget(parent)
{
	m_ui.setupUi(this);
	m_results = results;
	computeMean();
	m_outputMeanWidget = new OutputWidget(&m_mean);
	m_outputMeanWidget->setWindowTitle("Mean Correlation");
	m_outputMeanWidget->show();
}

maingui::~maingui()
{
	delete m_outputMeanWidget;
}


void maingui::computeMean() 
{
	// do the mean
	arma::uword row = m_results.first()->corr.n_rows;
	arma::uword col = m_results.first()->corr.n_cols;
	mat avg(row, col);
	avg.zeros();
	for (auto c : m_results)
		avg += c->corr;
	avg /= m_results.size();
	m_mean.corr = avg;
	m_mean.xLabels = m_results.first()->xLabels;
	m_mean.yLabels = m_results.first()->yLabels;

}

void maingui::saveToMatlab()
{
	QString fileName = QFileDialog::getSaveFileName(this, "Save to .mat", "/", "Matlab file (*.mat)");
	if (fileName.isEmpty())
		return;

	mat_t *matfp = Mat_CreateVer(fileName.toStdString().c_str(), NULL, MAT_FT_MAT5);
	if (!matfp) {
		AwMessageBox::information(this, "File Error", QString("Failed to create %1").arg(fileName));
		return;
	}

	// create cell array of strings for the electrode's labels
	size_t cellDimsXLabels[2] = { 1, (size_t)m_mean.xLabels.size() };
	matvar_t *mat_xlabels = Mat_VarCreate("xLabels", MAT_C_CELL, MAT_T_CELL, 2, cellDimsXLabels, NULL, 0);
	size_t cellDimsYLabels[2] = { 1, (size_t)m_mean.yLabels.size() };
	matvar_t *mat_ylabels = Mat_VarCreate("yLabels", MAT_C_CELL, MAT_T_CELL, 2, cellDimsYLabels, NULL, 0);
	if (!mat_xlabels) {
		AwMessageBox::information(this, "File Error", "Failed to create variable xLabels");
		Mat_Close(matfp);
		return;
	}
	if (!mat_ylabels) {
		AwMessageBox::information(this, "File Error", "Failed to create variable yLabels");
		Mat_VarFree(mat_xlabels);
		Mat_Close(matfp);
		return;
	}

	int index = 0;
	for (auto label : m_mean.xLabels) {
		size_t stringDims[2];
		char dummy[256];
		int size = qMin(label.size(), 255);
		memcpy(dummy, label.toStdString().c_str(), size);
		dummy[size] = '\0';
		stringDims[0] = 1;
		stringDims[1] = size;
		matvar_t *mat_elec = Mat_VarCreate("elec", MAT_C_CHAR, MAT_T_INT8, 2, stringDims, dummy, 0);
		if (!mat_elec) {
			AwMessageBox::information(this, "File Error", "matio: unable to create label for cell array.");
			Mat_VarFree(mat_xlabels);
			Mat_Close(matfp);
			return;
		}
		Mat_VarSetCell(mat_xlabels, index++, mat_elec);
	}
	index = 0;
	for (auto label : m_mean.yLabels) {
		size_t stringDims[2];
		char dummy[256];
		int size = qMin(label.size(), 255);
		memcpy(dummy, label.toStdString().c_str(), size);
		dummy[size] = '\0';
		stringDims[0] = 1;
		stringDims[1] = size;
		matvar_t *mat_elec = Mat_VarCreate("elec", MAT_C_CHAR, MAT_T_INT8, 2, stringDims, dummy, 0);
		if (!mat_elec) {
			AwMessageBox::information(this, "File Error", "matio: unable to create label for cell array.");
			Mat_VarFree(mat_ylabels);
			Mat_Close(matfp);
			return;
		}
		Mat_VarSetCell(mat_ylabels, index++, mat_elec);
	}
	// create data matrix
	size_t row = m_results.first()->corr.n_rows;
	size_t col = m_results.first()->corr.n_cols;
	size_t matrixDims[2] = { row, col };
	//// do the mean
	//mat avg(row, col);
	//avg.zeros();
	//for (auto c : m_results) 
	//	avg += c->corr;
	//avg /= m_results.size();
	matvar_t *mat_matrix = Mat_VarCreate("data", MAT_C_DOUBLE, MAT_T_DOUBLE, 2, matrixDims, m_mean.corr.memptr(), 0);
	if (!mat_matrix) {
		AwMessageBox::information(this, "File Error", "matio: unable to create data matrix variable.");
		Mat_VarFree(mat_xlabels);
		Mat_VarFree(mat_ylabels);
		Mat_Close(matfp);
		return;
	}
	Mat_VarWrite(matfp, mat_xlabels, MAT_COMPRESSION_NONE);
	Mat_VarWrite(matfp, mat_ylabels, MAT_COMPRESSION_NONE);
	Mat_VarWrite(matfp, mat_matrix, MAT_COMPRESSION_NONE);
	Mat_Close(matfp);
	AwMessageBox::information(this, "File Operation", "File saved successfully.");
}