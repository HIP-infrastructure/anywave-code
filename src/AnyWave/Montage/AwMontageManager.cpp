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

#include "AwMontageManager.h"
#include "AwMontageDial.h"
#include "Prefs/AwSettings.h"
#include <widget/AwMessageBox.h>
#include <QtCore>
#include <QFileDialog>
#include "ICA/AwICAManager.h"
#include "ICA/AwICAComponents.h"
#include "ICA/AwICAChannel.h"
#include "Source/AwSourceManager.h"
#include "Source/AwSourceChannel.h"
#include "AwAVGChannel.h"
#include <widget/AwWait.h>
#include <AwAmplitudeManager.h>
#include "IO/BIDS/AwBIDSManager.h"
#include <AwException.h>
#include <montage/AwMontage.h>
#include <AwCore.h>
#include <AwFileInfo.h>
#include "IO/BIDS/AwBIDSManager.h"
#include "Debug/AwDebugLog.h"
// statics init and definitions
AwMontageManager *AwMontageManager::m_instance = 0;


AwMontageManager *AwMontageManager::instance()
{
	if (!m_instance) 
		m_instance = new AwMontageManager;
	return m_instance;
}

QStringList AwMontageManager::loadBad(const QString& filePath)
{
	QFile file(filePath);
	QTextStream stream(&file);
	QStringList res;
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		while (!stream.atEnd()) 
			res << stream.readLine().trimmed();
		file.close();
	}
	return res;
}

/** transform any EEG into SEEG. Make SEEG bipolar montage. returns SEEG channels (monopolar channels may be present). **/
AwChannelList AwMontageManager::makeSEEGBipolar(AwChannelList& channels)
{
	QHash<QString, AwChannel *> hash;
	auto eegChannels = AwChannel::getChannelsOfType(channels, AwChannel::EEG);
	// convert them to SEEG
	for (auto c : eegChannels) {
		c->setType(AwChannel::SEEG);
		hash[c->name()] = c;
	}
	QRegularExpression exp("([A-Z]+'?)(\\d+)$", QRegularExpression::CaseInsensitiveOption);
	QRegularExpressionMatch match;

	for (auto c : eegChannels) {
		match = exp.match(c->name());
		if (match.hasMatch()) {
			auto elec = match.captured(1);
			auto number = match.captured(2);
			// some electodes may have preceding zeros before pad number
			while (number.startsWith("0")) {
				elec += "0";
				number.remove(0, 1);
			}
			auto ref = QString("%1%2").arg(elec).arg(number.toInt() + 1);
			// check if ref exists in asRecorded.
			if (hash.contains(ref))
				// build the ref
				c->setReferenceName(ref);
		}
	}
	return eegChannels;
}

//end of statics


