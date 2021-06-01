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
#include "AwStatChartWidget.h"
#include <epoch/AwEpochTree.h>
#include <armadillo>

AwStatChartWidget::AwStatChartWidget(QWidget *parent)
	: QChartView(parent)
{
	m_chart = new QChart();
	m_chart->setTitle("Statistical informations");
	m_axisY = new QValueAxis();
	m_axisX = new QValueAxis();
	this->setChart(m_chart);
	m_YMax = m_YMin = 0.;
	m_axisX->setTitleText("Time (s)");
	m_axisY->setTitleText("Amplitude");
	m_chart->addAxis(m_axisY, Qt::AlignLeft);
	m_chart->addAxis(m_axisX, Qt::AlignBottom);
	setRenderHint(QPainter::Antialiasing);
	resize(QSize(150, 300));
	m_flags = AwStatChartWidget::ShowErrorType;
}

AwStatChartWidget::~AwStatChartWidget()
{
	auto channels = m_nameToChannel.values();
	while (!channels.isEmpty())
		delete channels.takeLast();
}

void AwStatChartWidget::setDisplayOptions(int flags)
{
	m_flags = flags;
	setChannels(m_channels);
}

void AwStatChartWidget::setCondition(AwEpochTree *condition)
{
	m_condition = condition;
	int nTicks = (int)floor(condition->epochDuration() * 10) + 1;
	m_axisX->setTickCount(nTicks);
	m_axisX2 = new QCategoryAxis;
	m_axisX2->append("Baseline", 0);
	m_axisX2->append("Post Stimulus", condition->epochDuration() - condition->zeroPos());
	m_axisX2->setLinePenColor(QColor(Qt::red));
	m_axisX2->setGridLinePen(QPen(Qt::red));
	m_chart->addAxis(m_axisX2, Qt::AlignTop);
	switch (condition->modality())
	{
	case AwChannel::MEG:
		m_axisY->setTitleText("Amplitude (pT/cm)");
//		m_unitFactor = 1e12;
		break;
	case AwChannel::GRAD:
		m_axisY->setTitleText("Amplitude (pT/m/cm)");
//		m_unitFactor = 1e12;
		break;
	case AwChannel::EEG:
	case AwChannel::SEEG:
		m_axisY->setTitleText("Amplitude (µV/cm)");
//		m_unitFactor = 1e6;
		break;
	} 
}

void AwStatChartWidget::setChannels(const QList<AwEpochAverageChannel *>& channels)
{
	m_channels = channels;
	m_nameToChannel.clear();
	m_chart->removeAllSeries();
	m_YMax = m_YMin = 0.;
	fvec data, errorType;
	bool showError = m_flags &  AwStatChartWidget::ShowErrorType;
	// change Y scale
	for (auto c : channels) {
		data = c->dataVector();
		errorType = c->error();
		if (showError) {
			fvec tmp = data + errorType;
			m_YMax = std::max(m_YMax, tmp.max());
			tmp = data - errorType;
			m_YMin = std::min(m_YMin, tmp.min());
		}
		else {
			m_YMin = std::min(m_YMin, data.min());
			m_YMax = std::max(m_YMax, data.max());
		}
	}
	m_axisY->setRange(m_YMin, m_YMax);
	for (auto c : channels) {
		m_nameToChannel.insert(c->name(), c);
		addChannel(c);
	}
}

void AwStatChartWidget::addChannel(AwEpochAverageChannel *channel)
{
	fvec data = channel->dataVector(), errorType = channel->error();
	bool showError = m_flags &  AwStatChartWidget::ShowErrorType;
	fvec errorPlus = data + errorType;
	fvec errorMinus = data - errorType;

	QLineSeries *series = new QLineSeries();
	QLineSeries *errorUp = NULL;
	QLineSeries *errorDown = NULL;
	if (showError) {
		errorUp = new QLineSeries();
		errorDown = new QLineSeries();
	}
	series->setName(channel->name());
	series->pen().setColor(QColor(channel->color()));
	for (arma::uword i = 0; i < data.n_elem; i++) {
		float pos = (float(i) / channel->samplingRate()) - channel->zeroPosition();
		series->append(pos, data(i));
		if (showError) {
			errorUp->append(pos, errorPlus(i));
			errorDown->append(pos, errorMinus(i));
		}
	}
	if (showError) {
		QAreaSeries *errorSeries = new QAreaSeries(errorUp, errorDown);
		errorSeries->setName(QString("%1 et").arg(channel->name()));
		QPen pen(QColor(channel->color()).lighter());
		pen.setWidth(1);
		errorSeries->setPen(pen);
		errorSeries->setBrush(QBrush(QColor(channel->color()).lighter(), Qt::Dense6Pattern));
		errorSeries->attachAxis(m_axisY);
		errorSeries->attachAxis(m_axisX);
		m_chart->addSeries(errorSeries);
	}

	series->attachAxis(m_axisY);
	series->attachAxis(m_axisX);
	series->attachAxis(m_axisX2);
	m_chart->addSeries(series);
	repaint();
}