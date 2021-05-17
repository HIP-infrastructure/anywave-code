#include <widget/AwSpectroWidget.h>
#include <qwt_plot_layout.h>

AwSpectroWidget::AwSpectroWidget(QWidget *parent) : QwtPlot(parent)
{
	// A default title
	setTitle(QString("Spectrogram"));
	m_spectro = new QwtPlotSpectrogram;
	m_spectro->setRenderThreadCount(0); // use system specific thread count
	m_spectro->attach(this);
	plotLayout()->setAlignCanvasToScales(true);
}


void AwSpectroWidget::setData(mat& matrix, float position)
{
	QVector<double> tmp(matrix.n_elem);
	mat trans = matrix.t();
//	memcpy(tmp.data(), trans.memptr, trans.n_elem * sizeof(double));
	setData(tmp, matrix.n_cols, position);
}

void AwSpectroWidget::setData(QVector<double>& matrix, uword ncols, float position)
{
	m_matrix = new QwtMatrixRasterData;
	m_matrix->setResampleMode(QwtMatrixRasterData::BilinearInterpolation);
}

void AwSpectroWidget::setYLeftAxisVector(vec& axis)
{

}