QHash<QString, AwChannel *> AwMontageManager::cloneAsRecordedChannels()
{
	QHash<QString, AwChannel *> res;
	auto channels = m_asRecorded.values();
	for (auto c : channels)
		if (c != nullptr)
			res[c->name()] = c->duplicate();
	return res;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////::
// Source/ICA specific

void AwMontageManager::clearICA()
{
	// remove previous ICA channels from asRecorded
	// DO NOT DELETE channels as they belong to a AwComponent instance
	for (auto c : m_icaAsRecorded)
		m_asRecorded.remove(c->name());
	m_icaAsRecorded.clear();

	// remove ICA channels from current montage
	foreach(AwChannel *c, m_channels)
		if (c->isICA())	{
			m_channels.removeAll(c);
			delete c;
		}
}

void AwMontageManager::clearSource(int type)
{
	// remove Source of type 'type' from channels from asRecorded list
	foreach (AwSourceChannel *c, m_sourceAsRecorded) {
		if (c->type() == type) {
			m_asRecorded.remove(c->name());
			m_sourceAsRecorded.removeAll(c);
		}
	}

	// remove Source channels of type 'type' from current montage
	foreach (AwChannel *c, m_channels)
		if (c->isSource() && c->type() == type)	{
			m_channels.removeAll(c);
			delete c;
		}
}

void AwMontageManager::addNewSources(int type)
{
	AwSourceManager *sm = AwSourceManager::instance();
	clearSource(type);
	AwSourceChannelList channels = sm->sources(type);
	for (auto c : channels) {
		AwSourceChannel *source = new AwSourceChannel(c);
		source->setGain(AwAmplitudeManager::instance()->amplitude(AwChannel::Source));
		m_sourceAsRecorded << c;
		m_channels << source;
		m_asRecorded[source->name()] = source;
	}
	emit montageChanged(m_channels);
	AwMessageBox::information(0, tr("Source channels"), QString("%1 source channels added to the current montage.").arg(channels.size()));
	AwSettings::getInstance()->filterSettings().setBounds(type, sm->hp(type), sm->lp(type));
}

int AwMontageManager::loadICA()
{
	// default dir path = the directory containing the data file
	QString dir = AwSettings::getInstance()->fileInfo()->dirPath();
	// check if we are in a BIDS and then check for derivatives ica folder
	if (AwBIDSManager::isInstantiated()) {
		auto bm = AwBIDSManager::instance();
		if (bm->isBIDSActive()) {
			auto icaDir = bm->getDerivativePath(AwBIDSItem::ica);
			if (!icaDir.isEmpty())
				dir = icaDir;
		}
	}
	QString filter("ICA matrices (*.mat *.h5)");
	QString path;
#ifdef Q_OS_MAC
    QFileDialog dlg(0, "Load ICA matrices", dir);
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    dlg.setViewMode(QFileDialog::Detail);
    dlg.setFileMode(QFileDialog::ExistingFile);
    if (dlg.exec() == QDialog::Accepted)
        path = dlg.selectedFiles().at(0);
#else
    path = QFileDialog::getOpenFileName(0, tr("Add ICA components"), dir, filter);
#endif
	if (path.isEmpty())
		return -1;
	return loadICA(path);
}

int AwMontageManager::loadICA(const QString& path)
{
	clearICA();
	AwICAManager *ica_man = AwICAManager::instance();
	int count = 0;
	try {
		ica_man->loadComponents(path);
	}
	catch (const AwException& e) {
		AwMessageBox::critical(0, tr("ICA Components"),QString(tr("Failed to import ICA components: %1")).arg(e.errorString()));
		return -1;
	}
	// add ICA components to asRecorded and current montage.
	AwICAComponents **comps = ica_man->getAllComponents();
	for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
		if (comps[i] == NULL)
			continue;
		count += comps[i]->components().size();
		foreach(AwICAChannel *channel, comps[i]->components()) {
			m_icaAsRecorded << channel;
			AwICAChannel *newChan = new AwICAChannel(channel);
			newChan->setGain(AwAmplitudeManager::instance()->amplitude(AwChannel::ICA));
			m_channels << newChan;
			// add ica to as recorded
			m_asRecorded[channel->name()] = channel;
		}
		AwSettings::getInstance()->filterSettings().setFilterBounds(AwChannel::ICA, AwFilterBounds(comps[i]->type(), comps[i]->hpFilter(), comps[i]->lpFilter()));
	}

	emit montageChanged(m_channels);
	AwMessageBox::information(0, tr("ICA Components"), QString(tr("%1 components added to the current montage.").arg(count)));
	return 0;
}

// constructor
AwMontageManager::AwMontageManager()
{
	QStringList filter("*.mtg");
	AwSettings *aws = AwSettings::getInstance();

	m_path = aws->value(aws::montage_dir).toString();
	if (m_path.isEmpty())
		return;

	QDir montDir(m_path);
	montDir.setNameFilters(filter);
	QStringList files = montDir.entryList();

	// init quick montages
	for (int i = 0; i < files.count(); i++)	{
		QString key = files.value(i);
		key = key.remove(".mtg", Qt::CaseInsensitive);
		m_quickMontages << key;
		m_quickMontagesHT.insert(key, m_path + "/" + files.value(i));
	}
	std::sort(m_quickMontages.begin(), m_quickMontages.end());

	// connect to filter settings
	connect(&aws->filterSettings(), &AwFilterSettings::settingsChanged, this, &AwMontageManager::setNewFilters);

	AwDebugLog::instance()->connectComponent("Montage Manager", this);
}

