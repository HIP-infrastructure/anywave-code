#include <AwMontage.h>
#include <AwException.h>
#include <QFile>
#include <QDomElement>
#include <QDomDocument>


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
		float lp = -1, hp = -1;
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
				lp = ee.attribute("lowPass").toDouble();
				hp = ee.attribute("highPass").toDouble();
			}
			child = child.nextSibling();
		}
		auto channel = new AwChannel;
		channel->setName(name);
		channel->setType(type);
		channel->setReferenceName(ref);
		channel->setLowFilter(lp);
		channel->setHighFilter(hp);
		channel->setColor(color);
		res << channel;
		node = node.nextSibling();
	}
	return res;
}