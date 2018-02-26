#pragma once

#include <QWidget>
#include <QtCharts>
#include <epoch/AwEpochAverageChannel.h>
class AwEpochTree;
using namespace QtCharts;
class AwStatChartWidget : public QChartView
{
	Q_OBJECT

public:
	AwStatChartWidget(QWidget *parent = Q_NULLPTR);
	enum Options { ShowErrorType = 1 };
	~AwStatChartWidget();
	void setCondition(AwEpochTree *condition);

	/** set new epoch channels to the chart **/
	void setChannels(const QList<AwEpochAverageChannel *>& channels);
	void setDisplayOptions(int flags);
private:
	void addChannel(AwEpochAverageChannel *channel);

	QChart *m_chart;
	QValueAxis *m_axisY, *m_axisX;
	QCategoryAxis *m_axisX2;

	float m_YMin, m_YMax;	// range for the y axis
	AwEpochTree *m_condition;
	QMap<QString, AwEpochAverageChannel *> m_nameToChannel;
	AwEpochChannelList m_channels;
	int m_flags;	// display option flags
};
