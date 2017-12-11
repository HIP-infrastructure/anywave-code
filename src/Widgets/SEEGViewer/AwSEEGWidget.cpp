#include <vtkVersion.h>
#if VTK_MAJOR_VERSION > 5
#include <vtkAutoInit.h>
#endif
#include "AwSEEGWidget.h"
#include "ui_AwSEEGWidget.h"
#include <qstring.h>
#include <vtkPolyData.h>
#include <vtkSTLReader.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
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
#include <widget/AwSEEGViewer.h>

#define L	30	// 10 cm maximum radius for bubbles

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

//#include <qdebug.h>
AwSEEGWidget::AwSEEGWidget(const QString& meshFile, QWidget *parent)
	: QWidget(parent)
{
#if VTK_MAJOR_VERSION > 5
	VTK_MODULE_INIT(vtkRenderingOpenGL);
	VTK_MODULE_INIT(vtkInteractionStyle);
	VTK_MODULE_INIT(vtkRenderingFreeType);
#endif
	m_ui = new Ui::AwSEEGWidgetUi();
	m_ui->setupUi(this);
	m_meshMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	m_meshActor =  vtkSmartPointer<vtkActor>::New();
	m_meshActor->SetMapper(m_meshMapper);
	m_meshActor->GetProperty()->SetOpacity(0.2);
	vtkSmartPointer<vtkRenderWindow> window = m_ui->widget->GetRenderWindow();
	m_renderer =  vtkSmartPointer<vtkRenderer>::New();
	m_electrodesRenderer =  vtkSmartPointer<vtkRenderer>::New();
	window->AddRenderer(m_renderer);
	m_renderer->AddActor(m_meshActor);

	// check for Depth Peeling
	int depthPeelingUsed = m_renderer->GetLastRenderingUsedDepthPeeling();
	if (!depthPeelingUsed) {
		window->SetAlphaBitPlanes(1);
		window->SetMultiSamples(0);
		m_renderer->SetUseDepthPeeling(1);
		m_renderer->SetMaximumNumberOfPeels(50);
		m_renderer->SetOcclusionRatio(0.2);
	}

	window->Render();
	openMesh(meshFile);
}

AwSEEGWidget::~AwSEEGWidget()
{
	delete m_ui;
	clearElectrodes();
	clearBalls();
}

void AwSEEGWidget::closeEvent(QCloseEvent *e)
{
	emit closed();
}

void AwSEEGWidget::loadMesh()
{
	QString file = QFileDialog::getOpenFileName(this, tr("Load Mesh"), "/", "*.stl");
	if (!file.isEmpty())
		openMesh(file);
}

void AwSEEGWidget::changeFastRendering(bool on)
{
	vtkSmartPointer<vtkRenderWindow> window = m_ui->widget->GetRenderWindow();
	if (on) {
		window->SetAlphaBitPlanes(0);
		window->SetMultiSamples(1);
		m_renderer->SetUseDepthPeeling(0);
	}
	else {
		window->SetAlphaBitPlanes(1);
		window->SetMultiSamples(0);
		m_renderer->SetUseDepthPeeling(1);
		m_renderer->SetMaximumNumberOfPeels(50);
		m_renderer->SetOcclusionRatio(0.2);
	}
	window->Render();
}

void AwSEEGWidget::changeSmoothRendering(bool on)
{
	if (!on)
		m_meshMapper->SetInputData(m_mesh);
	else {
		m_meshMapper->SetInputData(m_smoothMesh->GetOutput());
	}
	vtkSmartPointer<vtkRenderWindow> window = m_ui->widget->GetRenderWindow();
	window->Render();
}

void AwSEEGWidget::openMesh(const QString& file)
{
	if (file.isEmpty())
		return;

	vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
	reader->SetFileName(file.toStdString().c_str());
	reader->Update();
	m_mesh = reader->GetOutput();
	m_smoothMesh = vtkSmartPointer<vtkPolyDataNormals>::New();
#if VTK_MAJOR_VERSION < 6
    m_smoothMesh->SetInput(reader->GetOutput());
#else
    m_smoothMesh->SetInputData(reader->GetOutput());
#endif
	m_smoothMesh->ComputeCellNormalsOn();
	m_smoothMesh->ComputePointNormalsOn();
	m_smoothMesh->ConsistencyOn();
	m_smoothMesh->SetFeatureAngle(60);
	m_smoothMesh->Update();

	m_mesh->GetBounds(m_meshBounds);
	m_l = sqrt(vtkMath::Distance2BetweenPoints(&m_meshBounds[0], &m_meshBounds[3]));
	m_a = 1 / (double)L * ((m_l / 3) - 1);

	changeSmoothRendering(true);
	vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
	camera->SetPosition(0, 0, 200);
	camera->SetFocalPoint(0, 0, 0);
	m_renderer->SetActiveCamera(camera);
	vtkSmartPointer<vtkRenderWindow> window = m_ui->widget->GetRenderWindow();
	window->Render();

}

void AwSEEGWidget::changeMeshOpacity(int value) 
{
	m_meshActor->GetProperty()->SetOpacity((double)(value) / 100.);
	vtkSmartPointer<vtkRenderWindow> window = m_ui->widget->GetRenderWindow();
	window->Render();
}

