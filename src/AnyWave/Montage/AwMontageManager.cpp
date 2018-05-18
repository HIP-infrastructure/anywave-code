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
#include <QDomElement> // For xml input/output
#include <widget/AwMessageBox.h>
#include <QtCore>
//#include "Filter/AwFilteringManager.h"
#include "Filter/AwFiltersManager.h"
#include "Process/AwProcessManager.h"
#include <QFileDialog>
#include "ICA/AwICAManager.h"
#include "ICA/AwICAComponents.h"
#include "ICA/AwICAChannel.h"
#include "Source/AwSourceManager.h"
#include "Source/AwSourceChannel.h"
#include "AwAVGChannel.h"
#include <widget/AwWait.h>
#include <AwAmplitudeManager.h>

// compare function for sorting labels of electrodes.
bool compareSEEGLabels(const QString& s1, const QString& s2)
{
	QRegularExpression re("\\d+$");
	Q_ASSERT(re.isValid());
	QString elec1 = s1, elec2 = s2;
	QRegularExpressionMatch match1 = re.match(elec1);
	QRegularExpressionMatch match2 = re.match(elec2);
	
	bool m2 = match2.hasMatch();
	bool m1 =  match1.hasMatch();
	
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

/////////////////////////////////////////////////////////////////////////////////
/// sorting function for AwChannel using labels
/// When sorting we want A2 to come before A10 for example.
bool channelLabelLessThan(AwChannel *c1, AwChannel *c2)
{
	return compareSEEGLabels(c1->name(),c2->name());
}

// statics init and definitions
AwMontageManager *AwMontageManager::m_instance = 0;


AwMontageManager *AwMontageManager::instance()
{
	if (!m_instance) 
		m_instance = new AwMontageManager;
	return m_instance;
}

AwChannelList AwMontageManager::makeSEEGBipolar(AwChannelList& channels)
{
	AwChannelList res = channels;
	QStringList chanLabels;
	foreach (AwChannel *c, res) {
		chanLabels << c->name();
		if (c->isEEG() && !c->isVirtual())
			c->setType(AwChannel::SEEG);
	}

	QStringList labels;
	foreach (AwChannel *chan, res) {
		if (chan->isSEEG())
			labels << chan->name();
	}

	// sort labels
	qSort(labels.begin(), labels.end(), compareSEEGLabels);
	AwChannelList bipolarChannels;
	QRegularExpression exp("\\d+$");
	QRegularExpressionMatch match1, match2;
	while (labels.size() > 1) {
		QString elec1 = labels.first();
		QString elec2 = labels.at(1);
		AwChannel *chan = res.at(chanLabels.indexOf(elec1));
		labels.removeFirst();
		if (chan == NULL)
			continue;
		match1 = exp.match(elec1);
		match2 = exp.match(elec2);
		if (!match1.hasMatch() || !match2.hasMatch())  // one of the electodre does not have a plot number, so skip it
			continue;
		// check if the both electrodes share the same base name (by removing the plot number).
		QString base1 = elec1;
		base1.remove(exp);
		QString base2 = elec2;
		base2.remove(exp);
		int plot1, plot2;
		plot1 = match1.captured(0).toInt();
		plot2 = match2.captured(0).toInt();
		if (base1 == base2 && plot1 == plot2 - 1) { // MATCH = > do the bipolar channel
			AwChannel *ref = NULL;
			if (chanLabels.contains(elec2))
				ref = res.at(chanLabels.indexOf(elec2));
			if (ref) {
				if (ref->isSEEG() && !ref->isBad()) {
					AwChannel *bipolar = new AwChannel(chan);
					bipolar->setReferenceName(elec2);
					bipolarChannels << bipolar;
				}
			}
		}
	}

	AwChannelList removedChannels;
	AwChannelList seegMonopolars;
	for (int i = 0; i < res.size(); i++) {
		AwChannel *chan = res.at(i);
		foreach(AwChannel *chan2, bipolarChannels) {
			if (chan2->name() == chan->name()) {
				res.removeAt(i);
				removedChannels << chan;
				res.insert(i, chan2);
			}
		}
	}

	foreach (AwChannel *chan, res) {
		// remove SEEG channels left as monopolar
		if (chan->isSEEG() && !chan->hasReferences()) {
			seegMonopolars << chan;
			res.removeAll(chan);
		}
	}

	removedChannels += seegMonopolars;

	while (!removedChannels.isEmpty())
		delete removedChannels.takeFirst();

	return res;
}

//end of statics


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////::
// Source/ICA specific

void AwMontageManager::clearICA()
{
	// remove ICA channels from asRecorded list
	foreach (AwICAChannel *c, m_icaAsRecorded)
		m_channelsAsRecorded.removeAll(c);
	m_icaHashTable.clear();

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
			m_channelsAsRecorded.removeAll(c);
			m_sourceHashTable.remove(c->name());
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
		m_channelsAsRecorded << c;
		m_channels << source;
		m_channelHashTable.insert(source->name(), source);
		m_sourceHashTable.insert(c->name(), c);
	}
	emit montageChanged(m_channels);
	AwMessageBox::information(0, tr("Source channels"), QString("%1 source channels added to the current montage.").arg(channels.size()));
//	AwFilteringManager *fm = AwFilteringManager::instance();
	AwFiltersManager *fm = AwFiltersManager::instance();
	fm->setSourceSettings(type, sm->hp(type), sm->lp(type));
}

