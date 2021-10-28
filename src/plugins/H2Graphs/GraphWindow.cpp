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
#include "GraphWindow.h"
#include "GraphSensorItem.h"
#include "GraphColorMap.h"
#include "GraphLegend.h"
#include "GraphArrow.h"
#include "GraphArrowDir.h"
#include "GraphArrowLag.h"
#include <QtMath>
#include <QSvgGenerator>
#include <qfiledialog.h>
#include <AwProcess.h>
#include "H2Graphs.h"
#include "H2SelectPairsDial.h"
#include <qcustomplot.h>

GraphWindow::GraphWindow(GraphSet *gs, QWidget *parent)
	: QWidget(parent)
{
	m_ui.setupUi(this);
	connect(this, SIGNAL(sendCommand(int, QVariantList)), H2Graphs::instance(), SIGNAL(sendCommand(int, QVariantList)));
	// change window title
	setWindowTitle(QString("%1:%2Hz-%3Hz:%4").arg(gs->band).arg(gs->hp).arg(gs->lp).arg(gs->method));
	m_labels = gs->labels;
	m_graphSet = gs;
	m_scene = new QGraphicsScene(this);
	m_scene->setSceneRect(QRectF(-500, -500, 1000, 1000));
	m_ui.graphicsView->setScene(m_scene);
	m_threshold = 0.4;
	m_ui.sbThreshold->setValue(0.4);
	m_ui.buttonBack->setEnabled(false);
	m_showValues = true;
	m_showAsym = false;
	m_playLoop = false;
	m_hideSensorIfNoConnection = false;
	m_delay = 1;
	m_isPlaying = false;
	m_timer = NULL;
	// start with the position of the first iteration
	m_posInFile = gs->positions().first();
	m_currentIteration = 0;
	m_totalIterations = gs->totalIterations();
	m_samplingRate = gs->samplingRate;
	initGraph();
	changeViewSize("80");
	updateGraph();
	connect(m_ui.sbThreshold, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &GraphWindow::setNewThreshold);
	connect(m_ui.buttonMean, &QPushButton::clicked, this, &GraphWindow::showMeanGraph);
	connect(m_ui.buttonTimeCourses, &QPushButton::clicked, this, &GraphWindow::showTimeCourses);
	m_signalView = nullptr;
	m_currentSection = -1;
	if (gs->isMean())
		m_ui.buttonMean->setEnabled(false);
}

GraphWindow::~GraphWindow()
{
	if (m_timer) 
		m_timer->stop();
	if (m_signalView)
		delete m_signalView;
	while (!m_plots.isEmpty()) {
		auto w = m_plots.takeFirst();
		w->close();
		delete w;
	}
}


void GraphWindow::closeEvent(QCloseEvent *event)
{
	emit sendCommand(AwProcessCommand::RemoveLastHighlightedSection, QVariantList());
	event->accept(); 
}


void GraphWindow::updateTimeCourses(int section)
{
	if (m_currentSection == section)
		return;
	m_currentSection = section;
	auto nIterations = m_graphSet->sectionIterations(m_currentSection);
	// compute total duration in s for the section
	float duration = (float)nIterations * m_graphSet->step;
	auto channels = m_signalView->channels();
	cube matrix = m_graphSet->correlationMatrix();
	qint64 nSamplesPerIteration = (qint64)std::floor(m_graphSet->step  * m_graphSet->samplingRate);

	for (uword i = 0; i < matrix.n_rows; i++) {  // X
		for (uword j = 0; j < matrix.n_cols; j++) { // Y
			if (i == j)
				continue;
			auto channelName = QString("%1->%2").arg(m_graphSet->labels.value(i)).arg(m_graphSet->labels.value(j));
			auto channel = m_timeCoursesChannels[channelName];
			if (channel == Q_NULLPTR)
				continue;
			auto dest = channel->newData(nSamplesPerIteration * nIterations);
			for (int iteration = 0; iteration < nIterations; iteration++) {
				double value = matrix(i, j, iteration);
				// number of samples per iterations
				for (auto n = 0; n < nSamplesPerIteration; n++) 
					*dest++ = (float)value;
			}
		}
	}
	m_signalView->setTotalDuration(duration);
	auto posInFile = m_graphSet->positions().value(m_currentIteration);
	m_signalView->setTimeShift(posInFile);
	m_signalView->setRecordedTime(H2Graphs::instance()->pdi.input.reader()->infos.recordingTime());
	m_signalView->setWindowTitle(QString("%1 computed between %2s and %3s").arg(m_graphSet->method).arg(posInFile).arg(posInFile + duration));
	m_signalView->update();
}

