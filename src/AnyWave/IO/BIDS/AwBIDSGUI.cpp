#include "AwBIDSGUI.h"
#include "AwBIDSManager.h"

AwBIDSGUI::AwBIDSGUI(AwBIDSManager *bids, const QString& rootDir, QWidget *parent)
	: QWidget(parent)
{
	m_ui.setupUi(this);
	m_ui.leDIR->setText(rootDir);
	m_bids = bids;
}

AwBIDSGUI::~AwBIDSGUI()
{
}


void AwBIDSGUI::refresh()
{
	// check for available source directories and subjects
	AwBIDSSubjectList raw = m_bids->getSubjectsFromSourceDir(AwBIDSManager::raw);
	AwBIDSSubjectList sourcedata = m_bids->getSubjectsFromSourceDir(AwBIDSManager::source);
	bool isRaw = !raw.isEmpty();
	bool isSource = !sourcedata.isEmpty();

	if (isRaw) 
		m_ui.comboSource->addItem("raw data", QVariant(AwBIDSManager::raw));
	if (isSource)
		m_ui.comboSource->addItem("source data", QVariant(AwBIDSManager::source));

	m_ui.comboSource->setCurrentIndex(0);
}