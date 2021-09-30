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

