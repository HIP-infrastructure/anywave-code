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
#include <montage/AwMontage.h>
#include <montage/AwAVGChannel.h>
#include <AwException.h>
#include <QFile>
#include <QDomElement>
#include <QDomDocument>
#include <QTextStream>
#include <AwFileIO.h>
#include <AwCore.h>
#include <QRegularExpression>

AwMontage::AwMontage(AwFileIO *reader)
{
	m_reader = reader;
	auto channels = reader->infos.channels();
	// init as recorded channels list
	for (auto c : channels) {
		// insert in hash table
		m_asRecordedHash[c->name()] = c;
		m_asRecorded << c;
		// make a copy a as recorded channel and insert it in channels.
		// do not insert Reference channels in default montage
		if (!c->isReference())
			m_channels << c->duplicate();
	}
	auto m_badChannelPath = reader->infos.badFile();
	if (!m_badChannelPath.isEmpty() && QFile::exists(m_badChannelPath))
		loadBadChannels();
	auto mtgFile = reader->infos.mtgFile();
	if (!mtgFile.isEmpty() && QFile::exists(mtgFile)) 
		load(mtgFile);
	// remove null channels if any
	foreach(AwChannel *c, m_channels)
		if (c == Q_NULLPTR) {
			m_channels.removeAll(c);
			m_asRecordedHash.remove(c->name());
		}
	
}

AwMontage::~AwMontage()
{
	AW_DESTROY_LIST(m_channels);
}


void AwMontage::save(const QString& path, const AwChannelList& channels, const AwChannelList& asRecordedChannels)
{
	QFile file(path);

	if (!file.open(QIODevice::WriteOnly)) {
		throw AwException("failed to open the file for writting.", "AwMontage::save");
		return;
	}

	QTextStream stream(&file);
	QDomDocument doc("AnyWaveMontage");
	QDomElement root = doc.createElement("Montage");
	QDomElement element;
	QDomElement child;

	doc.appendChild(root);
	for (auto chan : channels) {
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
		child.setAttribute("notch", chan->notch());
		element.appendChild(child);
	}
	if (asRecordedChannels.size()) {
		auto asRecordedRoot = doc.createElement("AsRecordedChannels");
		root.appendChild(asRecordedRoot);
		for (auto chan : asRecordedChannels) {
			element = doc.createElement("Channel");
			element.setAttribute("name", chan->name());
			element.setAttribute("type", AwChannel::typeToString(chan->type()));
			asRecordedRoot.appendChild(element);
		}
	}

	doc.save(stream, 3);
	file.close();
}



AwChannelList AwMontage::load(const QString& path, QMap<QString, int> *asRecordedChannels)
{
	AwChannelList res;
	QFile file(path);
	QString origin = "AwMontage::load";

	if (!file.open(QIODevice::ReadOnly)) {
		throw AwException(QString("Failed to open %1").arg(path), origin);
		return res;
	}
	QDomDocument doc;
	QDomElement element;
	QString error;
	int line, col;

	if (!doc.setContent(&file, &error, &line, &col)) {
		throw AwException(QString("%1 line %2 column %3").arg(error).arg(line).arg(col), origin);
		file.close();
		return res;
	}
	file.close();

	QDomElement root = doc.documentElement(); // get root tag
	if (root.tagName() != "Montage") {
		throw AwException(QString("This is not a valid Montage file."), origin);
		return res;
	}
	QDomNode node = root.firstChild();


	while (!node.isNull()) {
		element = node.toElement();
		QString name, color = "black", ref;
		float lp = -1, hp = -1, notch = -1;
		int type = AwChannel::Other;
		QDomNode child;
		if (element.tagName() == "Channel") {
			name = element.attribute("name");
			child = element.firstChild();
			while (!child.isNull()) {
				QDomElement ee = child.toElement();
				auto tagName = ee.tagName();
				if (tagName == "type") {
					type = AwChannel::stringToType(ee.text());
					if (type == -1)
						throw AwException("Invalid channel type in montage");
				}
				else if (tagName == "color")
					color = ee.text();
				else if (tagName == "reference")
					ref = ee.text();
				else if (tagName == "filters") {
					lp = ee.attribute("lowPass", "-1.").toDouble();
					hp = ee.attribute("highPass", "-1.").toDouble();
					notch = ee.attribute("notch", "-1.").toDouble();
				}
				child = child.nextSibling();
			}
			auto channel = new AwChannel;
			channel->setName(name);
			channel->setType(type);
			channel->setReferenceName(ref);
			channel->setLowFilter(lp);
			channel->setHighFilter(hp);
			channel->setNotch(notch);
			channel->setColor(color);
			res << channel;
		}
		else if (element.tagName() == "AsRecordedChannels" && asRecordedChannels) {
			child = element.firstChild();
			while (!child.isNull()) {
				auto item = child.toElement();
				QString name = item.attribute("name");
				int type = AwChannel::stringToType(item.attribute("type"));
				asRecordedChannels->insert(name, type);
				child = child.nextSibling();
			}
		}
		node = node.nextSibling();
	}
	return res;
}