int AwMontageManager::loadICA()
{
	QString dir = AwSettings::getInstance()->currentFileDir();
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
//	AwFilteringManager *fm = AwFilteringManager::instance();
	AwFiltersManager *fm = AwFiltersManager::instance();
	if (AwICAManager::instance()->loadComponents(path) == 0) { // if components sucessfully loaded, get components and make them as recorded channels.
		// add ICA components to asRecorded and current montage.
		AwICAComponents **comps = ica_man->getAllComponents();
		for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
			if (comps[i] == NULL)
				continue;
			count += comps[i]->components().size();
			foreach(AwICAChannel *channel, comps[i]->components()) {
				m_icaAsRecorded << channel;
				m_channelsAsRecorded << channel;
				AwICAChannel *newChan = new AwICAChannel(channel);
				newChan->setGain(AwAmplitudeManager::instance()->amplitude(AwChannel::ICA));
				m_channels << newChan;
				m_channelHashTable.insert(newChan->name(), newChan);
				m_icaHashTable.insert(channel->name(), channel);
			}
			fm->setICASettings(comps[i]->type(), comps[i]->hpFilter(), comps[i]->lpFilter());
		}

		emit montageChanged(m_channels);
		AwMessageBox::information(0, tr("ICA Components"), QString::number(count) + tr(" ICA components added to the current montage."));
		return 0;
	}
	AwMessageBox::critical(0, tr("ICA Components"), tr("Failed to import ICA components. Maybe the ICA components can't be applied to this data file?"));
	return -1;
}

