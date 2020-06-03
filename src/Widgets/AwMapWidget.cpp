/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit dAix Marseille (AMU) - 
//                 Institut National de la Sant et de la Recherche Mdicale (INSERM)
//                 Copyright  2013 AMU, INSERM
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
//    Author: Bruno Colombet  Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include <vtkVersion.h>
#if VTK_MAJOR_VERSION > 5
#include <vtkAutoInit.h>
#endif

#include <widget/AwMapWidget.h>
#include <vtkDiskSource.h>
#include <vtkKdTreePointLocator.h>
#include <vtkPointData.h>
#include <vtkFloatArray.h>
#include <vtkUnsignedCharArray.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkProperty.h>
#include <vtkActor.h>
#include <vtkFloatArray.h>
#include <vtkCylinderSource.h>
#include <vtkGlyph3D.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkTextProperty.h>
#include <vtkProperty2D.h>
#include <vtkVectorText.h>
#include <vtkIntArray.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkLookupTable.h>
#include <utils/time.h>
#include <widget/AwTopoBuilder.h>
#include <QDebug>

#define AW_PI 3.1415926535
#define AW_CONTOURS 32

AwMapWidget::AwMapWidget(QWidget *parent, int flags) : QVTK_CLASS(parent)
{
#if VTK_MAJOR_VERSION < 7
 	VTK_MODULE_INIT(vtkRenderingOpenGL);
	VTK_MODULE_INIT(vtkInteractionStyle);
	VTK_MODULE_INIT(vtkRenderingFreeType);
#else
	VTK_MODULE_INIT(vtkRenderingOpenGL2);
    VTK_MODULE_INIT(vtkInteractionStyle);
    VTK_MODULE_INIT(vtkRenderingFreeType);
#endif

	m_flags = flags;
	m_map = NULL;
	m_colorMap = AwColorMap::Parula;
	m_red[0] = 1.0;
	m_red[1] = m_red[2] = 0.0;
	m_white[0] = m_white[1] = m_white[2] = 1;
	m_latency = -1;
	m_contoursOn = false;

	connect(this, SIGNAL(topoChanged(AwMapWidget *, const QStringList&, float *)),
		AwTopoBuilder::instance(), SLOT(computeTopo(AwMapWidget *, const QStringList&, float *)));
	connect(this, SIGNAL(badLabelsChanged(AwMapWidget *, const QStringList&)),
		AwTopoBuilder::instance(), SLOT(updateBadSensors(AwMapWidget *, const QStringList&)));
	connect(AwTopoBuilder::instance(), SIGNAL(computationDone(AwMapWidget *)), 
		this, SLOT(update(AwMapWidget *)));

	// instantiate renderer
	m_renderer = vtkSmartPointer<vtkRenderer>::New();
#if VTK_MAJOR_VERSION >= 8
	m_window = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
	m_window->AddRenderer(m_renderer);
	SetRenderWindow(m_window.Get());
#else
	m_window = vtkSmartPointer<vtkRenderWindow>::New();
	m_window->AddRenderer(m_renderer);
	SetRenderWindow(m_window);
#endif
}


void AwMapWidget::initWithLayout(AwLayout *layout)
{
	m_map = new AwMap(layout);
	initVtk();
}

void AwMapWidget::setBackgroundColor(const QColor& color)
{
	m_renderer->SetBackground(color.redF(), color.blueF(), color.greenF());
}

void AwMapWidget::setBackgroundColor(const QString& name)
{
	QColor color(name);
	if (color.isValid())
		setBackgroundColor(color);
}

AwMapWidget::~AwMapWidget()
{
}


void AwMapWidget::updateMap(float lat, const QVector<float>& values, const QStringList& labels)
{
	if (m_map == NULL)
		return;
	m_data = values;
	buildTopo(labels);
}

void AwMapWidget::updateMap(float lat, arma::fvec& values, const QStringList& labels)
{
	if (m_map == NULL)
		return;
	m_data = QVector<float>(values.n_elem);
	for (arma::uword i = 0; i < values.n_elem; i++)
		m_data << values(i);
//	m_data = values;
	buildTopo(labels);
}


