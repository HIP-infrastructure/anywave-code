#ifndef AWSEEGWIDGET_H
#define AWSEEGWIDGET_H
#include <QWidget>
#include <vtkRenderer.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataNormals.h>
#include <vtkActor.h>
#include <vtkFollower.h>
#include <AwChannel.h>
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
	AwSEEGWidget(const QString& meshFile = QString(), QWidget *parent = 0);
	~AwSEEGWidget();
public slots:
	void loadElectrodes();
	void loadElectrodes(const QString& file);
	void changeMeshOpacity(int value);
	void openMesh(const QString& file);
	void loadMesh();
	void changeFastRendering(bool on);
	void changeSmoothRendering(bool on);
	void computeMap(AwChannelList& channels, float latency, qint64 sample);
	void showElectrodesLabels(bool on);
signals:
	void closed();
protected:
	void clearElectrodes();
	void clearBalls();
	void updateMap();
	void closeEvent(QCloseEvent *e);

	void generateElectrodesLabels();

	AwSEEGViewer *m_viewer;
	QHash<QString, AwSEEGPad *> m_hPads;
	QList<AwSEEGPad *> m_electrodes;  // real electrodes
	vtkSmartPointer<vtkPolyDataMapper> m_meshMapper;
	vtkSmartPointer<vtkActor> m_meshActor;
	vtkSmartPointer<vtkRenderer> m_renderer;
	vtkSmartPointer<vtkRenderer> m_electrodesRenderer;
	vtkSmartPointer<vtkPolyData> m_mesh;
	vtkSmartPointer<vtkPolyDataNormals> m_smoothMesh;
	QList<vtkSmartPointer<vtkFollower> > m_labelActors;
	QList<AwSEEGPad * > m_balls;	// temporary balls representing amplitude values
	QHash<QString, QStringList *> m_electrodesLabels;
	double m_meshBounds[6];
	double m_a, m_l;
	Ui::AwSEEGWidgetUi *m_ui;
};

#endif // AWSEEGWIDGET_H
