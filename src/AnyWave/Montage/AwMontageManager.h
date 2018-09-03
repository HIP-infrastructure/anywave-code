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

#ifndef AW_MONTAGE_H
#define AW_MONTAGE_H

#include <AwChannel.h>
class AwFileIO;
class AwAVGChannel;
#include <QMutex>
#include "ICA/AwICAChannel.h"
#include "Source/AwSourceChannel.h"
#include <filter/AwFilterSettings.h>

/////////////////////////////////////////////////////////////////////////////////
/// sorting function for AwChannel using labels
/// When sorting we want A2 to come before A10 for example.
bool channelLabelLessThan(AwChannel *c1, AwChannel *c2);
bool compareSEEGLabels(const QString& s1, const QString& s2);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// CustomReference
///
/// objet qui défini une reference definie par l'utilisateur.
/// la reference a un nom et une liste de labels d'electrodes.
/// L'utilisateur choisi un nom d'électrode et défini quelles électrodes réelles elle regroupe pour former une référence.
class CustomReference
{
public:
	/** Constructeur avec nom de référence **/
	CustomReference(QString referenceName) { m_name = referenceName; }
	/** Destructeur **/
	~CustomReference() {}
	inline QString& name() { return m_name; }
	inline QStringList& references() { return m_references; }
protected:
	QString m_name;				///< Nom de la référence personnalisée (par exemple MaRéférenceAMoi)
	QStringList m_references;		///< Listes des noms d'éléctrodes formant la référence personnalisée. Ex: (O1, O2, Cz).
};

///
/// MontageManager
///
/// Cet objet gère le montage et les paramètres d'affichage des canaux.
/// Il a sa propre interface utilisateur, qu'il gère également.
class AwMontageManager : public QObject
{
	Q_OBJECT

public:
	AwMontageManager();
	~AwMontageManager();

	/** Retourne la liste des objets AwChannel du montage courant **/
	AwChannelList& channels() { return m_channels;}
	/** Retourne la liste des AwChannels AsRecorded **/
	AwChannelList& asRecordedChannels() { return m_channelsAsRecorded; }
	/** Get Hash Table to get as recorded channels by their label. **/
	QHash<QString, AwChannel*> hashTable() { return m_channelHashTable; }
	/** Return a pointer to the as recorded channel with given name **/
	AwChannel * asRecordedChannel(const QString& name) { return m_channelHashTable.value(name); }
	/** Retourne le premier objet AwChannel trouvé du type spéficié **/
	AwChannel *containsChannelOfType(AwChannel::ChannelType t);  // Renvoie le pointeur vers le premier AwChannel de type t trouvé ou Zero si non trouvé	
	/** Get channels from current montage that matches the label. **/
	AwChannelList channelsWithLabel(const QString& label);
	/** Get channels from current montage that matches all the labels in labels **/
	AwChannelList channelsWithLabels(const QStringList& labels);
	/** Get the labels of current channels in montage **/
	QStringList labels();

