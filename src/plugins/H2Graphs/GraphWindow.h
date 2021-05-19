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

#include <QWidget>
#include "ui_GraphWindow.h"
class GraphSensorItem;
class GraphColorMap;
#include <qgraphicsscene.h>
#include "GraphSet.h"
#include <QTimer>
#include <widget/SignalView/AwBaseSignalView.h>

class GraphWindow : public QWidget
{
	Q_OBJECT

public:
	GraphWindow(GraphSet *gs, QWidget *parent = 0);
	~GraphWindow();
public slots:
	void setNewThreshold(double value);		// update the threshold value and the slider position when the user enters a value manually.
	void previousTimeWindow();
	void nextTimeWindow();
	void showValues(bool flag);
	void changeViewSize(QString size);
	void animate();
	void setLoop(bool on) { m_playLoop = on; }
	void showAsymetry(bool flag);
	void timeoutAnimation();
	void exportGraphToPNG();
	void exportGraphToSVG();
	void hideSensors(bool hide);
	void showMeanGraph();
	void showTimeCourses();
protected:
	void closeEvent(QCloseEvent *event);  
signals:
	void sendCommand(int, QVariantList);
private:
	Ui::GraphWindowUi m_ui;
	GraphSet *m_graphSet;
	bool m_playLoop;
	float m_samplingRate;
	float m_threshold;
	float m_processPosInFile;
	float m_posInFile;
	bool m_showValues, m_showAsym, m_isPlaying, m_hideSensorIfNoConnection;
	float m_delay;
	int m_currentIteration;	// current iteration (goes through all the sections)
	int m_totalIterations;
	int m_currentSection;
	QStringList m_labels;
	QTimer *m_timer;
	QList<GraphSensorItem *> m_sensorsItem;
	GraphColorMap *m_cmapItem;
	QGraphicsScene *m_scene;
	AwBaseSignalView *m_signalView;
	QHash<QString, AwChannel *> m_timeCoursesChannels;

	void initGraph();
	void updateGraph();
	void updateTimeCourses(int section);
};


