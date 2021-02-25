#pragma once

#include <QWidget>
#include "ui_TFWidget.h"
#include <process/AwProcessGUIWidget.h>
#include <AwMarker.h>
#include <widget/SignalView/AwBaseSignalView.h>
#include "tf_settings.h"
#include "TFPlot.h"
#include "display_settings.h"
#include <aw_armadillo.h>
#include "tfparam.h"

class TFWidget : public AwProcessGUIWidget
{
	Q_OBJECT

public:
	TFWidget(TFSettings *settings, AwGUIProcess *process, QWidget *parent = Q_NULLPTR);
	~TFWidget();

	void setMarkers(const AwMarkerList& markers);
	AwBaseSignalView *signalView() { return m_signalView; }
	void setChannels(const AwChannelList& channels);
	void updateBaselineOptions();
signals:
	void computeClicked();
	void freqScaleChanged(float min, float max, float step);
	void selectionMade(AwChannel *channel, float pos, int start, int duration, float fmin, int lrow, int hrow);
private slots:
	void switchLogScale(bool flag);
	void changeColorMap(int index);
	void changeNormalization(int index);
	void changeZScale(int index);
	void changeGain(int value);
	void compute();
	void recompute();
	void highlightSampleInterval(float start, float duration);
	void updatePlots();
	void showFreqScale(bool);
	void showColorMapScale(bool);
	void toggleBaselineCorrection(bool flag);
private:
	Ui::TFWidgetUi m_ui;
	AwBaseSignalView *m_signalView;
	QwtScaleWidget *m_colorMapWidget; // global color map
	double m_min, m_max;
	TFSettings *m_settings;
	DisplaySettings m_displaySettings;
	QList<TFParam *> m_tfComputations, m_baselineComputations;
	QList<TFPlot *> m_plots;
	bool m_baselineComputed;
	AwChannelList m_channels;
	AwMarkerList m_baselineMarkers;
};
