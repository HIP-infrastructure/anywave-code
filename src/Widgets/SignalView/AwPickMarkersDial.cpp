#include "AwPickMarkersDial.h"
#include <AwMarkingSettings.h>

AwPickMarkersDial::AwPickMarkersDial(AwMarkingSettings*settings, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_settings = settings;
	int index = 0;
	for (auto m : settings->predefinedMarkers) {
		Qt::CheckState state = Qt::Unchecked;
		auto item = new QListWidgetItem(m->label());
		ui.listWidget->addItem(item);
		if (settings->selectedPredefinedMarkers.contains(index++))
			state = Qt::Checked;
		item->setCheckState(state);
		item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
	}
}

AwPickMarkersDial::~AwPickMarkersDial()
{
}


void AwPickMarkersDial::accept()
{
	m_settings->selectedPredefinedMarkers.clear();
	for (int i = 0; i < ui.listWidget->count(); i++) {
		auto item = ui.listWidget->item(i);
		if (item->checkState() == Qt::Checked)
			m_settings->selectedPredefinedMarkers.append(i);
	}

	QDialog::accept();
}