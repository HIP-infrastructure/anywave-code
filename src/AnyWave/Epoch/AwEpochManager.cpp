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
#include "AwEpochManager.h"
#include "Marker/AwMarkerManager.h"
#include "AwEpochCreateUi.h"
#include <AwFileIO.h>
#include "Prefs/AwSettings.h"
#include <qdir.h>
#include "Data/AwDataServer.h"
#include "AwAverageDialog.h"

// statics init and definitions
AwEpochManager *AwEpochManager::m_instance = 0;

AwEpochManager *AwEpochManager::instance()
{
	if (!m_instance)
		m_instance = new AwEpochManager;
	return m_instance;
}

void AwEpochManager::destroy()
{
	if (!m_instance)
		return;
	delete m_instance;
	m_instance = NULL;
}

AwEpochManager::AwEpochManager()
{
	// Get the current reader information (total duration)
	m_totalDuration = AwSettings::getInstance()->currentReader()->infos.totalDuration();
	m_dataPath = AwSettings::getInstance()->fileInfo()->filePath();
	load();
	m_reviewWidget = NULL;
}

AwEpochManager::~AwEpochManager()
{
	if (m_reviewWidget)
		delete m_reviewWidget;
	clean();
}

void AwEpochManager::clean()
{
	for (auto condition : m_hashEpochs.values()) {
		if (condition->isConnected())
			AwDataServer::getInstance()->closeConnection(condition);
		delete condition;
	}
	m_hashEpochs.clear();
}


void AwEpochManager::load()
{
	QString path = QDir::toNativeSeparators(m_dataPath + ".epoch");
	if (QFile::exists(path))
		loadEpochFile(path);
	
}

void AwEpochManager::loadEpochFile(const QString& path)
{
	clean();
	QFile file(path);
	QDataStream stream(&file);
	stream.setVersion(QDataStream::Qt_4_4);
	if (!file.open(QIODevice::ReadOnly))
		return;
	int nConditions;
	stream >> nConditions;
	if (nConditions <= 0) { // error
		file.close();
		return;
	}
	for (int i = 0; i < nConditions; i++) {
		QString name;
		float totalDuration, zeroPos;
		int nChannels, type, zeroSample;
		float filters[3];
		stream >> name >> totalDuration >> filters[0] >> filters[1] >> filters[2] >> zeroSample >> zeroPos >> nChannels >> type;
		AwChannelList channels;
		for (int j = 0; j < nChannels; j++) {
			QString name, ref;
			float sr;
			stream >> name >> ref >> sr;
			AwChannel *c = new AwChannel;
			c->setType(type);
			c->setName(name);
			c->setReferenceName(ref);
			c->setSamplingRate(sr);
			channels << c;
		}
		AwEpochTree *tree = new AwEpochTree(name, channels, totalDuration);
		tree->setZeroPos(zeroPos);
		tree->setZeroSample(zeroSample);
		tree->setFilters(filters);
		int nEpochs;
		stream >> nEpochs;
		for (int j = 0; j < nEpochs; j++) {
			float start, duration;
			bool rejected;
			int tags;
			stream >> start >> duration >> rejected >> tags;
			AwEpoch *e = new AwEpoch;
			e->posAndDuration = new AwMarker("epoch", start, duration);
			e->rejected = rejected;
			e->tags = tags;
			e->condition = tree;
			tree->epochs().append(e);
		}
		m_hashEpochs.insert(name, tree);
		AwDataServer::getInstance()->openConnection(tree);
	}
	file.close();
}

void AwEpochManager::saveEpochFile(const QString& path)
{
	if (m_hashEpochs.isEmpty())
		return;
	QFile file(path);
	QDataStream stream(&file);
	stream.setVersion(QDataStream::Qt_4_4);
	if (!file.open(QIODevice::ReadWrite))
		return;
	stream << m_hashEpochs.count();
	for (auto key : m_hashEpochs.keys()) {
		auto condition = m_hashEpochs.value(key);
		float *filters = condition->filters();
		stream << condition->name();
		stream << condition->totalDuration();
		stream << filters[0] << filters[1] << filters[2];
		stream << condition->zeroSample() << condition->zeroPos();
		stream << condition->channels().size();
		// save first channel type
		stream << condition->channels().first()->type();
		for (auto c : condition->channels()) 
			stream << c->name() << c->referenceName() << c->samplingRate();
		
		stream << condition->epochs().size();
		for (auto epoch : condition->epochs()) {
			stream << epoch->posAndDuration->start();
			stream << epoch->posAndDuration->duration();
			stream << epoch->rejected;
			stream << epoch->tags;
		}
	}
	file.close();
}

void AwEpochManager::save()
{
	saveEpochFile(QDir::toNativeSeparators(m_dataPath + ".epoch"));
}

void AwEpochManager::closeFile()
{
	save();
	clean();
}

void AwEpochManager::create()
{
	AwEpochCreateUi dlg(AwMarkerManager::instance()->getMarkers());

	if (dlg.exec() == QDialog::Accepted) 
		visualise();
}

void AwEpochManager::average()
{
	AwAverageDialog dlg;
	if (dlg.exec() == QDialog::Accepted) {

		AwEpochAverageWidget *widget = new AwEpochAverageWidget(dlg.selectedConditions);
		m_avgWidgets.append(widget);
		widget->show();
	}
}

void AwEpochManager::visualise()
{
	if (m_reviewWidget == NULL) {
		m_reviewWidget = new AwEpochVisuWidget;
	}
	m_reviewWidget->show();
}


AwEpochTree *AwEpochManager::getCondition(const QString& name)
{
	if (m_hashEpochs.contains(name))
		return m_hashEpochs.value(name);
	return NULL;
}

/// Try to create a new condition. If a condition with the same name already exists, return it.
/// If the epoching of the condition failed, return a null pointer otherwise return the newly created condition.
///
AwEpochTree *AwEpochManager::createCondition(const QString& name, const AwChannelList& channels, const QString& event, float pre, float post)
{
	if (m_hashEpochs.contains(name))
		return m_hashEpochs.value(name);

	AwEpochTree *cond = new AwEpochTree(name, channels, m_totalDuration, this);
	if (cond->buildEpochs(AwMarkerManager::instance()->getMarkers(), event, pre, post) == 0) {
		m_hashEpochs.insert(name, cond);
		AwDataServer::getInstance()->openConnection(cond);
		return cond;
	}
	return NULL;
}

