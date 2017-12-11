#include "outputwidget.h"
#include <qwt_plot.h>

#include <qwt_matrix_raster_data.h>
#include <graphics/AwColorMap.h>
#include <graphics/AwQwtColorMap.h>
#include <qwt_color_map.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_layout.h>
#include "matio.h"
#include <QFileDialog>
#include <widget/AwMessageBox.h>

QwtText LabelDraw::label(double value) const
{
	int dec = floor(value);
	if (value - dec == 0.5 && dec < m_labels.size()) 
		return QwtText(m_labels.at(dec));
	return QwtText("");
}

QwtText CorrPicker::trackerText(const QPoint& pos) const
{
	QPointF res = invTransform(pos);
	int x = (int)floor(res.x());
	int y = (int)floor(res.y());
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if (x >= m_result->xLabels.size())
		x = m_result->xLabels.size() - 1;
	if (y >= m_result->yLabels.size())
		y = m_result->yLabels.size() - 1;
	QColor c(Qt::white);
	c.setAlpha(180);
	QwtText t(QString("%1 vs %2 v=%3").arg(m_result->yLabels.at(y)).arg(m_result->xLabels.at(x)).arg(m_result->corr(y,x)));
	t.setBackgroundBrush(QBrush(c));
	return t;
}

OutputWidget::OutputWidget(corr_result *res, QWidget *parent)
	: QWidget(parent)
{
	m_ui.setupUi(this);
	setWindowTitle(windowTitle() + QString(" - at pos:%1s duration:%2s").arg(res->position).arg(res->duration));
	m_spectro = new QwtPlotSpectrogram();
	m_spectro->setRenderThreadCount(0); // use system specific thread count
	m_spectro->setColorMap(AwQwtColorMap::newMap(AwColorMap::Parula));
	m_ui.plot->enableAxis(QwtPlot::yLeft, true);
	m_ui.plot->enableAxis(QwtPlot::xBottom, true);
	m_ui.plot->plotLayout()->setCanvasMargin(0);
	m_ui.plot->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	LabelDraw *newLabelDrawX = new LabelDraw(res->xLabels);
	LabelDraw *newLabelDrawY = new LabelDraw(res->yLabels);
	newLabelDrawY->enableComponent(QwtAbstractScaleDraw::ScaleComponent::Backbone, false);
	newLabelDrawY->enableComponent(QwtAbstractScaleDraw::ScaleComponent::Ticks, false);
	newLabelDrawX->enableComponent(QwtAbstractScaleDraw::ScaleComponent::Backbone, false);
	newLabelDrawX->enableComponent(QwtAbstractScaleDraw::ScaleComponent::Ticks, false);
	newLabelDrawX->setLabelRotation(90.);
	//newLabelDrawX->setLabelAlignment(Qt::AlignHCenter|Qt::AlignBottom);
	m_ui.plot->setAxisScaleDraw(m_ui.plot->xBottom, newLabelDrawX);
	m_ui.plot->setAxisScaleDraw(m_ui.plot->yLeft, newLabelDrawY);
	QwtMatrixRasterData *matrix = new QwtMatrixRasterData;
	m_result = res;
	mat data = res->corr.t();
	QVector<double> vdata(data.size());
	memcpy(vdata.data(), data.memptr(), data.size() * sizeof(double));
	matrix->setValueMatrix(vdata, res->corr.n_cols);
	QwtInterval ZInterval = QwtInterval(-1.0, 1.0);
	matrix->setInterval(Qt::ZAxis, ZInterval);
	m_spectro->setData(matrix);
	m_spectro->attach(m_ui.plot);
	m_spectro->setAxes(QwtPlot::xBottom, QwtPlot::yLeft);
	matrix->setInterval(Qt::XAxis, QwtInterval(0, res->corr.n_cols));
	matrix->setInterval(Qt::YAxis, QwtInterval(0, res->corr.n_rows));
	m_ui.plot->setAxisScale(QwtPlot::xBottom, 0, res->corr.n_cols, 0.5);
	m_ui.plot->setAxisScale(QwtPlot::yLeft, res->corr.n_rows, 0, 0.5);
	m_picker = new CorrPicker(res, m_ui.plot->canvas());
	m_picker->setAxis(QwtPlot::xBottom, QwtPlot::yLeft);
	m_picker->setRubberBand(QwtPlotPicker::NoRubberBand);
	m_picker->setTrackerMode(QwtPicker::AlwaysOn);
	m_spectro->invalidateCache();
	m_ui.plot->replot();
}

OutputWidget::~OutputWidget()
{
	delete m_picker;
}


void OutputWidget::applyThreshold(double value)
{
	mat copy = m_result->corr;

	for (arma::uword i = 0; i < copy.n_rows; i++) 
		for (arma::uword j = 0; j < copy.n_cols; j++) {
			if (std::abs(copy(i, j)) <= value)
				copy(i, j) = 0.;
		}
	mat data = copy.t();
	QwtMatrixRasterData *matrix = new QwtMatrixRasterData;
	QVector<double> vdata(data.size());
	memcpy(vdata.data(), data.memptr(), data.size() * sizeof(double));
	matrix->setValueMatrix(vdata, copy.n_cols);
	QwtInterval ZInterval = QwtInterval(-1.0, 1.0);
	matrix->setInterval(Qt::ZAxis, ZInterval);
	matrix->setInterval(Qt::XAxis, QwtInterval(0, copy.n_cols));
	matrix->setInterval(Qt::YAxis, QwtInterval(0, copy.n_rows));
	m_spectro->setData(matrix);
	m_spectro->invalidateCache();
	m_ui.plot->replot();
}


void OutputWidget::saveToMat()
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
	size_t cellDimsXLabels[2] = { 1, (size_t)m_result->xLabels.size() };
	matvar_t *mat_xlabels = Mat_VarCreate("xLabels", MAT_C_CELL, MAT_T_CELL, 2, cellDimsXLabels, NULL, 0);
	size_t cellDimsYLabels[2] = { 1, (size_t)m_result->yLabels.size() };
	matvar_t *mat_ylabels = Mat_VarCreate("yLabels", MAT_C_CELL, MAT_T_CELL, 2, cellDimsYLabels, NULL, 0);
	if (!mat_xlabels) {
		AwMessageBox::information(this, "File Error",  "Failed to create variable xLabels");
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
	foreach(QString label, m_result->xLabels) {
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
	foreach(QString label, m_result->yLabels) {
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
	size_t row = m_result->corr.n_rows;
	size_t col = m_result->corr.n_cols;
	size_t matrixDims[2] = { row, col };
	matvar_t *mat_matrix = Mat_VarCreate("data", MAT_C_DOUBLE, MAT_T_DOUBLE, 2, matrixDims, m_result->corr.memptr(), 0);
	if (!mat_matrix) {
		AwMessageBox::information(this, "File Error" ,"matio: unable to create data matrix variable.");
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
