#include <process/AwProcessGUIWidget.h>
#include <QCloseEvent>


void AwProcessGUIWidget::closeEvent(QCloseEvent *event)
{
	QWidget::closeEvent(event);
	emit closed();
}