bool AwMontage::loadMontage(const QString& mtgFile)
{
	try {
		m_channels = AwMontage::load(mtgFile);
	}
	catch (const AwException& e)
	{
		return false;
	}
	// duplicate channels and put them in the montage if they match the settings (bad, references, etc.)
	foreach(AwChannel *c, m_channels) {
		if (!m_asRecordedHash.contains(c->name())) {
			m_channels.removeAll(c);
			delete c;
			continue;
		}
		if (!m_asRecordedHash.contains(c->name())) {
			auto asRecorded = m_asRecordedHash.value(c->name());
			if (asRecorded->isBad())
				continue;
			if (c->hasReferences() && c->referenceName() != "AVG") {
				if (m_asRecordedHash.contains(c->referenceName())) {
					if (m_asRecordedHash.value(c->referenceName())->isBad())
						continue;
				}
				else
					c->clearRefName();
			}
			asRecorded->setType(c->type());
		}
		else {  // channel not present in as recorded
			// checking for special names
			if (c->name() == "SEEG_AVG") {
				m_channels << new AwAVGChannel(AwChannel::SEEG);
			}
			else if (c->name() == "EEG_AVG") {
				m_channels << new AwAVGChannel(AwChannel::EEG);
			}
			else if (c->name() == "MEG_AVG") {
				m_channels << new AwAVGChannel(AwChannel::MEG);
			}
		}
	}

	return true;
}

QStringList AwMontage::loadBadChannels(const QString & filePath)
{
	QStringList res;
	QFile file(filePath);
	QTextStream stream(&file);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		while (!stream.atEnd())
			res << stream.readLine().trimmed();
		file.close();
	}
	return res;
}

/// <summary>
///  From a channel list, create a list containing only bipolar channels.
/// </summary>
/// <param name="channels"></param>
/// <returns> list of new channels montaged as bipolar. Channels are duplicated from original list.</returns>
AwChannelList AwMontage::createSEEGBipolarMontage(const AwChannelList& channels, const QStringList& badLabels)
{
	// store channels by their names
	QMap<QString, AwChannel*> hash;
	for (auto c : channels)
		hash.insert(c->name(), c);

	AwChannelList res;
	QRegularExpression exp("(\\d+)$");
	QRegularExpression expZeros("^(0+)");
	QRegularExpressionMatch match, matchZero;
	for (auto c : channels) {
		if (badLabels.contains(c->name()))
			continue;
		match = exp.match(c->name());
		if (match.hasMatch()) {
			QString zeros;
			auto baseName = c->name();
			baseName = baseName.remove(exp);
			auto number = match.captured(1);
			// find leading zeros
			matchZero = expZeros.match(number);
			if (matchZero.hasMatch()) {
				zeros = matchZero.captured(1);
				number = number.remove(expZeros);
			}
			int n = number.toInt() + 1;

			// find the reference without considering zeros in number
			QString ref = QString("%1%2").arg(baseName).arg(n);
			if (!hash.contains(ref))
				ref = QString("%1%2%3").arg(baseName).arg(zeros).arg(n);
			if (hash.contains(ref) && !badLabels.contains(ref)) {
				c->setReferenceName(ref);
				res << c->duplicate();
			}
		}
	}
	return res;
}

/// <summary>
/// remove bad channels in the list passed as first paremeter based on the list of the bad labels.
/// the channels are removed and destroyed.
/// </summary>
/// <param name="channels">list of channels</param>
/// <param name="badLabels">list of bad labels</param>
void AwMontage::removeBadChannels(AwChannelList& channels, const QStringList& badLabels)
{
	if (badLabels.isEmpty())
		return;
	AwChannelList tmp;
	for (auto c : channels) {
		if (badLabels.contains(c->name()) || badLabels.contains(c->referenceName()))
			tmp << c;
	}
	for (auto c : tmp)
		channels.removeAll(c);
	qDeleteAll(tmp);
	tmp.clear();
}

/// <summary>
/// getElectrodeLabelAndIndex()
/// Should be applied to SEEG channel. Extract the base label name and the electrode number. (ie. A1 => A andt 1)
/// </summary>
/// <param name="chan"></param>
/// <param name="label"></param>
/// <returns></returns>
int AwMontage::getElectrodeLabelAndIndex(AwChannel* chan, QString& label)
{
	QRegularExpression exp("(\\d+)$");
	QRegularExpressionMatch match;

	match = exp.match(chan->name());
	if (match.hasMatch()) {
		label = chan->name();
		label = label.remove(exp);
		return match.captured(1).toInt();
	}
	return -1; // no electrode number in electrode label.
}


void AwMontage::loadBadChannels()
{
	m_badChannelLabels = AwMontage::loadBadChannels(m_badChannelPath);
	// reset as recorded channels bad status
	for (auto chan : m_asRecorded)
		chan->setBad(false);
	for (auto label : m_badChannelLabels) {
		AwChannel *chan = m_asRecordedHash.value(label);
		if (chan)
			chan->setBad(true);
	}
	foreach(AwChannel *chan, m_channels) {
		if (m_asRecordedHash.contains(chan->name())) {
			auto asRecorded = m_asRecordedHash.value(chan->name());
			if (asRecorded->isBad()) {
				m_channels.removeAll(chan);
				delete chan;
			}
		}
	}
}