#pragma once

#include <QDialog>
#include "ui_AwPickMarkersDial.h"
class AwMarkingSettings;

class AwPickMarkersDial : public QDialog
{
	Q_OBJECT

public:
	AwPickMarkersDial(AwMarkingSettings* settings, QWidget *parent = Q_NULLPTR);
	~AwPickMarkersDial();

	inline void setSettings(AwMarkingSettings* settings) { m_settings = settings; }
public slots:
	void accept() override;
protected:
	AwMarkingSettings* m_settings;
private:
	Ui::AwPickMarkersDialUi ui;
};