	/** Get instance pointer **/
	static AwMontageManager *instance();
	/** Compute virtual channel AVG for MEG, EEG or SEEG **/
	static float *computeAVG(AwChannel *chan);
	/** Convert EEG channels to SEEG and make a bipolar montage **/
	static AwChannelList makeSEEGBipolar(AwChannelList& channels);
	/** Remise à zero **/
	void closeFile();
	/** Quit **/
	void quit();
	/** Get user montages found in /Montages **/
	inline QStringList& quickMontages() { return m_quickMontages; }
	inline QStringList& localQuickMontages() { return m_localQuickMontages; }
	/** Browse for .mtg in a directory and add found montage to quick montages. **/
	void scanForMontagesInDirectory(const QString& dir);
	/** Load a montage file picked by user. **/
	bool apply(const QString& path);
	/** Load a montage file and returns the corresponding channels without applying the montage. **/
	AwChannelList load(const QString& path);
	/** Save a montage referenced by a name. **/
	bool saveMontage(const QString& name);
	/** Save current montage to specified fileName **/
	bool save(const QString& fileName);
	/** Save a list of channels as montage. **/
	bool save(const QString& fileName, const AwChannelList& channels);
	/** Get path to user created montages **/
	inline QString& path() { return m_path; }
	/** Get labels of sensors marked as bad. **/
	inline QStringList& badLabels() { return m_badChannelLabels; }
	/** Remove bad channels in list **/
	void removeBadChannels(AwChannelList& list);
	/** Scripting support specific **/
	AwChannelList loadAndApplyMontage(AwChannelList asRecorded, const QString& path);
	/** Scripting support specific **/
	void newMontage(AwFileIO *reader);
	// ICA Channels specific
	/** import ICA Components **/
	int loadICA();
	int loadICA(const QString& path);

signals:
	/** Signal émis lorsque un nouveau montage a ete calcule. La liste des canaux montés est passée en paramètre. **/
	void montageChanged(const AwChannelList &channels);
	/** Signal sent every time a montage is saved or a data file is open. **/
	void quickMontagesUpdated();
	void badChannelsSet(const QStringList& labels);
public slots:
	/** Affiche l'interface de montage. **/
	void showInterface();
	void saveCurrentMontage();

	/** Load a montage previously saved by user. **/
	void loadQuickMontage(const QString& name);
	bool loadMontage(const QString& path);
	void buildQuickMontagesList();
	/** Add channels to montage **/
	void addChannelsByName(AwChannelList& channels);
	///** Clear current montage and build a new one based on channels names passed as parameter **/
	//void buildNewMontageFomNames(const QStringList& names);
	/** Build a new montage based on a list of channels **/
	void buildNewMontageFromChannels(const AwChannelList& channels);
	void setNewFilters(const AwFilterSettings& settings);
	/** Ajout d'un canal aux canals AsRecorded. Typiquement utilise pour ajouter un canal de source Virtual **/
	void addChannelToAsRecorded(AwChannel *channel);
	/** Mark a channel as bad or not bad. Update montage if necessary **/
	void markChannelAsBad(const QString& channelName, bool bad = true);
	void markChannelsAsBad(const QStringList& labels);
	/** Update channels list, typically from MontageUi **/
	void setChannels(AwChannelList& channels) { m_channels = channels; }
	/** Add source channels into current montage **/
	void addNewSources(int type);
private:
	QHash<QString, AwChannel*> m_channelHashTable;	///< Hash table permettant de retrouver un pointeur vers un objet AwChannel à partir de son label
	AwChannelList m_channels;						///< Liste des canaux choisi dans le montage et envoyés à l'affichage.
	AwChannelList m_channelsAsRecorded;				///< Liste des canaux lus par le plugins (les canaux asrecorded dans le fichier)
	QList<CustomReference *> m_customReferences;	///< Une liste d'objet CustomReference (pour les références personnalisées).

	// ICA/Source specific
	AwICAChannelList m_icaAsRecorded;
	AwSourceChannelList m_sourceAsRecorded;
	QHash<QString, AwICAChannel *> m_icaHashTable;
	QHash<QString, AwSourceChannel *> m_sourceHashTable;

	QString m_path;									///< Chemin par défaut pour stocker les montages 
	QHash<QString, QString> m_quickMontagesHT;		///< Hash table pour stocker des paires "nom de montage/fichier".
	QHash<QString, QString> m_localQuickMontagesHT;	///< Handle montage files that could be present along with the data file.
	QStringList m_quickMontages;					///< Names of quick montages available sorted alphabetically.
	QStringList m_localQuickMontages;
	QStringList m_badChannelLabels;					///< updated list of sensors marked as bad.
	QMutex m_mutex;									// Mutex for scripting context
	QString m_montagePath;							// Path to montage file based on the data file name.
	QString m_badPath;								// Path to bad channels file.

	void saveBadChannels();
	void loadBadChannels();
	void scanForPrebuiltMontages();
	void clear();									///> clear current montage;
	void applyGains();
	static AwMontageManager *m_instance;
	// Source/ICA specific
	void clearICA();
	void clearSource(int type);
};


#endif