void AwSEEGWidget::clearElectrodes()
{
	while (!m_electrodes.isEmpty()) {
		AwSEEGPad *pad = m_electrodes.takeFirst();
		m_renderer->RemoveActor(pad->actor);
		delete pad;
	}
	QStringList keys = m_electrodesLabels.keys();
	foreach (QString k, keys) {
		delete m_electrodesLabels.value(k);
	}
	m_electrodesLabels.clear();
}

void  AwSEEGWidget::clearBalls()
{
	foreach(AwSEEGPad *pad, m_balls)
		m_renderer->RemoveActor(pad->actor);

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
	if (!elecFile.open(QIODevice::ReadOnly|QIODevice::Text))
		return;

	clearElectrodes();
	m_hPads.clear();
	while (!stream.atEnd()) {
		QString line;
		line = stream.readLine();
		if (line.toLower().startsWith("electrode")) // skip
			continue;
		QStringList tokens = line.split('\t');
		if (tokens.size() >= 5) {
			double x, y, z;
			x = tokens.at(2).toDouble();
			y = tokens.at(3).toDouble();
			z = tokens.at(4).toDouble();
			QString baseLabel =  tokens.at(0);
			if (!m_electrodesLabels.contains(baseLabel)) 
				m_electrodesLabels.insert(baseLabel, new QStringList());

			QString label = tokens.at(0) + tokens.at(1);
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
			m_renderer->AddActor(pad->actor);
		}
	}
	generateElectrodesLabels();
	vtkSmartPointer<vtkRenderWindow> window = m_ui->widget->GetRenderWindow();
	window->Render();
}

void AwSEEGWidget::generateElectrodesLabels()
{
	QStringList keys = m_electrodesLabels.keys();
	foreach (vtkSmartPointer<vtkFollower> f, m_labelActors) {
		m_renderer->RemoveActor(f);
	}
	m_labelActors.clear();
	foreach (QString k, keys) {
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
		// scale of text is 30% of minimal distance between sensors.
		//textActor->SetScale(dist * 0.3, dist * 0.3, dist * 0.3);
		textActor->GetProperty()->SetColor(1.0, 1.0 , 1.0);
		textActor->SetScale(3, 3, 3);
		// get the corresponding electrode
		QString lastPad = m_electrodesLabels.value(k)->last();
		AwSEEGPad *pad = m_hPads.value(lastPad); 
		// move the text to the right of the electrode
		//double pos[3];
		//memcpy(pos, pad->center, 3 * sizeof(double));
		//pos[2] += 10;
		//textActor->AddPosition(pos);
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
	clearBalls();
	QRegExp rx("(\\d+)");  // Regular Expression to get a number in a string
	foreach (AwChannel *c, channels) {
		AwSEEGPad *ball = NULL;
		//QString label = c->name().toLower();
		//int pos = rx.indexIn(label);
		float value = c->data()[sample];
		AwSEEGPad *pad = m_hPads.value(c->name());
		if (pad == NULL)
			continue;

		// compute radius based on amplitude
		double radius = m_a * (qAbs(value) / c->gain()) + 1;
		if (radius > m_l / 3)
			radius = m_l /3;

		if (c->hasReferences()) { // extract pad numbers from labels
			QString label = c->name();
			int pos = rx.indexIn(label);
			if (pos != -1) {
				label.truncate(pos);
				// get plot number
				int plot = rx.cap(1).toInt();
				AwSEEGPad *p2 = m_hPads.value(label + QString::number(plot + 1 ));
				if (p2) {
					double _center[3];
					vtkMath::Add(pad->center, p2->center, _center);
					_center[0] /= 2;
					_center[1] /= 2;
					_center[2] /= 2;
					ball = new AwSEEGPad(_center, radius);
				}
			}
		}
		else {
			ball = new AwSEEGPad(pad->center, radius);
		}

		if (ball) {
			// compute color based on value (positive = red, negative = blue)
			if (value > 0)
				ball->actor->GetProperty()->SetColor(1.0, 0, 0);
			else
				ball->actor->GetProperty()->SetColor(0, 0, 1.0);
			m_balls.append(ball);
		}
	}
	updateMap();
}

void AwSEEGWidget::updateMap()
{
	foreach (AwSEEGPad *pad, m_balls) {
		m_renderer->AddActor(pad->actor);
	}
	vtkSmartPointer<vtkRenderWindow> window = m_ui->widget->GetRenderWindow();
	window->Render();
}

void AwSEEGWidget::showElectrodesLabels(bool on)
{
	m_renderer->ResetCameraClippingRange();
	if (on)	{
		for (int i = 0; i < m_labelActors.size(); i++)	{
			m_renderer->AddActor(m_labelActors.at(i));
			m_labelActors.at(i)->SetCamera(m_renderer->GetActiveCamera());
		}
	}
	else {
		for (int i = 0; i < m_labelActors.size(); i++)	{
			m_renderer->RemoveActor(m_labelActors.at(i));
		}
	}
	vtkSmartPointer<vtkRenderWindow> window = m_ui->widget->GetRenderWindow();
	window->Render();
}