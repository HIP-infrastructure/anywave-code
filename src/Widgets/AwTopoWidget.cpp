/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
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
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include <widget/AwTopoWidget.h>
#include <layout/AwLayoutManager.h>
#include <layout/AwLayout.h>
#include <QMessageBox>
#include <QPainter>
#include "Aw2DGrid.h"
#include <graphics/AwColorMap.h>
#include <QtCore/qmath.h>
#include <math.h>
#include <QGridLayout>
#include <QApplication>
#include <QResizeEvent>
#include <widget/AwTopoBuilder.h>
#include <QMenu>
#include <AwUtilities.h>
#include "AwTopoWidgetSettings.h"
//////
/// QWT
#include "AwQwtColorMap.h"
#include <qwt_matrix_raster_data.h>
#include <qwt_scale_map.h>
#include <qwt_color_map.h>
#include <qwt_plot_renderer.h>
#include <qwt_plot_layout.h>
#include <qwt_plot.h>
#include <qwt_plot_spectrogram.h>
#include <qwt_plot_canvas.h>

Q_DECLARE_METATYPE(QVector<double>);
#include "ui_AwTopoWidget.h"

AwTopoWidget::AwTopoWidget(QWidget *parent, AwLayout *layout, int flags)
	: QWidget(parent)
{
	ui = new Ui::AwTopoWidgetUi();
	ui->setupUi(this);
	setLayout(layout);
	m_grid = NULL;
	m_res = 200;	// resolution of interpolation grid
	m_cmap = AwColorMap::Parula;

	connect(this, SIGNAL(topoChanged(AwTopoWidget *, const QStringList&, double *)),
		AwTopoBuilder::instance(), SLOT(computeTopo(AwTopoWidget *, const QStringList&, double *)));
	connect(AwTopoBuilder::instance(), SIGNAL(computationDone(AwTopoWidget *, Aw2DGrid *)), 
		this, SLOT(draw2(AwTopoWidget *, Aw2DGrid *)));

	connect(ui->labelImage, SIGNAL(positionChanged(double)), this, SLOT(showValue(double)));
	connect(ui->labelImage, SIGNAL(clickedAt(double, double)), this, SLOT(clickAtPosition(double,double)));
	connect(ui->buttonOptions, SIGNAL(clicked()), this, SLOT(openUI()));
	// prepare qwt objects for the mapping display
	m_plot = new QwtPlot(this);
	m_plot->setContentsMargins(0, 0, 0, 0);
	m_plot->plotLayout()->setCanvasMargin(0);
	QwtPlotCanvas *canvas = (QwtPlotCanvas *)m_plot->canvas();
	canvas->setFrameShape(QFrame::NoFrame);
	m_plot->hide();
	m_spectro = new QwtPlotSpectrogram;
	m_spectro->setRenderThreadCount(0);
	m_spectro->attach(m_plot);
	m_showContours = m_showSensors = true;
	m_showLabels = false;
	m_nContours = 5;
	m_latency = -1;
	setFlags(flags);

	m_size = qMin(ui->labelImage->geometry().width(), ui->labelImage->geometry().height());
	m_pad = m_size * 0.05; // 5% for each ear
	m_head_size = m_size - 2 * m_pad;
	ui->labelImage->setPadSize(m_pad, m_head_size);
	m_sensor_radius = m_head_size * 0.01;
}

