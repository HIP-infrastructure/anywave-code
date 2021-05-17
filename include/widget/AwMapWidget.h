#ifndef AW_MAPWIDGET_H
#define AW_MAPWIDGET_H
#include <AwGlobal.h>

#include <AwSensor.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkPolyDataMapper.h>
#include <vtkScalarBarActor.h>
#include <vtkTextActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkFollower.h>
#include <vtkContourFilter.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkObjectFactory.h>
#include <vtkKdTreePointLocator.h>
#include <vtkPointPicker.h>
#include <graphics/AwColorMap.h>
#include <layout/AwLayout.h>
#include <mapping/AwMap.h>

#if VTK_MAJOR_VERSION >= 8
#include <QVTKOpenGLWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>
#define QVTK_CLASS QVTKOpenGLWidget		
#else
#include <QVTKWidget.h>
#define QVTK_CLASS  QVTKWidget		
#endif

#include <aw_armadillo.h>

class AW_WIDGETS_EXPORT AwMapWidget : public QVTK_CLASS
{
	Q_OBJECT
public:
	enum updateFlags { Data = 1, Sensors = 2, Contours = 4 };
	enum settings { NoColorMap = 1, NoLabels = 2, NoContours = 4, NoLatencyDisplay = 8, NoRotation = 16, NoInteraction = 32 };
	AwMapWidget(QWidget *parent = 0, int flags = 0);
	void initWithLayout(AwLayout *layout);
	~AwMapWidget();
	vtkSmartPointer<vtkRenderWindow> window() { return GetRenderWindow(); }
	inline AwMap *map() { return m_map; }
	void setMap(AwMap *map);
	void updateSensorSelectionStatus(int id);
	void setFlags(int flags) { m_flags = flags; }
	void setBackgroundColor(const QString& color);
	void setBackgroundColor(const QColor& color);
	inline QPixmap& pixmap() { return m_pixmap; }
public slots:
	void setLatency(float lat, bool HMS);				// show a time position
	void setLatency(float start, float end, bool HMS);	// show a time interval
	void showSensors(bool flag);
	void showContours(bool flag);
	void showColorMap(bool flag);
	void changeColorMap(int colormap);
	void selectSensors(const QStringList& labels);
	void updateBadSensors(const QStringList& labels);	
	void update(AwMapWidget *widget);

	void updateMap(float latency, const QVector<float>& values, const QStringList& labels);
	void updateMap(float latency, arma::fvec& values, const QStringList& labels);
signals:
	void sensorSelectionStatusChanged(const QString& label, bool flag);
	void selectedLabelsChanged(const QStringList& labels);
	void topoChanged(AwMapWidget *w, const QStringList& labels, float *values);
	void badLabelsChanged(AwMapWidget *w, const QStringList& labels);
protected:
	void initVtk();
	void buildTopo(const QStringList& labels);
	
	int m_flags;
	QStringList m_selectedLabels;
	QStringList m_badLabels, m_validLabels;
	float m_latency;
	AwMap *m_map;
	int update_flags;
	double m_red[3];
	double m_white[3];
	QVector<float> m_data;
	// colormaps
	int m_colorMap;
	AwColorMapValues m_cvalues;	// Color Map Values for the current map type.
	// VTK
	vtkSmartPointer<vtkTextActor> m_latencyActor;
	vtkSmartPointer<vtkTextActor> m_textFlagActor;
	QString m_textFlag;
	vtkSmartPointer<vtkPolyDataMapper> m_mainMapper;
	vtkSmartPointer<vtkPolyDataMapper> m_contourMapper;
	vtkSmartPointer<vtkActor> m_contourActor;
	vtkSmartPointer<vtkActor> m_sensorsActor;
	vtkSmartPointer<vtkRenderer> m_renderer;
#if VTK8_SUPPORT
	vtkSmartPointer<vtkGenericOpenGLRenderWindow> m_window;
#else
	vtkSmartPointer<vtkRenderWindow> m_window;
#endif
	vtkSmartPointer<vtkScalarBarActor> m_cmActor;
	QList<vtkSmartPointer<vtkFollower> > m_labelActors;
	vtkSmartPointer<vtkContourFilter> m_contourFilter;
	vtkSmartPointer<vtkLookupTable> m_lut;
	bool m_contoursOn;
	QPixmap m_pixmap;

	// methods
	void buildColorMap();
	void buildContours();
};

Q_DECLARE_METATYPE(AwMapWidget *);


class AwInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
	static AwInteractorStyle* New();
	AwInteractorStyle()
	{
		m_picker = vtkSmartPointer<vtkPointPicker>::New();
		m_picker->SetTolerance(0.005);
		m_ctrlPressed = false;
		m_noRotation = false;
	}
	vtkTypeMacro(AwInteractorStyle, vtkInteractorStyleTrackballCamera);

	void setWidget(AwMapWidget *widget) { m_widget = widget; }
	void setPolyData(vtkSmartPointer<vtkPolyData> pdata);
	void setActor(vtkSmartPointer<vtkActor> actor) { m_sensorActor = actor; }
	// activate or disable the scene rotation with mouse.
	void setRotation(bool onoff) { m_noRotation = !onoff; }

	// EVENTS
	virtual void OnLeftButtonDown() override;
	virtual void OnKeyDown() override;
	virtual void OnKeyUp() override;
	void OnMouseMove() override;

private:
	bool m_ctrlPressed;
	bool m_noRotation;
	vtkSmartPointer<vtkPolyData> m_pdata;
	vtkSmartPointer<vtkActor> m_sensorActor;
	vtkSmartPointer<vtkPointPicker> m_picker;
	vtkSmartPointer<vtkKdTreePointLocator> m_locator;
	AwMapWidget *m_widget;
};


#endif // AW2DMAP_H
