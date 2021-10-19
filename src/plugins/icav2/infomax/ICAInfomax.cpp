#include "ICAInfomax.h"
#include <QHBoxLayout>

ICAInfomax::ICAInfomax(ICA* process) : ICAAlgorithm(process)
{
	m_name = "infomax";
	m_flags = ICAAlgorithm::CanDoPCA | ICAAlgorithm::HaveGUI;
	m_widget = nullptr;
}

QWidget* ICAInfomax::getGUI()
{
	if (m_widget == nullptr) {
		m_widget = new QWidget;
		QHBoxLayout* layout = new QHBoxLayout;
		m_checkExtended = new QCheckBox("Extended Mode");
		connect(m_checkExtended, &QCheckBox::clicked, this, &ICAInfomax::setExtendedMode);
		layout->addWidget(m_checkExtended);
		m_widget->setLayout(layout);
		m_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	}
	return m_widget;
}

QVariantMap ICAInfomax::getExtraSettings()
{
	return m_settings;
}

void ICAInfomax::setExtendedMode(bool flag)
{
	m_settings["infomax_extended"] = flag;
}
