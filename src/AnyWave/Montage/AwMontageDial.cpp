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

#include "AwMontageDial.h"
#include <QtDebug>
#include <QApplication>
#include <QDesktopWidget>
#include <widget/AwMessageBox.h>
#include <QDomElement> // For xml input/output
#include <QFileDialog>
#include <AwAmplitudeManager.h>
#include "AwMontageManager.h"
#include "Widgets/AwMontageRefWidget.h"
#include <hdf5/AwHDF5.h>
#include "AwAVGChannel.h"
#include "ICA/AwICAManager.h"
#include "ICA/AwICAComponents.h"
#include "ICA/AwICAChannel.h"
#include "AwECoGDialog.h"
#include <algorithm>
#include <AwCore.h>
using namespace std;

///////////////////////////////////////////////////////////////////////////////
/// constructor
///
AwMontageDial::AwMontageDial(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	
	AwMontageManager *mm = AwMontageManager::instance();
//	m_asRecorded = mm->cloneAsRecordedChannels();

	// cloning as recorded channels 
	for (auto c : mm->asRecordedChannels()) {
		if (c == nullptr)
			continue;
		auto duplicated = c->duplicate();
		m_asRecordedChannels << duplicated;
		m_hashAsRecorded.insert(duplicated->name(), duplicated);
	}

	m_badChannelsLabels = mm->badLabels();
	m_path = mm->path();

	m_ui.buttonAddTypes->setMenu(&m_addByTypesMenu);
	sortLabelsByTypes();
	
	// resize the dialog in order that it fits the screen height
	QRect rectScreen = QApplication::desktop()->screenGeometry(this);
	QSize sizeScreen = rectScreen.size();
	QRect rectDialog = this->geometry();

	QSize sizeDialog = rectDialog.size();
	sizeDialog.setHeight(sizeScreen.height() - 100);
	setMinimumHeight(sizeScreen.height() - 100);
	rectDialog.setSize(sizeDialog);
	setGeometry(rectDialog);
	move(QPoint(100,0));
	// resizing done
	connect(m_ui.buttonClean, &QPushButton::clicked, this, &AwMontageDial::cleanMontage);
	connect(m_ui.buttonReset, &QPushButton::clicked, this, &AwMontageDial::resetToAsRecorded);
	connect(m_ui.buttonUp, &QPushButton::clicked, this, &AwMontageDial::moveUp);
	connect(m_ui.buttonDown, &QPushButton::clicked, this, &AwMontageDial::moveDown);
	// Menu for button Re-Order
	auto sortMenu = new QMenu(m_ui.buttonSort);
	auto action = sortMenu->addAction("Sort by name");
	connect(action, &QAction::triggered, this, &AwMontageDial::sortMontageByName);
	action = sortMenu->addAction("Sort by type");
	connect(action, &QAction::triggered, this, &AwMontageDial::sortMontageByType);
	m_ui.buttonSort->setMenu(sortMenu);

	// sorting proxy for as recorded channels
	AsRecordedProxyModel *sortModelAsRecorded = new AsRecordedProxyModel(this);
	// Set the sort mode to UserRole, to give the possibility to sort the Bad column.
	sortModelAsRecorded->setSortRole(Qt::UserRole);
	m_asRecordedChannels = AwChannel::sortByName(m_asRecordedChannels);
	AwChannelListModelAsRecorded *modelAsRecorded = new AwChannelListModelAsRecorded(m_asRecordedChannels, this);
	connect(modelAsRecorded, &AwChannelListModelAsRecorded::badChannelSet, this, &AwMontageDial::setBadChannel);
	connect(modelAsRecorded, &AwChannelListModelAsRecorded::badChannelUnset, this, &AwMontageDial::unsetBadChannel);

	sortModelAsRecorded->setSourceModel(modelAsRecorded);
	AwChannelListModel *model = new AwChannelListModel(mm->channels(), this);
	connect(model, &AwChannelListModel::channelsDropped, this, &AwMontageDial::addDroppedChannels);
	m_ui.tvDisplay->setModel(model);
	m_ui.tvDisplay->setItemDelegate(new AwChannelListDelegate(m_labelsByTypes, this));

	m_ui.tvChannelsAsRecorded->setModel(sortModelAsRecorded);
	m_ui.tvChannelsAsRecorded->setItemDelegate(new AwChannelListDelegateAsRecorded(this));
	m_ui.tvChannelsAsRecorded->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_ui.tvChannelsAsRecorded->setSortingEnabled(true);

	m_ui.tvDisplay->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_ui.tvDisplay->resizeColumnsToContents();
	m_ui.tvChannelsAsRecorded->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_ui.tvChannelsAsRecorded->resizeColumnsToContents();

	// Context Menu rajouté à la TableView
	m_ui.tvDisplay->setContextMenuPolicy(Qt::CustomContextMenu);
	m_ui.tvChannelsAsRecorded->setContextMenuPolicy(Qt::CustomContextMenu);
	createContextMenuAndActions();

	// Connexions de certains widgets de l'objet dlg avec l'objet Montage
	connect(m_ui.buttonAddChannels, SIGNAL(clicked()), this, SLOT(addChannelsToMontage()));
	connect(m_ui.buttonRemove, SIGNAL(clicked()), this, SLOT(removeChannelsInMontage()));
	connect(m_ui.buttonSave, SIGNAL(clicked()), this, SLOT(save()));
	connect(m_ui.buttonLoad, SIGNAL(clicked()), this, SLOT(loadMontage()));

	connect(m_ui.tvDisplay, SIGNAL(customContextMenuRequested(const QPoint& )), this, SLOT(contextMenuMontageRequested(const QPoint&)));
	connect(m_ui.tvChannelsAsRecorded, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(contextMenuAsRecordedRequested(const QPoint&)));
	connect(m_ui.buttonEEGSEEG, SIGNAL(clicked()), this, SLOT(computeSEEGMontageFromEEGChannels()));

	// init special Montages menu
	QMenu *specialMontageMenu = new QMenu(m_ui.buttonSpecial);
	connect(specialMontageMenu->addAction("SEEG Bipolar"), SIGNAL(triggered()), this, SLOT(makeSEEGBipolar()));
	connect(specialMontageMenu->addAction("SEEG AVG Reference"), SIGNAL(triggered()), this, SLOT(setSEEGAVGRefMontage()));
	connect(specialMontageMenu->addAction("ECoG Bipolar"), SIGNAL(triggered()), this, SLOT(makeECOGBipolar()));
	connect(specialMontageMenu->addAction("EEG AVG Reference"), SIGNAL(triggered()), this, SLOT(setAVGRefMontage()));
	m_ui.buttonSpecial->setMenu(specialMontageMenu);

	/// Columns hide/show buttons
	connect(m_ui.checkColor, SIGNAL(toggled(bool)), this, SLOT(showColumn(bool)));
	connect(m_ui.checkHP, SIGNAL(toggled(bool)), this, SLOT(showColumn(bool)));
	connect(m_ui.checkLP, SIGNAL(toggled(bool)), this, SLOT(showColumn(bool)));
	connect(m_ui.checkNotch, SIGNAL(toggled(bool)), this, SLOT(showColumn(bool)));

	m_ui.tvDisplay->setColumnHidden(AW_MONTAGE_COLUMN_HPF, true);
	m_ui.tvDisplay->setColumnHidden(AW_MONTAGE_COLUMN_LPF, true);
	m_ui.tvDisplay->setColumnHidden(AW_MONTAGE_COLUMN_NOTCH, true);
	m_ui.tvDisplay->setColumnHidden(AW_MONTAGE_COLUMN_COLOR, true);
}