// destructeur
AwMontageManager::~AwMontageManager()
{
}

void AwMontageManager::scanForMontagesInDirectory(const QString& dir)
{
	QStringList filter("*.mtg");
	QDir montDir(dir);
	montDir.setNameFilters(filter);
	QStringList files = montDir.entryList();

	m_localQuickMontages.clear();
	m_localQuickMontagesHT.clear();

	for (auto file : files) {
		auto key = file.remove(".mtg");
		m_localQuickMontages << key;
		m_localQuickMontagesHT.insert(key, QString("%1/%2").arg(dir).arg(file));
	}
	// if file is an SEEG data file in a BIDS, check for GARDEL generated montages.
	if (AwBIDSManager::isInstantiated()) {
		auto bm = AwBIDSManager::instance();
		if (bm->isBIDSActive()) {
			auto montages = bm->getGardelMontages();
			for (auto montage : montages) {
				QFileInfo fi(montage);
				auto key = fi.fileName().remove(".mtg");
				m_localQuickMontages << key;
				m_localQuickMontagesHT.insert(key, montage);
			}
		}
	}
	std::sort(m_localQuickMontages.begin(), m_localQuickMontages.end());
	emit quickMontagesUpdated();
}

void AwMontageManager::scanForPrebuiltMontages()
{
	QStringList filter("*.mtg");
#ifdef Q_OS_MAC
	QDir montDir(qApp->applicationDirPath() + "/../montages/");
	QString path = qApp->applicationDirPath() + "/../montages/";
#else
	QDir montDir(qApp->applicationDirPath() + "/montages/");
	QString path = qApp->applicationDirPath() + "/montages/";
#endif
	montDir.setNameFilters(filter);
	QStringList files = montDir.entryList();


	// init hash table
	for (int i = 0; i < files.count(); i++)	{
		QString key = files.value(i);
		key = key.remove(".mtg", Qt::CaseInsensitive);
		m_quickMontages << key;
		m_quickMontagesHT.insert(key, path + key);
	}

	qSort(m_quickMontages.begin(), m_quickMontages.end());
	emit quickMontagesUpdated();
}

AwChannelList AwMontageManager::channelsWithLabels(const QStringList& labels)
{
	AwChannelList channels;
	foreach (QString label, labels) 
		channels += channelsWithLabel(label);

	return channels;
}

AwChannelList AwMontageManager::channelsWithLabel(const QString& label)
{
	AwChannelList list;

	for (auto c : m_channels) {
		if (c->name() == label) {
			if (c->isICA()) {
				for (auto ica_chan : m_icaAsRecorded) {
					if (ica_chan->name() == label) {
						list << new AwICAChannel(ica_chan);
						break;
					}
				}
			}
			else if (c->isVirtual()) {
				AwVirtualChannel *vc = static_cast<AwVirtualChannel *>(c);
				list << new AwVirtualChannel(vc);
			}
			else
				list << new AwChannel(c);
		}
	}
	return list;
}

QStringList AwMontageManager::labels()
{
	QStringList list;
	for (auto c : m_channels)
		if (!list.contains(c->name()))
			list << c->name();
	return list;
}

//
// containsChannelOfType(t)
//
// Retourne le premier canal de type t present dans la liste des canaux AsRecorded.
// Retourne NULL si aucun canal trouvé.
bool AwMontageManager::containsChannelOfType(AwChannel::ChannelType t)
{
	auto channels = m_asRecorded.values();
	for (auto c : channels)
		if (c->type() == t)
			return true;
	return false;
}

