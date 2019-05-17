#include <AwMontage.h>
#include <AwException.h>
#include <QFile>
#include <QDomElement>
#include <QDomDocument>
#include <QTextStream>


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