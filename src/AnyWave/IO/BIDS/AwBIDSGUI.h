#pragma once

#include <QWidget>
#include "ui_AwBIDSGUI.h"

class AwBIDSGUI : public QWidget
{
	Q_OBJECT

public:
	AwBIDSGUI(QWidget *parent = Q_NULLPTR);
	~AwBIDSGUI();

private:
	Ui::AwBIDSGUIUi m_ui;
};