/// Remove Bad Channels
/// Remove channel marked as bad in the list.
/// Modify the list directly.
void AwMontageManager::removeBadChannels(AwChannelList& list)
{
	QMutexLocker lock(&m_mutex);
	foreach (AwChannel *c, list) {
		if (c->isBad() || m_badChannelLabels.contains(c->name()))
			list.removeAll(c);
	}
}


void AwMontageManager::applyGains()
{
	AwAmplitudeManager *am = AwAmplitudeManager::instance();
	for (auto c : m_channels + m_asRecorded.values()) {
		if (c != NULL)
			c->setGain(am->amplitude(c->type()));
	}
}

void AwMontageManager::setNewFilters(const AwFilterSettings& settings)
{
	settings.apply(m_channels);
	settings.apply(m_asRecorded.values());
	settings.apply(AwSettings::getInstance()->currentReader()->infos.channels());
}

void AwMontageManager::quit()
{
	closeFile();
}

void AwMontageManager::clear()
{
	clearICA();
	clearSource(AwChannel::MEG);
	clearSource(AwChannel::EEG);

	while (!m_channels.isEmpty()) // delete current montage.
		delete m_channels.takeLast();

}

void AwMontageManager::closeFile()
{
	if (AwBIDSManager::isInstantiated()) {
		auto bm = AwBIDSManager::instance();
		if (bm->isBIDSActive())
			if (bm->updateChannelsTsvBadChannels(m_badChannelLabels) != 0 && !bm->lastError().isEmpty()) {
				AwMessageBox::information(nullptr, "BIDS", bm->lastError());
			}
			//checkForBIDSMods();
	}
			
	clear();
	m_montagePath = "";
	m_badPath = "";
	m_asRecorded.clear();
}

void AwMontageManager::newMontage(AwFileIO *reader)
{
	closeFile();
	AwChannelList channels = reader->infos.channels();

	// init as recorded channels list
	for (auto c : channels)  {
		// insert in hash table
		m_asRecorded[c->name()] = c;
		// make a copy a as recorded channel and insert it in channels.
		// do not insert Reference channels in default montage
		if (!c->isReference())
			m_channels << c->duplicate();
	}

	// check for .bad file
	m_badChannelLabels.clear();
	m_badPath = reader->getSideFile(".bad");
	if (QFile::exists(m_badPath))
		loadBadChannels();
	

	QFileInfo fi(reader->fullPath());
	// check for local montages.
	scanForMontagesInDirectory(fi.absolutePath());

	// get bids channels tsv if we are in BIDS mode
	// consider it the default montage.
	// also get bad channels from tsv and use them.
	if (AwBIDSManager::isInstantiated()) {
		auto bm = AwBIDSManager::instance();
		if (bm->isBIDSActive()) {
			// BEWARE: getChannelsTsvMontage() will build a montage based on what found in TSV file.
			// but TSV file does not contain all the information an AwChannel (miss the sampling rate at first)
			auto defaultTsvMontage = bm->getChannelsTsvMontage();
			if (!defaultTsvMontage.isEmpty()) {
				AW_DESTROY_LIST(m_channels);
				m_channels = defaultTsvMontage;
				// check for bad channels
				AwChannelList tmp;
				m_badChannelLabels.clear();
				for (auto c : defaultTsvMontage) {
					// update corresponding as recorded bad status
					auto asRecorded = m_asRecorded.value(c->name());
					if (asRecorded) {
						// copy information from as recorded
						c->setSamplingRate(asRecorded->samplingRate());
						c->setGain(asRecorded->gain());
						c->setUnit(asRecorded->unit());
						asRecorded->setBad(c->isBad());
						asRecorded->setType(c->type());
						if (c->isBad()) {
							m_badChannelLabels.append(c->name());
						}
					}
					else {  // not found in as recorded => should never happened
						tmp << c;
					}
				}
				saveBadChannels();
				if (!tmp.isEmpty()) {
					for (auto t : tmp) {
						m_channels.removeAll(t);
						delete t;
					}
				}
			}
		}
	}

	// check for .montage file
	// BEWARE: if a .mtg exists, it will override the default TSV montage
	m_montagePath = reader->getSideFile(".mtg");
	if (QFile::exists(m_montagePath))  {
		if (!loadMontage(m_montagePath)) {
			AwMessageBox::critical(NULL, tr("Montage"), tr("Failed to load autosaved .mtg file!"));
		}
	}

//	updateMontageFromChannelsTsv(reader);

	// check if filter settings is empty (this is the case when we open a new data file with no previous AnyWave processing)
	if (AwSettings::getInstance()->filterSettings().isEmpty()) {
		AwSettings::getInstance()->filterSettings().initWithChannels(m_channels);
	}
	setNewFilters(AwSettings::getInstance()->filterSettings());
	applyGains();

	// check that none NULL channels are present in asRecorder nor m_channels
	for (auto k : m_asRecorded.keys()) {
		if (m_asRecorded.value(k) == Q_NULLPTR)
			m_asRecorded.remove(k);
	}
	foreach(AwChannel *c, m_channels)
		if (c == Q_NULLPTR)
			m_channels.removeAll(c);

	emit montageChanged(m_channels);
}