void GraphWindow::showTimeCourses()
{
	H2SelectPairsDial dlg(m_graphSet->labels, m_pairs);
	if (dlg.exec() != QDialog::Accepted)
		return;
	m_pairs = dlg.pairs();
	plotTimeCourses();
	// build a widget to get selected pairs.



	//if (m_signalView == nullptr) {
	//	m_signalView = new AwBaseSignalView();
	//	m_signalView->setFlags(AwBaseSignalView::NoMarkerBar | AwBaseSignalView::ViewAllChannels | AwBaseSignalView::HidePositionInFile);
	//	m_signalView->setBaseSize(QSize(800, 300)); 
	//	m_signalView->setWindowIcon(QIcon(":/images/AnyWave_icon.png"));
	//	auto settings = m_signalView->settings();
	//	settings->limitChannels = true;
	//	settings->maxChannels = 2;
	//	settings->showZeroLine = true;
	//	m_signalView->changeSettings(settings, AwViewSettings::LimitNumberOfChannels | AwViewSettings::ShowBaseLine);
	//
	//	// init channels for the view
	//	AwChannelList channels;
	//	// create a list of X->Y channels and Y->X channels.
	//	auto purple = QColor(Qt::magenta).name();
	//	QString label;
	//	for (int i = 0; i < m_graphSet->labels.size() - 1; i++)
	//		for (int j = i + 1; j < m_graphSet->labels.size(); j++) {
	//			label = QString("%1->%2").arg(m_graphSet->labels.value(i)).arg(m_graphSet->labels.value(j));
	//			// XY Pair
	//			AwChannel *channel = new AwChannel;
	//			//channel->setUnit(m_graphSet->method);
	//			channel->setSamplingRate(m_graphSet->samplingRate);
	//			channel->setGain(1.);
	//			channel->setName(label);
	//			channel->setColor(purple);
	//			m_timeCoursesChannels.insert(channel->name(), channel);
	//			channels << channel;
	//			// YX Pair
	//			label = QString("%1->%2").arg(m_graphSet->labels.value(j)).arg(m_graphSet->labels.value(i));
	//			channel = new AwChannel(channel);
	//			channel->setName(label);
	//			channels << channel;
	//			m_timeCoursesChannels.insert(channel->name(), channel);
	//		}

	//	m_signalView->setChannels(channels);
	//	// update time courses for current section 
	//	updateTimeCourses(m_graphSet->sections().value(m_currentIteration));
	//}
	//m_signalView->show();
}


void GraphWindow::showMeanGraph()
{
	m_graphSet->newMeanGraphSet();
}

void GraphWindow::initGraph()
{
	QGraphicsScene* scene = m_ui.graphicsView->scene();
	// clear current graph if any
	scene->clear();
	m_sensorsItem.clear();

	m_ui.graphicsView->centerOn(0, 0);
	qreal radius = std::min(scene->width() / 3, scene->height() / 3) - 30;
	qreal angle = 0;
	qreal step = 2 * M_PI /  m_labels.size();

	for (auto s : m_labels) {
		GraphSensorItem *sitem = new GraphSensorItem(s, palette().color(QPalette::Window));
		QPointF sitemPos(qSin(angle) * radius, qCos(angle) * radius);
		sitem->setPos(sitemPos);
		scene->addItem(sitem);
		m_sensorsItem << sitem;
		angle += step;
	}
	// init colormap item and position it
	m_cmapItem = new GraphColorMap(m_graphSet->method);
	scene->addItem(m_cmapItem);
	m_cmapItem->setPos(radius + 50, -200);

	// init Legend Item
	GraphLegend *legend = new GraphLegend();
	scene->addItem(legend);
	legend->setPos(-radius - 50, 200);

	//// pre parse the 3D matrices to extract for each pair, which has the maximum h2 value and so set also the lag.
	//cube matrix = m_graphSet->correlationMatrix();
	//cube lag = m_graphSet->lagMatrix();
	//cube test = zeros(3, m_labels.size(), m_totalIterations);
	//int maxLag = (int)floor((m_graphSet->maxLag / m_samplingRate) * 1000);

	//for (auto iter = 0; iter < m_totalIterations; iter++) {
	//	for (uword i = 0; i < matrix.n_rows; i++) {  // X
	//		for (uword j = i + 1; j < matrix.n_cols; j++) { // Y
	//			double xy_value = matrix(i, j, iter);
	//			double yx_value = matrix(j, i, iter);
	//			double xy_lag = lag(i, j, iter);
	//			double yx_lag = lag(j, i, iter);
	//			double ah2 = (xy_value - yx_value) / std::max(xy_value, yx_value);

	//		}
	//	}
	//}

}


