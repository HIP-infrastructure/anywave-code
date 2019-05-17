#include "AwFilterAddButton.h"
#include <AwChannel.h>

AwFilterAddButton::AwFilterAddButton(QWidget *parent) : QPushButton(parent)
{
	m_menu = new QMenu(this);
	auto  channels = AwChannel::types();
	for (auto c : channels) {
		auto action = m_menu->addAction(c);
		connect(action, &QAction::triggered, this, &AwFilterAddButton::addNewFilter);
		action->setData(AwChannel::stringToType(c));
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