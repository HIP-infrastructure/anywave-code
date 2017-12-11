/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
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
	memcpy(tmp.data(), trans.memptr, trans.n_elem * sizeof(double));
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