void GraphWindow::updateGraph()
{
	// do nothing if index of timewindow is invalid. (due to current playing animation for example)
	if (m_currentIteration + 1 > m_totalIterations)
		return;
	QGraphicsScene* scene = m_ui.graphicsView->scene();
	// clear all arrows
	for(auto item : m_sensorsItem) {
		item->removeArrows();
		m_hideSensorIfNoConnection ? item->hide() : item->show();
	}

	// check pairs of channel and get the values from the tensor 
	cube matrix = m_graphSet->correlationMatrix();
	cube lag = m_graphSet->lagMatrix();

	int maxLag = (int)floor((m_graphSet->maxLag / m_samplingRate) * 1000);
	for (uword i = 0; i < matrix.n_rows; i ++) {  // X
		for (uword j = i + 1; j < matrix.n_cols; j++) { // Y
			double xy_value = matrix(i, j, m_currentIteration);
			double yx_value = matrix(j, i, m_currentIteration);
			int xy_lag = (int)lag(i, j, m_currentIteration);
			int yx_lag = (int)lag(j, i, m_currentIteration);
			xy_lag = (int)floor((xy_lag / m_graphSet->samplingRate) * 1000);
			yx_lag = (int)floor((yx_lag / m_graphSet->samplingRate) * 1000);
			
			// compute asymetric h2
			float ah2 = (xy_value - yx_value) / std::max(xy_value, yx_value);
			GraphArrow *arrow = NULL;
			GraphArrowLag *arrowLag = NULL;
			GraphArrowDir *arrowDir = NULL;

			if (std::max(xy_value, yx_value) >= m_threshold) {
				m_sensorsItem.at(i)->show();
				m_sensorsItem.at(j)->show();
				if (xy_value > yx_value)
					arrowDir = new GraphArrowDir(m_sensorsItem.at(i), m_sensorsItem.at(j), true, ah2);
				else
					arrowDir = new GraphArrowDir(m_sensorsItem.at(j), m_sensorsItem.at(i), true, ah2);
				if (ah2 >= 0) {
					arrow = new GraphArrow(m_sensorsItem.at(i), m_sensorsItem.at(j), false, xy_value);
					arrow->setColor(m_cmapItem->colormap()->color(QwtInterval(0.0, 1.0), xy_value));
					if (xy_lag < 0)
						arrowLag = new GraphArrowLag(m_sensorsItem.at(i), m_sensorsItem.at(j), true, std::abs(xy_lag), maxLag);
					else if (xy_lag > 0)
						arrowLag = new GraphArrowLag(m_sensorsItem.at(j), m_sensorsItem.at(i), true, std::abs(xy_lag), maxLag);
					else 
						arrowLag = new GraphArrowLag(m_sensorsItem.at(j), m_sensorsItem.at(i), true, 0, maxLag);
				}
				else {
					arrow = new GraphArrow(m_sensorsItem.at(j), m_sensorsItem.at(i), false, yx_value);
					arrow->setColor(m_cmapItem->colormap()->color(QwtInterval(0.0, 1.0), yx_value));
					if (yx_lag < 0)
						arrowLag = new GraphArrowLag(m_sensorsItem.at(j), m_sensorsItem.at(i), true, std::abs(yx_lag), maxLag);
					else if (yx_lag > 0)
						arrowLag = new GraphArrowLag(m_sensorsItem.at(i), m_sensorsItem.at(j), true, std::abs(yx_lag), maxLag);
				}
				m_sensorsItem.at(i)->addArrow(arrow);
				m_sensorsItem.at(j)->addArrow(arrow);
				if (arrowLag) {
					m_sensorsItem.at(i)->addArrow(arrowLag);
					m_sensorsItem.at(j)->addArrow(arrowLag);
					arrowLag->showValue(m_showValues);
				}
				if (arrowDir) {
					m_sensorsItem.at(i)->addArrow(arrowDir);
					m_sensorsItem.at(j)->addArrow(arrowDir);
					arrowDir->showValue(m_showValues);
					arrowDir->setVisible(m_showAsym);
				}
			}
			if (arrow) {
				arrow->showValue(m_showValues);
				scene->addItem(arrow);
				arrow->updatePosition();

			}
			if (arrowLag) {
				scene->addItem(arrowLag);
				arrowLag->updatePosition();
			}
			if (arrowDir) {
				scene->addItem(arrowDir);
				arrowDir->updatePosition();
			}
		}
	}
	
	// update ui 
	m_posInFile = m_graphSet->positions().at(m_currentIteration);
	m_ui.spinPosition->setValue(m_graphSet->positions().at(m_currentIteration));
	m_ui.lineEdit->setText(QString("%1/%2").arg(m_graphSet->sections().at(m_currentIteration) + 1).arg(m_graphSet->sectionsCount()));

	m_ui.buttonNext->setEnabled(m_currentIteration < m_totalIterations);
	m_ui.buttonBack->setEnabled(m_currentIteration > 1);
	// send command to AnyWave
	emit sendCommand(AwProcessCommand::RemoveLastHighlightedSection, QVariantList());
	QVariantList args;
	args << QString("H2");
	args << m_posInFile;
	args << m_graphSet->timeWindow;
	emit sendCommand(AwProcessCommand::AddHighlightedSection, args);
}


