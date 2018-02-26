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
#include "AwDisplaySetup.h"
#include "AwViewSetup.h"
#include <QDomElement> // For xml input/output
#include <QFile>
#include <QTextStream>
#include "Widgets/AwMarkersBar.h"
#include "Display/AwDisplay.h"
#include "Prefs/AwSettings.h"

#define LAST_VERSION	"AnyWaveDisplaySetup2.0"

AwDisplaySetup::AwDisplaySetup(const QString& name, QObject *parent)
	: QObject(parent)
{
	if (name.isEmpty())
		m_name = "Default Setup";
	else
		m_name = name;
	m_synchronize = true;
	m_orientation = Horizontal;
}

AwDisplaySetup::AwDisplaySetup(AwDisplaySetup *source, QObject *parent)
	: QObject(parent)
{
	m_synchronize = source->synchronizeViews();

	for (int i = 0; i < viewSetups().size(); i++) {
		//AwDisplaySetupView *dsv = new AwDisplaySetupView(source->viewSetup(i), this);
		AwViewSetup *dsv = new AwViewSetup(source->viewSetup(i), this);
		m_ds << dsv;
	}
	m_orientation = source->orientation();
}


AwDisplaySetup::~AwDisplaySetup()
{
//	while (!m_ds.isEmpty())
//		delete m_ds.takeFirst();
}

void AwDisplaySetup::setSynchronized(bool flag)
{
	m_synchronize = flag;
}

//
// Generates a new setup for a new view by copying the last view setup attached to the current Display Setup
AwViewSetup *AwDisplaySetup::newViewSetup()
{
	AwViewSetup *s;
	if (m_ds.isEmpty())
		s = new AwViewSetup(this);
	else
		s = new AwViewSetup(m_ds.last());
	m_ds << s;
	save();
	return s;
}


void AwDisplaySetup::setToDefault()
{
	// make the setup the Default Setup;
	m_name = "Default Setup";
	m_synchronize = true;
	m_orientation = Horizontal;
	// add a view setup
	while (!m_ds.isEmpty())
		delete m_ds.takeFirst();
	m_ds << new AwViewSetup(this);
}

void AwDisplaySetup::setOrientation(int ori)
{
	if (ori != AwDisplaySetup::Vertical || ori != AwDisplaySetup::Horizontal ||
		ori != AwDisplaySetup::Grid)
		return;

	m_orientation = ori;
}

void AwDisplaySetup::deleteViewSetup(int index)
{
	delete m_ds.takeAt(index);
}

void AwDisplaySetup::setName(const QString& name)
{
	m_name = name;
//	m_fullPath = AwSettings::getInstance()->setupDirectory() + m_name + ".aws";
	m_fullPath = AwSettings::getInstance()->displaySetupDir + m_name + ".aws";

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS

bool AwDisplaySetup::load()
{
	if (m_fullPath.isEmpty())
		return false;
	return loadFromFile(m_fullPath);
}

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
		else if (element.tagName() == "View") {
			AwViewSetup *setup = new AwViewSetup(this);

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
				else if( e.tagName() == "ShowSensors")
					setup->showSensors = e.text() == "true";
				else if (e.tagName() == "LimitNumberOfChannels") {
					setup->limitChannels = e.text() == "true";
					setup->maxChannels = e.attribute("NumberOfChannels").toInt();
				}
				else if (e.tagName() == "MarkersBar")
					if (e.text().toLower() == "show")
						setup->markerBarMode= AwViewSettings::ShowMarkerBar;
					else
						setup->markerBarMode = AwViewSettings::HideMarkerBar;
				else if (e.tagName() == "Filters") {
					// parse node's child list
					QDomNodeList list = n.childNodes();
					setup->filters.clear();
					for (int i = 0; i < list.size(); i++)
						setup->filters << list.at(i).toElement().text().toInt();
				}
				n = n.nextSibling();
			}
			m_ds.append(setup);
		}
		node = node.nextSibling();
	}
	file.close();
	return true;
}

bool AwDisplaySetup::save()
{
	if (m_fullPath.isEmpty())
		return false;
	return saveToFile(m_fullPath);
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

	qint32 count = 0;
	foreach (AwViewSetup *dsv, m_ds) {
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
		if (dsv->markerBarMode == AwViewSettings::ShowMarkerBar)
			e.appendChild(doc.createTextNode("Show"));
		else
			e.appendChild(doc.createTextNode("Hide"));
		element.appendChild(e);
		root.appendChild(element);

		e = doc.createElement("Filters");
		foreach (int f, dsv->filters) {
			QDomElement ee = doc.createElement("Filter");
			
			ee.appendChild(doc.createTextNode(QString("%1").arg(f)));
			e.appendChild(ee);
		}
		element.appendChild(e);
		root.appendChild(element);
	}
	doc.save(out, 3);
	file.close();
	return true;
}