AwTopoWidget::AwTopoWidget(const QString& title, QWidget *parent, AwLayout *layout, int flags) : QWidget(parent)
{
	ui = new Ui::AwTopoWidgetUi();
	ui->setupUi(this);
	setLayout(layout);
	m_grid = NULL;
	m_res = 200;	// resolution of interpolation grid
	m_cmap = AwColorMap::Parula;

	connect(this, SIGNAL(topoChanged(AwTopoWidget *, const QStringList&, float *)),
		AwTopoBuilder::instance(), SLOT(computeTopo(AwTopoWidget *, const QStringList&, float *)));
	connect(AwTopoBuilder::instance(), SIGNAL(computationDone(AwTopoWidget *, Aw2DGrid *)), 
		this, SLOT(draw2(AwTopoWidget *, Aw2DGrid *)));

	connect(ui->labelImage, SIGNAL(positionChanged(double)), this, SLOT(showValue(double)));
	connect(ui->labelImage, SIGNAL(clickedAt(double, double)), this, SLOT(clickAtPosition(double,double)));
	connect(ui->buttonOptions, SIGNAL(clicked()), this, SLOT(openUI()));
	// prepare qwt objects for the mapping display
	m_plot = new QwtPlot(this);
	m_plot->setContentsMargins(0, 0, 0, 0);
	m_plot->plotLayout()->setCanvasMargin(0);
	QwtPlotCanvas *canvas = (QwtPlotCanvas *)m_plot->canvas();
	canvas->setFrameShape(QFrame::NoFrame);
	m_plot->hide();
	m_spectro = new QwtPlotSpectrogram;
	m_spectro->setRenderThreadCount(0);
	m_spectro->attach(m_plot);
	m_showContours = m_showSensors = true;
	m_showLabels = false;
	m_nContours = 5;
	m_latency = -1;
	setFlags(flags);
	m_size = qMin(ui->labelImage->geometry().width(), ui->labelImage->geometry().height());
	m_pad = m_size * 0.05; // 5% for each ear
	m_head_size = m_size - 2 * m_pad;
	ui->labelImage->setPadSize(m_pad, m_head_size);
	m_sensor_radius = m_head_size * 0.01;
	setWindowTitle(title);
}

AwTopoWidget::~AwTopoWidget()
{
	if (m_grid)
		delete m_grid;
	while (!m_pixelSensors.isEmpty())
		delete m_pixelSensors.takeFirst();
}

