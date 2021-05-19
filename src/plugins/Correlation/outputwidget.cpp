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
#include "outputwidget.h"
#include <qwt_plot.h>
#include <qwt_plot_spectrogram.h>
#include <qwt_matrix_raster_data.h>
#include <graphics/AwColorMap.h>
#include <graphics/AwQwtColorMap.h>
#include <qwt_color_map.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_layout.h>
#include <matlab/AwMATLAB.h>
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
	QwtPlotSpectrogram *spectro = new QwtPlotSpectrogram();
    spectro->setRenderThreadCount(0); // use system specific thread count
	spectro->setColorMap(AwQwtColorMap::newMap(AwColorMap::Parula));
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
	spectro->setData(matrix);
	spectro->attach(m_ui.plot);
	spectro->setAxes(QwtPlot::xBottom, QwtPlot::yLeft);
	matrix->setInterval(Qt::XAxis, QwtInterval(0, res->corr.n_cols));
	matrix->setInterval(Qt::YAxis, QwtInterval(0, res->corr.n_rows));
	m_ui.plot->setAxisScale(QwtPlot::xBottom, 0, res->corr.n_cols, 0.5);
	m_ui.plot->setAxisScale(QwtPlot::yLeft, res->corr.n_rows, 0, 0.5);
	m_picker = new CorrPicker(res, m_ui.plot->canvas());
	m_picker->setAxis(QwtPlot::xBottom, QwtPlot::yLeft);
	m_picker->setRubberBand(QwtPlotPicker::NoRubberBand);
	m_picker->setTrackerMode(QwtPicker::AlwaysOn);
	spectro->invalidateCache();
	m_ui.plot->replot();
}

OutputWidget::~OutputWidget()
{
	delete m_picker;
}


void OutputWidget::saveToMat()
{
	QString fileName = QFileDialog::getSaveFileName(this, "Save to .mat", "/", "Matlab file (*.mat)");
	if (fileName.isEmpty())
		return;

	AwMATLABFile file;
	try {
		file.create(fileName);
		file.writeStringCellArray("xLabels", m_result->xLabels);
		file.writeStringCellArray("yLabels", m_result->yLabels);
		file.writeMatrix("data", m_result->corr);
	}
	catch (const AwException& e) {
		AwMessageBox::critical(this, "File Operation", e.errorString());
		return;
	}

	AwMessageBox::information(this, "File Operation", "File saved successfully.");
}