AwMontageDial::~AwMontageDial()
{
	AW_DESTROY_LIST(m_asRecordedChannels);
}	

// EVENTS

void AwMontageDial::closeEvent(QCloseEvent *e)
{
	reject();
}

void AwMontageDial::loadMontage()
{
	QString path = QFileDialog::getOpenFileName(this, tr("Load a Montage"), m_path, tr("Montage file (*.mtg)"));
	if (path.isEmpty())
		return;
	AwChannelList channels = AwMontageManager::instance()->load(path);

	for (auto c : AwMontageManager::instance()->asRecordedChannels()) {

		// just set the type of as recorded channel in case the new montage changed it.
		if (c == nullptr)
			continue;
		auto asRecorded = m_hashAsRecorded.value(c->name());
		if (asRecorded)
			asRecorded->setType(c->type());
	}

	removeBadChannels(channels);
	if (channels.isEmpty())
		return;
	static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->setMontage(channels);
	sortLabelsByTypes();
}


void AwMontageDial::moveUp()
{
	AwChannelList channels = static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->currentMontage();
	if (channels.isEmpty())
		return;
	QItemSelectionModel *selectModel = m_ui.tvDisplay->selectionModel();
	if (!selectModel->hasSelection())
		return;

	QModelIndexList selectedIndexes = selectModel->selectedIndexes();
	// sort selection by rows, ascending order
	sort(selectedIndexes.begin(), selectedIndexes.end(),
		[](QModelIndex a, QModelIndex b) -> bool { return a.row() < b.row(); });

	// check if first row can be up
	if (selectedIndexes.first().row() == 0) {
		m_ui.tvDisplay->setFocus();
		return;
	}

	for (auto i : selectedIndexes) {
		if (i.column() == 0) {
			if (i.row() > 0)
				channels.move(i.row(), i.row() - 1);
		}
	}
	static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->updateMontage(channels);

	// update selection
	QItemSelection newSelection;
	for (auto i : selectedIndexes) {
		if (i.column() == 0) {
			QModelIndex begin = m_ui.tvDisplay->model()->index(i.row() - 1, AW_MONTAGE_COLUMN_NAME);
			QModelIndex end = m_ui.tvDisplay->model()->index(i.row() - 1, AW_MONTAGE_COLUMN_HPF);
			newSelection.select(begin, end);
		}
	}
	m_ui.tvDisplay->selectionModel()->select(newSelection, QItemSelectionModel::Select);
	m_ui.tvDisplay->setFocus();
}