///
/// saveBadChannels()
/// Save a text file containing all channels marked as bad.
/// Called each time a channel is marked as Bad or is unmarked.
void AwMontageManager::saveBadChannels()
{
	if (m_badPath.isEmpty())
		return;

	if (m_badChannelLabels.isEmpty()) { // no bad channels.
		if (QFile::exists(m_badPath)) 
			QFile::remove(m_badPath);
		return;
	}

	QFile file(m_badPath);
	QTextStream stream(&file);
	if (!file.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Truncate))
		return;

	foreach (QString s, m_badChannelLabels)
		stream << s << endl;
	file.close();
}


void AwMontageManager::loadBadChannels()
{
	QFile file(m_badPath);
	QTextStream stream(&file);
	
	if (file.open(QIODevice::ReadOnly|QIODevice::Text))  {
		m_badChannelLabels.clear();
		while (!stream.atEnd()) 	
			m_badChannelLabels << stream.readLine().trimmed();
		file.close();
	}
	// reset as recorded channels bad status
	for (auto chan : m_asRecorded.values())
		chan->setBad(false);
	for (auto label : m_badChannelLabels) {
		AwChannel *chan = m_asRecorded.value(label);
		if (chan)
			chan->setBad(true);
	}
	foreach(AwChannel *chan, m_channels) {
		AwChannel *asRecorded =  m_asRecorded.value(chan->name());
		if (asRecorded)  
			if (asRecorded->isBad()) {
				m_channels.removeAll(chan);
				delete chan;
			}
	}
}

void AwMontageManager::saveCurrentMontage()
{
	save(m_montagePath);
}

//
// showInterface()
//
void AwMontageManager::showInterface()
{
	AwMontageDial ui;

	if (ui.exec() == QDialog::Accepted) {
		// destroy current montage
		while (!m_channels.isEmpty())
			delete m_channels.takeFirst();

		// Get as recorded channels and check if their types and bad status have changed.
		for (auto c : ui.asRecordedChannels()) {
			auto asRecorded = m_asRecorded.value(c->name()); 
			if (asRecorded) {
				asRecorded->setBad(c->isBad());
				asRecorded->setType(c->type());
			}
		}
		
		// now browse channels defined as montage and instantiate them properly 
		QStringList labels = AwChannel::getLabels(ui.channels());
		for (int i = 0; i < labels.size(); i++) {
			auto asRecorded = m_asRecorded.value(labels.value(i));
			if (asRecorded) {
				if (asRecorded->isVirtual())
					m_channels << static_cast<AwVirtualChannel *>(asRecorded)->duplicate();
				else
					m_channels << asRecorded->duplicate();
				// copy properties from ui montage channels to newly instantiated channels
				m_channels.last()->setReferenceName(ui.channels().at(i)->referenceName());
				m_channels.last()->setColor(ui.channels().at(i)->color());
				m_channels.last()->setLowFilter(ui.channels().at(i)->lowFilter());
				m_channels.last()->setHighFilter(ui.channels().at(i)->highFilter());
			}
		}
		m_badChannelLabels = ui.badLabels();
		save(m_montagePath);
		saveBadChannels();
		emit montageChanged(m_channels);
		emit badChannelsSet(m_badChannelLabels);
	}
}

