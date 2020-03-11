#include <montage/AwMontage.h>
#include <montage/AwAVGChannel.h>
#include <AwException.h>
#include <QFile>
#include <QDomElement>
#include <QDomDocument>
#include <QTextStream>
#include <AwFileIO.h>
#include <AwCore.h>

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
	auto m_badChannelPath = reader->getSideFile("*.bad");
	if (!m_badChannelPath.isEmpty() && QFile::exists(m_badChannelPath))
		loadBadChannels();
	auto mtgFile = reader->getSideFile("*.mtg");
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


void AwMontage::save(const QString& path, const AwChannelList& channels)
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
	doc.save(stream, 3);
	file.close();
}


AwChannelList AwMontage::load(const QString& path)
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
		if (element.tagName() != "Channel") {
			node = node.nextSibling();
			continue;
		}

		name = element.attribute("name");
		child = element.firstChild();
		while (!child.isNull()) {
			QDomElement ee = child.toElement();
			auto tagName = ee.tagName();
			if (tagName == "type")
				type = AwChannel::stringToType(ee.text());
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
			auto asRecorded = m_asRecordedHash[c->name()];
			if (asRecorded->isBad())
				continue;
			if (c->hasReferences() && c->referenceName() != "AVG") {
				if (m_asRecordedHash.contains(c->referenceName())) {
					if (m_asRecordedHash[c->referenceName()]->isBad())
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

void AwMontage::loadBadChannels()
{
	QFile file(m_badChannelPath);
	QTextStream stream(&file);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		m_badChannelLabels.clear();
		while (!stream.atEnd())
			m_badChannelLabels << stream.readLine().trimmed();
		file.close();
	}
	// reset as recorded channels bad status

	for (auto chan : m_asRecorded)
		chan->setBad(false);
	for (auto label : m_badChannelLabels) {
		AwChannel *chan = m_asRecordedHash[label];
		if (chan)
			chan->setBad(true);
	}
	foreach(AwChannel *chan, m_channels) {
		if (m_asRecordedHash.contains(chan->name())) {
			auto asRecorded = m_asRecordedHash[chan->name()];
			if (asRecorded->isBad()) {
				m_channels.removeAll(chan);
				delete chan;
			}
		}
	}
}