void AwMontageDial::sortMontageByName()
{
	static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->sortByName();
}

void AwMontageDial::sortMontageByType()
{
	static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->sortByNameAndType();
}


void AwMontageDial::moveDown()
{
	AwChannelList channels = static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->currentMontage();
	if (channels.isEmpty())
		return;
	QItemSelectionModel *selectModel = m_ui.tvDisplay->selectionModel();
	if (!selectModel->hasSelection())
		return;

	QModelIndexList selectedIndexes = selectModel->selectedIndexes();
	// sort selection by rows, descending order
	sort(selectedIndexes.begin(), selectedIndexes.end(),
		[](QModelIndex a, QModelIndex b) -> bool { return a.row() > b.row(); });

	// check if the last row can go down
	if (selectedIndexes.first().row() == channels.size() - 1) {
		m_ui.tvDisplay->setFocus();
		return;
	}

	for (auto i : selectedIndexes) {
		if (i.column() == 0) {
			if (i.row() < channels.size() - 1)
				channels.move(i.row(), i.row() + 1);
		}
	}
	static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->updateMontage(channels);

	// update selection
	QItemSelection newSelection;
	for (auto i : selectedIndexes) {
		if (i.column() == 0) {
			QModelIndex begin = m_ui.tvDisplay->model()->index(i.row() + 1, AW_MONTAGE_COLUMN_NAME);
			QModelIndex end = m_ui.tvDisplay->model()->index(i.row() + 1, AW_MONTAGE_COLUMN_HPF);
			newSelection.select(begin, end);
		}
	}
	m_ui.tvDisplay->selectionModel()->select(newSelection, QItemSelectionModel::Select);
	m_ui.tvDisplay->setFocus();
}

void AwMontageDial::updateButtonAddByTypes()
{
	// create menu and actions for the Add By Types button, based on available types of channels in the As Recorded list.
	m_addByTypesMenu.clear();
	QAction *act;

	for (auto k : m_labelsByTypes.keys()) {
		act = m_addByTypesMenu.addAction(AwChannel::typeToString(k));
		act->setData(k);
		connect(act, &QAction::triggered, this, &AwMontageDial::addChannelsByTypes);
	}
}

/*!
 * \brief
 * Rebuild list of labels for different types of channels.
 * 
 * 
 * Parse As Recorded Channels and build list of labels for different types.
 * Update buttons in UI. 
 * 
 * \remarks
 * This method is called each time a channel type has been changed in UI or set as bad.
 * 
 */
void AwMontageDial::sortLabelsByTypes()
{
	m_labelsByTypes.clear();
//	auto asRecordedChannels = m_asRecorded.values();
	for (auto c : m_asRecordedChannels) {
		for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
			auto channels = AwChannel::getChannelsOfType(m_asRecordedChannels, i);
			if (!channels.isEmpty()) {
				auto labels = AwChannel::getLabels(channels);
				labels.sort();
				m_labelsByTypes[i] = labels;
			}
		}
	}
	updateButtonAddByTypes();
}


