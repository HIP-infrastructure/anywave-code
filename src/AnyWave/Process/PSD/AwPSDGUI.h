#pragma once

#include <QWidget>
#include "ui_AwPSDGUI.h"
#include <process/AwProcessGUIWidget.h>

class AwPSDGUI : public AwProcessGUIWidget
{
	Q_OBJECT

public:
	AwPSDGUI(AwGUIProcess *process, QWidget *parent = Q_NULLPTR);
	~AwPSDGUI();

private:
	Ui::AwPSDGUIUi m_ui;
};
