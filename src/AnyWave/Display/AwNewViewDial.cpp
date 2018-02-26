#include "AwNewViewDial.h"
#include <AwChannel.h>

AwNewViewDial::AwNewViewDial(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);

	m_checkBoxes << m_ui.checkEEG << m_ui.checkSEEG << m_ui.checkEMG << m_ui.checkECG;
	m_checkBoxes << m_ui.checkMEG << m_ui.checkReference << m_ui.checkTrigger << m_ui.checkOther;
	m_checkBoxes << m_ui.checkICA << m_ui.checkSource << m_ui.checkGRAD;
	m_types << AwChannel::EEG << AwChannel::SEEG << AwChannel::EMG << AwChannel::ECG;
	m_types << AwChannel::MEG << AwChannel::Reference << AwChannel::Trigger << AwChannel::Other;
	m_types << AwChannel::ICA << AwChannel::Source << AwChannel::GRAD;

}

AwNewViewDial::~AwNewViewDial()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// EVENTS
void AwNewViewDial::changeEvent(QEvent *e)
{
	if (e) {
		if (e->type() == QEvent::LanguageChange) 
			m_ui.retranslateUi(this);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS


void AwNewViewDial::accept()
{
	for (int i = 0; i < m_checkBoxes.size(); i++)
	{
		if (m_checkBoxes.at(i)->isChecked())
			m_filters << m_types.at(i);
	}
	QDialog::accept();
}

void AwNewViewDial::all()
{
	foreach (QCheckBox *cb, m_checkBoxes)
		cb->setChecked(true);
}

void AwNewViewDial::none()
{
	foreach (QCheckBox *cb, m_checkBoxes)
		cb->setChecked(false);
}