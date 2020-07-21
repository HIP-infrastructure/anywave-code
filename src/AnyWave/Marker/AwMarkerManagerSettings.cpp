/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
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
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "AwMarkerManagerSettings.h"
#include "Prefs/AwSettings.h"
#include "AwPickChannelsDial.h"
#include "AwMarkersExportWidget.h"
#include "AwMarkerRuleManageDial.h"
#include <QMenu>
#include <widget/AwMessageBox.h>
#include <AwFileIO.h>
#include "Process/AwMarkerExporter.h"
#include "Process/AwProcessManager.h"
#include <QFileDialog>
#include <widget/AwGetValueDialog.h>
#include <Plugin/AwPluginManager.h>
#include <AwFileInfo.h>

AwMarkerManagerSettings::AwMarkerManagerSettings(AwMarkerList& markers, QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);
	if (markers.isEmpty())
		this->setEnabled(false);

	m_markers = markers;
	m_displayedMarkers = markers;

	// init models and QTableView Markers
	m_model = new AwMarkerListModel(m_displayedMarkers, tvMarkers);
	QSortFilterProxyModel *sortModel = new QSortFilterProxyModel(this);
	sortModel->setSourceModel(m_model);
	tvMarkers->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	tvMarkers->setModel(sortModel);
	tvMarkers->setItemDelegate(new AwMarkerListDelegate(m_displayedMarkers, this));
	tvMarkers->setContextMenuPolicy(Qt::CustomContextMenu);

	// Context Menu for QTableView
	m_menu = new QMenu(tvMarkers);
	auto processes = AwPluginManager::getInstance()->processesWithFlags(Aw::ProcessFlags::PluginAcceptsTimeSelections);
	if (!processes.isEmpty()) {
		QMenu *menu = m_menu->addMenu("Launch process");
		for (auto p : processes) {
			auto action = menu->addAction(QString("%1 - %2").arg(p->name).arg(p->description));
			action->setData(p->name);
			connect(action, &QAction::triggered, this, &AwMarkerManagerSettings::launchProcess);
		}
	}

	m_selectAllLabel = new QAction(tr("Select all"), this);
	m_menu->addAction(m_selectAllLabel);
	connect(m_selectAllLabel, SIGNAL(triggered()), this, SLOT(selectAllLabels()));

	QAction *action;

	action = new QAction(tr("Rename all markers"), this);
	m_menu->addAction(action);
	connect(action, SIGNAL(triggered()), this, SLOT(renameAllMarkers()));
	action = new QAction(tr("Rename selected markers"), this);
	m_menu->addAction(action);
	connect(action, SIGNAL(triggered()), this, SLOT(renameSelectedMarkers()));
	action = new QAction(tr("Change values of all markers"), this);
	m_menu->addAction(action);
	connect(action, SIGNAL(triggered()), this, SLOT(changeValueAllMarkers()));
	action = new QAction(tr("Change values of selected markers"), this);
	m_menu->addAction(action);
	connect(action, SIGNAL(triggered()), this, SLOT(changeValueSelectedMarkers()));
	m_menu->addSeparator();
	action = new QAction(tr("Edit"), this);
	m_menu->addAction(action);
	connect(action, SIGNAL(triggered()), this, SLOT(editCurrentItem()));

	m_menu->addSeparator();
	
	action = new QAction(tr("Write to TRIGGER channel"), this);
	m_menu->addAction(action);
	connect(action, SIGNAL(triggered()), this, SLOT(writeTrigger()));

	action = new QAction(tr("Clear a TRIGGER channel"), this);
	m_menu->addAction(action);
	connect(action, SIGNAL(triggered()), this, SLOT(clearTrigger()));

	action = new QAction(tr("Center views on marker"), this);
	m_menu->addAction(action);
	connect(action, SIGNAL(triggered()), this, SLOT(goToMarkerPos()));

	m_menu->addSeparator();
	action = new QAction(tr("Save markers to .mrk file"), this);
	m_menu->addAction(action);
	connect(action, &QAction::triggered, this, &AwMarkerManagerSettings::saveSelectedMarkers);
	action = new QAction(tr("Save data to MATLAB file"), this);
	m_menu->addAction(action);
	connect(action, &QAction::triggered, this, &AwMarkerManagerSettings::saveSelectedMarkersToMATLAB);
	m_menu->addSeparator();
	action = new QAction(tr("Remove"), this);
	m_menu->addAction(action);
	connect(action, SIGNAL(triggered()), this, SLOT(removeMarkers()));
	m_removeAllLabel = new QAction(tr("Remove all"), this);
	m_menu->addAction(m_removeAllLabel);
	connect(m_removeAllLabel, SIGNAL(triggered()), this, SLOT(removeAllLabels()));

	m_markerDir = AwSettings::getInstance()->value(aws::marker_rules_dir).toString();

	// always add a rule set as "No Rule" at first index
	m_noRuleString = QString(tr("No rule"));
	comboFilters->addItem(QString(m_noRuleString));
	
	if (!m_markerDir.isEmpty())	{
		// parse marker directory for saved rules
		QStringList filter("*.awr");
		QDir dir(m_markerDir);
		dir.setNameFilters(filter);
		QStringList files = dir.entryList();

		// parse files to fill combo
		for (int i = 0; i < files.count(); i++) {
			QString key = files[i];
			key = key.remove(".awr", Qt::CaseInsensitive);
			m_loadedRules << key;
			comboFilters->addItem(key);
		}
	}
	
	m_markerRuleDial = new AwMarkerRuleManageDial(m_loadedRules, this);
	m_currentRule = NULL;

	// Connections
	connect(m_model, &AwMarkerListModel::dataChanged, this, &AwMarkerManagerSettings::updateMarkerList);

	connect(tvMarkers, SIGNAL(customContextMenuRequested(const QPoint& )), this, SLOT(contextMenuRequested(const QPoint&)));
	connect(tvMarkers, SIGNAL(clickedRow(int)), this, SLOT(goToMarkerAtRow(int)));
	connect(comboFilters, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(setRule(const QString&)));
	connect(m_markerRuleDial, SIGNAL(ruleRemoved(const QString&)), this, SLOT(removeRule(const QString&)));
	connect(buttonExportWizard, SIGNAL(clicked()), this, SLOT(exportWizard()));
	connect(buttonEditRules, SIGNAL(clicked()), this, SLOT(editRules()));
	connect(comboNames, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(updateNamesStats(const QString&)));
	connect(comboValues, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(updateValuesStats(const QString&)));
	// marker navigation
	connect(buttonPrevName, SIGNAL(clicked()), this, SLOT(prevName()));
	connect(buttonNextName, SIGNAL(clicked()), this, SLOT(nextName()));
	connect(buttonGoName, SIGNAL(clicked()), this, SLOT(goToName()));
	connect(buttonPrevValue, SIGNAL(clicked()), this, SLOT(prevValue()));
	connect(buttonNextValue, SIGNAL(clicked()), this, SLOT(nextValue()));
	connect(buttonGoValue, SIGNAL(clicked()), this, SLOT(goToValue()));
	// clean trigger
	connect(buttonClearTriggerChannel, SIGNAL(clicked()), this, SIGNAL(clearTriggerClicked()));
	// save / load
	connect(buttonLoad, SIGNAL(clicked()), this, SIGNAL(loadMarkersClicked()));
	connect(buttonSave, SIGNAL(clicked()), this, SIGNAL(saveMarkersClicked()));
	m_isAddingMarker = false;
	// some buttons are disabled when the marker list is empty
	buttonExportWizard->setEnabled(!m_markers.isEmpty());
	buttonSave->setEnabled(!m_markers.isEmpty());

	// Connect check boxes to show/hide columns
	connect(checkLabel, SIGNAL(toggled(bool)), this, SLOT(showColumn(bool)));
	connect(checkPos, SIGNAL(toggled(bool)), this, SLOT(showColumn(bool)));
	connect(checkValue, SIGNAL(toggled(bool)), this, SLOT(showColumn(bool)));
	connect(checkDuration, SIGNAL(toggled(bool)), this, SLOT(showColumn(bool)));
	connect(checkTargets, SIGNAL(toggled(bool)), this, SLOT(showColumn(bool)));
	connect(checkColor, SIGNAL(toggled(bool)), this, SLOT(showColumn(bool)));

	tvMarkers->setColumnHidden(MARKER_COLUMN_CODE, true);
	tvMarkers->setColumnHidden(MARKER_COLUMN_COLOR, true);
	tvMarkers->setColumnHidden(MARKER_COLUMN_TARGET, true);
}