void AwMapWidget::initVtk()
{
	// instantiate a the lookup table
	m_lut = vtkSmartPointer<vtkLookupTable>::New();

	vtkSmartPointer<vtkPolyData> sensor_pdata;

	if (!(m_flags & AwMapWidget::NoLabels))	{
		// build sensors
		sensor_pdata = vtkSmartPointer<vtkPolyData>::New();
		vtkSmartPointer<vtkPoints> sensorPoints = vtkSmartPointer<vtkPoints>::New();
		
		foreach (AwSensor *s, m_map->sensors())
			vtkIdType pid = sensorPoints->InsertNextPoint(s->coordinates());

		sensor_pdata->SetPoints(sensorPoints);

		// check shorter distance between sensors
		AwSensor *first = m_map->sensors().at(0);
		double dist = 10000;
		for (int i = 1; i < m_map->sensors().size(); i++) {
			double d =  m_map->distance(first->coordinates(), m_map->sensors().at(i)->coordinates());
			if (d < dist)
				dist = d;
		}

		vtkSmartPointer<vtkCylinderSource> cylinder = vtkSmartPointer<vtkCylinderSource>::New();
		cylinder->SetRadius(dist * 0.1);	// size of cylinder is 10% of minimal distance between sensors.
		cylinder->SetHeight(dist * 0.1);
		cylinder->SetResolution(100);
		cylinder->Update();
		m_sensorsActor = vtkSmartPointer<vtkActor>::New();
		m_sensorsActor->GetProperty()->SetColor(m_white);
		m_sensorsActor->SetPickable(true);

		vtkSmartPointer<vtkGlyph3D> glyph = vtkSmartPointer<vtkGlyph3D>::New();
#if VTK_MAJOR_VERSION < 6
		glyph->SetSource(cylinder->GetOutput());
        glyph->SetInput(sensor_pdata);
#else
		glyph->SetSourceConnection(cylinder->GetOutputPort());
        glyph->SetInputData(sensor_pdata);
#endif
		glyph->Update();

		vtkSmartPointer<vtkPolyDataMapper> sensorMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
#if VTK_MAJOR_VERSION < 6
		sensorMapper->SetInput(glyph->GetOutput());
#else
        sensorMapper->SetInputConnection(glyph->GetOutputPort());
#endif
		m_sensorsActor->SetMapper(sensorMapper);

		// build sensor labels
		foreach (AwSensor *s, m_map->sensors()) 	{
			vtkSmartPointer<vtkVectorText> tlabel = vtkSmartPointer<vtkVectorText>::New();
#if QT_VERSION < 5
			tlabel->SetText(s->label().toAscii().data());
#else
			tlabel->SetText(s->label().toLatin1().data());
#endif
			tlabel->Update();
			vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
#if VTK_MAJOR_VERSION < 6
			mapper->SetInput(tlabel->GetOutput());
#else
            mapper->SetInputData(tlabel->GetOutput());
#endif
			vtkSmartPointer<vtkFollower> textActor = vtkSmartPointer<vtkFollower>::New();
			textActor->SetPickable(false);
			textActor->SetMapper(mapper);
			// scale of text is 30% of minimal distance between sensors.
			textActor->SetScale(dist * 0.3, dist * 0.3, dist * 0.3);
			textActor->GetProperty()->SetColor(m_white);

			// translate positions of text labels
			// add offset to coordinates depending on the coordinates itself => locate the labels outside of mesh.
			double *p = s->coordinates();
			
			// positioning Text at 20% of distance.
			textActor->AddPosition(p[0], p[1], p[2] + dist * 0.2);

			m_labelActors << textActor;
		}
	}

	if (!(m_flags & AwMapWidget::NoLatencyDisplay))	{
		// Latency
		m_latencyActor = vtkSmartPointer<vtkTextActor>::New();
		m_latencyActor->SetTextScaleMode(vtkTextActor::TEXT_SCALE_MODE_NONE);
		m_latencyActor->GetProperty()->SetDisplayLocationToForeground();
		m_latencyActor->GetTextProperty()->SetFontFamilyToArial();
		m_latencyActor->GetTextProperty()->SetFontSize(14);
		m_latencyActor->GetProperty()->SetColor(1, 0, 0);
		m_latencyActor->GetTextProperty()->SetJustificationToLeft();
		m_latencyActor->SetPosition(10, 10);
		m_latencyActor->SetPickable(false);
		m_renderer->AddActor(m_latencyActor);
	}

	// create a special FLAG text actor (for example to specify the map is a PCA representation)
	m_textFlagActor = vtkSmartPointer<vtkTextActor>::New();
	m_textFlagActor->SetTextScaleMode(vtkTextActor::TEXT_SCALE_MODE_NONE);
	m_textFlagActor->GetProperty()->SetDisplayLocationToForeground();
	m_textFlagActor->GetTextProperty()->SetFontFamilyToArial();
	m_textFlagActor->GetTextProperty()->SetFontSize(14);
	m_textFlagActor->GetProperty()->SetColor(1, 0, 0);
	m_textFlagActor->GetTextProperty()->SetJustificationToLeft();
	m_textFlagActor->SetPosition(10, 30);
	m_textFlagActor->SetPickable(false);
	m_textFlag.isEmpty() ? m_renderer->RemoveActor(m_textFlagActor) : m_renderer->AddActor(m_textFlagActor);
		
	m_mainMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
#if VTK_MAJOR_VERSION < 6
    m_mainMapper->SetInput(m_map->pdata());
#else
   m_mainMapper->SetInputData(m_map->pdata());
#endif
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetPickable(false);
	actor->SetMapper(m_mainMapper);

	if (!(m_flags & AwMapWidget::NoColorMap)) {
		// colormap display
		m_cmActor =	vtkSmartPointer<vtkScalarBarActor>::New();
		m_cmActor->SetLookupTable(m_mainMapper->GetLookupTable());
		m_cmActor->SetTitle("Amplitudes");
		m_cmActor->GetTitleTextProperty()->SetFontSize(16);
		m_cmActor->GetTitleTextProperty()->SetFontFamilyToArial();
		m_cmActor->SetNumberOfLabels(4);
		m_cmActor->SetWidth(0.1);
		m_cmActor->SetHeight(0.8);
		m_cmActor->SetPosition(0.85, 0.1);
		m_cmActor->SetLabelFormat("%-#6.3E");
		m_cmActor->GetLabelTextProperty()->SetFontFamilyToArial();
		m_cmActor->GetLabelTextProperty()->SetFontSize(12);
		m_cmActor->GetLabelTextProperty()->SetJustificationToCentered();
		m_cmActor->SetPickable(false);
		m_renderer->AddActor2D(m_cmActor);
	}

	if (!(m_flags & AwMapWidget::NoContours))	{
		m_contourActor = vtkSmartPointer<vtkActor>::New();
		m_contourActor->SetPickable(false);
	}
	
	m_renderer->AddActor(actor);
//	GetRenderWindow()->AddRenderer(m_renderer);
	if (!(m_flags & AwMapWidget::NoInteraction))	{
		// insert AnyWave style to handle mouse and keyboard.
		vtkSmartPointer<AwInteractorStyle> style = vtkSmartPointer<AwInteractorStyle>::New();
		style->setPolyData(sensor_pdata);
		style->setWidget(this);
		style->setActor(m_sensorsActor);
		style->SetDefaultRenderer(m_renderer);
#if VTK8_SUPPORT
		m_window->GetInteractor()->SetInteractorStyle(style);
#else
		GetInteractor()->SetInteractorStyle(style);
#endif
		if (m_flags & AwMapWidget::NoRotation)
			style->setRotation(false);
	}
	showContours(false);
}


