#pragma once

#include <qwt_plot.h>
#include <qwt_plot_spectrogram.h>
#include <qwt_matrix_raster_data.h>
#include "tf_settings.h"
#include "display_settings.h"
#include "TFPicker.h"
#include <AwChannel.h>
#include <aw_armadillo.h>

class TFPlot : public QwtPlot // public QObject
{
	Q_OBJECT
public:
	TFPlot(TFSettings *settings, DisplaySettings *ds, AwChannel *channel, QWidget *parent = Q_NULLPTR);
	~TFPlot();

	inline QWidget *leftWidget() { return (QWidget *)m_freqScaleWidget; }
	inline QWidget *rightWidget() { return (QWidget *)m_colorMapWidget; }
public slots:
	void setNewData(float position, const QVector<double>& data, int row, int col);
	void updateDisplaySettings();
	void showFreqScale(bool flag);
	void showColorMapScale(bool flag);
	void updateFreqScale(float min, float max, float step);
signals:
	void selectionDone(float pos, float duration);
	void selectionMade(AwChannel *channel, float pos, int start, int dur, float fmin, int lrow, int hrow);
private slots:
	void select(int start, int duration);
	void select(int start, int duration, float fmin, int lrow, int hrow);
protected:
	void updateZScale();

	AwChannel * m_channel;
	float m_positionInData;
	QwtPlotSpectrogram *m_spectro;
	QwtMatrixRasterData *m_matrix;
	QwtScaleWidget *m_freqScaleWidget;
	QwtScaleWidget *m_colorMapWidget;
	QwtInterval ZInterval;
	QVector<double> m_valueMatrix;
	TFSettings *m_settings;
	DisplaySettings *m_displaySettings;
	DisplaySettings m_displayCopy;
	TFPicker *m_picker;
	mat m_resultMatrix;
	double m_min, m_max;
	int m_raw_rows, m_raw_cols;
	double *m_raw_data;

	void applyNormalization();
	void applyColorMap();
	void refresh();
};