#ifndef AW_PROCESS_GUI_WIDGET_H
#define AW_PROCESS_GUI_WIDGET_H
#include <AwProcess.h>
#include <AwGlobal.h>
#include <QWidget>
class AwGUIProcess;

class AW_PROCESS_EXPORT AwProcessGUIWidget : public QWidget
{
	Q_OBJECT
public:
	AwProcessGUIWidget(AwGUIProcess *process, QWidget *parent) : QWidget(parent) { m_process = process; }
signals:
	void closed();
protected:
	/** reimplementing close event to send the closed signal to AnyWave **/
	void closeEvent(QCloseEvent *event);
	AwGUIProcess *m_process;
};

#endif