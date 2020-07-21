#include "AwPSDGUI.h"

AwPSDGUI::AwPSDGUI(AwGUIProcess *process, QWidget *parent)
	: AwProcessGUIWidget(process, parent)
{
	m_ui.setupUi(this);
}

AwPSDGUI::~AwPSDGUI()
{
}
