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
#include <vtkRenderer.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataNormals.h>
#include <vtkActor.h>
#include <vtkFollower.h>
#include <AwChannel.h>
#include <widget/AwVTKWidget.h>
namespace Ui {class AwSEEGWidgetUi;};
class AwSEEGViewer;

class AwSEEGPad 
{
public:
	AwSEEGPad(vtkSmartPointer<vtkPolyData> sphere);
	AwSEEGPad(double *center, double radius); // create a new pad at center, with radius

	vtkSmartPointer<vtkPolyData> sphere;
	vtkSmartPointer<vtkActor> actor;
	vtkSmartPointer<vtkPolyDataMapper> mapper;
	double center[3];
	double value;
	QString label;
};


class AwSEEGWidget : public QWidget
{
	Q_OBJECT
	friend class AwSEEGViewer;
public:
	AwSEEGWidget(QWidget *parent = 0);
	~AwSEEGWidget();

	void setSelectedActor(vtkActor *actor);
	inline bool electrodesLoaded() { return m_electrodesLoaded;  }
	void update3DScene() { m_widget->window()->Render(); }
	void setBIDSDriven(bool flag = true);
public slots:
	void loadElectrodes();
	void loadElectrodes(const QString& file);
	void changeMeshOpacity(int value);
	void openMesh(const QString& file);
	void loadMesh();
	void setMeshes(const QStringList& meshes);
	void changeFastRendering(bool on);
	void computeMap(AwChannelList& channels, float latency, qint64 sample);
	void showElectrodesLabels(bool on);
	/** reset to initial state with loaded mesh and electrodes. **/
protected slots:
	void updateDisplayedMeshes(bool);
	void updateOpacity(int);
signals:
	void closed();
	void selectedElectrodes(const QStringList& labels);
protected:
	void clearElectrodes();
	void clearBalls();
	void clearMeshes();
	void updateMap();
	void closeEvent(QCloseEvent *e);

	void generateElectrodesLabels();
	void updateMeshes();

	AwSEEGViewer *m_viewer;
	QHash<QString, AwSEEGPad *> m_hPads;
	// m_electrodes = all loaded electrodes from electrodes file.
	// m_bipolarElectodes = virtual bipolard electrodes computed when loading the electrode file.
	// m_balls = virtual electrodes used in mapping mode to represent amplitude values.
	QList<AwSEEGPad *> m_electrodes, m_bipolarElectrodes, m_balls; 
	QHash<QString, QStringList *> m_electrodesLabels;
	double m_meshBounds[6];
	double m_a, m_l;
	Ui::AwSEEGWidgetUi *m_ui;
	// VTK Objects
	QList<vtkSmartPointer<vtkFollower> > m_labelActors;
	AwVTKWidget *m_widget;
	QList<vtkSmartPointer<vtkActor>> m_actors;
	QStringList m_meshLabels;
	QList<vtkSmartPointer<vtkPolyData>> m_meshes;
	QList<QWidget*> m_checkBoxes, m_sliders;
	bool m_electrodesLoaded;
};
