#pragma once

#include <QWidget>
#include "ui_SSTWidget.h"
#include <AwChannel.h>
#include <AwDataClient.h>
#include "SEEGPlot.h"
#include <qwt_scale_draw.h>

class LabelDraw : public QwtScaleDraw
{
public:
	LabelDraw(const QStringList& labels) : QwtScaleDraw() { m_labels = labels; }
	QwtText label(double value) const;
private:
	QStringList m_labels;
};

class TimeDraw : public QwtScaleDraw
{
public:
	TimeDraw(int sampleSecond, float position) : QwtScaleDraw() { m_sampleSecond = sampleSecond; m_currentPosition = position; }
	QwtText label(double value) const;
private:
	int m_sampleSecond;
	float m_currentPosition;
};

class SSTWidget : public QWidget
{
	Q_OBJECT

public:
	SSTWidget(AwChannelList& channels, float duration, QWidget *parent = Q_NULLPTR);
	~SSTWidget();
	inline AwDataClient *client() { return &m_client;  }
public slots:
	void updateData();
protected slots:
	void setNewCMap(int);
	void goBack();
	void goForward();
private:
	Ui::SSTWidgetUi m_ui;
	float m_dataDuration;
	SEEGPlot *m_plot;
	AwDataClient m_client;
	AwChannelList m_channels;
	float m_position, m_step, m_totalDuration;

	void buildXScale();
	void buildYScale();
};
