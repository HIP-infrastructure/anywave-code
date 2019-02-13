/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////

#include "AwProcessFromMarkersDial.h"
#include "AwProcessMarkerModel.h"
#include "Process/AwProcessManager.h"
#include <AwProcessInterface.h>
#include <widget/AwMessageBox.h>

AwProcessFromMarkersDial::AwProcessFromMarkersDial(const AwMarkerList &markers, const AwMarkerList &selectedMarkers, QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	QButtonGroup *groupRadio = new QButtonGroup(this);
	groupRadio->addButton(m_ui.radioButtonType);
	groupRadio->addButton(m_ui.radioButtonName);
	groupRadio->addButton(m_ui.radioButtonCode);

	m_ui.radioButtonType->setChecked(true);
	AwProcessMarkerModel *model = new AwProcessMarkerModel(selectedMarkers);
	m_ui.tvDroppedMarkers->setModel(model);
	m_markers = markers;
	m_selectedMarkers = selectedMarkers;

	// hide or show some widgets depending on selected markers list
	bool noSelectedMarkers = m_selectedMarkers.isEmpty();

	m_ui.labelSelectMarker->setVisible(noSelectedMarkers);
	m_ui.radioButtonType->setVisible(noSelectedMarkers);
	m_ui.radioButtonName->setVisible(noSelectedMarkers);
	m_ui.radioButtonCode->setVisible(noSelectedMarkers);
	m_ui.comboType->setVisible(noSelectedMarkers);
	m_ui.comboName->setVisible(noSelectedMarkers);
	m_ui.comboCode->setVisible(noSelectedMarkers);

	m_ui.labelSelectedMarkers->setVisible(!noSelectedMarkers);
	m_ui.tvDroppedMarkers->setVisible(!noSelectedMarkers);

	QHeaderView *header = m_ui.tvDroppedMarkers->horizontalHeader();
#if QT_VERSION < 5
	header->setResizeMode(QHeaderView::Interactive);
#else
	header->setSectionResizeMode(QHeaderView::Interactive);
#endif
	header->setStretchLastSection(true);
}

AwProcessFromMarkersDial::~AwProcessFromMarkersDial()
{

}


void AwProcessFromMarkersDial::changeEvent(QEvent *e)
{
	if (e)
		if (e->type() == QEvent::LanguageChange)
			m_ui.retranslateUi(this);
}

void AwProcessFromMarkersDial::accept()
{
	bool error = false;
	if (m_selectedMarkers.isEmpty()) {
		// Fill list depending on parameters choosen by the user
		if (m_ui.radioButtonType->isChecked()) {
			QString dum = m_ui.comboType->itemText(m_ui.comboType->currentIndex());
			int t = AwMarker::stringToType(dum);

			foreach (AwMarker *m, m_markers) {
				if (m->type() == t) {
					AwMarker *marker = new AwMarker(m);
					m_processMarkers << marker;
				}
			}
		}
		else if (m_ui.radioButtonName->isChecked()) 	{
			foreach (AwMarker *m, m_markers) {
				if (m->label() == m_ui.comboName->itemText(m_ui.comboName->currentIndex())) 	{
					AwMarker *marker = new AwMarker(m);
					m_processMarkers << marker;
				}
			}
		}
		else if (m_ui.radioButtonCode->isChecked()) {
			foreach (AwMarker *m, m_markers) {
				if (m->value() == m_ui.comboCode->itemText(m_ui.comboCode->currentIndex()).toInt()) 	{
					AwMarker *marker = new AwMarker(m);
					m_processMarkers << marker;
				}
			}
		}
	}
	else {	// some markers have been choosen before
		foreach (AwMarker *m, m_selectedMarkers) {
			AwMarker *marker = new AwMarker(m);
			if (marker->type() == AwMarker::Single)	{
				// apply post and pre latencies parameters
				float start = marker->start() - m_ui.sbPreMarkerTime->value();
				marker->setStart(start);
				if (marker->start() < 0)
					marker->setStart(0);
				// marker becomes a selection
				marker->setDuration(m_ui.sbPreMarkerTime->value() + m_ui.sbPostMarkerTime->value());
				if (marker->duration() <= 0) {
					error = true;
					break;
				}
			}
			m_processMarkers << marker;
		}
	}
	if (error)	{
		QMessageBox::critical(this, QString(tr("Incorrect parameters")), QString(tr("Post and Pre latencies must not be null if used with Single markers")),
			QMessageBox::Cancel);
		return;
	}
	// launch process
	// Instantiate process
	AwProcessManager *process_manager = AwProcessManager::instance();
	//AwProcessPlugin *plugin = pm->getProcessPluginByName(comboProcess->itemText(comboProcess->currentIndex()));
	// the plugin may be of type DisplayBackground, so force it to be Background only to launch the process.
	AwBaseProcess *process = process_manager->newProcessFromPluginName(m_ui.comboProcess->itemText(m_ui.comboProcess->currentIndex()));
	// set markers to compute data on
	process->pdi.input.setNewMarkers(m_processMarkers);
	// start process
	process_manager->runProcess(process);
	QDialog::accept();
}


int AwProcessFromMarkersDial::exec()
{
	m_ui.comboType->clear();
	m_ui.comboName->clear();
	m_ui.comboCode->clear();
	m_ui.comboProcess->clear();
	m_names.clear();
	m_codes.clear();
	m_types.clear();
	m_plugins.clear();

	foreach (AwMarker *m, m_markers) {
		if (!m_names.contains(m->label()))
			m_names << m->label();
		QString sCode = QString("%1").arg(m->value());
		if (!m_codes.contains(sCode))
			m_codes  << sCode;
		QString type = AwMarker::typeToString(m->type());
		if (!m_types.contains(type))
			m_types << type;
	}
	m_ui.comboType->addItems(m_types);
	m_ui.comboName->addItems(m_names);
	m_ui.comboCode->addItems(m_codes);

	// Get plugins that accept marked signal as input
	QList<AwProcessPlugin *> plugins = AwProcessManager::instance()->processes();
	for (auto p : plugins)
		if (p->type != AwProcessPlugin::Internal && p->flags() & Aw::ProcessFlags::PluginAcceptsTimeSelections)
			m_plugins << p->name;

	m_ui.comboProcess->addItems(m_plugins);

	// no plugin that support time selections
	if (m_plugins.isEmpty()) {
		AwMessageBox::critical(this, QString(tr("Missing plugins")), QString(tr("No process plugins suitable for this operation")), QMessageBox::Cancel);
		return QDialog::Rejected; 
	}
	// check if there is a least one marker
	if (m_names.isEmpty()) {
		AwMessageBox::critical(this, QString(tr("No markers")), QString(tr("Insert markers in signal before launching a process")), QMessageBox::Cancel);
		return QDialog::Rejected;
	}
	return QDialog::exec();
}