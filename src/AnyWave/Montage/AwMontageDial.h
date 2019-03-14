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

	/** Overload accept() **/

	void initMontageList();
	/** Sauvegarde un montage dans le chemin spécifié. **/
	bool saveMontage(const QString& path);
	AwChannelList channels() { return static_cast<AwChannelListModel *>(m_ui.tvDisplay->model())->currentMontage(); }
	AwChannelList asRecordedChannels() { return m_channelsAsRecorded; }
	inline QStringList badLabels() { return m_badChannelsLabels; }
//	AwChannel *asRecordedChannel(const QString& name) { return m_channelHashTable.value(name); }
protected:
	void closeEvent(QCloseEvent *e);
public slots:
	void accept();
	void reject();
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
	//void addChannelsWithType(int type);
	void computeSEEGMontageFromEEGChannels();

private slots:
	void addChannelsByTypes();
	void moveUp();
	void moveDown();
	/* Sort the current montage by names (alphabeticaly) */
	void sortNames();
	void addDroppedChannels(const QStringList& labels, int beginRow);

	void loadMontage();
	
	/** Browse the current montage for SEEG channels and make them bipolar **/
	void makeSEEGBipolar();
	/** Create a grid or a strip for ECOG channels **/
	void makeECOGBipolar();

	/** Set AVG references on compatible channels **/
	void setAVGRefMontage();

	void setBadChannel(const QString& electrodeLabel);
	void unsetBadChannel(const QString& electrode);
	//void updateChannelType(QString& electrodeLabel, int newType);
	void updateChannelsType(const QStringList& labels, int type);

	void contextMenuMontageRequested(const QPoint& pos);
	void contextMenuAsRecordedRequested(const QPoint& pos);

	void changeAsRecordedType();
	void markAsRecordedAsBad();

	void contextMenuApplyRefToAll();
	void contextMenuApplyColorToAll();
	void contextMenuApplyColorToSelection();
	
private:
	Ui::MontageDialClass m_ui;
	QMap<QString, AwChannel *> m_channelsMap;

	///< copy of the list from Montage Manager.
	QList<AwChannel *> m_channelsAsRecorded;

	QStringList m_badChannelsLabels;					/// store the labels of bad channels
	QStringList m_labelTypes[AW_CHANNEL_TYPES];			/// store the labels for each types of channels

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
	bool channelsLessThan(const AwChannel*& s1, const AwChannel*& s2);
};

#endif // MONTAGEDIAL_H