bool AwMontageManager::save(const QString& filename, const AwChannelList& channels)
{
	try {
		AwMontage::save(filename, channels);
	}
	catch (const AwException& e) {
		return false;
	}
	return true;
}

bool AwMontageManager::save(const QString& filename)
{
    bool res =  save(filename, m_channels);
	if (!res)
		emit log(QString("saving %1 failed."));
	return res;
}


bool AwMontageManager::saveMontage(const QString& name)
{
	if (m_quickMontagesHT.keys().contains(name)) {
		int ret = AwMessageBox::information(0, tr("Montage"), tr("A montage with the same name already exists.\nOverwrite it?"), QMessageBox::Yes|QMessageBox::No);
		if (ret == QMessageBox::No)
			return false;
		else
			// overwrite
			m_quickMontagesHT.remove(name);
	}

	QString path = m_path + name + ".mtg";
	m_quickMontagesHT.insert(name, path);

	if (!save(path))
		return false;

	buildQuickMontagesList();
	return true;
}

AwChannelList AwMontageManager::load(const QString& path)
{

	AwChannelList channels, res;
	try {
		channels = AwMontage::load(path);
	}
	catch (const AwException& e)
	{
		AwMessageBox::information(0, tr("Error"), e.errorString());
		return channels;
	}

	// duplicate channels and put them in the montage if they match the settings (bad, references, etc.)
	foreach(AwChannel *c, channels) {
		if (!m_asRecorded.contains(c->name())) {
			channels.removeAll(c);
			delete c;
			continue;
		}
		// search for a match in as recorded
		auto asRecorded = m_asRecorded.value(c->name());
		if (asRecorded) {
			// Change asrecorded type to match the one in .mtg
			asRecorded->setType(c->type());
			if (asRecorded->isBad())
				continue;
			if (c->hasReferences() && c->referenceName() != "AVG") {
				auto asRecordedRef = m_asRecorded.value(c->referenceName());
				if (asRecordedRef) {
					if (asRecorded->isBad())
						continue;
				}
				else  // reference not found => remove it
					c->clearRefName();
			}
			// instantiante from as recorded
			auto newChan = asRecorded->duplicate();
			// copy settings set in .mtg file
			newChan->setColor(c->color());
			newChan->setLowFilter(c->lowFilter());
			newChan->setHighFilter(c->highFilter());
			newChan->setReferenceName(c->referenceName());
			res << newChan;
		}
		else {  // not present in as recorded
			// checking for special names
			if (c->name() == "SEEG_AVG") {
				res << new AwAVGChannel(AwChannel::SEEG);
			}
			else if (c->name() == "EEG_AVG") {
				res << new AwAVGChannel(AwChannel::EEG);
			}
			else if(c->name() == "MEG_AVG") {
				res << new AwAVGChannel(AwChannel::MEG);
			}
		}
	}

	while (!channels.isEmpty())
		delete channels.takeFirst();

	return res;
}

bool AwMontageManager::apply(const QString& path)
{
	// deleting current channels.
	while (!m_channels.isEmpty())
		delete m_channels.takeLast();

	m_channels = load(path);
	return true;
}

/// SCRIPTING support

