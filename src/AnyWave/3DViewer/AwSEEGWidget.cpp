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
#include "ui_AwSEEGWidget.h"
#include <qstring.h>
#include <vtkPolyData.h>
#include <vtkSTLReader.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCylinderSource.h>
#include <vtkSphereSource.h>
#include <vtkGlyph3D.h>
#include <vtkPoints.h>
#include <vtkProperty.h>
#include <vtkDepthSortPolyData.h>
#include <vtkCamera.h>
#include <vtkVectorText.h>
#include <vtkMath.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qfiledialog.h>
#include "AwSEEGViewer.h"
#include <AwException.h>
#include "AwSEEGInteractor.h"
#include <QRegularExpression>
#include "vtkPialReader.h"
#include <QLabel>
#include <QSlider>

//#define L	30	// 10 cm maximum radius for balls
constexpr double L = 30.;

constexpr auto ELECTRODE_SECTION = "MRI_FS";

AwSEEGPad::AwSEEGPad(vtkSmartPointer<vtkPolyData> s)
{
	sphere = s;
	actor = vtkSmartPointer<vtkActor>::New();
	actor->SetPickable(true);
	actor->GetProperty()->SetColor(1., 0., 1.);
	mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(sphere);
	actor->SetMapper(mapper);
}

AwSEEGPad::AwSEEGPad(double *c, double radius)
{
	vtkSmartPointer<vtkSphereSource> source = vtkSmartPointer<vtkSphereSource>::New();
	source->SetRadius(radius);
	source->SetCenter(c);
	source->Update();
	sphere =  source->GetOutput();
	actor = vtkSmartPointer<vtkActor>::New();
	actor->SetPickable(true);
	actor->GetProperty()->SetColor(1., 0., 1.);
	mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(sphere);
	actor->SetMapper(mapper);
	memcpy(center, c, 3 * sizeof(double));
}


AwSEEGWidget::AwSEEGWidget(QWidget *parent)
	: QWidget(parent)
{
	m_widget = new AwVTKWidget(this);
	m_ui = new Ui::AwSEEGWidgetUi();
	m_ui->setupUi(this);
	setWindowTitle("SEEG Viewer");
	m_ui->mainLayout->replaceWidget(m_ui->widget, m_widget);
	m_electrodesLoaded = false;
	// An interactor
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(m_widget->window());
	// Our custom style interactor
	vtkSmartPointer<AwSEEGInteractor> style =
		vtkSmartPointer<AwSEEGInteractor>::New();
	style->setWidget(this);
	style->SetDefaultRenderer(m_widget->renderer());
	renderWindowInteractor->SetInteractorStyle(style);

	// check for Depth Peeling
	int depthPeelingUsed = m_widget->renderer()->GetLastRenderingUsedDepthPeeling();
	if (!depthPeelingUsed) {
		m_widget->window()->SetAlphaBitPlanes(1);
		m_widget->window()->SetMultiSamples(0);
		m_widget->renderer()->SetUseDepthPeeling(1);
		m_widget->renderer()->SetMaximumNumberOfPeels(50);
		m_widget->renderer()->SetOcclusionRatio(0.2);
	}
	vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
	camera->SetPosition(0, 0, 400);
	camera->SetFocalPoint(0, 0, 0);
	m_widget->renderer()->SetActiveCamera(camera);
	m_widget->window()->Render();
}

AwSEEGWidget::~AwSEEGWidget()
{
	clearElectrodes();
	clearBalls();
	clearMeshes();
	delete m_ui;
}

void AwSEEGWidget::setSelectedActor(vtkActor *actor)
{
	// Should be better to use a Hash Table.
	bool found = false;
	AwSEEGPad *p = NULL;
	for (auto pad : m_electrodes) {
		if (pad->actor == actor) {
			p = pad;
			found = true;
			break;
		}
	}
	if (!found) {
		for (auto pad : m_balls) {
			if (pad->actor == actor) {
				p = pad;
				found = true;
				break;
			}
		}
	}
	if (found) {
		QStringList labels = { p->label };
		emit selectedElectrodes(labels);
	}
	
}

void AwSEEGWidget::setBIDSDriven(bool flag)
{
	m_ui->buttonLoadElectrodes->setVisible(!flag);
	m_ui->buttonLoadMesh->setVisible(!flag);
}

void AwSEEGWidget::closeEvent(QCloseEvent *e)
{
	emit closed();
}

void AwSEEGWidget::updateDisplayedMeshes(bool flag)
{
	QWidget* w = qobject_cast<QWidget*>(sender());
	if (!w)
		return;
	int index = m_checkBoxes.indexOf(w);
	Q_ASSERT(index != -1);
	auto actor = m_actors.at(index);
	if (!flag) 
		m_widget->renderer()->RemoveActor(actor);
	else
		m_widget->renderer()->AddActor(actor);
	m_widget->window()->Render();
}

