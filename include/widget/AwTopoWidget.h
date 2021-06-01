#ifndef AWTOPOWIDGET_H
#define AWTOPOWIDGET_H

#include <AwSensor.h>
#include <AwGlobal.h>
#include <aw_armadillo.h>
#include <QWidget>
class AwLayout;
class Aw2DGrid;
class QwtPlot;
class QwtPlotSpectrogram;

namespace Ui {class AwTopoWidgetUi;};
class AW_WIDGETS_EXPORT AwTopoWidget : public QWidget
{
	Q_OBJECT

public:
	AwTopoWidget(QWidget *parent = 0, AwLayout *layout = NULL, int flags = 0);
	AwTopoWidget(const QString& title, QWidget *parent = 0, AwLayout *layout = NULL, int flags = 0);
	~AwTopoWidget();

	enum Flags { ShowLatency = 1, ShowValue = 2, ShowOptions = 4, ShowFLAGS = 8, ShowLabel = 16 };
	

	inline AwLayout *layout() { return m_layout; }
	void setLayout(AwLayout *layout);
	inline int colorMap() { return m_cmap; }
	void setLabel(const QString& label);
	void set2DGrid(Aw2DGrid *grid);
	void setBackgroundColor(const QString& color);
	inline bool sensorsEnabled() { return m_showSensors; }
	inline bool contoursEnabled() { return m_showContours; }
	inline bool labelsEnabled() { return m_showLabels; }
	inline int contourLevels() { return m_nContours; }
	const QRect& imageGeometry() const;
	void setUnit(const QString& unit);
	void setFlags(int flags);
	void setContourLevels(int nContours);
	void redraw();
	inline QPixmap& pixmap() { return m_pixmap; }
protected:
	void changeEvent(QEvent*) override;
	void resizeEvent(QResizeEvent*) override;
	void buildTopo();
	void setLatency(float lat);
	void setLatency(float start, float end);
signals:
	void topoChanged(AwTopoWidget *w, const QStringList& labels, double *values);
	void selectedLabelsChanged(const QStringList& labels);
public slots:
	void enableContours(bool flag);
	void enableSensors(bool flag);
	void enableLabels(bool flag);
	void draw2(AwTopoWidget *widget, Aw2DGrid *grid);
	void updateMap(float latency, const QVector<float>& values, const QStringList& labels);
	void updateMap(float start, float end, const QVector<float>& values, const QStringList& labels);
	void updateMap(float latency, const QVector<double>& values, const QStringList& labels);
	void updateMap(float start, float end, const QVector<double>& values, const QStringList& labels);
	void updateMap(float latency, arma::fvec& values, const QStringList& labels);
	void openUI();
	void selectSensors(const QStringList& labels);
	void updateBadSensors(const QStringList& labels);
	void setColorMap(int cmap) { m_cmap = cmap; redraw(); }
	void setComputationFlags(const QString& flags);	// display a flag indicating that the mapping was computing using a special method (PCA, etc..)
	void resetComputationFlags();
protected slots:
	void showCL();
	void changeNumberOfContours(bool flag);
	void showValue(double value);
	void clickAtPosition(double x, double y);
private:
	Ui::AwTopoWidgetUi *ui;

	AwLayout *m_layout;
	Aw2DGrid *m_grid;
	QImage m_image;
	QPixmap m_pixmap;
	int m_res;
	int m_cmap;	// colormap used to draw topography.
	QwtPlotSpectrogram *m_spectro;
	QwtPlot *m_plot;
	bool m_showContours, m_showSensors, m_showLabels, m_redrawing;
	int m_nContours;	// number of contour lines to be displayed.
	QStringList m_selectedLabels;
	QStringList m_badLabels;
	QStringList m_validLabels;	// valid labels (not bad ones)
	QVector<double> m_data;
	int m_flags;
	float m_latency;	// actual position of mapping in seconds.
	QString m_unit;		// unit of values

	// min and max values for X and Y coordinates of the current layout
	qreal m_smin_x, m_smin_y, m_smax_x, m_smax_y;
	qreal m_sensor_radius;	// radius of sensors base on current head size.
	qreal m_pad, m_head_size;	// ears and nose padding and head size (in pixels)
	int m_size;	// total size in pixel for the head (including ears and nose).
	// A sensor list with adjusted 2D coordinates 
	AwSensorList m_pixelSensors;
};

Q_DECLARE_METATYPE(AwTopoWidget *);


#endif // AWTOPOWIDGET_H
