#include "widget/AwAmplitudeButton.h"
#include <QMenu>
#include <QSettings>
#include <QTextCodec>
#include <AwChannel.h>
#include <AwAmplitudeManager.h>

void AwAmplitudeButton::createActionGroup(QMenu *menu, int type) 
{
	m_groups[type] = new QActionGroup(this);
	QAction *act;
	foreach (float v, m_am->getScale(type)) {
		act = menu->addAction(QString::number(v) + " " + m_am->unitOfChannel(type));
		m_groups[type]->addAction(act);
		act->setCheckable(true);
		act->setChecked(v == m_am->amplitude(type));
		act->setData(QVariant(v));
		connect(act, SIGNAL(toggled(bool)), this, SLOT(changeAmplitude(bool)));
	}
}

AwAmplitudeButton::AwAmplitudeButton(QWidget *parent)
	: QPushButton(parent)
{
	m_gainLevelForChannel = -1;	// default is to up/down levels for ALL channels
	for (int i = 0; i < AW_CHANNEL_TYPES; i++)
		m_groups[i] = NULL;

	m_menu = new QMenu(this);
	m_am = AwAmplitudeManager::instance();
	
	connect(m_am, SIGNAL(amplitudesChanged()), this, SLOT(setAmplitudes()));

	for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
		auto menu = new QMenu(AwChannel::types.value(i), m_menu);
		createActionGroup(menu, i);
		m_menu->addMenu(menu);
	}
	this->setMenu(m_menu);
}

AwAmplitudeButton::~AwAmplitudeButton()
{

}

void AwAmplitudeButton::switchGainLevelToChannel(int type) 
{
	m_gainLevelForChannel = type;

}

void AwAmplitudeButton::upActions(QActionGroup *group)
{
	if (group == NULL)
		return;
	int index = 0;
	// found the currently checked action
	foreach(QAction *a, group->actions()) {
		if (a->isChecked())
			break;
		index++;
	}
	if (index > 0)
		group->actions().at(index - 1)->setChecked(true);
}

void AwAmplitudeButton::downActions(QActionGroup *group)
{
	if (group == NULL)
		return;
	int index = 0;
	// found the currently checked action
	foreach(QAction *a, group->actions())	{
		if (a->isChecked())
			break;
		index++;
	}
	if (index < group->actions().size() - 1)
		group->actions().at(index + 1)->setChecked(true);

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS

void AwAmplitudeButton::up()
{
	if (m_gainLevelForChannel != -1)
		upActions(m_groups[m_gainLevelForChannel]);
	else {
		for (int i = 0; i < AW_CHANNEL_TYPES; i++)
			upActions(m_groups[i]);
	}
}

void AwAmplitudeButton::down()
{
	if (m_gainLevelForChannel != -1)
		downActions(m_groups[m_gainLevelForChannel]);
	else {
		for (int i = 0; i < AW_CHANNEL_TYPES; i++) 
			downActions(m_groups[i]);
	}
}

void AwAmplitudeButton::setAmplitudes()
{
	// all amplitudes have changed
	// Get the amplitudes scales from Amplitude Manager
	for (int i = 0; i < AW_CHANNEL_TYPES; i++) 
		setAmplitude(i, m_am->amplitude(i));
	emit amplitudesChanged();
}


void AwAmplitudeButton::setAmplitude(int type, float value)
{
    if (m_groups[type] == NULL)
		return;

	foreach (QAction *a, m_groups[type]->actions()) {
		if ((float)a->data().toDouble() == value) {
			a->setChecked(true);
			break;
		}
	}
}

void AwAmplitudeButton::changeAmplitude(bool checked)
{
	if (!checked)
		return;

	QAction *act = (QAction *)sender();
	if (act == NULL)
		return;

	float v = (float)act->data().toDouble();
	for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
		// skip empty groups
		if (m_groups[i] == NULL)
			continue;
		if (m_groups[i]->actions().contains(act)) {
			m_am->setAmplitude(i, v);
			emit amplitudeChanged(i, v);
			break;
		}
	}
}



