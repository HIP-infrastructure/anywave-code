#pragma once

#include <qwt_plot.h>
#include <qwt_plot_spectrogram.h>
#include <qwt_matrix_raster_data.h>
#include <aw_armadillo.h>

#include <AwGlobal.h>
// A widget based on QwtPlot that will display a spectrogram using data in a matrix.

class AW_WIDGETS_EXPORT AwSpectroWidget : public QwtPlot
{
	Q_OBJECT
public:
	AwSpectroWidget(QWidget *parent = 0);

	/** Set a new matrix and a position in time. **/
	void setData(mat& matrix, float position = 0.);
	void setData(QVector<double>& matrix, uword ncols, float position = 0.);
	void setYLeftAxisVector(vec& axis);
	void setXBottomAxisVector(vec& axis);

protected:
	QwtPlotSpectrogram *m_spectro;
	QwtMatrixRasterData *m_matrix;
};