void AwMontageDial::addDroppedChannels(const QStringList& labels, int beginRow)
{
	AwChannelList montage = static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->currentMontage();
	for (auto l : labels) {
		auto asRecorded = m_hashAsRecorded[l];
		montage.insert(beginRow++, new AwChannel(asRecorded));
	}
	static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->updateMontage(montage);
}

void AwMontageDial::addChannelsByTypes()
{
	QAction *act = (QAction *)sender();
	if (act == NULL)
		return;

	int type = act->data().toInt();
	auto labels = m_labelsByTypes[type];

	AwChannelList channels;
	for (auto l : labels) {
		auto asRecorded = m_hashAsRecorded[l];
		if (asRecorded)
			if (!asRecorded->isBad())
				channels << new AwChannel(asRecorded);
	}
	if (!channels.isEmpty())
		static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->addChannels(channels);
}


void AwMontageDial::computeSEEGMontageFromEEGChannels()
{
	int res = QMessageBox::question(this, tr("EEG as SEEG channels"), tr("Transform all EEG channels to SEEG?"), 
		QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);
	if (res == QMessageBox::No)
		return;
	// Transform EEG into SEEG is As Recorded
	for (auto c : AwChannel::getChannelsOfType(m_asRecordedChannels, AwChannel::EEG)) 
			c->setType(AwChannel::SEEG);
	// change also current montage
	AwChannelList montage = static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->currentMontage();
	for (auto c : montage)
		if (c->isEEG())
			c->setType(AwChannel::SEEG);
	static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->updateMontage(montage);
}


void AwMontageDial::removeBadChannels(AwChannelList& channels)
{

	foreach(AwChannel *c, channels)
		if (c->isBad() || m_badChannelsLabels.contains(c->name()))
			channels.removeAll(c);
}

void AwMontageDial::makeECOGBipolar()
{
	auto channels = AwChannel::getChannelsOfType(m_asRecordedChannels, AwChannel::ECoG);
	removeBadChannels(channels);
	if (channels.isEmpty()) {
		AwMessageBox::information(this, tr("ECoG Bipolar"), tr("Could not make a bipolar montage (no ECoG channels in data file.)"));
		return;
	}
	auto response = AwMessageBox::question(this, tr("ECoG Bipolar"), "Add ECoG montage to the current montage?)", QMessageBox::Yes|QMessageBox::No);
	if (response == QMessageBox::No)
		return;
	// remove channels from montage
	// keep a copy of original montage
	AwChannelList dup = AwChannel::duplicateChannels(channels);
	AwECoGDialog dlg(dup);
	if (dlg.exec() == QDialog::Accepted) {
		// check for valid references
		for (auto c : dup) {
			if (c->hasReferences())
				if (m_badChannelsLabels.contains(c->referenceName()))
					c->clearRefName();
		}

		// check for remaining monopolar channels
		foreach(AwChannel *c, dup) {
			if (!c->hasReferences()) {
				dup.removeAll(c);
				delete c;
			}
		}
		AwChannel::sortByName(dup);
		// add to the current montage !
		auto montage = static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->currentMontage();
		montage += dup;
		static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->updateMontage(montage);
	}
	else {
		AW_DESTROY_LIST(dup);
	}
}

void AwMontageDial::setAVGRefMontage()
{
	auto montage = static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->currentMontage();
	for (auto c : montage) {
		if (!c->isEEG())
			continue;
		c->setReferenceName("AVG");
	}
	static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->updateMontage(montage);
}

void AwMontageDial::setSEEGAVGRefMontage()
{
	auto montage = static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->currentMontage();
	for (auto c : montage) {
		if (!c->isSEEG())
			continue;
		c->setReferenceName("AVG");
	}
	static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->updateMontage(montage);
}