AwMarkerManagerSettings::~AwMarkerManagerSettings()
{
	if (m_currentRule)
		delete m_currentRule;
}

void AwMarkerManagerSettings::changeEvent(QEvent *e)
{
	if (e) {
		switch (e->type()) {
		case QEvent::LanguageChange:
			retranslateUi(this);
			break;
		}
	}
}

void AwMarkerManagerSettings::applyRule(AwMarkerRule *rule)
{
	// If rule parameter is null when reset rules filter to no rule => so all markers are sent to display
	if (rule == NULL)
		m_displayedMarkers = m_markers;
	else
		m_displayedMarkers = rule->applyRule(m_markers);

	m_model->updateMarkers(m_displayedMarkers);
	emit markersChanged(m_displayedMarkers);
	updateStats();
}

void AwMarkerManagerSettings::setMarkers(const AwMarkerList& markers)
{
	m_markers = markers;
	applyRule(m_currentRule);
	// some buttons are disabled when the marker list is empty
	buttonExportWizard->setEnabled(!m_markers.isEmpty());
	buttonSave->setEnabled(!m_markers.isEmpty());
}

void AwMarkerManagerSettings::cleanUp()
{
	m_markers.clear();
	m_displayedMarkers.clear();
	m_model->clear();
	updateStats();
	// some buttons are disabled when the marker list is empty
	buttonExportWizard->setEnabled(!m_markers.isEmpty());
	buttonSave->setEnabled(!m_markers.isEmpty());
}

