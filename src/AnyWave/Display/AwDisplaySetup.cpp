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
#include "AwDisplaySetup.h"
#include "AwViewSetup.h"
#include <QDomElement> // For xml input/output
#include <QFile>
#include <QTextStream>
#include "Widgets/AwMarkersBar.h"
#include "Display/AwDisplay.h"
#include "Prefs/AwSettings.h"
#include <AwGainLevels.h>

constexpr auto LAST_VERSION = "AnyWaveDisplaySetup2.0";

AwDisplaySetup::AwDisplaySetup(QObject *parent)
	: QObject(parent)
{
	m_synchronize = true;
	m_orientation = Horizontal;
}

AwDisplaySetup::~AwDisplaySetup()
{
}

void AwDisplaySetup::setSynchronized(bool flag)
{
	m_synchronize = flag;
}

AwViewSettings* AwDisplaySetup::addViewSettings()
{
	auto settings = new AwViewSettings(this);
	int markerMode = AwSettings::getInstance()->value(aws::markerbar_mode_default).toInt();
	if (markerMode == 0)
		settings->markerBarMode = AwViewSettings::Global;
	else
		settings->markerBarMode = AwViewSettings::Classic;
	m_viewSettings << settings;
	return settings;
}

void AwDisplaySetup::setOrientation(int ori)
{
	m_orientation = ori;
}