// constructor
AwMontageManager::AwMontageManager()
{
	QStringList filter("*.mtg");
	AwSettings *aws = AwSettings::getInstance();

	m_path = aws->montageDir;
	if (m_path.isEmpty())
		return;

	QDir montDir(m_path);
	montDir.setNameFilters(filter);
	QStringList files = montDir.entryList();

	// init quick montages
	for (int i = 0; i < files.count(); i++)	{
		QString key = files[i];
		key = key.remove(".mtg", Qt::CaseInsensitive);
		m_quickMontages << key;
		m_quickMontagesHT.insert(key, m_path + "/" + files[i]);
	}
	qSort(m_quickMontages.begin(), m_quickMontages.end());
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

	// init hash table
	for (int i = 0; i < files.count(); i++)	{
		QString key = files[i];
		key = key.remove(".mtg", Qt::CaseInsensitive);
		m_localQuickMontages << key;
		m_localQuickMontagesHT.insert(key, dir + "/" + files[i]);
	}
	
	qSort(m_localQuickMontages.begin(), m_localQuickMontages.end());
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
		QString key = files[i];
		key = key.remove(".mtg", Qt::CaseInsensitive);
		m_quickMontages << key;
		m_quickMontagesHT.insert(key, path + files[i]);
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

	foreach(AwChannel *c, m_channels) {
		if (c->name() == label) {
			if (c->isICA()) {
				foreach (AwICAChannel *ica_chan, m_icaAsRecorded) {
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
	foreach (AwChannel *c, m_channels)
		if (!list.contains(c->name()))
			list << c->name();
	return list;
}

//
// containsChannelOfType(t)
//
// Retourne le premier canal de type t present dans la liste des canaux AsRecorded.
// Retourne NULL si aucun canal trouvé.
AwChannel* AwMontageManager::containsChannelOfType(AwChannel::ChannelType t)
{
	if (m_channelsAsRecorded.isEmpty())
		return 0;

	foreach (AwChannel *c, m_channelsAsRecorded)
		if (c->type() == t)
			return c;
	return 0;
}


void AwMontageManager::applyGains()
{
	AwAmplitudeManager *am = AwAmplitudeManager::instance();
	foreach (AwChannel *c, m_channels)
		c->setGain(am->amplitude(c->type()));
	foreach(AwChannel *c, m_channelsAsRecorded)
		c->setGain(am->amplitude(c->type()));
}


void AwMontageManager::newFilters()
{
	//AwFilteringManager *fm = AwFilteringManager::instance();

	//foreach (AwChannel *c, m_channels)	{
	//	c->setLowFilter(fm->lowPass(c->type()));
	//	c->setHighFilter(fm->highPass(c->type()));
	//	c->setNotch(fm->notch(c->type()));
	//}

	//foreach (AwChannel *c, m_channelsAsRecorded) {
	//	c->setLowFilter(fm->lowPass(c->type()));
	//	c->setHighFilter(fm->highPass(c->type()));
	//	c->setNotch(fm->notch(c->type()));
	//}

	AwFiltersManager *fm = AwFiltersManager::instance();
	fm->fo().setFilters(m_channels);
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

	m_channelHashTable.clear();
	m_channelsAsRecorded.clear(); // clear as recorded Channels
}

void AwMontageManager::closeFile()
{
	clear();

	if (!m_badChannelLabels.isEmpty()) // save bad channels
		saveBadChannels();
	else if (QFile::exists(m_badPath))  // no bad channels but may be a previous bad file is present, so delete the file.
		QFile::remove(m_badPath);
		
	m_badChannelLabels.clear();
	m_montagePath = "";
	m_badPath = "";
}

void AwMontageManager::newMontage(AwFileIO *reader)
{
	closeFile();
	AwChannelList channels = reader->infos.channels();

	// init as recorded channels list
	foreach (AwChannel *c, channels)  {
		// insert in hash table
		m_channelHashTable.insert(c->name(), c);
		// insert the as recorded channel in asRecorded list
		m_channelsAsRecorded << c;
		// make a copy a as recorded channel and insert it in channels.
		// do not insert Reference channels in default montage
		if (!c->isReference())
			m_channels << new AwChannel(c);
	}

	// check for .bad file
	m_badPath = reader->infos.fileName() + ".bad";
	if (QFile::exists(m_badPath))
		loadBadChannels();
	else
		m_badChannelLabels.clear();

	QFileInfo fi(reader->infos.fileName());
	// check for local montages.
	scanForMontagesInDirectory(fi.absolutePath());

	// check for .montage file
	m_montagePath = reader->infos.fileName() + ".mtg";
	if (QFile::exists(m_montagePath))  {
		if (!loadMontage(m_montagePath)) {
			AwMessageBox::critical(NULL, tr("Montage"), tr("Failed to load autosaved .mtg file!"));
		}
	}
	
	AwProcessManager::instance()->setMontageChannels(m_channels);
	applyGains();
	newFilters();
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
		while (!stream.atEnd()) 	{
			QString line = stream.readLine();
			line = line.trimmed();
			//markChannelAsBad(line);
			m_badChannelLabels << line;
		}
		file.close();
	}
	// reset as recorded channels bad status
	foreach(AwChannel *chan, m_channelsAsRecorded)
		chan->setBad(false);
	foreach (QString label, m_badChannelLabels) {
		AwChannel *chan = m_channelHashTable.value(label);
		if (chan)
			chan->setBad(true);
	}
	foreach(AwChannel *chan, m_channels) {
		AwChannel *asRecorded =  m_channelHashTable.value(chan->name());
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
//	AwMontageDial *ui = new AwMontageDial(this);
	AwMontageDial ui;

	if (ui.exec() == QDialog::Accepted) {
		// destroy current montage
		while (!m_channels.isEmpty())
			delete m_channels.takeFirst();

		// Get as recorded channels and check if their types and bas status have changed.
		for (auto c : ui.asRecordedChannels()) {
			AwChannel *asRecorded = m_channelHashTable.value(c->name());
			asRecorded->setBad(c->isBad());
			asRecorded->setType(c->type());
		}
		
		// now browse channels defined as montage and instantiate them properly 
		QStringList labels = AwChannel::getLabels(ui.channels());
		for (int i = 0; i < labels.size(); i++) {
			AwChannel *asRecorded = m_channelHashTable.value(labels.at(i));
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

		//while (!m_channels.isEmpty()) {
		//	AwChannel *c = m_channels.takeFirst();

		//	if (!c->isVirtual() || c->isICA() || c->isSource()) // destroy all channels except virtual ones which are not ICA
		//		delete c;
		//}
		//m_channels = ui->channels();
		//m_badChannelLabels = ui->badLabels();

		//emit montageChanged(m_channels);
		//emit badChannelsSet(m_badChannelLabels);
		//save(m_montagePath);
		//saveBadChannels();
	}
	else {// reject
		//// restore bad status for as recorded Channels
		//foreach (AwChannel *c, m_channelsAsRecorded)
		//	c->setBad(false);
		//foreach (QString label, m_badChannelLabels) {
		//	AwChannel *asRecorded = m_channelHashTable.value(label);
		//	if (asRecorded)
		//		asRecorded->setBad(true);
		//}
	}
//	delete ui;
}

bool AwMontageManager::save(const QString& filename, const AwChannelList& channels)
{
	QFile file(filename);
	
	if (!file.open(QIODevice::WriteOnly))
		return false;
	
	QTextStream stream(&file);
	QDomDocument doc("AnyWaveMontage");
	QDomElement root = doc.createElement("Montage");
	QDomElement element;
	QDomElement child;

	doc.appendChild(root);
	foreach (AwChannel *chan, channels)	{
		element = doc.createElement("Channel");
		element.setAttribute("name", chan->name());
		root.appendChild(element);

		child = doc.createElement("type");
		element.appendChild(child);
		child.appendChild(doc.createTextNode(AwChannel::typeToString(chan->type())));

		child = doc.createElement("reference");
		element.appendChild(child);
		child.appendChild(doc.createTextNode(chan->referenceName()));

		child = doc.createElement("color");
		element.appendChild(child);
		if (chan->color().isEmpty())
			chan->setColor("black");
		child.appendChild(doc.createTextNode(chan->color()));

		// add filtering options
		child = doc.createElement("filters");
		child.setAttribute("lowPass", chan->lowFilter());
		child.setAttribute("highPass", chan->highFilter());
		element.appendChild(child);

	}
	doc.save(stream, 3);
	file.close();
	return true;
}

bool AwMontageManager::save(const QString& filename)
{
	return save(filename, m_channels);
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
	QFile file(path);
	AwChannelList res;

	if (!file.open(QIODevice::ReadOnly))
		return AwChannelList();

	QDomDocument doc;
	QDomElement element;
	QString error;
	int line, col;
	
	if (!doc.setContent(&file, &error, &line, &col)) {
		AwMessageBox::information(0, tr("Error"), QString("%1 line %2 column %3").arg(error).arg(line).arg(col));
		file.close();
		return AwChannelList();
	}

	QDomElement root = doc.documentElement(); // get root tag
	if (root.tagName() != "Montage") {
		file.close();
		return AwChannelList();
	}

	QDomNode node = root.firstChild();
	while (!node.isNull())	{
		element = node.toElement();
		QDomNode child;
		AwChannel *channel = NULL;
		if (element.tagName() == "Channel")	{
			QString name = element.attribute("name");
			AwChannel *asRecorded = m_channelHashTable.value(name);
			if (!asRecorded) { // channel not found in as recorded => skipping
				if (name == "SEEG_AVG")	{
					channel = new AwAVGChannel(AwChannel::SEEG);
				}
				else if (name == "MEG_AVG")	{
					channel = new AwAVGChannel(AwChannel::MEG);
					}
				else if ( name == "EEG_AVG") {
					channel =  new AwAVGChannel(AwChannel::EEG);
				}
				else {// unknowm channel type => skip
					node = node.nextSibling();
					continue;
				}
			}
			// skip channels marked as bad in as Recorded.
			else if (asRecorded->isBad()) {
				node = node.nextSibling();
				continue;
			}
			else 
				channel = new AwChannel(asRecorded);
			// Apply current global filtering options to the channel
			applyGlobalFilter(channel);
			child = element.firstChild();

			while (!child.isNull())	{
				QDomElement ee = child.toElement();
				if (ee.tagName() == "type") {
					int t = AwChannel::stringToType(ee.text());
					if (channel->type() != t) {
						asRecorded->setType(t);
						channel->setType(t);
					}
				}
				else if (ee.tagName() == "reference") {
					// find reference in asRecorded
					QString ref = ee.text();
					if (m_channelHashTable.value(ref))
						channel->setReferenceName(ref);
				}
				else if (ee.tagName() == "color") {
					if (ee.text().isEmpty())
						channel->setColor("black");
					else
						channel->setColor(ee.text());
				}
				else if (ee.tagName() == "filters") {
					// overwrite global filtering options if the channel was saved with a specific filtering setting.
					QString lp = ee.attribute("lowPass");
					QString hp = ee.attribute("highPass");

					channel->setLowFilter(lp.toDouble());
					channel->setHighFilter(hp.toDouble());
				}
				child = child.nextSibling();
			}
			// check for reference type
			if (!channel->referenceName().isEmpty()) {
				AwChannel *ref = m_channelHashTable.value(channel->referenceName());
				AwChannel *asRecorded = m_channelHashTable.value(ref->name());
				ref->setType(channel->type());
				asRecorded->setType(channel->type());
			}
			res << channel;
		}
		node = node.nextSibling();
	}

	file.close();
	return res;
}

bool AwMontageManager::apply(const QString& path)
{
	QFile file(path);

	if (!file.open(QIODevice::ReadOnly))
		return false;

	QDomDocument doc;
	QDomElement element;
	QString error;
	int line, col;
	
	if (!doc.setContent(&file, &error, &line, &col)) {
		AwMessageBox::information(0, tr("Error"), QString("%1 line %2 column %3").arg(error).arg(line).arg(col));
		file.close();
		return false;
	}

	QDomElement root = doc.documentElement(); // get root tag
	if (root.tagName() != "Montage") {
		file.close();
		return false;
	}

	// Save ICA channels if already loaded
	AwICAChannelList savedICAChannels;
	foreach (AwChannel *c, m_channels)  {
		if (c->isICA() && c->isVirtual()) { // we saved the As Recored parent, not the channel itself which will be destroyed just after that.
			savedICAChannels << m_icaHashTable.value(c->name());	
			m_channelHashTable.remove(c->name());
		}
	}

	// Save ICA channels if already loaded
	AwSourceChannelList savedSourceChannels;
	foreach (AwChannel *c, m_channels)  {
		if (c->isSource() && c->isVirtual()) { // we saved the As Recored parent, not the channel itself which will be destroyed just after that.
			savedSourceChannels << m_sourceHashTable.value(c->name());	
			m_channelHashTable.remove(c->name());
		}
	}

	// deleting current channels.
	while (!m_channels.isEmpty())
		delete m_channels.takeLast();

	QDomNode node = root.firstChild();
	while (!node.isNull())	{
		element = node.toElement();
		QDomNode child;
		AwChannel *channel = NULL;
		if (element.tagName() == "Channel")	{
			QString name = element.attribute("name");
			AwChannel *asRecorded = m_channelHashTable.value(name);
			if (!asRecorded) { // channel not found in as recorded => skipping
				if (name == "SEEG_AVG")	{
					channel = new AwAVGChannel(AwChannel::SEEG);
				}
				else if (name == "MEG_AVG")	{
					channel = new AwAVGChannel(AwChannel::MEG);
				}
				else if ( name == "EEG_AVG") {
					channel = new AwAVGChannel(AwChannel::EEG);
				}
				else {// unknowm channel type => skip
					node = node.nextSibling();
					continue;
				}
			}
			// skip channels marked as bad in as Recorded.
			else if (asRecorded->isBad()) {
				node = node.nextSibling();
				continue;
			}
			else 
				channel = new AwChannel(asRecorded);
			// Apply current global filtering options to the channel
			applyGlobalFilter(channel);
			child = element.firstChild();

			while (!child.isNull())	{
				QDomElement ee = child.toElement();
				if (ee.tagName() == "type") 	{
					int t = AwChannel::stringToType(ee.text());
					if (channel->type() != t) {
						asRecorded->setType(t);
						channel->setType(t);
					}
				}
				else if (ee.tagName() == "reference") {
					// find reference in asRecorded
					QString ref = ee.text();
					if (m_channelHashTable.value(ref))
						channel->setReferenceName(ref);
				}
				else if (ee.tagName() == "color")
					channel->setColor(ee.text());
				else if (ee.tagName() == "filters") {
					// overwrite global filtering options if the channel was saved with a specific filtering setting.
					QString lp = ee.attribute("lowPass");
					QString hp = ee.attribute("highPass");

					channel->setLowFilter(lp.toDouble());
					channel->setHighFilter(hp.toDouble());
				}
				child = child.nextSibling();
			}
			// check for reference type
			if (!channel->referenceName().isEmpty()) {
				AwChannel *ref = m_channelHashTable.value(channel->referenceName());
				AwChannel *asRecorded = m_channelHashTable.value(ref->name());
				ref->setType(channel->type());
				asRecorded->setType(channel->type());
			}
			m_channels << channel;
		}
		node = node.nextSibling();
	}

	// Restore ICA channels if any
	foreach (AwICAChannel *channel, savedICAChannels) {
		AwICAChannel *newChan = new AwICAChannel(channel);
		m_channels << newChan;
		m_channelHashTable.insert(newChan->name(), newChan);
	}

	// Restore Source channels if any
	foreach (AwSourceChannel *channel, savedSourceChannels) {
		AwSourceChannel *newChan = new AwSourceChannel(channel);
		m_channels << newChan;
		m_channelHashTable.insert(newChan->name(), newChan);
	}

	file.close();
	return true;
}

void AwMontageManager::applyGlobalFilter(AwChannel *channel)
{
	//AwFilteringManager *fm = AwFilteringManager::instance();
	//channel->setLowFilter(fm->lowPass(channel->type()));
	//channel->setHighFilter(fm->highPass(channel->type()));
	//channel->setNotch(fm->notch(channel->type()));

	AwFiltersManager::instance()->fo().setFilters(channel);
}


/// SCRIPTING support

// loadAndApplyMonage()
// in: List of as Recorded channels
// path to montage file to apply.
// out: List of montaged channels or empty list if error.
AwChannelList AwMontageManager::loadAndApplyMontage(AwChannelList asRecorded, const QString &path)
{
	QMutexLocker lock(&m_mutex); // threading lock

	// reimplementing loading of montage files to only apply on asRecorded channels
	// removing all procedures related to AnyWave UI
	QFile file(path);


	QHash<QString, AwChannel *> asRecordedH;

	if (!file.open(QIODevice::ReadOnly))
		return AwChannelList();

	QDomDocument doc;
	QDomElement element;
	
	if (!doc.setContent(&file))	{
		file.close();
		return  AwChannelList();
	}

	QDomElement root = doc.documentElement(); // get root tag
	if (root.tagName() != "Montage") {
		file.close();
		return AwChannelList();
	}

	foreach (AwChannel *c, asRecorded)
		asRecordedH.insert(c->name(), c);

	QDomNode node = root.firstChild();
	AwChannel *chan = NULL;
	AwChannelList channels;

	while (!node.isNull())	{
		element = node.toElement();
		QDomNode child;
		if (element.tagName() == "Channel")	{
			QString name = element.attribute("name");
			AwChannel *asRecorded = asRecordedH.value(name);
			if (asRecorded) {
				child = element.firstChild();
				chan = new AwChannel(asRecorded);
				while (!child.isNull()) {
					QDomElement ee = child.toElement();
					// check type and force it if necessary
					if (ee.tagName() == "type")	{
						int t = AwChannel::stringToType(ee.text());
						if (chan->type() != t)	{
							asRecorded->setType(t);
							chan->setType(t);
						}
					}
					else if (ee.tagName() == "reference") {
							// find reference in asRecorded
						QString ref = ee.text();
						if (asRecordedH.value(ref))
							chan->setReferenceName(ref);
					}
					child = child.nextSibling();
				} // end while (!child.isNull())
				
				// check for reference type
				if (!chan->referenceName().isEmpty()) {
					AwChannel *ref = asRecordedH.value(chan->referenceName());
					AwChannel *asRecorded = asRecordedH.value(ref->name());
					ref->setType(chan->type());
					asRecorded->setType(chan->type());
				}
				channels << chan;
			} //end if(asRecorded)
		} // end if (element.tagName() == "Channel")
		node = node.nextSibling();
	} // end while (!node.isNull())
	return channels;
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
	foreach(QString label, labels)	{
		AwChannel *asRecorded = m_channelHashTable.value(label);
		if (asRecorded == NULL)
			continue;
		m_badChannelLabels << label;
		foreach(AwChannel *c, m_channels) {
			if (c->name() == asRecorded->name() || c->referenceName() == asRecorded->name()) {
				m_channels.removeAll(c);
				updateMontage = true;
			}
		}
	}
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
	AwChannel *asRecorded = m_channelHashTable.value(channelName);
	if (asRecorded) { // channel found
		if (bad) { // set it to bad
			if (!m_badChannelLabels.contains(channelName))
				m_badChannelLabels << channelName;

			asRecorded->setBad(true);
			bool updateMontage = false;
			foreach (AwChannel *c, m_channels) { // check in montage for reference to channelName. If any reference found, remove channel from montage.
				if (c->name() == asRecorded->name() || c->referenceName() == asRecorded->name()) {
					m_channels.removeAll(c);
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
	m_channelsAsRecorded.append(channel);
	// ajout dans la hastable
	m_channelHashTable.insert(channel->name(), channel);
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
	m_path = aws->montageDir;
	if (m_path.isEmpty())
		return;

	QDir montDir(m_path);
	montDir.setNameFilters(filter);
	QStringList files = montDir.entryList();

	// init hash table
	for (int i = 0; i < files.count(); i++)	{
		QString key = files[i];
		key = key.remove(".mtg", Qt::CaseInsensitive);
		m_quickMontages << key;
		m_quickMontagesHT.insert(key, m_path + files[i]);
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
	AwProcessManager::instance()->setMontageChannels(m_channels);
	applyGains();
	newFilters();
	emit montageChanged(m_channels);

}



//
// addChannelsByName()
//
void AwMontageManager::addChannelsByName(AwChannelList &channelsToAdd)
{
	foreach (AwChannel *c, channelsToAdd)	{
		AwChannel *asRecorded = m_channelHashTable.value(c->name());
		if (asRecorded == NULL)
			continue;
		if (asRecorded->isBad())
			continue;
		m_channels << asRecorded->duplicate();

	}
	emit montageChanged(m_channels);
}

//
// Build a new montage based on selected labels from a view.
// Keep the references if any (bipolar channels)

void AwMontageManager::buildNewMontageFomNames(const QStringList& names)
{
	if (names.isEmpty())
		return;

	AwChannelList newMontage;
	for (QString label : names) {
		if (label.contains("-")) {
			QStringList tokens = label.split("-");
			if (tokens.size() < 2)
				continue;
			AwChannel *asRecorded = m_channelHashTable.value(tokens.first().trimmed());
			if (asRecorded) {
				if (!asRecorded->isBad()) {
					AwChannel *new_c = static_cast<AwChannel *>(asRecorded->duplicate());
					new_c->setReferenceName(tokens.at(1).trimmed());
					newMontage << new_c;
				}
			}
		}
		else { // no ref
			AwChannel *asRecorded = m_channelHashTable.value(label);
			if (asRecorded) {
				if (!asRecorded->isBad()) {
					newMontage << asRecorded->duplicate();
				}
			}
		}
	}

	//foreach (QString name, names)	{
	//	AwChannel *asRecorded = m_channelHashTable.value(name);
	//	if (asRecorded == NULL)
	//		continue;
	//	if (asRecorded->isBad())
	//		continue;
	//	newMontage << asRecorded->duplicate();
	//}
	// clear current Montage
	clear();
	// set new one
	m_channels = newMontage;
	emit montageChanged(m_channels);
}