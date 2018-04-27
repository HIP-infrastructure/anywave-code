#include "AwBIDSGUI.h"

AwBIDSGUI::AwBIDSGUI(const QString& rootDir, QWidget *parent)
	: QWidget(parent)
{
	m_ui.setupUi(this);
	m_ui.leDIR->setText(rootDir);
}

AwBIDSGUI::~AwBIDSGUI()
{
}



void AwBIDSGUI::setSubjects(const AwBIDSSubjectList& subjects)
{
	m_ui.comboSubject->clear();
	for (auto s : subjects) {
		m_ui.comboSubject->addItem(s->ID());
	}
	m_ui.comboSubject->setCurrentIndex(0);
}