void AwSEEGWidget::updateOpacity(int value)
{
	QWidget *w = qobject_cast<QWidget *>(sender());
	if (!w)
		return;

	int index = m_sliders.indexOf(w);
	Q_ASSERT(index != -1);
	auto actor = m_actors.at(index);
	actor->GetProperty()->SetOpacity((double)(value) / 100.);
	m_widget->window()->Render();
}

void AwSEEGWidget::loadMesh()
{
	QString file = QFileDialog::getOpenFileName(this, tr("Load Mesh"), "/", "*.*");
	if (!file.isEmpty())
		openMesh(file);
}

void AwSEEGWidget::updateMeshes()
{
	constexpr int maxCols = 2;
	auto layout = m_ui->layoutMeshes;
	int col = 0, row = 0;
	int index = 0;
	for (const QString& key : m_meshLabels) {
		QCheckBox* check = new QCheckBox();
		check->setText(key);
		check->setChecked(true);
		connect(check, &QCheckBox::toggled, this, &AwSEEGWidget::updateDisplayedMeshes);
		layout->addWidget(check, row, col++);
		m_checkBoxes << check;
		layout->addWidget(new QLabel("Alpha: "), row, col++);
		QSlider* slider = new QSlider(Qt::Horizontal);
		slider->setMaximum(100);
		slider->setMinimum(0);
		slider->setValue(30);
		connect(slider, &QSlider::valueChanged, this, &AwSEEGWidget::updateOpacity);
		layout->addWidget(slider, row, col);
		m_sliders << slider,
		row++;
		col = 0;
		auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		auto actor = vtkSmartPointer<vtkActor>::New();
		mapper->SetInputData(m_meshes.at(index++));
		actor->SetMapper(mapper);
		actor->GetProperty()->SetOpacity(0.3);
		actor->SetPickable(false);
		m_actors << actor;
		m_widget->renderer()->AddActor(actor);
	}
	m_widget->window()->Render();
	repaint();
}


void AwSEEGWidget::clearMeshes()
{
	for (auto actor : m_actors)
		m_widget->renderer()->RemoveActor(actor);
	m_actors.clear();
}

void AwSEEGWidget::setMeshes(const QStringList& meshes)
{
	QRegularExpression ext("\\.(pial)+$");
	// get bounds of every meshes
	double bounds[6] = { 0., 0., 0., 0., 0., 0. }, temp[6];
	for (auto const& mesh : meshes) {
		QFileInfo fi(mesh);
		auto name = fi.fileName();
		name.remove(ext);
		if (!m_meshLabels.contains(name)) {
			vtkSmartPointer<vtkPialReader> reader = vtkSmartPointer<vtkPialReader>::New();
			vtkSmartPointer<vtkPolyDataNormals> smooth = vtkSmartPointer<vtkPolyDataNormals>::New();
			m_meshLabels << name;
			std::string str = mesh.toStdString();
			reader->SetFileName(str.c_str());
			reader->Update();
			smooth->SetInputData(reader->GetOutput());
			smooth->ComputeCellNormalsOn();
			smooth->ComputePointNormalsOn();
			smooth->ConsistencyOn();
			smooth->SetFeatureAngle(60);
			smooth->Update();
			auto mesh = smooth->GetOutput();
			mesh->GetBounds(temp);
			bounds[0] = std::min(bounds[0], temp[0]);
			bounds[1] = std::max(bounds[1], temp[1]);
			bounds[2] = std::min(bounds[2], temp[2]);
			bounds[3] = std::max(bounds[3], temp[3]);
			bounds[4] = std::min(bounds[4], temp[4]);
			bounds[5] = std::max(bounds[5], temp[5]);
			m_meshes.append(smooth->GetOutput());
		}
	}
	m_l = sqrt(vtkMath::Distance2BetweenPoints(&bounds[0], &bounds[3]));
	m_a = 1 / L * ((m_l / 3) - 1);
	updateMeshes();
}

void AwSEEGWidget::changeFastRendering(bool on)
{
	if (on) {
		m_widget->window()->SetAlphaBitPlanes(0);
		m_widget->window()->SetMultiSamples(1);
		m_widget->renderer()->SetUseDepthPeeling(0);
	}
	else {
		m_widget->window()->SetAlphaBitPlanes(1);
		m_widget->window()->SetMultiSamples(0);
		m_widget->renderer()->SetUseDepthPeeling(1);
		m_widget->renderer()->SetMaximumNumberOfPeels(50);
		m_widget->renderer()->SetOcclusionRatio(0.2);
	}
	m_widget->window()->Render();
}


