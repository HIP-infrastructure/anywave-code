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
#include <aw_armadillo.h>

class TFWidget : public AwProcessGUIWidget
{
	Q_OBJECT

public:
	TFWidget(TFSettings *settings, AwGUIProcess *process, QWidget *parent = Q_NULLPTR);
	~TFWidget();

	void setMarkers(const AwSharedMarkerList& markers);
	AwBaseSignalView *signalView() { return m_signalView; }
//	void setChannels(const AwChannelList& channels);
	void setChannels(const QList<QSharedPointer<AwChannel>>& channels);
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
	void changeModulus(bool flag);

	void compute2(float, float);
	void recompute();
	void highlightSampleInterval(float start, float duration);
	void showFreqScale(bool);
	void showColorMapScale(bool);
	void toggleBaselineCorrection(bool flag);
	void lockZRange();
	void unlockZRange();
private:
	arma::mat computeFunction(AwChannel*);
	void applyNormalisation();
	void setZScale();

	Ui::TFWidgetUi m_ui;
	AwBaseSignalView *m_signalView;
//	QwtScaleWidget *m_colorMapWidget; // global color map
	double m_min, m_max, m_zmin, m_zmax;
	TFSettings *m_settings;
	DisplaySettings m_displaySettings;
	QList<TFPlot *> m_plots;
	bool m_baselineComputed, m_zRangeLocked, m_computeBaseline;
	//AwChannelList m_channels; 
	QList<QSharedPointer<AwChannel>> m_channels;
	AwSharedMarkerList m_baselineMarkers;
	QList<arma::mat> m_baselines, m_rawTF, m_normalizedTF;
	QList<QPair<mat, mat>> m_results;
	float m_pos;
};