void AwMontageDial::makeSEEGBipolar()
{
	auto montage = static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->currentMontage();

	QRegularExpression exp("(\\d+)$");
	QRegularExpression expZeros("^(0+)");
	QRegularExpressionMatch match, matchZero;
	for (auto c : montage) {
		if (!c->isSEEG())
			continue;
		match = exp.match(c->name());
		if (match.hasMatch()) {
			QString zeros;
			auto baseName = c->name();
			baseName = baseName.remove(exp);
			auto number = match.captured(1);
			// fing leading zeros
			matchZero = expZeros.match(number);
			if (matchZero.hasMatch()) {
				zeros = matchZero.captured(1);
				number = number.remove(expZeros);
			}
			int n = number.toInt() + 1;


			// find the reference without considering zeros in number
			QString ref = QString("%1%2").arg(baseName).arg(n);
			if (!m_hashAsRecorded.contains(ref))
				ref = QString("%1%2%3").arg(baseName).arg(zeros).arg(n);
			if (m_hashAsRecorded.contains(ref) && !m_badChannelsLabels.contains(ref))
				c->setReferenceName(ref);
				
		}
	}

	//// expect characters from a to z  AND _.'
	//QRegularExpression exp("([A-Z_.]+'?)(\\d+)$", QRegularExpression::CaseInsensitiveOption);
	//QRegularExpressionMatch match;
	//for (auto c : dup) {
	//	match = exp.match(c->name());
	//	if (match.hasMatch()) {
	//		auto elec = match.captured(1);
	//		auto elecCopy = elec;
	//		auto number = match.captured(2);
	//		// some electodes may have preceding zeros before pad number
	//		while (number.startsWith("0")) {
	//			elec += "0";
	//			number.remove(0, 1);
	//		}
	//		// next electrode could be weird 
	//		// example : X09 and X10 not X010
	//		// try to guess the possible reference
	//		bool refFound = false;
	//		auto nextNumber = number.toInt() + 1;
	//		auto ref = QString("%1%2").arg(elec).arg(nextNumber);
	//		if (m_hashAsRecorded.contains(ref))
	//			if (!m_badChannelsLabels.contains(ref))
	//				c->setReferenceName(ref);
	//		else {
	//			ref = QString("%1%2").arg(elecCopy).arg(nextNumber);
	//			if (m_hashAsRecorded.contains(ref))
	//				if (!m_badChannelsLabels.contains(ref))
	//					c->setReferenceName(ref);
	//		}
	//	}
	//}

	//// check for remaining monopolar channels
	//foreach (AwChannel *c,  dup) {
	//	if (!c->hasReferences()) {
	//		dup.removeAll(c);
	//		delete c;
	//	}
	//}
	//AwChannel::sortByName(dup);
	//static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->updateMontage(dup);

	auto response = AwMessageBox::question(this, "SEEG Bipolar", "Remove resting monopolar channels?", QMessageBox::Yes | QMessageBox::No);
	if (response == QMessageBox::Yes) {
		AwChannelList tmp;
		for (auto c : montage)
			if (c->isSEEG() && !c->hasReferences())
				tmp << c;
		for (auto c : tmp)
			montage.removeAll(c);
		AW_DESTROY_LIST(tmp);

	}

	static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->updateMontage(montage);
}

void AwMontageDial::unsetBadChannel(const QString& label)
{
	m_badChannelsLabels.removeAll(label);
	if (m_hashAsRecorded.contains(label))
		m_hashAsRecorded[label]->setBad(false);
}

//
// updateBadChannel()
// Browse the model to remove all channels that are marked as bad
void AwMontageDial::setBadChannel(const QString& electrodeLabel)
{
	AwChannelList montage = static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->currentMontage();
	AwChannelList channels;
	foreach(AwChannel *c, montage) {
		if (c->name() == electrodeLabel || c->referenceName() == electrodeLabel) {
			montage.removeAll(c);
			channels << c;
		}
	}
	static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->updateMontage(montage);
	while (!channels.isEmpty())
		delete channels.takeFirst();

	m_badChannelsLabels << electrodeLabel;
	if (m_hashAsRecorded.contains(electrodeLabel))
		m_hashAsRecorded[electrodeLabel]->setBad(true);
}


void AwMontageDial::updateChannelsType(const QStringList& labels, int type)
{
	AwChannelList channels = static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->currentMontage();
	for (auto c : channels) {
		if (labels.contains(c->name())) {
			c->setType(type);
			c->clearRefName();
			if (m_hashAsRecorded.contains(c->name()))
				m_hashAsRecorded[c->name()]->setType(type);
		}
	}
	static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->updateMontage(channels);
}

//
// accept()
// Do not accept the Dialog if the current montage is empty
void AwMontageDial::accept()
{
	AwChannelList channels = static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->currentMontage();
	if (channels.isEmpty()) {
		AwMessageBox::critical(this, tr("Montage"), tr("The montage is empty"), QMessageBox::Ok);
		return;
	}
	QDialog::accept();
}

