#ifndef AWAMPLITUDEWIDGET_H
#define AWAMPLITUDEWIDGET_H

#include <QWidget>
#include <QPushButton>
namespace Ui {class AwAmplitudeWidgetUi;};
#include <AwChannel.h>
class AwAmplitudeButton;
//class AwAmplitudeManager;
#include <AwGainLevels.h>

class AW_WIDGETS_EXPORT AwAmplitudeWidget : public QWidget
{
	Q_OBJECT

public:
	AwAmplitudeWidget(QWidget *parent = 0);
	//AwAmplitudeWidget(AwGainLevels *gl, QWidget* parent = 0);
	~AwAmplitudeWidget();
	void setGainLevels(AwGainLevels* gl);
public slots:
	void changeLevelForChannel(int type);
	void up();
	void down();
	/** change the combobox current type of channel: type must be an AwChannel type. **/
	void changeCurrentChannelTypeAndValue(int type, float amplitude);
signals:
	void amplitudesChanged();
	void amplitudeChanged(int, float);
private slots:
	void changeAmplitude(double value);
private:
	QPushButton *m_buttonUp;
	int m_channelType;
	int m_indexes[AW_CHANNEL_TYPES];	// keep current index of levels scales
	Ui::AwAmplitudeWidgetUi *m_ui;
//	AwAmplitudeManager *m_am;
	AwGainLevels *m_gainLevels;
};

#endif // AWAMPLITUDEWIDGET_H
