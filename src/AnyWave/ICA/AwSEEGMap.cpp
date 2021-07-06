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
#include "AwSEEGMap.h"
#include "AwICAComponents.h"
#include <aw_armadillo.h>
#include <qcustomplot.h>

AwSEEGMap::AwSEEGMap(AwICAComponents* c, QWidget* parent) : QWidget(parent)
{
	m_components = c;
	this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
	QGridLayout* layout = new QGridLayout;
	m_plot = new QCustomPlot;
	layout->addWidget(m_plot, 0, 0);
	QCheckBox* check = new QCheckBox("abs values");
	layout->addWidget(check, 1, 0);
	connect(check, &QCheckBox::toggled, this, &AwSEEGMap::plotAbsValues);
	this->setLayout(layout);
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	resize(QSize(400, 300));
	setWindowTitle("SEEG-ICA Heat Map");
	setWindowIcon(QIcon(":/images/AnyWave_icon.png"));
	m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	m_plot->axisRect()->setupFullAxesBox(true);
	m_plot->yAxis->setLabel("Channels");
	m_plot->xAxis->setLabel("Components");
	fmat mat = c->mixing();
	// configure left axis text labels:
	QSharedPointer<QCPAxisTickerText> channelTicker(new QCPAxisTickerText);
	double count = 0.;
	for (auto const& label : m_components->labels()) 
		channelTicker->addTick(count++, label);
	m_plot->yAxis->setTicker(channelTicker);
	QSharedPointer<QCPAxisTickerText> compTicker(new QCPAxisTickerText);
	for (int i = 0; i < mat.n_cols; i++)
		compTicker->addTick(i, QString("IC-%1").arg(i + 1));
	m_plot->xAxis->setTicker(compTicker);
	m_plot->xAxis->setRange(0, mat.n_cols - 1);
	m_plot->yAxis->setRange(0, mat.n_rows - 1);

	m_colorMap = new QCPColorMap(m_plot->xAxis, m_plot->yAxis);
	m_colorMap->data()->setSize(mat.n_cols, mat.n_rows);
	// keys are columns, values are rows of mixing matrix
	m_colorMap->data()->setRange(QCPRange(0, mat.n_cols - 1), QCPRange(0, mat.n_rows - 1));

	m_colorMap->setInterpolate(false); // get the pixels boxes per channels

	for (auto xIndex = 0; xIndex < mat.n_cols; xIndex++) {
		for (auto yIndex = 0; yIndex < mat.n_rows; yIndex++) {
			m_colorMap->data()->setCell(xIndex, yIndex, mat(yIndex, xIndex));
		}
	}
	QCPColorScale* colorScale = new QCPColorScale(m_plot);
	m_plot->plotLayout()->addElement(0, 1, colorScale);
	colorScale->setType(QCPAxis::atRight);
	m_colorMap->setColorScale(colorScale);
	colorScale->axis()->setLabel("Magnitude");
	m_colorMap->setGradient(QCPColorGradient::gpJet);
	m_colorMap->rescaleDataRange();
	// make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
	QCPMarginGroup* marginGroup = new QCPMarginGroup(m_plot);
	m_plot->axisRect()->setMarginGroup(QCP::msBottom | QCP::msTop, marginGroup);
	colorScale->setMarginGroup(QCP::msBottom | QCP::msTop, marginGroup);
	// rescale the key (x) and value (y) axes so the whole color map is visible:
	m_plot->rescaleAxes();

	connect(m_plot, &QCustomPlot::mouseMove, this, &AwSEEGMap::mouseMove);
}

void AwSEEGMap::plotAbsValues(bool flag)
{
	fmat mat;
	if (flag)
		mat = abs(m_components->mixing());
	else
		mat = m_components->mixing();
	m_colorMap->data()->clear();
	m_colorMap->data()->setSize(mat.n_cols, mat.n_rows);
	// keys are columns, values are rows of mixing matrix
	m_colorMap->data()->setRange(QCPRange(0, mat.n_cols - 1), QCPRange(0, mat.n_rows - 1));

	for (auto xIndex = 0; xIndex < mat.n_cols; xIndex++) {
		for (auto yIndex = 0; yIndex < mat.n_rows; yIndex++) {
			m_colorMap->data()->setCell(xIndex, yIndex, mat(yIndex, xIndex));
		}
	}
	m_colorMap->rescaleDataRange();
	m_plot->replot();
	//setToolTipDuration(2000);
}

void AwSEEGMap::mouseMove(QMouseEvent* evt)
{
	double x = m_plot->xAxis->pixelToCoord(evt->pos().x());
	double y = m_plot->yAxis->pixelToCoord(evt->pos().y());
	int xx, yy;
	m_colorMap->data()->coordToCell(x, y, &xx, &yy);
	if (yy >= 0 && yy < m_components->labels().size()) {
	//if (m_plot->axisRect()->rect().contains(evt->pos())) {
		double z = m_colorMap->data()->data(x, y);
		
		setToolTip(QString("Channel %1 - Value: %2").arg(m_components->labels().at(yy)).arg(z));
	}
}
