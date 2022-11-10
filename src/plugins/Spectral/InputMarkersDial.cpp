#include "InputMarkersDial.h"

InputMarkersDial::InputMarkersDial(const AwSharedMarkerList& markers, QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	m_ui.widget->setMarkers(markers);
}

InputMarkersDial::~InputMarkersDial()
{
}

void InputMarkersDial::accept()
{
	m_skippedLabels = m_ui.widget->skippedMarkers();
	m_usedLabels = m_ui.widget->usedMarkers();
	QDialog::accept();
}