void AwTopoWidget::setLabel(const QString& label)
{
	ui->label->setText(label);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// EVENTS
void AwTopoWidget::changeEvent(QEvent *e)
{
	if (e) {
		if (e->type() == QEvent::LanguageChange) 
			ui->retranslateUi(this);
	}
}



void AwTopoWidget::setLayout(AwLayout *layout)
{
	m_layout = layout;
	if (layout == NULL) 
		return;
	// extract min and max X and Y values from current layout
	m_smin_x = m_layout->sensors().first()->x();
	m_smax_x = m_smin_x;
	m_smin_y = m_layout->sensors().first()->y();
	m_smax_y = m_smin_y;
	for (int i = 1; i < m_layout->sensors().size(); i++) {
		double x, y;
		x = m_layout->sensors().at(i)->x();
		y = m_layout->sensors().at(i)->x();
		if (x > m_smax_x)
			m_smax_x = x;
		if (x < m_smin_x)
			m_smin_x = x;
		if (y > m_smax_y)
			m_smax_y = y;
		if (y < m_smin_y)
			m_smin_y = y;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// EVENTS

void AwTopoWidget::resizeEvent(QResizeEvent *e)
{
	if (m_layout == NULL)
		return;

	// get the actual size of QLabel image in the widget
	QRect geo = ui->labelImage->geometry();
	m_size = qMin(geo.width(), geo.height());
	m_pad = m_size * 0.05; // 5% for each ear
	m_head_size = m_size - 2 * m_pad;
	ui->labelImage->setPadSize(m_pad, m_head_size);
	m_sensor_radius = m_head_size * 0.01;

	// delete previous pixel sensors coordinates
	while (!m_pixelSensors.isEmpty())
		delete m_pixelSensors.takeFirst();

	foreach (AwSensor *s, m_layout->sensors()) {
		qreal xi = (s->x() - m_smin_x)/(m_smax_x - m_smin_x) * m_head_size;
		qreal yi = m_head_size - (s->y() - m_smin_y)/(m_smax_y - m_smin_y) * m_head_size;
		// build a sensor with coordinates relative to current head.
		AwSensor *new_s = new AwSensor();
		new_s->setLabel(s->label());
		double coord[3];
		coord[0] = xi;
		coord[1] = yi;
		coord[2] = 0.;
		new_s->setCoordinates(coord);
		m_pixelSensors << new_s;
	}
	redraw();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// END OF EVENTS

const QRect& AwTopoWidget::imageGeometry() const 
{
	return ui->labelImage->geometry();
}

void AwTopoWidget::setBackgroundColor(const QString& color)
{
	int index = QColor::colorNames().indexOf(color);
	if (index != -1 ) {
		QString style = "background-color: " + color + ";";
		ui->labelImage->setStyleSheet(style);
	}
}

void AwTopoWidget::setFlags(int flags) 
{
	m_flags = flags; // default flags to zero => hide value and latency
	ui->labelLatency->setVisible(flags & AwTopoWidget::ShowLatency);
	ui->labelValue->setVisible(flags & AwTopoWidget::ShowValue);
	ui->buttonOptions->setVisible(flags & AwTopoWidget::ShowOptions);
	ui->labelFlags->setVisible(flags & AwTopoWidget::ShowFLAGS);
	ui->label->setVisible(flags & AwTopoWidget::ShowLabel);
}

void AwTopoWidget::showCL()
{
	QAction *act = (QAction *)sender();
	Q_ASSERT(act != NULL);
	enableContours(act->data().toBool());
}

void AwTopoWidget::changeNumberOfContours(bool flag)
{
	if (flag) {
		QAction *act = (QAction *)sender();
		Q_ASSERT(act != NULL);
		setContourLevels(act->data().toInt());
	}
}

void AwTopoWidget::clickAtPosition(double x, double y)
{
	// try to select a sensor if sensors are enable
	if (m_showSensors) {
		// browse all pixel sensors to find if one is close the position
		foreach (AwSensor *s, m_pixelSensors) {
			qreal dist = sqrt((x - s->x()) * (x - s->x()) + (y - s->y()) * (y - s->y()));
			if (dist <= m_sensor_radius) { // found a sensor near the click !
				if (m_selectedLabels.contains(s->label())) // sensor already selected
					m_selectedLabels.removeAll(s->label()); // set sensors as unselected.
				else
					m_selectedLabels.append(s->label());
				emit selectedLabelsChanged(m_selectedLabels);
				redraw();
				break;
			}
		}
	}
}

void AwTopoWidget::showValue(double value)
{
	ui->labelValue->setText(tr("Value:") + QString::number(value, 'g', 3) + m_unit);
}

void AwTopoWidget::setLatency(float lat)
{
	QString latency = QString::number(lat, 'g', 3) + "s ";
	QString HMS = AwUtilities::hmsTime(lat);
	ui->labelLatency->setText(tr("Latency:") + latency + "(" + HMS + ")");
}

void AwTopoWidget::setLatency(float start, float end)
{
	QString s_start = QString::number(start, 'g', 3) + "s ";
	QString s_end = QString::number(end, 'g', 3) + "s ";
	QString HMSStart = AwUtilities::hmsTime(start);
	QString HMSEnd = AwUtilities::hmsTime(end);
	if (AwUtilities::isTimeHMS())
		ui->labelLatency->setText("[ " + HMSStart + " - " + HMSEnd + " ]");
	else
		ui->labelLatency->setText("[ " + s_start + " - " + s_end + " ]");
}

void AwTopoWidget::updateMap(float start, float end, const QVector<double>& values, const QStringList& labels)
{
	if (m_layout == NULL)
		return;

	setLatency(start, end);
	m_data.clear();
	int index = 0;
	m_validLabels.clear();

	foreach (QString l, labels) {
		if (!m_badLabels.contains(l)) {
			m_validLabels << l;
			m_data << values.at(index);
		}
		index++;
	}
	buildTopo();
}

void AwTopoWidget::updateMap(float start, float end, const QVector<float>& values, const QStringList& labels)
{
	if (m_layout == NULL)
		return;
	QVector<double> val(values.size());
	for (int i = 0; i < values.size(); i++)
		val[i] = values[i];
	updateMap(start, end, val, labels);
}

void AwTopoWidget::updateMap(float lat, arma::fvec& values, const QStringList& labels)
{
	if (m_layout == NULL)
		return;

	setLatency(lat);
	m_data.clear();
	int index = 0;
	m_validLabels.clear();

	foreach(QString l, labels) {
		if (!m_badLabels.contains(l) && m_layout->labels().contains(l)) {
			m_validLabels << l;
			m_data << values.at(index);
		}
		index++;
	}
	buildTopo();
}

void AwTopoWidget::updateMap(float lat, const QVector<double>& values, const QStringList& labels)
{
	if (m_layout == NULL)
		return;

	setLatency(lat);
	m_data.clear();
	int index = 0;
	m_validLabels.clear();

	foreach (QString l, labels) {
		if (!m_badLabels.contains(l) && m_layout->labels().contains(l)) {
			m_validLabels << l;
			m_data << values.at(index);
		}
		index++;
	}
	buildTopo();
}

void AwTopoWidget::updateMap(float lat, const QVector<float>& values, const QStringList& labels)
{
	if (m_layout == NULL)
		return;
	QVector<double> val(values.size());
	for (int i = 0; i < values.size(); i++)
		val[i] = values[i];
	updateMap(lat, val, labels);
}

//
// set a new 2D interpolation grid
void AwTopoWidget::set2DGrid(Aw2DGrid *grid)
{
	if (m_grid)
		delete m_grid;
	m_grid = grid;
}


void AwTopoWidget::setContourLevels(int nContours)
{
	m_nContours = nContours;
	redraw();
}

void AwTopoWidget::buildTopo()
{
#ifndef NDEBUG
	qDebug() << Q_FUNC_INFO << "started." << endl;
#endif
	ui->labelImage->setText(tr("Computing mapping..."));
	repaint();
#ifndef NDEBUG
	qDebug() << Q_FUNC_INFO << "request for mapping." << endl;
#endif
	emit topoChanged(this, m_validLabels, m_data.data());
#ifndef NDEBUG
	qDebug() << Q_FUNC_INFO << "request emited." << endl;
#endif
}

void AwTopoWidget::redraw()
{
	if (m_grid == NULL)
		return;
	
	// do not display anything if size is too small
	if (m_size < 100)
		return;

	m_image = QImage(m_size, m_size, QImage::Format_ARGB32);
	m_image.fill(QColor(255, 255, 255, 0));

	QPainter painter, painterSpectro;
	painter.begin(&m_image);

	QPen outlinePen(QColor(0, 0, 0));
	outlinePen.setWidth(2);
	painter.setPen(outlinePen);
	// draw head limit
	painter.drawEllipse(QRectF(m_pad, m_pad, m_head_size, m_head_size));
	// draw nose
	painter.setBrush(QBrush(Qt::white));
	QPolygonF nose;
	nose << QPointF(m_size / 2, 1) << QPointF( m_size / 2 + m_pad, m_pad) << QPointF(m_size / 2 - m_pad, m_pad) ;
	painter.drawConvexPolygon(nose);

	QPolygonF ear;
	ear << QPointF(0, m_size / 2 + m_pad) << QPointF(0, m_size / 2 - m_pad) << QPointF(m_pad, m_size / 2 - m_pad) << QPointF(m_pad, m_size / 2 + m_pad);
	painter.drawConvexPolygon(ear);
	painter.drawConvexPolygon(ear.translated(m_size - m_pad, 0.0));

	painter.setBrush(Qt::NoBrush);

	// define the region to overlay the Qwt Spectro map
	painter.setClipRegion(QRegion(m_pad, m_pad, m_head_size, m_head_size, QRegion::Ellipse));

	QwtPlotRenderer renderer;
	m_plot->setFixedSize(QSize(qCeil(m_head_size), qCeil(m_head_size)));
	m_plot->setAxisScale(QwtPlot::xBottom, 0, m_grid->res);
	m_plot->setAxisScale(QwtPlot::yLeft, 0, m_grid->res);
	m_plot->enableAxis(QwtPlot::xBottom, false);
	m_plot->enableAxis(QwtPlot::yLeft, false);

	QImage imageSpectro(qCeil(m_head_size), qCeil(m_head_size), QImage::Format_ARGB32);
	imageSpectro.fill(QColor(255,255,255,0));
	painterSpectro.begin(&imageSpectro);
	m_spectro->setDisplayMode(QwtPlotSpectrogram::ImageMode);
	m_spectro->setDisplayMode(QwtPlotSpectrogram::ContourMode, false);
	m_spectro->setColorMap(AwQwtColorMap::newMap(m_cmap));
	m_spectro->invalidateCache();
	m_plot->replot();
	renderer.setDiscardFlag(QwtPlotRenderer::DiscardBackground) ;
	renderer.setDiscardFlag(QwtPlotRenderer::DiscardCanvasBackground);
	renderer.setDiscardFlag(QwtPlotRenderer::DiscardCanvasFrame );
	renderer.render(m_plot, &painterSpectro, painterSpectro.viewport());

	// contour lines
	if (m_showContours) {
		m_spectro->setDisplayMode(QwtPlotSpectrogram::ContourMode, true);
		QList<double> levels;
		double step = (m_grid->max_zi - m_grid->min_zi) / m_nContours;
		for (int i = 0; i < m_nContours; i++) {
			levels << m_grid->min_zi + i  * step;
		}
		m_spectro->setContourLevels(levels);
		m_spectro->invalidateCache();
		m_plot->replot();
		renderer.render(m_plot, &painterSpectro, painterSpectro.viewport());
	}

	// Paint sensors
	if (m_showSensors) {
		QBrush bSensors(QColor(255, 255, 255));
		QBrush bSelected(QColor(255, 0, 0));
		
		// compute sensor size based on head size (1% of total size)
		//qreal radius = head_size * 0.01;
		// compute font size based on radius
		QFont font("Arial", 2 * m_sensor_radius);
		painterSpectro.setFont(font);

		foreach(AwSensor *s, m_pixelSensors) {
			// check if sensor is selected
			if (m_selectedLabels.contains(s->label())) 
				painterSpectro.setBrush(bSelected);
			else
				painterSpectro.setBrush(bSensors);

			painterSpectro.drawEllipse(QPointF(s->x(), s->y()), m_sensor_radius, m_sensor_radius);

			// plot label
			if (m_showLabels)
				painterSpectro.drawText(QPointF(s->x() - m_sensor_radius, s->y() - m_sensor_radius), s->label());
		}
	}

	painterSpectro.end();
	painter.drawImage(m_pad, m_pad, imageSpectro);
	painter.end();
	ui->labelImage->setPixmap(QPixmap::fromImage(m_image));
	repaint();

	// auto paint to pixmap but only the mapping and the label
	int flags = m_flags;
	setFlags(AwTopoWidget::ShowLabel);
	repaint();
	m_pixmap = QPixmap(size());
	render(&m_pixmap);
	setFlags(flags);
}

void AwTopoWidget::setComputationFlags(const QString& flags)
{
	ui->labelFlags->setText(flags);
	ui->labelFlags->show();
}

void AwTopoWidget::resetComputationFlags()
{
	ui->labelFlags->setText("");
	ui->labelFlags->hide();
}

void AwTopoWidget::enableContours(bool flag)
{
	m_showContours = flag;
	redraw();
}

void AwTopoWidget::enableSensors(bool flag)
{
	m_showSensors = flag;
	redraw();
}

void  AwTopoWidget::enableLabels(bool flag)
{
	m_showLabels = flag;
	redraw();
}

void AwTopoWidget::setUnit(const QString& unit)
{
	m_unit = unit;
}

void AwTopoWidget::draw2(AwTopoWidget *widget, Aw2DGrid *grid)
{
    if (widget != this)
		return;

	if (m_grid) // already got a grid
		delete m_grid;
	m_grid = grid;

	QwtMatrixRasterData *data = new QwtMatrixRasterData;
	QVector<qreal> rasterMatrix(m_grid->ni);

	int ni = m_grid->ni;
	int res = m_grid->res;
	int startingYPos = ni - res; // starting with last line (Y axis inverted)
	for (int i = 0; i < res; i++)
		for (int j = 0; j < res; j++)  // Y axis must be inverted
			rasterMatrix[(startingYPos - (i *  res)) + j] =  m_grid->zi[i * m_res + j];

	data->setValueMatrix(rasterMatrix, m_grid->res);
	data->setResampleMode(QwtMatrixRasterData::BilinearInterpolation);
	m_spectro->setData(data);
	data->setInterval(Qt::ZAxis, QwtInterval(m_grid->min_zi, m_grid->max_zi));
	data->setInterval(Qt::XAxis, QwtInterval(0, m_grid->res));
	data->setInterval(Qt::YAxis, QwtInterval(0, m_grid->res));
	ui->labelImage->setData(data);
	redraw();
	
}

void AwTopoWidget::selectSensors(const QStringList& labels)
{
	m_selectedLabels = labels;
	redraw();
}

void AwTopoWidget::updateBadSensors(const QStringList& labels)
{
	m_badLabels = labels;
}

void AwTopoWidget::openUI()
{
	AwTopoWidgetSettings ui(this, NULL);
	ui.exec();
}

//void AwTopoWidget::draw()
//{
//	if (m_grid == NULL)
//		return;
//
//	// get the actual size of widget
//	QRect geo = geometry();
//	int min = qMin(geo.width(), geo.height());
//
//	if (min < 200)
//		min = 200;
//	if (min > 200)
//	{
//		m_image = QImage(min, min, QImage::Format_ARGB32);
//		m_labelImage->setPixmap(QPixmap::fromImage(m_image));
//	}
//
//	m_image.fill(QColor(255, 255, 255, 0));
//	QPainter painter(&m_image);
//	/* draw to qimage inside circle */
//	int w = m_image.width();
//	int h = m_image.height();
//	int pad = 10;
//	int iw=w-2*pad, ih=h-2*pad, iiw=w-4*pad, iih=h-4*pad;
//	float bw = iiw * 1.0f / m_res, bh = iih * 1.0f / m_res;
//	QPainterPath clipPath;
//	clipPath.addEllipse(pad, pad, iw, ih);
//	painter.setClipPath(clipPath);
//	painter.setPen(Qt::NoPen);
//	AwColorMapValues cm = AwColorMap::valuesForMap(m_cmap);
//	QRectF box(0.0f, 0.0f, bw, bh);
//
//	double *zi = m_grid->zi();
//	double min_z = m_grid->minz();
//	double max_z = m_grid->maxz();
//
//    double dz = ((-min_z)>max_z) ? -min_z : max_z;
//	double r, g, b, v;
//	double *rgb1, *rgb2;
//	int k;
//	for (int i = 0; i < m_res; i++)
//	{
//		for (int j = 0; j < m_res; j++)
//		{
//			v = zi[i * m_res + j] / dz + 0.5f;
//			if (isnan(v))
//				continue;
//			if (v < 0.0)
//				v = 0.0;
//			if (v > 1.0)
//				v = 0.9999;
//			k = qFloor(v*cm.count());
//			rgb1 = cm.colorAt(k>=cm.count()?cm.count():k);
//			rgb2 = k==cm.count() ? rgb1 : cm.colorAt(k+1);
//			r = rgb1[0];// + (rgb2[0] - rgb1[0]) * (v - k);
//			g = rgb1[1];// + (rgb2[1] - rgb1[1]) * (v - k);
//			b = rgb1[2];// + (rgb2[2] - rgb1[2]) * (v - k);
//			int ri=255, gi=255, bi=255;
//			if (v > 0.5)
//				gi = bi = 255 - (v-0.5)*2*255;
//			else
//				ri = gi = 255 - (0.5-v)*2*255;
//			//qDebug() << zi[i*res+j] << w/res*j << h/res*i << w/res << h/res << r << g << b;
//			if (r>1) r=1;
//			box.moveTo(bw*j+pad*2, bh*i+pad*2);
//		//	painter.fillRect(box, QBrush(QColor(ri, gi, bi)));
//			painter.fillRect(box, QBrush(AwColorMap::rgbToColor(r,g ,b)));
//		}
//	}
//
//	painter.setRenderHint(QPainter::HighQualityAntialiasing);
//	painter.setPen(QColor(0, 0, 0));
//
//	double *xi = m_grid->xi();
//	double *yi = m_grid->yi();
//	double *x = m_grid->x();
//	double *y = m_grid->y();
//	double min_x = m_grid->minx();
//	double max_x = m_grid->maxx();
//	double min_y = m_grid->miny();
//	double max_y = m_grid->maxy();
//	int np = m_grid->np();
//	
//	for (int i = 0; i < np; i++) {
//		int xi = (x[i] - min_x)/(max_x - min_x)*iiw + 2*pad;
//		int yi = iih - (y[i] - min_y)/(max_y - min_y)*iih + 2*pad;
//		painter.drawEllipse(xi, yi, 3, 3);
//	}
//
//	painter.setClipPath(clipPath, Qt::NoClip);
//	
//	QPen outlinePen(QColor(0, 0, 0));
//	outlinePen.setWidth(2);
//	painter.setPen(outlinePen);
//	painter.drawEllipse(pad, pad, iw, ih);
//	
//	painter.setBrush(Qt::NoBrush);
//	QPolygonF nose;
//	nose << QPointF(w/2, 1) << QPointF(w/2+pad, pad) << QPointF(w/2-pad, pad) ;
//	painter.drawConvexPolygon(nose);
//
//	QPolygonF ear;
//	ear << QPointF(0, h/2+pad) << QPointF(0, h/2-pad) << QPointF(pad, h/2-pad) << QPointF(pad, h/2+pad);
//	painter.drawConvexPolygon(ear);
//	painter.drawConvexPolygon(ear.translated(w-pad, 0.0));
//
//	m_labelImage->setPixmap(QPixmap::fromImage(m_image));
//	repaint();
//}