void AwMapWidget::setMap(AwMap *map)
{
	if (m_map)
		delete m_map;
	m_map = map;
	initVtk();
}

void AwMapWidget::setLatency(float lat, bool HMS)
{
	m_latency = lat;
	m_textFlag.clear();
	m_textFlag.isEmpty() ? m_renderer->RemoveActor(m_textFlagActor) : m_renderer->AddActor(m_textFlagActor);
	if (m_flags & AwMapWidget::NoLatencyDisplay)
		return;
	if (m_latency > -1)	{
		QString sLatency;
		if (HMS)
			sLatency = AwUtilities::time::hmsTime(lat);
		else
			sLatency.sprintf("%.3fs", m_latency);  
		m_latencyActor->SetInput(sLatency.toLatin1().data());
	}
}

void AwMapWidget::setLatency(float start, float end, bool HMS)
{
	m_latency = start;
	if (m_flags & AwMapWidget::NoLatencyDisplay)
		return;
	if (m_latency > -1)	{
		QString sLatency, sEnd;
		if (HMS) {
			sLatency = AwUtilities::time::hmsTime(start);
			sEnd = AwUtilities::time::hmsTime(end);
		}
		else {
			sLatency.sprintf("%.3fs", m_latency);
			sEnd.sprintf("%.3fs", end);
		}
		QString sInterval = "[ " + sLatency + " - " + sEnd + " ]";
		m_textFlag = "PCA";

		m_latencyActor->SetInput(sInterval.toLatin1().data());
		m_textFlagActor->SetInput(m_textFlag.toLatin1().data());
		m_textFlag.isEmpty() ? m_renderer->RemoveActor(m_textFlagActor) : m_renderer->AddActor(m_textFlagActor);
	}
}

