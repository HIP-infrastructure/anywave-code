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
#include "IO/BIDS/AwBIDSManager.h"
#include <AwException.h>
#include <montage/AwMontage.h>
#include <AwCore.h>
#include "IO/BIDS/AwBIDSManager.h"
#include "Debug/AwDebugLog.h"
#include "Data/AwDataManager.h"
// statics init and definitions
AwMontageManager *AwMontageManager::m_instance = 0;


AwMontageManager* AwMontageManager::instance()
{
	if (!m_instance)
		m_instance = new AwMontageManager;
	return m_instance;
}


AwMontageManager* AwMontageManager::newInstance()
{
	return new AwMontageManager;
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



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////::
// Source/ICA specific

void AwMontageManager::clearICA()
{
	for (auto c : m_icaAsRecorded) 
		m_asRecordedSharedPointerMap.remove(c->name());
	foreach (auto const& c , m_channelsShrdPtrs) {
		if (c->isICA() && c->isVirtual())
			m_channelsShrdPtrs.removeAll(c);
	}
	m_channels = AwChannel::toChannelList(m_channelsShrdPtrs);
	// DO NOT DELETE channels as they belong to a AwComponent instance
	m_icaAsRecorded.clear();
	m_channels = AwChannel::toChannelList(m_channelsShrdPtrs);
}

void AwMontageManager::clearSource(int type)
{
	foreach(AwSourceChannel * c, m_sourceAsRecorded) {
		if (c->type() == type) {
			m_asRecordedSharedPointerMap.remove(c->name());
			m_sourceAsRecorded.removeAll(c);
		}
	}
	foreach(auto const& c, m_channelsShrdPtrs) {
		if (c->isSource() && c->type() == type)
			m_channelsShrdPtrs.removeAll(c);
	}

	m_channels = AwChannel::toChannelList(m_channelsShrdPtrs);
}

void AwMontageManager::reorderChannels(const QStringList& labels)
{
	// build current order list 
	QMap<QString, QSharedPointer<AwChannel>> map;
	for (const auto& channel : m_channelsShrdPtrs) 
		map.insert(channel->fullName(), channel);
	AwSharedChannelList tmp;
	for (const auto& l : labels) 
		tmp.append(map.value(l));
	m_channelsShrdPtrs = tmp;
	m_channels = AwChannel::toChannelList(m_channelsShrdPtrs);
	save(m_montagePath);

}

void AwMontageManager::addNewSources(int type)
{
	AwSourceManager *sm = AwSourceManager::instance();
	clearSource(type);
	AwSourceChannelList channels = sm->sources(type);

	for (auto c : channels) {
		if (m_asRecordedSharedPointerMap.contains(c->name()))
			m_asRecordedSharedPointerMap.remove(c->name());
		m_asRecordedSharedPointerMap.insert(c->name(), QSharedPointer<AwChannel>(c->duplicate()));
		m_channelsShrdPtrs << QSharedPointer<AwChannel>(c->duplicate());
	}
	m_channels = AwChannel::toChannelList(m_channelsShrdPtrs);

	emit montageChanged(m_channels);
	AwMessageBox::information(0, tr("Source channels"), QString("%1 source channels added to the current montage.").arg(channels.size()));
}

int AwMontageManager::loadICA()
{
	// default dir path = the directory containing the data file
	QString dir = AwDataManager::instance()->dataDir();
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
	// clear ICA should remove all ICA typed channels from as recorded list AND current montage.
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

	AwICAChannelList icaChannels = ica_man->getChannelsOfAllComponents();
	// put loaded ica channels into as recorded map and current montage
	
	for (auto ica : icaChannels) {
		if (m_asRecordedSharedPointerMap.contains(ica->name()))
			m_asRecordedSharedPointerMap.remove(ica->name());
		m_asRecordedSharedPointerMap.insert(ica->name(), QSharedPointer<AwChannel>(ica->duplicate()));
		m_channelsShrdPtrs << QSharedPointer<AwChannel>(ica->duplicate());
	}
	m_channels = AwChannel::toChannelList(m_channelsShrdPtrs);
	setNewFilters(AwDataManager::instance()->filterSettings());
	emit montageChanged(m_channels);
	AwMessageBox::information(0, tr("ICA Components"), QString(tr("%1 components added to the current montage.").arg(icaChannels.size())));
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
	auto files = montDir.entryInfoList();

	m_localQuickMontages.clear();
	m_localQuickMontagesHT.clear();

	for (const auto& file : files) {
		auto key = file.fileName().remove(".mtg");
		if (!m_localQuickMontages.contains(key)) {
			m_localQuickMontages << key;
			m_localQuickMontagesHT.insert(key, file.absoluteFilePath());
		}
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
			// scan bids dir
			QStringList filter("*.mtg");
			QDir montDir(AwDataManager::instance()->bidsDir());
			montDir.setNameFilters(filter);
			auto files = montDir.entryInfoList();
			for (auto const& file : files) {
				auto key = file.fileName().remove(".mtg");
				if (!m_localQuickMontages.contains(key)) {
					m_localQuickMontages << key;
					m_localQuickMontagesHT.insert(key, file.absoluteFilePath());
				}
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

void AwMontageManager::setNewFilters(const AwFilterSettings& settings)
{
	settings.apply(m_channels);
	settings.apply(AwChannel::toChannelList(m_asRecordedSharedPointerMap.values()));
	settings.apply(AwDataManager::instance()->reader()->infos.channels());
}

void AwMontageManager::quit()
{
	disconnect(this, nullptr, nullptr, nullptr);
	closeFile();
}

void AwMontageManager::clear()
{
	clearICA();
	clearSource(AwChannel::MEG);
	clearSource(AwChannel::EEG);
	m_channels.clear();
	m_channelsShrdPtrs.clear();
}

void AwMontageManager::closeFile()
{
	if (AwBIDSManager::isInstantiated()) {
		auto bm = AwBIDSManager::instance();
		if (bm->isBIDSActive())
			if (bm->updateChannelsTsvBadChannels(m_badChannelLabels) != 0 && !bm->lastError().isEmpty()) {
				AwMessageBox::information(nullptr, "BIDS", bm->lastError());
			}
	}
			
	clear();
	m_montagePath = "";
	m_badPath = "";
	m_asRecordedSharedPointerMap.clear();
}

void AwMontageManager::newMontage(AwFileIO *reader)
{
	AwChannelList channels = reader->infos.channels();
	m_channelsShrdPtrs.clear();
	// init as recorded channels list
	for (auto c : channels)  {
		QSharedPointer<AwChannel> shrdChannel = QSharedPointer<AwChannel>(c->duplicate());
		m_channelsShrdPtrs << shrdChannel;
		m_asRecordedSharedPointerMap.insert(c->name(), QSharedPointer<AwChannel>(c->duplicate()));
		if (!c->isReference()) 
			m_channels << shrdChannel.get();
	}
	// check for .bad file
	m_badChannelLabels.clear();
	m_badPath = AwDataManager::instance()->badFilePath();
	if (QFile::exists(m_badPath))
		loadBadChannels();
	// check for local montages.
	scanForMontagesInDirectory(AwDataManager::instance()->currentMontageDir());
	// check for .montage file
	auto dm = AwDataManager::instance();
	m_montagePath = dm->mtgFilePath();
	if (QFile::exists(m_montagePath)) {
		if (!loadMontage(m_montagePath)) {
			AwMessageBox::critical(nullptr, tr("Montage"), tr("Failed to load .mtg file.\nDefault montage applied!"));
			// reset to default (as recorded channels)
			m_channelsShrdPtrs.clear();
			// init as recorded channels list
			for (auto c : channels) {
				QSharedPointer<AwChannel> shrdChannel = QSharedPointer<AwChannel>(c->duplicate());
				m_channelsShrdPtrs << shrdChannel;
				if (!c->isReference())
					m_channels << shrdChannel.get();
			}
			save(m_montagePath);
		}
	}
	else {  // no default montage found => read channels.tsv if BIDS is active
		if (AwBIDSManager::isInstantiated()) {
			auto bm = AwBIDSManager::instance();
			if (bm->isBIDSActive()) {
				auto defaultTsvMontage = bm->getChannelsTsvMontage();
				if (!defaultTsvMontage.isEmpty()) {
					m_channels.clear();
					m_channelsShrdPtrs.clear();
					m_badChannelLabels.clear();
					for (AwChannel *defaultChannel : defaultTsvMontage) {
						if (m_asRecordedSharedPointerMap.contains(defaultChannel->name())) {
							QSharedPointer<AwChannel> asRecorded = m_asRecordedSharedPointerMap.value(defaultChannel->name());
							asRecorded->setType(defaultChannel->type());
							asRecorded->setBad(defaultChannel->isBad());
							if (asRecorded->isBad())
								m_badChannelLabels << asRecorded->name();
							m_channelsShrdPtrs.append(QSharedPointer<AwChannel>(asRecorded.get()->duplicate()));
						}
					}
					qDeleteAll(defaultTsvMontage);
					saveBadChannels();
					m_channels = AwChannel::toChannelList(m_channelsShrdPtrs);
				}
			}
		}
	}
	setNewFilters(AwDataManager::instance()->filterSettings());
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
	if (m_badChannelLabels.isEmpty())
		return;

	// reset as recorded channels bad status
	for (const auto& chan : m_asRecordedSharedPointerMap.values())
		chan->setBad(false);
	// update bad status on bad labels list
	for (const auto& label : m_badChannelLabels) {
		auto asRecorded = m_asRecordedSharedPointerMap.value(label);
		if (!asRecorded.isNull()) 
			asRecorded->setBad(true);
	}
	// now update current montage
	AwSharedPointerChannelList tmp;
	for (const auto& channel : m_channelsShrdPtrs) {
		auto asRecorded = m_asRecordedSharedPointerMap.value(channel->name());
		if (!asRecorded.isNull())
			if (asRecorded->isBad())
				tmp << channel;
	}
	// check for references of channels (are they set to bad?)
	for (const auto& channel : m_channelsShrdPtrs) {
		auto asRecorded = m_asRecordedSharedPointerMap.value(channel->referenceName());
		if (!asRecorded.isNull())
			if (asRecorded->isBad())
				tmp << channel;
	}
	for (const auto& t : tmp)
		m_channelsShrdPtrs.removeAll(t);
	m_channels = AwChannel::toChannelList(m_channelsShrdPtrs);
}

void AwMontageManager::saveCurrentMontage()
{
	save(m_montagePath);
}

void AwMontageManager::processEvent(QSharedPointer<AwEvent> e)
{
	auto data = e->data();
	switch (e->id())
	{
	case AwEvent::LoadICAMatFile:
	{
		emit log("Received event to load ICA .mat file");
		QStringList args = data.value("args").toStringList();
		if (args.isEmpty()) 
		   emit log("Empty args. Skipping.");
		QString file = args.first();
		if (QFile::exists(file))
			loadICA(file);
	}
		break;
	}
}

//
// showInterface()
//
void AwMontageManager::showInterface()
{
	AwMontageDial ui;

	if (ui.exec() == QDialog::Accepted) {
		m_channels.clear();

		// Get as recorded channels and check if their types and bad status have changed.
		for (auto c : ui.asRecordedChannels()) {
			auto asRecordedShrd = m_asRecordedSharedPointerMap.value(c->name());
			if (!asRecordedShrd.isNull()) {
				asRecordedShrd->setBad(c->isBad());
				asRecordedShrd->setType(c->type());
			}
		}
		
		// now browse channels defined as montage and instantiate them properly 
		QStringList labels = AwChannel::getLabels(ui.channels());
		m_channelsShrdPtrs.clear();
		for (int i = 0; i < labels.size(); i++) {
			auto asRecorded = m_asRecordedSharedPointerMap.value(labels.value(i));
			if (!asRecorded.isNull()) {
				QSharedPointer<AwChannel> newChannel = QSharedPointer<AwChannel>(asRecorded.get()->duplicate());
				// copy properties from ui montage channels to newly instantiated channels
				newChannel->setReferenceName(ui.channels().at(i)->referenceName());
				newChannel->setColor(ui.channels().at(i)->color());
				newChannel->setLowFilter(ui.channels().at(i)->lowFilter());
				newChannel->setHighFilter(ui.channels().at(i)->highFilter());
				newChannel->setNotch(ui.channels().at(i)->notch());
				m_channelsShrdPtrs << newChannel;
			}
		}
		m_channels = AwChannel::toChannelList(m_channelsShrdPtrs);
		m_badChannelLabels = ui.badLabels();
		save(m_montagePath);
		saveBadChannels();
		emit montageChanged(m_channels);
		emit badChannelsSet(m_badChannelLabels);
	}
}

bool AwMontageManager::save(const QString& filename, const AwChannelList& channels, const AwChannelList& asRecordedChannels)
{
	try {
		AwMontage::save(filename, channels, asRecordedChannels);
	}
	catch (const AwException& e) {
		return false;
	}
	return true;
}

bool AwMontageManager::save(const QString& filename)
{
    //bool res =  save(filename, m_channels, m_asRecordedChannels);
	bool res = save(filename, m_channels, AwChannel::toChannelList(m_asRecordedSharedPointerMap.values()));
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
	QMap<QString, int> asRecordedMap;
	try {
		channels = AwMontage::load(path, &asRecordedMap);
	}
	catch (const AwException& e)
	{
		AwMessageBox::information(0, tr("Error"), e.errorString());
		return res;
	}

	if (asRecordedMap.size()) {
		// got as recorded info from .mtg file
		bool changed = false;
		for (auto const& label : asRecordedMap.keys()) {
			auto asRecorded = m_asRecordedSharedPointerMap.value(label);
			if (asRecorded.isNull())
				continue;
			if (asRecorded->type() != asRecordedMap.value(label)) {
				changed = true;
				asRecorded->setType(asRecordedMap.value(label));
			}
		}
	}
	// duplicate channels and put them in the montage if they match the settings (bad, references, etc.)
	for (auto c : channels) {
		auto asRecorded = m_asRecordedSharedPointerMap.value(c->name());
		if (!asRecorded.isNull()) {
			// change type to match the one coming from mtg file
			asRecorded->setType(c->type());
			if (asRecorded->isBad())
				continue;
			if (c->hasReferences() && c->referenceName() != "AVG") {
				auto asRecordedRef = m_asRecordedSharedPointerMap.value(c->referenceName());
				if (!asRecordedRef.isNull()) {
					if (asRecordedRef->isBad())
						continue;
				}
				else  // reference not found, remove it
					c->clearRefName();
			}
			// instiante the channel from as recorded channels to current montage
			auto newChan = asRecorded->duplicate();
			// copy settings set in .mtg file
			newChan->setColor(c->color());
			newChan->setLowFilter(c->lowFilter());
			newChan->setHighFilter(c->highFilter());
			newChan->setReferenceName(c->referenceName());
			res << newChan;
		}
		else {  // channel not found in as recorded (could be AVG or other Virtual)
			// checking for special names
			if (c->name() == "SEEG_AVG") {
				res << new AwAVGChannel(AwChannel::SEEG);
			}
			else if (c->name() == "EEG_AVG") {
				res << new AwAVGChannel(AwChannel::EEG);
			}
			else if (c->name() == "MEG_AVG") {
				res << new AwAVGChannel(AwChannel::MEG);
			}
		}
	}
	return res;
}

bool AwMontageManager::apply(const QString& path)
{
	m_channelsShrdPtrs = AwChannel::toSharedPointerList(load(path));
	m_channels = AwChannel::toChannelList(m_channelsShrdPtrs);
	return !m_channelsShrdPtrs.isEmpty();
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

	//if (!apply(path)) {
	//	QMessageBox::warning(0, tr("Loading a montage"), tr("Error loading montage, montage defined in file may not be compatible."), QMessageBox::Ok);
	//	return false;
	//}
	return apply(path);
//	return true;
}


// Mark a list of channels identified by their label as Bad.
void AwMontageManager::markChannelsAsBad(const QStringList& labels)
{
	bool updateMontage = false;
	for (const auto& label : labels) {
		if (!m_asRecordedSharedPointerMap.contains(label))
			continue;
		auto asRecorded = m_asRecordedSharedPointerMap.value(label);
		asRecorded->setBad(true);
		if (!m_badChannelLabels.contains(label))
			m_badChannelLabels << label;
		AwSharedPointerChannelList tmp;
		for (const auto& channel : m_channelsShrdPtrs) {
			if (channel->name() == label || channel->referenceName() == label)
				tmp << channel;
		}
		if (!tmp.isEmpty()) {
			updateMontage = true;
			for (const auto& t : tmp)
				m_channelsShrdPtrs.removeAll(t);
		}
	}
	if (updateMontage) {
		emit badChannelsSet(m_badChannelLabels);
		m_channels = AwChannel::toChannelList(m_channelsShrdPtrs);
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
	auto asRecorded = m_asRecordedSharedPointerMap.value(channelName);
	if (asRecorded.isNull())
		return;

	if (bad) {
		if (!m_badChannelLabels.contains(channelName))
			m_badChannelLabels << channelName;
		asRecorded->setBad(true);
		bool updateMontage = false;
		AwSharedPointerChannelList tmp;
		for (const auto& c : m_channelsShrdPtrs) {
			if (c->name() == channelName || c->referenceName() == channelName) {
				tmp << c;
				updateMontage = true;
			}
		}
		if (updateMontage) {
			emit badChannelsSet(m_badChannelLabels);
			for (const auto& t : tmp)
				m_channelsShrdPtrs.removeAll(t);
			m_channels = AwChannel::toChannelList(m_channelsShrdPtrs);
			emit montageChanged(m_channels);
		}
	}
	else { // from bad to not bad
		m_badChannelLabels.removeAll(channelName);
		asRecorded->setBad(false);
		emit badChannelsSet(m_badChannelLabels);
	}
	saveBadChannels();
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
//	m_asRecorded[channel->name()] = channel;
	m_asRecordedSharedPointerMap.insert(channel->name(), QSharedPointer<AwChannel>(channel->duplicate()));
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

	setNewFilters(AwDataManager::instance()->filterSettings());
	emit montageChanged(m_channels);

}



//
// addChannelsByName()
//
void AwMontageManager::addChannelsByName(AwChannelList &channelsToAdd)
{
	for (auto c : channelsToAdd) {
		if (!m_asRecordedSharedPointerMap.contains(c->name()))
			continue;
		auto asRecorded = m_asRecordedSharedPointerMap.value(c->name());
		if (asRecorded->isBad())
			continue;
		m_channelsShrdPtrs << QSharedPointer<AwChannel>(c->duplicate());
	}
	m_channels = AwChannel::toChannelList(m_channelsShrdPtrs);
	emit montageChanged(m_channels);
}



void AwMontageManager::buildNewMontageFromChannels(const AwChannelList& channels)
{
	if (channels.isEmpty())
		return;
	AwSharedPointerChannelList newMontage;
	for (auto c : channels) {
		if (!m_asRecordedSharedPointerMap.contains(c->name()))
			continue;
		auto asRecorded = m_asRecordedSharedPointerMap.value(c->name());
		if (asRecorded->isBad())
			continue;
		if (c->hasReferences() && m_asRecordedSharedPointerMap.contains(c->referenceName())) {
			if (m_asRecordedSharedPointerMap.value(c->name())->isBad())
				continue;
		}
		newMontage << QSharedPointer<AwChannel>(c->duplicate());
	}
	if (newMontage.isEmpty())
		return;
	m_channelsShrdPtrs = newMontage;
	m_channels = AwChannel::toChannelList(m_channelsShrdPtrs);
	save(m_montagePath);
	emit montageChanged(m_channels);
}