// loadAndApplyMonage()
// in: List of as Recorded channels
// path to montage file to apply.
// out: List of montaged channels or empty list if error.
AwChannelList AwMontageManager::loadAndApplyMontage(AwChannelList asRecorded, const QString &path, const QStringList& badLabels)
{
	QMutexLocker lock(&m_mutex); // threading lock

	AwChannelList channels, res;
	try {
		channels = AwMontage::load(path);
	}
	catch (const AwException& e) {
		return channels;
	}

	QHash<QString, AwChannel *> hash;
	for (auto c : asRecorded)
		hash[c->name()] = c;

	if (!badLabels.isEmpty()) {
		for (auto k : hash.keys())
			if (badLabels.contains(k))
				hash.value(k)->setBad(true);
	}
	
	for (auto c : channels) {
		if (!hash.contains(c->name()))
			continue;
		if (hash.value(c->name())->isBad())
			continue;
		auto newChannel = new AwChannel(hash.value(c->name()));
		hash.value(c->name())->setType(c->type());
		/* Handle AVG references */
		if (c->referenceName() == "AVG") {
			newChannel->setReferenceName("AVG");
			res << newChannel;
			continue;
		}
		if (c->hasReferences() && hash.contains(c->referenceName())) {
			// make sure the type of ref channel is matching the one in montage
			if (!hash.value(c->referenceName())->isBad()) {
				hash.value(c->referenceName())->setType(c->type());
				newChannel->setReferenceName(c->referenceName());
			}
			else
				newChannel->clearRefName();
		}
		else
			newChannel->clearRefName();
		res << newChannel;
	}
	AW_DESTROY_LIST(channels);
	return res;
}