void AwMarkerManagerSettings::updateStats()
{
	bool enable = !m_displayedMarkers.isEmpty();
	groupBoxDisplayRules->setEnabled(enable);
	groupBoxQuickNav->setEnabled(enable);

	if (m_isAddingMarker)
		return;

	// Total
	labelTotal->setText(QString(tr("%1 marker(s)")).arg(m_displayedMarkers.size()));

	// populate combo boxes but keep previous name and value selected if any
	QString prevValue = comboValues->currentText();
	QString prevName = comboNames->currentText();
	int prevValueIndex = -1, prevNameIndex = -1;
	QStringList names;
	QStringList values;

	comboNames->clear();
	comboValues->clear();
	foreach (AwMarker *m, m_displayedMarkers)	{
		QString value = QString("%1").arg(m->value());
		if (!names.contains(m->label()))
			names << m->label();
		if (!values.contains(value))
			values << value;
	}
	comboNames->setEnabled(!names.isEmpty());
	comboValues->setEnabled(!values.isEmpty());
	comboNames->addItems(names);
	comboValues->addItems(values);

	// reset index to previous selection if any.
	if (!prevValue.isEmpty()) {
		prevValueIndex = values.indexOf(prevValue);
		if (prevValueIndex != -1)
			comboValues->setCurrentIndex(prevValueIndex);
	}
	if (!prevName.isEmpty()) {
		prevNameIndex = names.indexOf(prevName);
		if (prevNameIndex != -1)
			comboNames->setCurrentIndex(prevNameIndex);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// SLOTS

void AwMarkerManagerSettings::showColumn(bool flag)
{
	QCheckBox *cb = (QCheckBox *)sender();
	if (cb == NULL)
		return;
	if (cb == checkDuration) {
		tvMarkers->setColumnHidden(MARKER_COLUMN_DURATION, !flag);
	}
	else if (cb == checkPos) {
		tvMarkers->setColumnHidden(MARKER_COLUMN_POS, !flag);
	}
	else if (cb == checkLabel) {
		tvMarkers->setColumnHidden(MARKER_COLUMN_LABEL, !flag);
	}
	else if (cb == checkTargets) {
		tvMarkers->setColumnHidden(MARKER_COLUMN_TARGET, !flag);
	}
	else if (cb == checkValue) {
		tvMarkers->setColumnHidden(MARKER_COLUMN_CODE, !flag);
	}
	else if (cb == checkColor) {
		tvMarkers->setColumnHidden(MARKER_COLUMN_COLOR, !flag);
	}
}

void AwMarkerManagerSettings::setMarkerAddingMode(bool on)
{
	m_isAddingMarker = on;
	if (!on)
		updateStats();
}

void AwMarkerManagerSettings::editCurrentItem()
{
	QModelIndexList indexes = tvMarkers->selectionModel()->selectedIndexes();
	if (indexes.isEmpty())
		return;

	QModelIndex index = tvMarkers->currentIndex();
	if (index.column() == MARKER_COLUMN_TARGET) {
		// get the marker
		// run the UI to edit targets channels for the marker
		AwPickChannelsDial dlg(tvMarkers->model()->data(index, Qt::EditRole).toStringList());
		if (dlg.exec() == QDialog::Accepted)  {
			tvMarkers->model()->setData(index, dlg.targets, Qt::EditRole);
		}
	}
	else
		tvMarkers->edit(tvMarkers->currentIndex());
}

void AwMarkerManagerSettings::show()
{
	AwFileIO *reader = AwSettings::getInstance()->currentReader();
	if (!reader->triggerChannels().isEmpty())
		buttonClearTriggerChannel->setEnabled(true);
	else
		buttonClearTriggerChannel->setEnabled(false);
	QWidget::show();
}

void AwMarkerManagerSettings::updateMarkerList()
{
	m_displayedMarkers = m_model->markers();
	emit markersChanged(m_displayedMarkers);
	updateStats();
}



void AwMarkerManagerSettings::addRule(const QString& name)
{
	m_loadedRules << name;
	comboFilters->addItem(name);
}

void AwMarkerManagerSettings::modifyRule(const QString &oldName, const QString &newName)
{
	int index = m_loadedRules.indexOf(oldName);
	int indexCombo = comboFilters->currentIndex();
	int findIndex = comboFilters->findText(oldName);

	if (index != -1)
		m_loadedRules.replace(index, newName);
	if (findIndex != -1)
		comboFilters->setItemText(findIndex, newName);

	comboFilters->setCurrentIndex(indexCombo);
}


void AwMarkerManagerSettings::removeRule(const QString& name)
{
	int index = m_loadedRules.indexOf(name);

	if (index != -1) {
		m_loadedRules.removeAt(index);
		if (m_currentRule)
			if (m_currentRule->name() == name)
				setRule(m_noRuleString);

		AwMarkerRule::remove(name);

		// reset combo box items
		comboFilters->clear();
		// always add a rule set as "No Rule" at first index
		comboFilters->addItem(m_noRuleString);
		comboFilters->addItems(m_loadedRules);
	}
}

void AwMarkerManagerSettings::setRule(const QString& rule)
{
	if (m_currentRule == NULL && rule == m_noRuleString)
		return;

	if (m_currentRule)	{
		delete m_currentRule;
		m_currentRule = NULL;
	}

	if (rule == m_noRuleString) {
		applyRule(NULL);
		return;
	}

	AwMarkerRule *mr = new AwMarkerRule();

	mr->setName(rule);
	if (mr->load())
		m_currentRule = mr;
	else
		delete mr;

	if (m_currentRule)
		applyRule(m_currentRule);
}

void AwMarkerManagerSettings::goToMarkerAtRow(int row)
{
	AwMarkerList currentMarkers = m_model->markers();
	QSortFilterProxyModel *proxy = (QSortFilterProxyModel *)tvMarkers->model();
	
	QModelIndex index = tvMarkers->model()->index(row, MARKER_COLUMN_POS);
	int i = proxy->mapToSource(index).row();
	emit moveRequest(currentMarkers.at(i)->start());
}

void AwMarkerManagerSettings::goToMarkerPos()
{
	QModelIndexList indexes = tvMarkers->selectionModel()->selectedIndexes();
	if (indexes.isEmpty())
		return;

	QSortFilterProxyModel *proxy = (QSortFilterProxyModel *)tvMarkers->model();
	// if several markers are selected, only consider the first one
	foreach (QModelIndex index, indexes) {
		if (index.column() == MARKER_COLUMN_POS) {
			int row = proxy->mapToSource(index).row();
			emit moveRequest(m_model->markers().at(row)->start());
			break;
		}
	}
}

void AwMarkerManagerSettings::renameAllMarkers()
{

	AwGetValueDialog dlg("Set new label");
	if (dlg.exec() == QDialog::Rejected)
		return;

	AwMarkerList currentMarkers = m_model->markers();
	for (auto m : currentMarkers)
		m->setLabel(dlg.value());

	m_displayedMarkers = currentMarkers;
	m_model->updateMarkers(currentMarkers);
	emit markersChanged(m_displayedMarkers);
	updateStats();
}

void AwMarkerManagerSettings::renameSelectedMarkers()
{
	AwGetValueDialog dlg("Set new label");
	if (dlg.exec() == QDialog::Rejected)
		return;
	QModelIndexList indexes = tvMarkers->selectionModel()->selectedIndexes();
	AwMarkerList currentMarkers = m_model->markers();
	QSortFilterProxyModel *proxy = (QSortFilterProxyModel *)tvMarkers->model();

	for (auto i : indexes) {
		if (i.column() == MARKER_COLUMN_LABEL) {
			int row = proxy->mapToSource(i).row();
			//tvMarkers->model()->setData(i, dlg.value(), Qt::EditRole);
			auto m = currentMarkers.value(row);
			m->setLabel(dlg.value());
		}
	}
	m_displayedMarkers = currentMarkers;
	m_model->updateMarkers(currentMarkers);
	emit markersChanged(m_displayedMarkers);
	updateStats();
}

void AwMarkerManagerSettings::changeValueAllMarkers()
{
	AwGetValueDialog dlg("Set new value");
	if (dlg.exec() == QDialog::Rejected)
		return;

	auto newValue = dlg.value().toDouble();
	AwMarkerList currentMarkers = m_model->markers();
	for (auto m : currentMarkers)
		m->setValue(newValue);

	m_displayedMarkers = currentMarkers;
	m_model->updateMarkers(currentMarkers);
	emit markersChanged(m_displayedMarkers);
	updateStats();
}

void AwMarkerManagerSettings::changeValueSelectedMarkers()
{
	AwGetValueDialog dlg("Set new value");
	if (dlg.exec() == QDialog::Rejected)
		return;

	QModelIndexList indexes = tvMarkers->selectionModel()->selectedIndexes();
	AwMarkerList currentMarkers = m_model->markers();
	QSortFilterProxyModel *proxy = (QSortFilterProxyModel *)tvMarkers->model();

	for (auto i : indexes) {
		if (i.column() == MARKER_COLUMN_LABEL) {
			int row = proxy->mapToSource(i).row();
			//tvMarkers->model()->setData(i, dlg.value(), Qt::EditRole);
			auto m = currentMarkers.value(row);
			m->setValue(dlg.value().toDouble());
		}
	}
	m_displayedMarkers = currentMarkers;
	m_model->updateMarkers(currentMarkers);
	emit markersChanged(m_displayedMarkers);
	updateStats();
}


void AwMarkerManagerSettings::launchProcess()
{
	// get selected indexes in tvMarkers
	QModelIndexList indexes = tvMarkers->selectionModel()->selectedIndexes();
	if (indexes.isEmpty())
		return;

	AwMarkerList currentMarkers = m_model->markers();

	AwMarkerList markers; // marker that has been removed
	QSortFilterProxyModel *proxy = (QSortFilterProxyModel *)tvMarkers->model();
	for (auto i : indexes) {
		if (i.column() == 0)
			markers << currentMarkers.at(proxy->mapToSource(i).row());
	}

	QAction *action = (QAction *)sender();
	if (action == Q_NULLPTR)
		return;
	auto plugin = AwPluginManager::getInstance()->getProcessPluginByName(action->data().toString());
	if (plugin) {
		auto process = AwProcessManager::instance()->newProcess(plugin);
		process->pdi.input.setNewMarkers(markers, true);
		AwProcessManager::instance()->runProcess(process);
	}
}

void AwMarkerManagerSettings::selectAllLabels() 
{
	QItemSelection newSelection;
	for (int i = 0; i < tvMarkers->model()->rowCount(); i++) {
		QModelIndex index = tvMarkers->model()->index(i, 0);
		if (index.data().toString() == m_selectAllLabel->data().toString()) {
			QModelIndex begin = tvMarkers->model()->index(index.row(), MARKER_COLUMN_LABEL);
			QModelIndex end = tvMarkers->model()->index(index.row(), MARKER_COLUMN_TARGET);
			newSelection.select(begin, end);
		}
	}
	tvMarkers->selectionModel()->select(newSelection, QItemSelectionModel::Select);
	tvMarkers->setFocus();
}

/** remove all the markers with the same label **/
void AwMarkerManagerSettings::removeAllLabels() 
{
	QString label = m_removeAllLabel->data().toString();
	if (AwMessageBox::question(this, tr("Remove Markers"), QString(tr("Do you really want to remove all %1?").arg(label))) == QMessageBox::No)
		return;

	AwMarkerList markers;
	foreach (AwMarker *m, m_displayedMarkers) {
		if (m->label() == label)
			markers << m;
	}
	emit markersRemoved(markers);
	m_displayedMarkers = m_model->markers();
	emit markersChanged(m_displayedMarkers);
}

void AwMarkerManagerSettings::saveSelectedMarkersToMATLAB()
{
	// get selected indexes in tvMarkers
	QModelIndexList indexes = tvMarkers->selectionModel()->selectedIndexes();

	if (indexes.isEmpty())
		return;

	AwFileInfo afio(AwSettings::getInstance()->currentReader());
	auto path = QFileDialog::getSaveFileName(0, tr("Save marked data"), afio.dirPath(), "*.mat");
	if (!path.isEmpty()) {
		AwMarkerList currentMarkers = m_model->markers();

		AwMarkerList markers; // marker to save
		QSortFilterProxyModel *proxy = (QSortFilterProxyModel *)tvMarkers->model();
		for (auto i : indexes) {
			if (i.column() == 0) {
				auto m = currentMarkers.at(proxy->mapToSource(i).row());
				if (m->duration() > 0.)
					markers << m;
			}
		}
		// check for marker with duration
		if (markers.isEmpty()) {
			AwMessageBox::information(0, tr("Error"), QString("Selected markers have no duration."));
			return;
		}
		// Instantiate process
		AwProcessManager *process_manager = AwProcessManager::instance();
		auto process = process_manager->newProcessFromPluginName("MATLAB_MARKERS_EXPORTER");
		if (process == Q_NULLPTR) {
			AwMessageBox::information(0, tr("Error"), QString("Missing process plugin to do the job."));
			return;
		}
		// set markers to compute data on
		process->pdi.input.setNewMarkers(markers, true);
		process->pdi.input.addArgument("output_file", path);
		// start process
		process_manager->runProcess(process);
	}

}

void AwMarkerManagerSettings::saveSelectedMarkers()
{
	// get selected indexes in tvMarkers
	QModelIndexList indexes = tvMarkers->selectionModel()->selectedIndexes();

	if (indexes.isEmpty())
		return;
	
	AwFileInfo afio(AwSettings::getInstance()->currentReader());

	auto path = QFileDialog::getSaveFileName(0, tr("Save markers"), afio.dirPath(), "*.mrk");
	if (!path.isEmpty()) {
		AwMarkerList currentMarkers = m_model->markers();

		AwMarkerList markers; // marker to save
		QSortFilterProxyModel *proxy = (QSortFilterProxyModel *)tvMarkers->model();
		for (auto i : indexes) {
			if (i.column() == 0)
				markers << currentMarkers.at(proxy->mapToSource(i).row());
		}
		AwMarker::save(path, markers);
		AwMessageBox::information(0, tr("Success"), QString(tr("Markers saved to %1")).arg(path));
	}
}

void AwMarkerManagerSettings::removeMarkers()
{
	// get selected indexes in tvMarkers
	QModelIndexList indexes = tvMarkers->selectionModel()->selectedIndexes();

	if (indexes.isEmpty())
		return;

	if (AwMessageBox::question(this, tr("Remove Markers"), QString(tr("Remove all the selected markers?"))) == QMessageBox::No)
		return;

	AwMarkerList currentMarkers = m_model->markers();

	AwMarkerList markers; // marker that has been removed
	QSortFilterProxyModel *proxy = (QSortFilterProxyModel *)tvMarkers->model();
	for (auto i : indexes) {
		if (i.column() == 0)
			markers << currentMarkers.at(proxy->mapToSource(i).row());
	}
	emit markersRemoved(markers);
	m_displayedMarkers = m_model->markers();
	emit markersChanged(m_displayedMarkers);
}

void AwMarkerManagerSettings::highlightMarker(AwMarker *marker)
{
	// find the marker in the list
	AwMarkerList currentMarkers = m_model->markers();
	int index = currentMarkers.indexOf(marker);
	if (index == -1)
		return;
	QSortFilterProxyModel *proxy = (QSortFilterProxyModel *)tvMarkers->model();

	QModelIndex mindex = m_model->index(index, MARKER_COLUMN_LABEL);
	auto row = proxy->mapFromSource(mindex).row();
	tvMarkers->setFocus();
	tvMarkers->showRow(row);
	tvMarkers->selectRow(row);
}

void AwMarkerManagerSettings::writeTrigger()
{
	AwFileIO *reader = AwSettings::getInstance()->currentReader();
	bool ok = reader->flags() & Aw::AwIOFlags::TriggerChannelIsWritable &&  !reader->triggerChannels().isEmpty();
	if (ok) {
		// get selected indexes in tvMarkers
		QModelIndexList indexes = tvMarkers->selectionModel()->selectedIndexes();

		if (indexes.isEmpty())
			return;

		AwMarkerList currentMarkers = m_model->markers();

		AwMarkerList markers; // marker that has been removed
		QSortFilterProxyModel *proxy = (QSortFilterProxyModel *)tvMarkers->model();
		for (auto i : indexes) {
			if (i.column() == 0)
				markers << currentMarkers.at(proxy->mapToSource(i).row());
		}

		//emit triggerMarkersToWrite(markers);

		// Create the Process AwTriggerWriter
		AwBaseProcess *process = AwProcessManager::instance()->newProcessFromPluginName("Trigger Writer");
		if (process == Q_NULLPTR) {
			AwMessageBox::critical(0, "Missing Plugin", "The Trigger Writer plugin is not loaded.");
			return;
		}
		process->pdi.input.setNewMarkers(markers, true);
		AwProcessManager::instance()->runProcess(process);
	}
	else 
		AwMessageBox::information(0, "TRIGGER Writing", "The file does not support writing to TRIGGER channels");
}

void AwMarkerManagerSettings::clearTrigger()
{
	AwFileIO *reader = AwSettings::getInstance()->currentReader();
	bool ok = reader->flags() & Aw::AwIOFlags::TriggerChannelIsWritable && !reader->triggerChannels().isEmpty();
	if (ok) {
		if (QMessageBox::question(0, tr("Clear Trigger Channel?"), tr("Do you really want to clear all the data in the trigger channel?"),
			QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
			return;

		// Create the Process AwTriggerEraser
		AwBaseProcess *process = AwProcessManager::instance()->newProcessFromPluginName("Trigger Eraser");
		if (process == Q_NULLPTR) {
			AwMessageBox::critical(0, "Missing Plugin", "The Trigger Eraser plugin is not loaded.");
			return;
		}
		AwProcessManager::instance()->runProcess(process);
	}
	else
		AwMessageBox::information(0, "TRIGGER Channel", "The file does not support writing to TRIGGER channels");
}

void AwMarkerManagerSettings::contextMenuRequested(const QPoint& pos)
{
	QModelIndex index = tvMarkers->indexAt(pos);
	if (!index.isValid()) 
		return;
	QSortFilterProxyModel *proxy = (QSortFilterProxyModel *)tvMarkers->model();
	AwMarker *firstMarker = m_displayedMarkers.at(proxy->mapToSource(index).row());

	m_removeAllLabel->setText(QString(tr("Remove all %1")).arg(firstMarker->label()));
	m_removeAllLabel->setData(firstMarker->label());
	m_selectAllLabel->setText(QString(tr("Select all %1")).arg(firstMarker->label()));
	m_selectAllLabel->setData(firstMarker->label());
	m_menu->exec(tvMarkers->mapToGlobal(pos));
}


void AwMarkerManagerSettings::editRules()
{
	AwMarkerRuleManageDial dlg(m_loadedRules, this);

	connect(&dlg, SIGNAL(ruleRemoved(const QString&)), this, SLOT(removeRule(const QString&)));
	connect(&dlg, SIGNAL(ruleAdded(const QString&)), this, SLOT(addRule(const QString&)));
	connect(&dlg, SIGNAL(ruleChanged(const QString&, const QString&)), this, SLOT(modifyRule(const QString&, const QString&)));

	dlg.exec();

	// apply current rule again (in case it has been modified)
	if (m_currentRule)
		applyRule(m_currentRule);
}

void AwMarkerManagerSettings::prevName()
{
	--m_nameIndex;
	if (m_nameIndex < 0)
		m_nameIndex = 0;
	
	emit moveRequest(m_statsNames.at(m_nameIndex)->start());
}

void AwMarkerManagerSettings::nextName()
{
	m_nameIndex++;
	if (m_nameIndex > m_statsNames.size() - 1)
		m_nameIndex = 0;

	emit moveRequest(m_statsNames.at(m_nameIndex)->start());
}

void AwMarkerManagerSettings::goToName()
{
	emit moveRequest(m_statsNames.at(m_nameIndex)->start());
}

void AwMarkerManagerSettings::prevValue()
{
	--m_valueIndex;
	if (m_valueIndex < 0)
		m_valueIndex = 0;
	
	emit moveRequest(m_statsValues.at(m_valueIndex)->start());
}

void AwMarkerManagerSettings::nextValue()
{
	m_valueIndex++;
	if (m_valueIndex > m_statsValues.size() - 1)
		m_valueIndex = 0;

	emit moveRequest(m_statsValues.at(m_valueIndex)->start());
}

void AwMarkerManagerSettings::goToValue()
{
	emit moveRequest(m_statsValues.at(m_valueIndex)->start());
}



void AwMarkerManagerSettings::updateNamesStats(const QString &name)
{
	qint32 count = 0;
	m_nameIndex = 0;
	m_statsNames.clear();

	foreach (AwMarker *m, m_displayedMarkers) {
		if (m->label() == name)	{
			count++;
			m_statsNames << m;
		}
	}
	labelNames->setText(QString(tr("%1 marker(s)")).arg(count));
	
	// depending on stats, activate navigation buttons
	buttonPrevName->setVisible(count > 1);
	buttonNextName->setVisible(count > 1);
	buttonGoName->setVisible(count <= 1); 
}

void AwMarkerManagerSettings::updateValuesStats(const QString &value)
{
	qint32 count = 0;
	m_valueIndex = 0;
	int code = value.toInt();
	m_statsValues.clear();

	foreach (AwMarker *m, m_displayedMarkers) 	{
		if (m->value() == code) 	{
			count++;
			m_statsValues << m;
		}
	}

	labelValues->setText(QString(tr("%1 marker(s)")).arg(count));

	// depending on stats, activate navigation buttons
	buttonPrevValue->setVisible(count > 1);
	buttonNextValue->setVisible(count > 1);
	buttonGoValue->setVisible(count <= 1); 
}