void GraphWindow::showValues(bool flag)
{
	for (auto item : m_sensorsItem)
		for (auto arrow : item->arrows())
			arrow->showValue(flag);
	m_showValues = flag;
}

void GraphWindow::showAsymetry(bool flag)
{
	for (auto item : m_sensorsItem) {
		for (auto arrow : item->arrows()) {
			GraphArrowDir *dir = qgraphicsitem_cast<GraphArrowDir *>(arrow);
			if (dir)
				dir->setVisible(flag);
		}
	}
	m_showAsym = flag;
}

void GraphWindow::changeViewSize(QString size)
{
	double scale = size.toDouble() / 100.0;
	
	if (scale == 0)
		return;

	QMatrix oldMatrix = m_ui.graphicsView->matrix();
	m_ui.graphicsView->resetMatrix();
	m_ui.graphicsView->translate(oldMatrix.dx(), oldMatrix.dy());
	m_ui.graphicsView->scale(scale, scale);
}

void GraphWindow::setNewThreshold(double value)
{
	m_threshold = value;
	updateGraph();
}

void GraphWindow::hideSensors(bool hide)
{
	m_hideSensorIfNoConnection = hide;
	updateGraph();
}

void GraphWindow::previousTimeWindow()
{
	m_currentIteration--;
	if (m_currentIteration < 0)
		m_currentIteration = 0;
	updateGraph();
	if (m_signalView)
		updateTimeCourses(m_graphSet->sections().value(m_currentIteration));
}

void GraphWindow::nextTimeWindow()
{
	m_currentIteration++;
	if (m_currentIteration >= m_totalIterations)	
		m_currentIteration = 0;
	updateGraph();
	if (m_signalView)
		updateTimeCourses(m_graphSet->sections().value(m_currentIteration));
}

void GraphWindow::timeoutAnimation()
{
	if (m_currentIteration >=  m_totalIterations)	{
		if (m_playLoop)	
			m_currentIteration = 0;
		else {
			m_isPlaying = false;
			m_timer->stop();
			m_ui.buttonAnimate->setIcon(QIcon(":/images/Play-icon_32x32.png"));
			return;
		}
	}
	updateGraph();
	m_currentIteration++;
}

void GraphWindow::animate()
{
	m_delay = m_ui.sbDelay->value();
	if (!m_isPlaying)	{
		m_isPlaying = true;
		if (!m_timer)	{
			m_timer = new QTimer(this);
			connect(m_timer, SIGNAL(timeout()), this, SLOT(timeoutAnimation()));
		}
		else
			m_timer->stop();
		// set to first time window and start playing
		m_currentIteration = 0;
		m_timer->start(m_delay * 1000);
		m_ui.buttonAnimate->setIcon(QIcon(":/images/Stop-icon_32x32.png"));
	}
	else { // we are playing, so stop animation
		m_isPlaying = false;
		m_timer->stop();
		m_ui.buttonAnimate->setIcon(QIcon(":/images/Play-icon_32x32.png"));
	}
}


void GraphWindow::exportGraphToPNG()
{
	QString file = QFileDialog::getSaveFileName(this, "Export graph to PNG", "/", "Image (*.png)");
	if (file.isEmpty())
		return;

	QImage image(3840, 2160, QImage::Format_RGB32);
	image.fill(Qt::white); 
	QPainter painter(&image);
	painter.setRenderHint(QPainter::Antialiasing);
	m_scene->render(&painter, QRectF(0, 0, 3840, 2160));
	image.save(file, "PNG");
}

void GraphWindow::exportGraphToSVG()
{
	QString file = QFileDialog::getSaveFileName(this, "Export graph to SVG", "/", "Image (*.svg)");
	if (file.isEmpty())
		return;

	QPainter painter;
	QSvgGenerator generator;
    generator.setFileName(file);
	generator.setSize(QSize(3840, 2160));
	painter.begin(&generator);
	m_scene->render(&painter, QRectF(0, 0, 3840, 2160));
	painter.end();
}