void AwSEEGWidget::openMesh(const QString& file)
{
	if (file.isEmpty())
		return;
//	m_smoothMesh = vtkSmartPointer<vtkPolyDataNormals>::New();
//	if (file.endsWith(".pial")) {
//		vtkSmartPointer<vtkPialReader> reader = vtkSmartPointer<vtkPialReader>::New();
//		std::string str = file.toStdString();
//		reader->SetFileName(str.c_str());
//		reader->Update();
//		m_mesh = reader->GetOutput();
//#if VTK_MAJOR_VERSION < 6
//		m_smoothMesh->SetInput(reader->GetOutput());
//#else
//		m_smoothMesh->SetInputData(reader->GetOutput());
//#endif
//	}
//	else {
//
//		vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
//		reader->SetFileName(file.toStdString().c_str());
//		reader->Update();
//		m_mesh = reader->GetOutput();
//#if VTK_MAJOR_VERSION < 6
//		m_smoothMesh->SetInput(reader->GetOutput());
//#else
//		m_smoothMesh->SetInputData(reader->GetOutput());
//#endif
//	}
//
//	m_smoothMesh->ComputeCellNormalsOn();
//	m_smoothMesh->ComputePointNormalsOn();
//	m_smoothMesh->ConsistencyOn();
//	m_smoothMesh->SetFeatureAngle(60);
//	m_smoothMesh->Update();
//
//	m_mesh->GetBounds(m_meshBounds);
//	m_l = sqrt(vtkMath::Distance2BetweenPoints(&m_meshBounds[0], &m_meshBounds[3]));
//	m_a = 1 / (double)L * ((m_l / 3) - 1);
//
//	changeSmoothRendering(true);
//	vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
//	camera->SetPosition(0, 0, 400);
//	camera->SetFocalPoint(0, 0, 0);
//	m_widget->renderer()->SetActiveCamera(camera);
//	m_widget->window()->Render();

}

void AwSEEGWidget::changeMeshOpacity(int value) 
{
	//m_meshActor->GetProperty()->SetOpacity((double)(value) / 100.);
	//m_widget->window()->Render();
}

void AwSEEGWidget::clearElectrodes()
{
	while (!m_electrodes.isEmpty()) {
		AwSEEGPad *pad = m_electrodes.takeFirst();
		m_widget->renderer()->RemoveActor(pad->actor);
		delete pad;
	}
	while (!m_bipolarElectrodes.isEmpty()) {
		AwSEEGPad *pad = m_bipolarElectrodes.takeFirst();
		m_widget->renderer()->RemoveActor(pad->actor);
		delete pad;
	}
	QStringList keys = m_electrodesLabels.keys();
	for (auto k : keys) {
		delete m_electrodesLabels.value(k);
	}
	m_electrodesLabels.clear();
	m_electrodesLoaded = false;
}

void  AwSEEGWidget::clearBalls()
{
	for (auto pad : m_balls)
		m_widget->renderer()->RemoveActor(pad->actor);

	while (!m_balls.isEmpty()) {
		delete m_balls.takeFirst();
	}
}

void AwSEEGWidget::loadElectrodes()
{
	QString file = QFileDialog::getOpenFileName(this, tr("Load Electrodes"), "/");
	if (!file.isEmpty()) 
		loadElectrodes(file);
}