AwChannelList& AwMontageManager::channels()
{
	QMutexLocker lock(&m_mutex);
	return m_channels;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SLOTS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool AwMontageManager::loadMontage(const QString& path)
{
	if (path.isEmpty())
		return false;

	if (!apply(path)) {
		QMessageBox::warning(0, tr("Loading a montage"), tr("Error loading montage, montage defined in file may not be compatible."), QMessageBox::Ok);
		return false;
	}
	return true;
}


// Mark a list of channels identified by their label as Bad.
void AwMontageManager::markChannelsAsBad(const QStringList& labels)
{
	bool updateMontage = false;
	AwChannelList toDelete;
	foreach(QString label, labels)	{
		if (!m_asRecorded.contains(label))
			continue;
		auto asRecorded = m_asRecorded.value(label);
		asRecorded->setBad(true);
		m_badChannelLabels << label;
		foreach(AwChannel *c, m_channels) {
			if (c->name() == asRecorded->name() || c->referenceName() == asRecorded->name()) {
				m_channels.removeAll(c);
				toDelete << c;
				updateMontage = true;
			}
		}
	}
	while (!toDelete.isEmpty())
		delete toDelete.takeFirst();

	if (updateMontage) {
		emit badChannelsSet(m_badChannelLabels);
		emit montageChanged(m_channels);
	}

	saveBadChannels();
}

// 
// markChannelAsBad()
//
// set the asRecorded channel with same name than channel passed in parameter as Bad.
// compute new montage.
void AwMontageManager::markChannelAsBad(const QString& channelName, bool bad)
{
	auto asRecorded = m_asRecorded.value(channelName);
	if (asRecorded) { // channel found
		AwChannelList toDelete;
		if (bad) { // set it to bad
			if (!m_badChannelLabels.contains(channelName))
				m_badChannelLabels << channelName;

			asRecorded->setBad(true);
			bool updateMontage = false;
			foreach (AwChannel *c, m_channels) { // check in montage for reference to channelName. If any reference found, remove channel from montage.
				if (c->name() == asRecorded->name() || c->referenceName() == asRecorded->name()) {
					m_channels.removeAll(c);
					toDelete << c;
					updateMontage = true;
				}
			}
			emit badChannelsSet(m_badChannelLabels);
			if (updateMontage) {
				emit montageChanged(m_channels);
			}
		}
		else { // set it to not bad!
			m_badChannelLabels.removeAll(channelName);
			asRecorded->setBad(false);
			emit badChannelsSet(m_badChannelLabels);
		}
		while (!toDelete.isEmpty())
			delete toDelete.takeFirst();
		saveBadChannels();
	}
}

//
// addChannelToAsRecorded()
// Ajoute un canal dont la source est Virtual aux canaux AsRecorded
// Permet de rajouter ensuite au montage des canaux virtuels alimentes par des process ou servant dans des plugins d'affichage.
void AwMontageManager::addChannelToAsRecorded(AwChannel *channel)
{
	// si le canal n'est pas de source virtual on ne fait rien
	if (!channel->isVirtual())
		return;
	m_asRecorded[channel->name()] = channel;
}


////// Quick Montages

void AwMontageManager::buildQuickMontagesList()
{
	m_quickMontages.clear();
	m_quickMontagesHT.clear();

	scanForPrebuiltMontages();
	// now check in user montage directory
	AwSettings *aws = AwSettings::getInstance();
	// check in AnyWave's montage directory
	QStringList filter("*.mtg");
	m_path = aws->value(aws::montage_dir).toString();
	if (m_path.isEmpty())
		return;

	QDir montDir(m_path);
	montDir.setNameFilters(filter);
	QStringList files = montDir.entryList();

	// init hash table
	for (int i = 0; i < files.count(); i++)	{
		QString key = files.value(i);
		key = key.remove(".mtg", Qt::CaseInsensitive);
		m_quickMontages << key;
		m_quickMontagesHT.insert(key, m_path + files.value(i));
	}

	qSort(m_quickMontages.begin(), m_quickMontages.end());
	emit quickMontagesUpdated();
}

//
// loadQuickMontage()
//
void AwMontageManager::loadQuickMontage(const QString& name)
{
	QString path;
	// search on quick montages
	path = m_quickMontagesHT.value(name);
	if (path.isEmpty())	{
		// search in local montages
		path = m_localQuickMontagesHT.value(name);
		if (path.isEmpty()) {
			QMessageBox::warning(0, tr("Quick Montage"), tr("The quick montage does not exist."));
			return;
		}
	}
	
	if (!apply(path)) {
		QMessageBox::warning(0, tr("Loading a montage"), tr("Error loading montage, montage defined in file may not be compatible."), QMessageBox::Ok);
		return;
	}
	applyGains();
	setNewFilters(AwSettings::getInstance()->filterSettings());
	emit montageChanged(m_channels);

}



//
// addChannelsByName()
//
void AwMontageManager::addChannelsByName(AwChannelList &channelsToAdd)
{
	foreach (AwChannel *c, channelsToAdd)	{
		if (!m_asRecorded.contains(c->name()))
			continue;
		auto asRecorded = m_asRecorded.value(c->name());
		if (asRecorded->isBad())
			continue;
		m_channels << asRecorded->duplicate();

	}
	emit montageChanged(m_channels);
}



void AwMontageManager::buildNewMontageFromChannels(const AwChannelList& channels)
{
	if (channels.isEmpty())
		return;
	AwChannelList newMontage; // check the channels against the as recorded ones and also check the bad status.
	for (auto c : channels) {
		if (!m_asRecorded.contains(c->name()))
			continue;
		auto asRecorded = m_asRecorded.value(c->name());
		// do not add a bad channel in the montage
		if (asRecorded->isBad())
			continue;
		if (c->hasReferences() && m_asRecorded.contains(c->referenceName())) { // do not add the channel if the reference channel is bad.
			if (m_asRecorded.value(c->referenceName())->isBad())
				continue;
		}

		newMontage << c->duplicate();
	}
	if (!newMontage.isEmpty()) {
		//clear();
		while (!m_channels.isEmpty())
			delete m_channels.takeFirst();
		m_channels = newMontage;
		emit montageChanged(m_channels);
	}
}