void AwDisplaySetup::removeViewSettings(int index)
{
	m_viewSettings.removeAt(index);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS

bool AwDisplaySetup::loadFromFile(const QString& path)
{
	QFile file(path);

	if (!file.open(QIODevice::ReadOnly))
		return false;

	QDomDocument doc;
	QString error;
	int row, col;

	if (!doc.setContent(&file, false, &error, &row, &col)) {
		file.close();
		return false;
	}
	QDomElement root = doc.documentElement(); // get root tag
	QDomNode node;
	QDomElement element;
	int viewNumber;

	if (root.tagName() != QString(LAST_VERSION)) {
		file.close();
		return false;
	}
	node = root.firstChild();
	while (!node.isNull()) 	{
		element = node.toElement();
		// read the setup name
		if (element.tagName() == "Name")
			m_name = element.text();
		else if (element.tagName() == "SynchronizedViews")
			m_synchronize = element.text() == "true";
		else if (element.tagName() == "Orientation") {
			if (element.text() == "Vertical")
				m_orientation = AwDisplaySetup::Vertical;
			else
				m_orientation = AwDisplaySetup::Horizontal;
		}
		else if (element.tagName() == "View") {
			auto setup = new AwViewSettings(this);
			// apply default marker mode
			if (AwSettings::getInstance()->value(aws::markerbar_mode_default).toInt() == 0)
				setup->markerBarMode = AwViewSettings::Global;
			else
				setup->markerBarMode = AwViewSettings::Classic;
		
			QDomNode n = element.firstChild();
			while (!n.isNull()) {
				QDomElement e = n.toElement();
				
				if (e.tagName() == "ShowMarkerLabels")
					setup->showMarkerLabels = e.text() == "true";

				else if (e.tagName() == "SecondsPerCm")
					setup->secsPerCm = (float)e.text().toDouble();
				else if (e.tagName() == "ShowMarkerValues")
					setup->showMarkerValues = e.text() == "true";
				else if (e.tagName() == "BaseLineVisible")
					setup->showZeroLine = e.text() == "true";
				else if (e.tagName() == "GridVisible")
					setup->showTimeGrid = e.text() == "true";
				else if (e.tagName() == "DisplaySeconds")
					setup->showSeconds = e.text() == "true";
				else if (e.tagName() == "OverlayChannels")
					setup->stackChannels = e.text() == "true";
				else if (e.tagName() == "ShowSensors")
					setup->showSensors = e.text() == "true";
				else if (e.tagName() == "GridTimingRepresentation") {
					if (e.text() == "ShowRelativeTime")
						setup->timeMode = AwViewSettings::ShowRelativeTime;
					else
						setup->timeMode = AwViewSettings::ShowRecordedTime;
				}
				else if (e.tagName() == "LimitNumberOfChannels") {
					setup->limitChannels = e.text() == "true";
					setup->maxChannels = e.attribute("NumberOfChannels").toInt();
				}
				else if (e.tagName() == "MarkersBar")
					setup->showMarkerBar = e.text().toLower() == "show";
				else if (e.tagName() == "MarkersBarMode") {
					if (e.text().toLower() == "global")
						setup->markerBarMode = AwViewSettings::Global;
					else
						setup->markerBarMode = AwViewSettings::Classic;
				}
				else if (e.tagName() == "Filters") {
					// parse node's child list
					QDomNodeList list = n.childNodes();
					setup->filters.clear();
					for (int i = 0; i < list.size(); i++)
						setup->filters << list.at(i).toElement().text().toInt();
				}
				else if (e.tagName() == "GainLevels") {
					QDomNodeList list = n.childNodes();
					for (int i = 0; i < list.size(); i++) {
						// get type of channel
						int type = list.at(i).toElement().text().toInt();
						auto gl = setup->gainLevels->getGainLevelFor(type);
						// get gain level value
						gl->setValue(list.at(i).toElement().attribute("value").toFloat());
					}
				}
				else if (e.tagName() == "TimeScaleMode")
					setup->timeScaleMode = e.text().toInt();
				else if (e.tagName() == "TimeScaleFixedPageDuration")
					setup->fixedPageDuration = e.text().toFloat();
				else if (e.tagName() == "ChannelSelection") {
					QDomNodeList list = n.childNodes();
					for (int i = 0; i < list.size(); i++) {
						// get channel label
						setup->channelSelection << list.at(i).toElement().text();
					}
				}
				n = n.nextSibling();
			}
			m_viewSettings.append(setup);
		}
		node = node.nextSibling();
	}
	file.close();
	return true;
}

bool AwDisplaySetup::saveToFile(const QString& filename)
{
	QFile file(filename);

	if (!file.open(QIODevice::WriteOnly))
		return false;

	QTextStream out;
	out.setDevice(&file);
	QDomDocument doc("AnyWaveSetup");
	QDomElement root = doc.createElement(QString(LAST_VERSION));
	QDomElement element;
	QString sTrue = "true";
	QString sFalse = "false";

	doc.appendChild(root);

	// new version: store the display setup name (the name will be used in the UI of AnyWave)
	element = doc.createElement("Name");
	element.appendChild(doc.createTextNode(m_name));
	root.appendChild(element);

	element = doc.createElement("SynchronizedViews");
	element.appendChild(doc.createTextNode(m_synchronize ? sTrue : sFalse));
	root.appendChild(element);
	element = doc.createElement("Orientation");
	if (m_orientation == AwDisplaySetup::Vertical)
		element.appendChild(doc.createTextNode("Vertical"));
	else
		element.appendChild(doc.createTextNode("Horizontal"));
	root.appendChild(element);

	qint32 count = 0;
	for (auto dsv : m_viewSettings) {
		QDomElement e;
		
		element = doc.createElement("View");
		root.appendChild(element);

		e = doc.createElement("SecondsPerCm");
		e.appendChild(doc.createTextNode(QString::number((double)dsv->secsPerCm)));
		element.appendChild(e);
		root.appendChild(element);

		e = doc.createElement("ShowMarkerLabels");
		e.appendChild(doc.createTextNode(dsv->showMarkerLabels ? sTrue : sFalse));
		element.appendChild(e);
		root.appendChild(element);

		e = doc.createElement("ShowMarkerValues");
		e.appendChild(doc.createTextNode(dsv->showMarkerValues ? sTrue : sFalse));
		element.appendChild(e);
		root.appendChild(element);		

		e = doc.createElement("BaseLineVisible");
		e.appendChild(doc.createTextNode(dsv->showZeroLine ? sTrue : sFalse));
		element.appendChild(e);
		root.appendChild(element);

		e = doc.createElement("GridVisible");
		e.appendChild(doc.createTextNode(dsv->showTimeGrid ? sTrue : sFalse));
		element.appendChild(e);
		root.appendChild(element);

		e = doc.createElement("DisplaySeconds");
		e.appendChild(doc.createTextNode(dsv->showSeconds ? sTrue : sFalse));
		element.appendChild(e);
		root.appendChild(element);

		e = doc.createElement("OverlayChannels");
		e.appendChild(doc.createTextNode(dsv->stackChannels ? sTrue : sFalse));
		element.appendChild(e);
		root.appendChild(element);

		e = doc.createElement("ShowSensors");
		e.appendChild(doc.createTextNode(dsv->showSensors ? sTrue : sFalse));
		element.appendChild(e);
		root.appendChild(element);

		e = doc.createElement("LimitNumberOfChannels");
		e.appendChild(doc.createTextNode(dsv->limitChannels ? sTrue : sFalse));
		e.setAttribute("NumberOfChannels", QString("%1").arg(dsv->maxChannels));
		element.appendChild(e);
		root.appendChild(element);

		e = doc.createElement("MarkersBar");
		if (dsv->showMarkerBar)
			e.appendChild(doc.createTextNode("Show"));
		else
			e.appendChild(doc.createTextNode("Hide"));
		element.appendChild(e);
		root.appendChild(element);

		e = doc.createElement("MarkersBarMode");
		if (dsv->markerBarMode == AwViewSettings::Global)
			e.appendChild(doc.createTextNode("Global"));
		else
			e.appendChild(doc.createTextNode("Classic"));
		element.appendChild(e);
		root.appendChild(element);

		e = doc.createElement("GridTimingRepresentation");
		if (dsv->timeMode == AwViewSettings::ShowRelativeTime)
			e.appendChild(doc.createTextNode("ShowRelativeTime"));
		else
			e.appendChild(doc.createTextNode("ShowRecordedTime"));
		element.appendChild(e);
		root.appendChild(element);

		e = doc.createElement("Filters");
		for  (int f : dsv->filters) {
			QDomElement ee = doc.createElement("Filter");
			
			ee.appendChild(doc.createTextNode(QString("%1").arg(f)));
			e.appendChild(ee);
		}
		element.appendChild(e);

		e = doc.createElement("GainLevels");
		for (auto gl : dsv->gainLevels->gainLevels()) {
			QDomElement ee = doc.createElement("GainLevel");
			ee.appendChild(doc.createTextNode(QString("%1").arg(gl->type)));
			ee.setAttribute("value", gl->value());
			e.appendChild(ee);
		}
		element.appendChild(e);

		if (dsv->channelSelection.size()) {
			e = doc.createElement("ChannelSelection");
			for (const auto& label : dsv->channelSelection) {
				QDomElement ee = doc.createElement("Channel");
				ee.appendChild(doc.createTextNode(label));
				e.appendChild(ee);
			}
			element.appendChild(e);
		}

		e = doc.createElement("TimeScaleMode");
		e.appendChild(doc.createTextNode(QString("%1").arg(dsv->timeScaleMode)));
		element.appendChild(e);
		e = doc.createElement("TimeScaleFixedPageDuration");
		e.appendChild(doc.createTextNode(QString("%1").arg(dsv->fixedPageDuration)));
		element.appendChild(e);

		root.appendChild(element);
	}
	doc.save(out, 3);
	file.close();
	return true;
}
