#ifndef AWAMPLITUDEBUTTON_H
#define AWAMPLITUDEBUTTON_H
#include <AwGlobal.h>
#include <QPushButton>
#include <QActionGroup>
#include <AwChannel.h>
class QMenu;
class AwAmplitudeManager;

class AW_WIDGETS_EXPORT AwAmplitudeButton : public QPushButton
{
	Q_OBJECT

public:
	AwAmplitudeButton(QWidget *parent);
	~AwAmplitudeButton();
signals:
	void amplitudeChanged(int, float);
	void amplitudesChanged();
private slots:
	void changeAmplitude(bool checked);
public slots:
	void setAmplitudes();
	void setAmplitude(int, float);
	void up();			// raise signal amplitudes (reduces the amplitude scale).
	void down();		// lower signal amplitudes (raises the amplitude scale).
	void switchGainLevelToChannel(int type);
private:
	void upActions(QActionGroup *group);
	void downActions(QActionGroup *group);
	void createActionGroup(QMenu *menu, int type);

	AwAmplitudeManager *m_am;
	QMenu *m_menu;
	QActionGroup *m_groups[AW_CHANNEL_TYPES];
	int m_gainLevelForChannel;	
};

#endif // AWAMPLITUDEBUTTON_H