void AwSEEGWidget::loadElectrodes(const QString& file)
{
	if (file.isEmpty())
		return;

	QFile elecFile(file);
	QTextStream stream(&elecFile);
	if (!elecFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		throw AwException("Failed to open electrode file.", "AwSEEGWidget::loadElectrodes");
		return;
	}

	clearElectrodes();
	m_hPads.clear();

	QString line;
	// skip all until section is found
	while (!stream.atEnd()) {
		line = stream.readLine();
		if (line.trimmed() == ELECTRODE_SECTION) 
			break;
	}
	// read until end of file or other section is found
	while (!stream.atEnd()) {
		line = stream.readLine();
		QStringList tokens = line.split('\t'); // must have at least 5 columns
		if (tokens.size() < 5) { // skip line with less than 5 columuns
			continue;
		}
		double x, y, z;
		x = tokens.at(2).toDouble();
		y = tokens.at(3).toDouble();
		z = tokens.at(4).toDouble();
		auto baseLabel = tokens.first();
		if (!m_electrodesLabels.contains(baseLabel))
			m_electrodesLabels.insert(baseLabel, new QStringList());
		auto label = tokens.first() + tokens.at(1);
		QStringList *labels = m_electrodesLabels.value(baseLabel);
		labels->append(label);
		vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
		sphere->SetRadius(1.0);
		sphere->SetCenter(x, y, z);
		sphere->Update();
		AwSEEGPad *pad = new AwSEEGPad(sphere->GetOutput());
		pad->center[0] = x;
		pad->center[1] = y;
		pad->center[2] = z;
		pad->label = label;
		if (!m_hPads.contains(label))
			m_hPads.insert(label, pad);
		m_electrodes.append(pad);
		m_widget->renderer()->AddActor(pad->actor);
	}
	elecFile.close();

	// generate bipolar virtual sphere to use in bipolar montages.
	for (QStringList* labels : m_electrodesLabels.values()) {
		AwSEEGPad *firstPad = m_hPads.value(labels->first());
		for (int i = 1; i < labels->size(); i++) {
			AwSEEGPad *nextPad = m_hPads.value(labels->at(i));
			double x = (firstPad->center[0] + nextPad->center[0]) / 2;
			double y = (firstPad->center[1] + nextPad->center[1]) / 2;
			double z = (firstPad->center[2] + nextPad->center[2]) / 2;
			vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
			sphere->SetRadius(1.0);
			sphere->SetCenter(x, y, z);
			sphere->Update();
			AwSEEGPad *pad = new AwSEEGPad(sphere->GetOutput());
			pad->center[0] = x;
			pad->center[1] = y;
			pad->center[2] = z;
			pad->label = QString("%1-%2").arg(firstPad->label).arg(nextPad->label);
			m_hPads.insert(pad->label, pad);
			m_bipolarElectrodes.append(pad);
			firstPad = nextPad;
		}

	}
	generateElectrodesLabels();
	m_electrodesLoaded = true;
	for (auto pad : m_electrodes) 
		m_widget->renderer()->AddActor(pad->actor);
	m_widget->window()->Render();
	repaint();
}

void AwSEEGWidget::generateElectrodesLabels()
{
	QStringList keys = m_electrodesLabels.keys();
	for (auto f : m_labelActors)
		m_widget->renderer()->RemoveActor(f);
	m_labelActors.clear();
	for (auto k : keys) {
		vtkSmartPointer<vtkVectorText> textSource = vtkSmartPointer<vtkVectorText>::New();
		textSource->SetText(k.toLatin1().data());
		textSource->Update();
		vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
#if VTK_MAJOR_VERSION < 6
		mapper->SetInput(textSource->GetOutput());
#else
        mapper->SetInputData(textSource->GetOutput());
#endif
		vtkSmartPointer<vtkFollower> textActor = vtkSmartPointer<vtkFollower>::New();
		textActor->SetPickable(false);
		textActor->SetMapper(mapper);
		textActor->GetProperty()->SetColor(1.0, 1.0 , 1.0);
		textActor->SetScale(3, 3, 3);
		// get the corresponding electrode
		QString lastPad = m_electrodesLabels.value(k)->last();
		AwSEEGPad *pad = m_hPads.value(lastPad); 
		textActor->AddPosition(pad->center);
		m_labelActors.append(textActor);
	}
}




/**
compute mapping of SEEG electrodes:
channels = data
latency = position in seconds where the user clicked
sample = position in sample in the data
**/
void AwSEEGWidget::computeMap(AwChannelList& channels, float latency, qint64 sample)
{
	setWindowTitle(QString("SEEG Viewer - Mapping at position: %1s").arg(latency));
	clearBalls();
	for (auto c : channels) {
		float value = c->data()[sample];
		AwSEEGPad *ball = NULL;
		auto pad = m_hPads.value(c->fullName());
		if (pad == NULL)
			continue;
		// compute radius based on amplitude
		double radius = m_a * (std::abs(value) / c->gain()) + 1;
		if (radius > m_l / 3)
			radius = m_l / 3;
		ball = new AwSEEGPad(pad->center, radius);
		// compute color based on value (positive = red, negative = blue)
		if (value > 0)
			ball->actor->GetProperty()->SetColor(1.0, 0, 0);
		else
			ball->actor->GetProperty()->SetColor(0, 0, 1.0);
		m_balls.append(ball);
	}
	updateMap();
}

void AwSEEGWidget::updateMap()
{
	for (auto pad : m_balls) 
		m_widget->renderer()->AddActor(pad->actor);
	
	m_widget->window()->Render();
}

void AwSEEGWidget::showElectrodesLabels(bool on)
{
	m_widget->renderer()->ResetCameraClippingRange();
	if (on)	{
		for (int i = 0; i < m_labelActors.size(); i++)	{
			m_widget->renderer()->AddActor(m_labelActors.at(i));
			m_labelActors.at(i)->SetCamera(m_widget->renderer()->GetActiveCamera());
		}
	}
	else {
		for (int i = 0; i < m_labelActors.size(); i++)	{
			m_widget->renderer()->RemoveActor(m_labelActors.at(i));
		}
	}
	m_widget->window()->Render();
}
