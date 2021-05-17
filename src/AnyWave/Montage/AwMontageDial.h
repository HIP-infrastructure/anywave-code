// AnyWave
// Copyright (C) 2013-2021  INS UMR 1106
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#ifndef AW_MONTAGEDIAL_H
#define AW_MONTAGEDIAL_H

#include <QDialog>
#include "ui_AwMontageDial.h"
#include <QHeaderView>
#include <QAbstractTableModel>
#include <QModelIndex>
#include <QItemDelegate>
#include <QComboBox>
#include <QMenu>
#include <QListWidgetItem>
#include <QSortFilterProxyModel>
#include <AwChannel.h>

using namespace Ui;

// sort function for bipolar montage. 
bool compareSEEGLabels(const QString&, const QString&);

class AwMontageDial : public QDialog
{
	Q_OBJECT

public:
	AwMontageDial(QWidget *parent = 0);
	~AwMontageDial();


	void initMontageList();
	/** Sauvegarde un montage dans le chemin spécifié. **/
	bool saveMontage(const QString& path);
	AwChannelList channels() { return static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->currentMontage(); }
//	AwChannelList asRecordedChannels() { return m_asRecorded.values(); }
	AwChannelList& asRecordedChannels() { return m_asRecordedChannels; }
	inline QStringList badLabels() { return m_badChannelsLabels; }
protected:
	void closeEvent(QCloseEvent *e);
public slots:
	void accept() override;
	void reject() override;
	/** Détruit le montage actuel **/
	void cleanMontage();
	/** Restaure le montage As Recorded, c'est-à-dire le montage lu dans le fichier de donnés. (le plus souvent il s'agit des canaux monopolaires du fichier) **/
	void resetToAsRecorded();
	/** Sauvegarde du montage. Ouvre une boite de dialogue et met à jour la liste des montages définis par l'utilisateur. **/
	void save();
	/** Rajoute les canaux sélectionnés dans la liste de gauche au montage. **/
	void addChannelsToMontage();
	/** Efface les canaux actuellement sélectionnés dans la liste de montage. **/
	void removeChannelsInMontage();
	///** Add All Channels of specified type to Montage  **/
	void computeSEEGMontageFromEEGChannels();

private slots:
	void showColumn(bool flag);
	void addChannelsByTypes();
	void moveUp();
	void moveDown();

	void addDroppedChannels(const QStringList& labels, int beginRow);

	void loadMontage();
	
	/** Browse the current montage for SEEG channels and make them bipolar **/
	void makeSEEGBipolar();
	/** Create a grid or a strip for ECOG channels **/
	void makeECOGBipolar();

	/** Set AVG references on compatible channels **/
	void setAVGRefMontage();
	void setSEEGAVGRefMontage();

	void setBadChannel(const QString& electrodeLabel);
	void unsetBadChannel(const QString& electrode);
	void updateChannelsType(const QStringList& labels, int type);

	void contextMenuMontageRequested(const QPoint& pos);
	void contextMenuAsRecordedRequested(const QPoint& pos);

	void changeAsRecordedType();
	void markAsRecordedAsBad();

	void contextMenuApplyRefToAll();
	void contextMenuApplyColorToAll();
	void contextMenuApplyColorToSelection();
	void sortMontageByName();
	void sortMontageByType();
	
private:
	Ui::MontageDialClass m_ui;
//	QHash<QString, AwChannel *> m_asRecorded;	
	QHash<QString, AwChannel *> m_hashAsRecorded;
	AwChannelList m_asRecordedChannels;
	///< copy of the list from Montage Manager.

	QStringList m_badChannelsLabels;					/// store the labels of bad channels
	QHash<int, QStringList> m_labelsByTypes;

	QString m_path;
	QMenu *m_contextMenuMontage;					///< Menu contextuel de la QTableView des montages.
	QMenu m_addByTypesMenu;
	QMenu *m_menuAsRecorded;
	QAction *m_actRemoveSelected;					// Efface les canaux sélectionnés
	QAction *m_actRemoveAll;						// Efface tous les canaux.
	QAction *m_actApplyRefToAll;					//  Applique la reference d'un canal à tous les autres.
	QAction *m_actAutomaticMontage;
	QAction *m_actApplyColorToAll;
	QAction *m_actApplyColorToSelection;

	void sortLabelsByTypes();
	void updateButtonAddByTypes();
	void createContextMenuAndActions();
	/** remove bad channels directly in the specified list. **/
	void removeBadChannels(AwChannelList& channels);
};

#endif // MONTAGEDIAL_H
