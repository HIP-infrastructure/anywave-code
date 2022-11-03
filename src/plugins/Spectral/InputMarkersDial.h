#pragma once

#include <QDialog>
#include "ui_InputMarkersDial.h"

class InputMarkersDial : public QDialog
{
	Q_OBJECT

public:
	InputMarkersDial(const AwSharedMarkerList& markers, QWidget *parent = nullptr);
	~InputMarkersDial();

	QStringList m_skippedLabels, m_usedLabels;
public slots:
	void accept() override;
private:
	Ui::InputMarkersDialUi m_ui;
};
