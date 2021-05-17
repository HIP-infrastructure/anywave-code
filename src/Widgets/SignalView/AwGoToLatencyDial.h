#ifndef AW_GOTOLATENCYDIAL_H
#define AW_GOTOLATENCYDIAL_H
#include <AwGlobal.h>
#include <QDialog>
#include "ui_AwGoToLatencyDial.h"

using namespace Ui;

class AW_WIDGETS_EXPORT AwGoToLatencyDial : public QDialog, public GoToLatencyDialClass
{
	Q_OBJECT

public:
	AwGoToLatencyDial(QWidget *parent = 0);

	void setFileDuration(float d) { m_duration = d; }
	inline float position() { return m_position; }

public slots:
	void accept();
private:
	float m_duration;
	float m_position;
};

#endif // GOTOLATENCYDIAL_H
