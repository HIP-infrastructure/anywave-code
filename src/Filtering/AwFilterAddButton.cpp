#include "AwFilterAddButton.h"
#include <AwChannel.h>

AwFilterAddButton::AwFilterAddButton(QWidget *parent) : QPushButton(parent)
{
	m_menu = new QMenu(this);
	QList<int> channels = { AwChannel::EEG, AwChannel::SEEG, AwChannel::MEG, AwChannel::GRAD, AwChannel::ICA, AwChannel::Source, AwChannel::EMG, AwChannel::ECG };
	for (auto c : channels) {
		auto action = m_menu->addAction(AwChannel::typeToString(c));
		connect(action, &QAction::triggered, this, &AwFilterAddButton::addNewFilter);
		action->setData(c);
	}
	this->setMenu(m_menu);
}

void AwFilterAddButton::addNewFilter()
{
	QAction *action = (QAction *)sender();
	if (action == Q_NULLPTR)
		return;
	emit addFilterClicked(action->data().toInt());
}