void AwMapWidget::update(AwMapWidget *widget)
{
	if (widget != this)
		return;
	buildColorMap();
	buildContours();
	GetRenderWindow()->Render();
	m_pixmap = QPixmap(size());
	render(&m_pixmap);
}

void AwMapWidget::buildTopo(const QStringList& labels)
{
#ifndef NDEBUG
	qDebug() << Q_FUNC_INFO << "started." << endl;
#endif
#ifndef NDEBUG
	qDebug() << Q_FUNC_INFO << "request for mapping." << endl;
#endif
	emit topoChanged(this, labels, m_data.data());
#ifndef NDEBUG
	qDebug() << Q_FUNC_INFO << "request emited." << endl;
#endif
}

void AwMapWidget::updateBadSensors(const QStringList& labels)
{
	m_badLabels = labels;
	emit badLabelsChanged(this, labels);
}

void AwMapWidget::updateSensorSelectionStatus(int id)
{
	if (id < m_map->sensors().size())	{
		AwSensor *s = m_map->sensors().at(id);
		if (m_selectedLabels.contains(s->label()))	{
			// label was previously selected => unselect it
			m_labelActors.at(id)->GetProperty()->SetColor(m_white);
			m_selectedLabels.removeAll(s->label());
		}
		else  {  // label was not selected => select it
			m_selectedLabels << s->label();
			m_labelActors.at(id)->GetProperty()->SetColor(m_red);
		}
		emit selectedLabelsChanged(m_selectedLabels);
	}
}


void AwMapWidget::buildContours()
{
	vtkFloatArray *scalars = vtkFloatArray::SafeDownCast(m_map->pdata()->GetPointData()->GetScalars());
	m_contourFilter = vtkSmartPointer<vtkContourFilter>::New();

	double range[2];
	double rangeContour[2];
	
	scalars->GetRange(range);
	double interv = (range[1] - range[0]) / (AW_CONTOURS - 1);
	for (int i = 0; i < AW_CONTOURS; i++)	{
		double val = range[0] + i * interv;
		m_contourFilter->SetValue(i, val);
	}
#if VTK_MAJOR_VERSION < 6
   m_contourFilter->SetInput(m_map->pdata());
#else
    m_contourFilter->AddInputData(m_map->pdata());
#endif
	m_contourFilter->Update();
	m_contourFilter->GetOutput()->GetPointData()->GetScalars()->GetRange(rangeContour);

	m_contourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	m_contourMapper->SetInputConnection(m_contourFilter->GetOutputPort()); 
	m_contourMapper->SetScalarRange(rangeContour[0], rangeContour[1]);
	m_contourMapper->SetLookupTable(m_mainMapper->GetLookupTable()); // use the same lut than main mapper (mesh).
	m_contourActor->SetMapper(m_contourMapper);
#if VTK8_SUPPORT
	m_window->Render();
#else
	GetRenderWindow()->Render();
#endif
}

