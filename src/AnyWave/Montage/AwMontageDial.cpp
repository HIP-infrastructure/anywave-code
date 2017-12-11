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
#include <algorithm>
using namespace std;


///////////////////////////////////////////////////////
// compareChannels (using the name)
bool compareChannelsAlphaOrder(AwChannel *a, AwChannel *b)
{
	static QRegularExpression re("\\d+$");
	QString s1 = a->name(), s2 = b->name();
	QRegularExpressionMatch match1 = re.match(s1);
	QRegularExpressionMatch match2 = re.match(s2);
	bool m1 = match1.hasMatch();
	bool m2 = match2.hasMatch();
	QString elec1 = s1, elec2 = s2;
	if (m2 && m1) { // the electode got a terminating plot number, remove it
		elec1.remove(re);
		elec2.remove(re);

		// base name without plot number are not the same
		if (elec1 != elec2)
			return s1 < s2;

		int plot1, plot2;
		plot1 = match1.captured(0).toInt();
		plot2 = match2.captured(0).toInt();
		return plot1 < plot2;
	}
	return s1 < s2;
}

bool compareChannelsInverseAlpha(AwChannel *a, AwChannel *b)
{
	return !compareChannelsAlphaOrder(a, b);
}


///////////////////////////////////////////////////////////////////////////////
/// constructor
///
AwMontageDial::AwMontageDial(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	
	AwMontageManager *mm = AwMontageManager::instance();

	// copy as recorded channels as well
	for (auto c : mm->asRecordedChannels()) {
		AwChannel *chan = new AwChannel(c);
		m_channelsAsRecorded << chan;
		m_channelsMap.insert(chan->name(), chan);
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
	connect(m_ui.buttonSortName, &QPushButton::clicked, this, &AwMontageDial::sortNames);

	// sorting proxy for as recorded channels
	AsRecordedProxyModel *sortModelAsRecorded = new AsRecordedProxyModel(this);
	// Set the sort mode to UserRole, to give the possibility to sort the Bad column.
	sortModelAsRecorded->setSortRole(Qt::UserRole);
	AwChannelListModelAsRecorded *modelAsRecorded = new AwChannelListModelAsRecorded(m_channelsAsRecorded, this);
	connect(modelAsRecorded, &AwChannelListModelAsRecorded::badChannelSet, this, &AwMontageDial::setBadChannel);
	connect(modelAsRecorded, &AwChannelListModelAsRecorded::badChannelUnset, this, &AwMontageDial::unsetBadChannel);

	sortModelAsRecorded->setSourceModel(modelAsRecorded);
	AwChannelListModel *model = new AwChannelListModel(mm->channels(), this);
	connect(model, &AwChannelListModel::channelsDropped, this, &AwMontageDial::addDroppedChannels);
	m_ui.tvDisplay->setModel(model);
	m_ui.tvDisplay->setItemDelegate(new AwChannelListDelegate(m_labelTypes, this));

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

	connect(m_ui.buttonAutomaticMontage, SIGNAL(clicked()), this, SLOT(makeSEEGBipolar()));
	connect(m_ui.buttonAVG,  SIGNAL(clicked()), this, SLOT(setAVGRefMontage()));
	connect(m_ui.tvDisplay, SIGNAL(customContextMenuRequested(const QPoint& )), this, SLOT(contextMenuMontageRequested(const QPoint&)));
	connect(m_ui.tvChannelsAsRecorded, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(contextMenuAsRecordedRequested(const QPoint&)));
	connect(m_ui.buttonEEGSEEG, SIGNAL(clicked()), this, SLOT(computeSEEGMontageFromEEGChannels()));
}

AwMontageDial::~AwMontageDial()
{
	while (!m_channelsAsRecorded.isEmpty())
		delete m_channelsAsRecorded.takeFirst();
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
	if (channels.isEmpty())
		return;
	static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->setMontage(channels);
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


void AwMontageDial::sortNames()
{
	static bool alphaSorted = false;
	AwChannelList channels = static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->currentMontage();
	if (channels.isEmpty())
		return;
	if (!alphaSorted) {
		std::sort(channels.begin(), channels.end(), compareChannelsAlphaOrder);
		alphaSorted = true;
	}
	else {
		std::sort(channels.begin(), channels.end(), compareChannelsInverseAlpha);
		alphaSorted = false;
	}
	static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->updateMontage(channels);
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
	for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
		if (!m_labelTypes[i].isEmpty()) {
			act = m_addByTypesMenu.addAction(AwChannel::typeToString(i));
			act->setData(i);
			connect(act, &QAction::triggered, this, &AwMontageDial::addChannelsByTypes);
		}
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
	for (int i = 0; i < AW_CHANNEL_TYPES; i++)
		m_labelTypes[i].clear();
	for (auto c : m_channelsAsRecorded) {
		if (!c->isBad()) 
			m_labelTypes[c->type()].append(c->name());
	}
	updateButtonAddByTypes();
}


void AwMontageDial::addDroppedChannels(const QStringList& labels, int beginRow)
{
	AwChannelList montage = static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->currentMontage();
	for (auto l : labels) {
		AwChannel *asRecorded = m_channelsMap.value(l);
		montage.insert(beginRow++, new AwChannel(asRecorded));
	}
	static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->updateMontage(montage);
}

void AwMontageDial::addChannelsByTypes()
{
	QAction *act = (QAction *)sender();
	if (act == NULL)
		return;

	QStringList labels = m_labelTypes[act->data().toInt()];
	AwChannelList channels;
	for (auto l : labels) {
		AwChannel *asRecorded = m_channelsMap.value(l);
		if (asRecorded)
			channels << new AwChannel(asRecorded);
	}
	if (!channels.isEmpty())
		static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->addChannels(channels);
}


void AwMontageDial::setAVGRefMontage()
{
	AwChannelList channels = static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->currentMontage();
	for (auto c : channels) {
		if (c->isEEG())
			c->setReferenceName("AVG");
	}
	static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->updateMontage(channels);
}

void AwMontageDial::computeSEEGMontageFromEEGChannels()
{
	int res = QMessageBox::question(this, tr("EEG as SEEG channels"), tr("Consider EEG channels as SEEG and make them as bipolar?"), 
		QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);
	if (res == QMessageBox::No)
		return;

	for (auto c : m_channelsAsRecorded) {
		if (c->isEEG())
			c->setType(AwChannel::SEEG);
	}
	sortLabelsByTypes();
	// Get current montage , duplicate it, remove EEG channels, make them SEEG with references
	AwChannelList channels = AwChannel::duplicateChannels(static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->currentMontage());
	foreach(AwChannel *c, channels) {
		if (c->isEEG()) {
			channels.removeAll(c);
			delete c;
		}
	}
	for (auto c : m_channelsAsRecorded) {
		if (c->isEEG())
			channels.append(new AwChannel(c));
	}
	static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->setMontage(channels);
	makeSEEGBipolar();
	while (!channels.isEmpty())
		delete channels.takeFirst();
}


void AwMontageDial::makeSEEGBipolar()
{
	AwChannelList montage = static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->currentMontage();
	AwChannelList seegChannels = AwChannel::getChannelsOfType(montage, AwChannel::SEEG);
	if (seegChannels.isEmpty()) {
		AwMessageBox::information(this, tr("SEEG Bipolar"), tr("Could not make a bipolar montage (no SEEG channels in the current montage)"));
		return;
	}
	AwChannelList temp;
	QRegularExpression exp("([A-Z]+'?)(\\d+)$", QRegularExpression::CaseInsensitiveOption);
	QRegularExpressionMatch match;
	bool monopolarLeft = false;
	for (int i = 0; i < seegChannels.size(); i++) {
		AwChannel *seegChan = seegChannels.at(i);
		match = exp.match(seegChan->name());
		if (match.hasMatch()) {
			QString elec = match.captured(1);
			QString number = match.captured(2);
			QString ref = QString("%1%2").arg(elec).arg(number.toInt() + 1);
			// check that the ref electrode exists and is a SEEG electrode.
			AwChannel *c = m_channelsMap.value(ref);
			bool exist = c != NULL;
			if (!exist) {
				monopolarLeft = true;
				montage.removeAll(seegChan);
				temp << seegChan;
				continue;
			}
			// be sure ref exists is SEEG and is not bad
			if (!c->isBad() && m_labelTypes[AwChannel::SEEG].contains(ref))
				seegChan->setReferenceName(ref);
			else {
				monopolarLeft = true;
				montage.removeAll(seegChan);
				temp << seegChan;
			}
		}
	}
	static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->updateMontage(montage);
	if (monopolarLeft) 
		AwMessageBox::information(this, tr("SEEG Bipolar"), tr("Some SEEG monopolar channels have been removed from the montage"));
	while (!temp.isEmpty())
		delete temp.takeFirst();
}

void AwMontageDial::unsetBadChannel(const QString& label)
{
	m_badChannelsLabels.removeAll(label);
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
}


void AwMontageDial::updateChannelsType(const QStringList& labels, int type)
{
	AwChannelList channels = static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->currentMontage();
	for (auto c : channels) {
		if (labels.contains(c->name())) {
			c->setType(type);
			c->clearRefName();
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
	// Reset the montage, whit no virtual channels
	channels.clear();
	for (auto c : m_channelsAsRecorded)
		if (!c->isVirtual())
			channels << new AwChannel(c);
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
			AwChannel *channel = m_channelsMap.value(name);
			if (channel) {
				if (channel->isBad())
					continue;
				channels << new AwChannel(channel);
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

	QAction *actChangeEEG = new QAction(AwChannel::typeToString(AwChannel::EEG), menuChangeType);
	actChangeEEG->setData(AwChannel::EEG);
	menuChangeType->addAction(actChangeEEG);
	connect(actChangeEEG, SIGNAL(triggered()), this, SLOT(changeAsRecordedType()));

	QAction *actChangeSEEG = new QAction(AwChannel::typeToString(AwChannel::SEEG), menuChangeType);
	actChangeSEEG->setData(AwChannel::SEEG);
	menuChangeType->addAction(actChangeSEEG);
	connect(actChangeSEEG, SIGNAL(triggered()), this, SLOT(changeAsRecordedType()));

	QAction *actChangeMEG = new QAction(AwChannel::typeToString(AwChannel::MEG), menuChangeType);
	actChangeMEG->setData(AwChannel::MEG);
	menuChangeType->addAction(actChangeMEG);
	connect(actChangeMEG, SIGNAL(triggered()), this, SLOT(changeAsRecordedType()));

	QAction *actChangeECG = new QAction(AwChannel::typeToString(AwChannel::ECG), menuChangeType);
	actChangeECG->setData(AwChannel::ECG);
	menuChangeType->addAction(actChangeECG);
	connect(actChangeECG, SIGNAL(triggered()), this, SLOT(changeAsRecordedType()));

	QAction *actChangeEMG = new QAction(AwChannel::typeToString(AwChannel::EMG), menuChangeType);
	actChangeEMG->setData(AwChannel::EMG);
	menuChangeType->addAction(actChangeEMG);
	connect(actChangeEMG, SIGNAL(triggered()), this, SLOT(changeAsRecordedType()));

	QAction *actChangeReference = new QAction(AwChannel::typeToString(AwChannel::Reference), menuChangeType);
	actChangeReference->setData(AwChannel::Reference);
	menuChangeType->addAction(actChangeReference);
	connect(actChangeReference, SIGNAL(triggered()), this, SLOT(changeAsRecordedType()));

	QAction *actChangeTrigger = new QAction(AwChannel::typeToString(AwChannel::Trigger), menuChangeType);
	actChangeTrigger->setData(AwChannel::Trigger);
	menuChangeType->addAction(actChangeTrigger);
	connect(actChangeTrigger, SIGNAL(triggered()), this, SLOT(changeAsRecordedType()));

	QAction *actChangeOther = new QAction(AwChannel::typeToString(AwChannel::Other), menuChangeType);
	actChangeOther->setData(AwChannel::Other);
	menuChangeType->addAction(actChangeOther);
	connect(actChangeOther, SIGNAL(triggered()), this, SLOT(changeAsRecordedType()));
	
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