void AwMontageDial::reject()
{
	static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->clearMontage();
	QDialog::reject();
}

//
// resetToAsRecorded()
//
void AwMontageDial::resetToAsRecorded()
{
	AwChannelList channels = static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->currentMontage();
	// Le montage actuel est-il vide ?
	if (!channels.isEmpty()) {
		int reponse = QMessageBox::warning(this, tr("Montage"),
			tr("Warning, reset the montage to default channels in data file?"),
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		if (reponse == QMessageBox::No)
			return;
	}
	static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->clearMontage();
	channels.clear();
	for (auto c : m_asRecordedChannels) {
		if (!c->isVirtual() && !c->isBad())
			channels << new AwChannel(c);
	}
	static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->setMontage(channels);
}


//
// cleanMontage()
//
void AwMontageDial::cleanMontage()
{
	AwChannelList channels = static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->currentMontage();
	// Le montage actuel est-il vide ?
	if (!channels.isEmpty())	{
		int reponse = QMessageBox::warning(0, tr("Montage"), 
			tr("Clear the current montage?"),
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		if (reponse == QMessageBox::No)
			return;
		static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->clearMontage();
	}
}

//
// addChannelsToMontage()
//
void AwMontageDial::addChannelsToMontage()
{
	QItemSelectionModel *selectModel = m_ui.tvChannelsAsRecorded->selectionModel();

	if (!selectModel->hasSelection())
		return;
	QModelIndexList selectedIndexes = selectModel->selectedIndexes();
	AwChannelList channels;

	QString name;
	for (auto i : selectedIndexes) {
		if (i.column() == AW_ASRECORDED_COLUMN_NAME) {
			name = m_ui.tvChannelsAsRecorded->model()->data(i, Qt::DisplayRole).toString();
			auto channel = m_hashAsRecorded[name];
			if (channel) {
				if (channel->isBad())
					continue;
				channels << channel->duplicate();
			}
		}
	}
	static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->addChannels(channels);
}

//
// removeChannelsInMontage()
//
// Efface les lignes sélectionnés dans le display setup.
// Ne fait rien si le display setup est vide.
void AwMontageDial::removeChannelsInMontage()
{
	QItemSelectionModel *selectModel = m_ui.tvDisplay->selectionModel();
	
	if (!selectModel->hasSelection())
		return;

	if (AwMessageBox::question(this, tr("Removing channels"), tr("Remove selected channels?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
		return;

	QModelIndexList selectedIndexes = selectModel->selectedIndexes();
	AwChannelList montage = static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->currentMontage();
	AwChannelList temp; // build a temporary list with channels to be deleted
	for (auto i : selectedIndexes) {
		if (i.column() == 0) {
			temp << montage.at(i.row());
		}	
	}
	for (auto c : temp)
		montage.removeAll(c);
	static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->updateMontage(montage);
	while (!temp.isEmpty())
		delete temp.takeFirst();
}


//
// save()
//
void AwMontageDial::save()
{
	AwChannelList channels = static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->currentMontage();
	if (channels.isEmpty())
		return;

	QString fileName = QFileDialog::getSaveFileName(0, tr("Save a Montage"), m_path, tr("Montage file (*.mtg)"));
	if (fileName.isNull())
		return;

	if (AwMontageManager::instance()->save(fileName, channels)) {
		AwMessageBox::information(this, "Montage", "Montage saved.");
		AwMontageManager::instance()->buildQuickMontagesList();	//ask montage manager to check for quick montages in AnyWave's montage dir and in current open data file directory.
	}
	else
		AwMessageBox::critical(this, "Montage", "Failed to save the montage.");
}

//
// createContextMenuAndActions()
// creer les menu contextuels disponibles dans l'interface de montage
// 
void AwMontageDial::createContextMenuAndActions()
{
	// Add a context menu for As Recorded TableView
	m_menuAsRecorded = new QMenu(m_ui.tvChannelsAsRecorded);
	QMenu *menuChangeType = m_menuAsRecorded->addMenu(tr("Change type"));

	for (auto type = 0; type < AW_CHANNEL_TYPES; type++) {
		auto action = menuChangeType->addAction(AwChannel::typeToString(type));
		action->setData(type);
		connect(action, &QAction::triggered, this, &AwMontageDial::changeAsRecordedType);
	}
	
	QAction *actMarkAsBad = new QAction(tr("Mark as bad"), m_ui.tvChannelsAsRecorded);
	m_menuAsRecorded->addAction(actMarkAsBad);
	connect(actMarkAsBad, SIGNAL(triggered()), this, SLOT(markAsRecordedAsBad()));

	m_contextMenuMontage = new QMenu(m_ui.tvDisplay);

	m_actApplyRefToAll = new QAction(tr("Apply reference to all"), m_ui.tvDisplay);
	m_actApplyRefToAll->setStatusTip(tr("Apply the same Reference to all channels"));
	connect(m_actApplyRefToAll, SIGNAL(triggered()), this, SLOT(contextMenuApplyRefToAll()));

	m_actApplyColorToAll = new QAction(tr("Apply color to all"), m_ui.tvDisplay);
	m_actApplyColorToAll->setStatusTip(tr("Apply the same color to all channels of the same type"));
	connect(m_actApplyColorToAll, SIGNAL(triggered()), this, SLOT(contextMenuApplyColorToAll()));

	m_actApplyColorToSelection = new QAction(tr("Apply color to the selection"), m_ui.tvDisplay);
	m_actApplyColorToSelection->setStatusTip(tr("Apply the same color to all selected channels"));
	connect(m_actApplyColorToSelection, SIGNAL(triggered()), this, SLOT(contextMenuApplyColorToSelection()));

	m_contextMenuMontage->addAction(m_actApplyColorToAll);
	m_contextMenuMontage->addAction(m_actApplyColorToSelection);
	m_contextMenuMontage->addSeparator();
	m_contextMenuMontage->addAction(m_actApplyRefToAll);
}


void AwMontageDial::contextMenuAsRecordedRequested(const QPoint& pos)
{
	QModelIndexList indexes = m_ui.tvChannelsAsRecorded->selectionModel()->selectedIndexes();
	if (indexes.isEmpty()) // no selection
		return;

	m_menuAsRecorded->exec(m_ui.tvChannelsAsRecorded->mapToGlobal(pos));
}

void AwMontageDial::markAsRecordedAsBad()
{
	// need a least one selection channel
	QModelIndexList indexes = m_ui.tvChannelsAsRecorded->selectionModel()->selectedIndexes();
	if (indexes.isEmpty()) // no selection
		return;
	QAction *action = qobject_cast<QAction *>(sender());

	if (action == NULL)
		return;


	for (auto i : indexes) {
		if (i.column() == AW_ASRECORDED_COLUMN_BAD)
			m_ui.tvChannelsAsRecorded->model()->setData(i, Qt::Checked, Qt::CheckStateRole);
	}
}

void AwMontageDial::changeAsRecordedType()
{
	// need a least one selection channel
	QModelIndexList indexes = m_ui.tvChannelsAsRecorded->selectionModel()->selectedIndexes();
	if (indexes.isEmpty()) // no selection
		return;

	QAction *action = qobject_cast<QAction *>(sender());

	if (action == NULL)
		return;

	// get new type from action
	int type = action->data().toInt();
	
	QStringList labels;
	for (auto i : indexes) {
		if (i.column() == AW_ASRECORDED_COLUMN_TYPE) {
			m_ui.tvChannelsAsRecorded->model()->setData(i, AwChannel::typeToString(type), Qt::EditRole);
		}
		else if (i.column() == AW_ASRECORDED_COLUMN_NAME)
			labels << m_ui.tvChannelsAsRecorded->model()->data(i, Qt::DisplayRole).toString();
	}
	updateChannelsType(labels, type);
	sortLabelsByTypes();	// rebuild list of channel labels by type
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Context Menu du widget tvDisplay (TableView affichant les montages)
/////////////////////////////////////////////////////////////////////////////////////////////////////////


void AwMontageDial::contextMenuMontageRequested(const QPoint& pos)
{
	AwChannelList channels = static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->currentMontage();
	QItemSelectionModel *selectModel = m_ui.tvDisplay->selectionModel();
	bool selection = m_ui.tvDisplay->selectionModel()->hasSelection();

	// Si montage vide on quitte
	if (channels.isEmpty())
		return;

	QModelIndex index = m_ui.tvDisplay->indexAt(pos);
	if (!index.isValid())
		return;
	m_actApplyColorToSelection->setVisible(selection && index.column() == AW_MONTAGE_COLUMN_COLOR);

	int type = channels.at(index.row())->type();
	if (index.column() == AW_MONTAGE_COLUMN_COLOR) {
		m_actApplyColorToAll->setText(QString(tr("Apply color to all %1 channels")).arg(AwChannel::typeToString(type)));
		m_actApplyColorToAll->setData(type);
	}
	else if (index.column() == AW_MONTAGE_COLUMN_REF) {
		m_actApplyRefToAll->setText(QString(tr("Apply ref to all %1 channels")).arg(AwChannel::typeToString(type)));
		m_actApplyRefToAll->setData(type);
	}
	m_contextMenuMontage->exec(m_ui.tvDisplay->mapToGlobal(pos));
}


//
// contextMenuApplyRefToAll
//
void AwMontageDial::contextMenuApplyRefToAll()
{
	AwChannelList channels = static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->currentMontage();

	// Si montage vide ou pas de selection => on quitte
	if (channels.isEmpty())
		return;

	QItemSelectionModel *selectModel = m_ui.tvDisplay->selectionModel();
	bool selection = m_ui.tvDisplay->selectionModel()->hasSelection();
	if (!selection)
		return;

	QAction *act = (QAction *)sender();
	if (act == NULL)
		return;


	int type = act->data().toInt();
	QString ref;
	for (auto i : selectModel->selectedIndexes()) {
		if (i.column() == AW_MONTAGE_COLUMN_REF) {
			ref = m_ui.tvDisplay->model()->data(i, Qt::DisplayRole).toString();
			break;
		}
	}
	for (auto c : channels) 
		if (c->type() == type)
			c->setReferenceName(ref);
}

//
// contextMenuApplyColorToAll()
//
void AwMontageDial::contextMenuApplyColorToAll()
{
	AwChannelList channels = static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->currentMontage();

	if (channels.isEmpty())
		return;

	QItemSelectionModel *selectModel = m_ui.tvDisplay->selectionModel();
	bool selection = m_ui.tvDisplay->selectionModel()->hasSelection();
	if (!selection)
		return;

	QAction *act = (QAction *)sender();
	if (act == NULL)
		return;

	int type = act->data().toInt();
	QString color;
	for (auto i : selectModel->selectedIndexes()) {
		if (i.column() == AW_MONTAGE_COLUMN_COLOR) {
			color = m_ui.tvDisplay->model()->data(i, Qt::DisplayRole).toString();
			break;
		}
	}
	for (auto c : channels)
		if (c->type() == type)
			c->setColor(color);
}

//
// contextMenuApplyColorToSelection()
//
void AwMontageDial::contextMenuApplyColorToSelection()
{
	AwChannelList channels = static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->currentMontage();
	QItemSelectionModel *selectModel = m_ui.tvDisplay->selectionModel();

	if (channels.isEmpty() || !selectModel->hasSelection())
		return;

	// Get the color of the first selected row.
	QString color;
	for (auto i : selectModel->selectedIndexes()) {
		if (i.column() == AW_MONTAGE_COLUMN_COLOR) {
			color = m_ui.tvDisplay->model()->data(i, Qt::DisplayRole).toString();
			break;
		}
	}
	// Apply it to the other selected rows
	for (auto i : selectModel->selectedIndexes()) {
		if (i.column() == AW_MONTAGE_COLUMN_COLOR) {
			m_ui.tvDisplay->model()->setData(i, color, Qt::EditRole);
		}
	}
}


void AwMontageDial::showColumn(bool flag)
{
	QCheckBox *cb = (QCheckBox *)sender();
	if (cb == NULL)
		return;
	if (cb == m_ui.checkColor) {
		m_ui.tvDisplay->setColumnHidden(AW_MONTAGE_COLUMN_COLOR, !flag);
	}
	else if (cb == m_ui.checkHP) {
		m_ui.tvDisplay->setColumnHidden(AW_MONTAGE_COLUMN_HPF, !flag);
	}
	else if (cb == m_ui.checkLP) {
		m_ui.tvDisplay->setColumnHidden(AW_MONTAGE_COLUMN_LPF, !flag);
	}
	else if (cb == m_ui.checkNotch) {
		m_ui.tvDisplay->setColumnHidden(AW_MONTAGE_COLUMN_NOTCH, !flag);
	}
}