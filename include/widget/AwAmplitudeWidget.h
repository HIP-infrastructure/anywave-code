#ifndef AWAMPLITUDEWIDGET_H
#define AWAMPLITUDEWIDGET_H

#include <QWidget>
#include <QPushButton>
namespace Ui {class AwAmplitudeWidgetUi;};
#include <AwChannel.h>
class AwAmplitudeButton;
#include <AwGainLevels.h>
class AwGainLevelsWidget;

class AW_WIDGETS_EXPORT AwAmplitudeWidget : public QWidget
{
	Q_OBJECT

public:
	AwAmplitudeWidget(QWidget *parent = 0);
	~AwAmplitudeWidget();
	void setGainLevels(AwGainLevels* gl);
public slots:
	void up();
	void down();
	void editGains();
signals:
	void amplitudesChanged();
	void amplitudeChanged(int, float);
private:
	int m_indexes[AW_CHANNEL_TYPES];	// keep current index of levels scales
	Ui::AwAmplitudeWidgetUi *m_ui;
	AwGainLevels *m_gainLevels;
	AwGainLevelsWidget* m_widget;
};

#endif // AWAMPLITUDEWIDGET_H
