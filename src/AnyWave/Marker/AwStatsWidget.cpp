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
#include "AwStatsWidget.h"
#include "AwMarkerManager.h"
#include <widget/AwWaitWidget.h>
#include <QMouseEvent>
#include <algorithm>
#include <execution>

AwStatsWidget::AwStatsWidget(QWidget *parent)
	: QWidget(parent)
{
	m_ui.setupUi(this);
	setWindowFlags(Qt::WindowStaysOnTopHint);
	m_ui.buttonUpdate->setEnabled(false);
	connect(m_ui.buttonUpdate, &QPushButton::clicked, this, &AwStatsWidget::compute);
	m_barPlot = new QCPBars(m_ui.widget->xAxis, m_ui.widget->yAxis);
	m_barPlot->setAntialiased(false);
	//// set dark background gradient:
	//QLinearGradient gradient(0, 0, 0, 400);
	//gradient.setColorAt(0, QColor(90, 90, 90));
	//gradient.setColorAt(0.38, QColor(105, 105, 105));
	//gradient.setColorAt(1, QColor(70, 70, 70));
	//m_ui.widget->setBackground(QBrush(gradient));
	//m_ui.widget->xAxis->setBasePen(QPen(Qt::white));
	//m_ui.widget->xAxis->setTickPen(QPen(Qt::white));
	m_ui.widget->xAxis->grid()->setVisible(true);
	//m_ui.widget->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
	//m_ui.widget->xAxis->setTickLabelColor(Qt::white);
	//m_ui.widget->xAxis->setLabelColor(Qt::white);
	// prepare y axis:
	m_ui.widget->yAxis->setPadding(5); // a bit more space to the left border
	m_ui.widget->yAxis->setLabel("Occurrences");
	//m_ui.widget->yAxis->setBasePen(QPen(Qt::white));
	//m_ui.widget->yAxis->setTickPen(QPen(Qt::white));
	//m_ui.widget->yAxis->setSubTickPen(QPen(Qt::white));
	m_ui.widget->xAxis->setSubTicks(false);
	m_ui.widget->yAxis->grid()->setSubGridVisible(false);
	//m_ui.widget->yAxis->setTickLabelColor(Qt::white);
	//m_ui.widget->yAxis->setLabelColor(Qt::white);
	//m_ui.widget->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
	//m_ui.widget->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
	// setup legend:
	m_ui.widget->legend->setVisible(false);
	m_ui.widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

	// connections
	connect(AwMarkerManager::instance(), &AwMarkerManager::updateStats, this, &AwStatsWidget::enableUpdate);
	connect(m_ui.widget, &QCustomPlot::mouseMove, this, &AwStatsWidget::handleMouseMove);
	compute();
}

AwStatsWidget::~AwStatsWidget()
{
}

void AwStatsWidget::handleMouseMove(QMouseEvent* evt)
{
	QCPAbstractPlottable* plottable =
		m_ui.widget->plottableAt(evt->localPos());
	if (plottable) {
		QCPBars* bar =
			qobject_cast<QCPBars*>(plottable);
		if (bar) {
			QCPBarsDataContainer::const_iterator it = bar->data()->constEnd();
			QVariant details;
			if (bar->selectTest(evt->pos(), false, &details)) // QPoint could be e.g. event->pos() of a mouse event
			{
				QCPDataSelection dataPoints = details.value<QCPDataSelection>();
				if (dataPoints.dataPointCount() > 0)
					it = bar->data()->at(dataPoints.dataRange().begin());
			}
			//if (it != bar->data()->constEnd()) {
				QToolTip::showText(mapToGlobal(evt->pos()), QString::number(it->value));
			//}
			return;
		}
	}
	QToolTip::hideText();
}

void AwStatsWidget::refresh()
{
	// prepare x axis with marker labels
	QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
	QMap<double, QString> ticks;
	QStringList keys = m_histogram.keys();
	std::sort(keys.begin(), keys.end());
	auto values = m_histogram.values();
	double v = 1.;
	for (auto& k : keys) {
		ticks.insert(v, k);
		m_barPlot->addData(v++, (double)m_histogram.value(k));
	}
	textTicker->addTicks(ticks);
	m_ui.widget->xAxis->setTicker(textTicker);
	m_ui.widget->xAxis->setTickLabelRotation(60);
	m_ui.widget->xAxis->setSubTicks(false);
	m_ui.widget->xAxis->setTickLength(0, 4);
	m_ui.widget->xAxis->setRange(0, keys.size() + 1);

	m_ui.widget->yAxis->setRange(0, *(std::max_element(values.begin(), values.end())));
	m_ui.widget->replot();
	m_ui.spinTotal->setValue(m_markers.size());
}

void AwStatsWidget::computeHistogram()
{
	m_histogram = AwMarker::computeHistogram(m_markers);
	emit finished();
}

void AwStatsWidget::compute()
{
	m_markers = AwMarkerManager::instance()->getMarkers();
	if (m_markers.size() > MARKERS_THREAD_THRESHOLD) {
		AwWaitWidget wait("Updating Stats");
		wait.setText("Computing...");
		connect(this, &AwStatsWidget::finished, &wait, &QDialog::accept);
		wait.run(std::bind(&AwStatsWidget::computeHistogram, this));  // bind a void method without parameters. The method must emit finished signals when finished.
	}
	else 
		computeHistogram();
	refresh();
}