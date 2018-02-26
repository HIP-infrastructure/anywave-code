#ifndef AWTOPOWIDGETSETTINGS_H
#define AWTOPOWIDGETSETTINGS_H
#include <AwGlobal.h>
#include <QDialog>
namespace Ui {class AwTopoWidgetSettings;};
#include <widget/AwTopoWidget.h>

class AW_WIDGETS_EXPORT AwTopoWidgetSettings : public QDialog
{
	Q_OBJECT

public:
	AwTopoWidgetSettings(AwTopoWidget *widget, QWidget *parent);
	~AwTopoWidgetSettings();
protected:
	void changeEvent(QEvent*);
public slots:
	void accept();
protected:
	Ui::AwTopoWidgetSettings *ui;
	AwTopoWidget *m_widget;
	int m_cmap;
};

#endif // AWTOPOWIDGETSETTINGS_H
