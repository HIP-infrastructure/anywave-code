#pragma once
#include <AwGlobal.h>
#include <QWidget>
namespace Ui { class AwDownSamplingWidget; };

class AW_WIDGETS_EXPORT AwDownSamplingWidget : public QWidget
{
	Q_OBJECT

public:
	AwDownSamplingWidget(QWidget *parent = Q_NULLPTR);
	~AwDownSamplingWidget();

	void setMaxSR(float m_maxSR);
	inline float newSR() { return m_newSR; }
protected slots:
	void changeSR(int index);
private:
	Ui::AwDownSamplingWidget *m_ui;
	float m_maxSR, m_newSR;
	QVector<float> m_possibleSRs;
};