void AwMapWidget::buildColorMap()
{
	vtkFloatArray *scalars = vtkFloatArray::SafeDownCast(m_map->pdata()->GetPointData()->GetScalars());
	double range[2];
	
	scalars->GetRange(range);
	m_cvalues = AwColorMap::valuesForMap(m_colorMap);
	m_lut->SetNumberOfTableValues(m_cvalues.count());

	for (int i = 0; i < m_cvalues.count(); i++)	{
		double colors[3];
		m_cvalues.colorAt(i, colors);
		m_lut->SetTableValue(i, colors[0], colors[1], colors[2], 1.0);
	}
	m_lut->SetTableRange(range);
	m_lut->Build();

#if VTK_MAJOR_VERSION < 6
    m_mainMapper->SetInput(m_map->pdata());
#else
    m_mainMapper->SetInputData(m_map->pdata());
#endif

	if (!m_contoursOn)
		m_mainMapper->ScalarVisibilityOn();
	else
		m_mainMapper->ScalarVisibilityOff();

	m_mainMapper->UseLookupTableScalarRangeOn();
	m_mainMapper->SetColorModeToMapScalars();
	m_mainMapper->SetLookupTable(m_lut);
	if (!(m_flags & AwMapWidget::NoColorMap))
		m_cmActor->SetLookupTable(m_lut);
#if VTK_MAJOR_VERSION >= 8
	m_window->Render();
#else
	GetRenderWindow()->Render();
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////.
/// SLOTS


void AwMapWidget::selectSensors(const QStringList& labels)
{
	m_selectedLabels = labels;
	// unselect all sensors
	for (int i = 0; i < m_map->sensors().size(); i++)	{
		AwSensor *s = m_map->sensors().at(i);
		double *color;
		if (labels.contains(s->label()))
			color =  m_red;
		else 
			color = m_white;
		m_labelActors.at(i)->GetProperty()->SetColor(color);
	}
#if VTK_MAJOR_VERSION >= 8
	m_window->Render();
#else
	GetRenderWindow()->Render();
#endif
}


void AwMapWidget::showSensors(bool flag)
{
	if (m_flags & AwMapWidget::NoLabels)
		return;

	m_renderer->ResetCameraClippingRange();
	if (flag)	{
		m_renderer->AddActor(m_sensorsActor);
		for (int i = 0; i < m_labelActors.size(); i++)	{
			m_renderer->AddActor(m_labelActors.at(i));
			m_labelActors.at(i)->SetCamera(m_renderer->GetActiveCamera());
		}
	}
	else {
		m_renderer->RemoveActor(m_sensorsActor);
		for (int i = 0; i < m_labelActors.size(); i++)	{
			m_renderer->RemoveActor(m_labelActors.at(i));
		}
	}
#if VTK_MAJOR_VERSION >= 8
	m_window->Render();
#else
	GetRenderWindow()->Render();
#endif
}


void AwMapWidget::showContours(bool flag)
{
	if (m_flags & AwMapWidget::NoContours)
		return;

	if (flag)	{
		// hide color mapping of scalars for the mesh
		m_mainMapper->ScalarVisibilityOff();
		m_renderer->AddActor(m_contourActor);
		m_contoursOn = true;
	}
	else	{
		m_mainMapper->ScalarVisibilityOn();
		m_renderer->RemoveActor(m_contourActor);
		m_contoursOn = false;
	}
#if VTK_MAJOR_VERSION >= 8
	m_window->Render();
#else
	GetRenderWindow()->Render();
#endif
}

void AwMapWidget::changeColorMap(int colorMap)
{
	m_colorMap = colorMap;
	buildColorMap();
	buildContours();
}

void AwMapWidget::showColorMap(bool flag)
{
	if (m_flags & AwMapWidget::NoColorMap)
		return;

	if (!flag)
		m_renderer->RemoveActor2D(m_cmActor);
	else
		m_renderer->AddActor2D(m_cmActor);
#if VTK_MAJOR_VERSION >= 8
	m_window->Render();
#else
	GetRenderWindow()->Render();
#endif
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// AwInteractorStyle

vtkStandardNewMacro(AwInteractorStyle);

void AwInteractorStyle::setPolyData(vtkSmartPointer<vtkPolyData> pdata)
{
	m_pdata = pdata;
	m_locator = vtkSmartPointer<vtkKdTreePointLocator>::New();
	m_locator->SetDataSet(m_pdata);
	m_locator->BuildLocator();
}

void AwInteractorStyle::OnKeyDown()
{
	std::string key = this->Interactor->GetKeySym();
	QString skey(key.c_str());
	if (skey.contains("Control"))
		m_ctrlPressed = true;
}

void AwInteractorStyle::OnMouseMove()
{
	if (m_noRotation)
		return;
	vtkInteractorStyleTrackballCamera::OnMouseMove();
}

void AwInteractorStyle::OnKeyUp()
{
	std::string key = this->Interactor->GetKeySym();
	QString skey(key.c_str());
	if (skey.contains("Control"))
		m_ctrlPressed = false;
}


void AwInteractorStyle::OnLeftButtonDown()
{
	int x = this->Interactor->GetEventPosition()[0];
	int y = this->Interactor->GetEventPosition()[1];
	this->FindPokedRenderer(x, y);

	vtkInteractorStyleTrackballCamera::OnLeftButtonDown();

	if (!m_ctrlPressed)
		return;

	m_picker->Pick(x, y, 0, this->GetDefaultRenderer());

	// pick only in sensors actor
	if (m_picker->GetActor() != m_sensorActor)
		return;

	// verify if point was pick
	vtkIdType id = m_picker->GetPointId();
	if (id == -1)
		return;

	// get world coodinates.
	double *pos = m_picker->GetPickPosition();
	id = m_locator->FindClosestPoint(pos);
	if (id == -1)
		return;

	m_widget->updateSensorSelectionStatus((int)id);
}
