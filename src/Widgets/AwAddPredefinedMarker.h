#ifndef AWADDPREDEFINEDMARKER_H
#define AWADDPREDEFINEDMARKER_H
#include <AwGlobal.h>
#include <QDialog>
#include "ui_AwAddPredefinedMarker.h"
class AwMarker;

class AW_WIDGETS_EXPORT AwAddPredefinedMarker : public QDialog
{
	Q_OBJECT

public:
	AwAddPredefinedMarker(QWidget *parent = 0);
	~AwAddPredefinedMarker();

	AwMarker *marker() { return m_marker; }
protected:
	void changeEvent(QEvent *);
public slots:
	void accept();
private:
	Ui::AwAddPredefinedMarkerUi m_ui;
	AwMarker *m_marker;
};

#endif // AWADDPREDEFINEDMARKER_H
