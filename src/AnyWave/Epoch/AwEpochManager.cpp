/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
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
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
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
#include <AwException.h>
#include "Montage/AwMontageManager.h"
#include <widget/AwWaitWidget.h>

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
	QFile file(path);
	QString origin("AwEpochManager::loadEpochFile()");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		throw AwException(QString("Failed to open %1 for reading.").arg(path), origin);
		return;
	}
	QJsonParseError error;
	QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
	file.close();

	if (doc.isNull() || doc.isEmpty() || error.error != QJsonParseError::NoError) {
		throw AwException(QString("Json error: %1.").arg(error.errorString()), origin);
		return;
	}
	QJsonObject root = doc.object();
	if (!root.contains("conditions") || !root["conditions"].isArray()) {
		throw AwException(QString("Unknown format."), origin);
		return;
	}
	clean();
	QJsonArray conditions = root["conditions"].toArray();
	for (int i = 0; i < conditions.size(); i++) {
		QJsonObject jsonCondition = conditions[i].toObject();
		QJsonArray flags;
		float pre = (float)jsonCondition["pre_latency"].toDouble();
		float post = (float)jsonCondition["post_latency"].toDouble();
		QString label = jsonCondition["marker"].toString();
		QString name = jsonCondition["condition"].toString();
		int modality = AwChannel::stringToType(jsonCondition["modality"].toString());
		int nEpochs = jsonCondition["number of epochs"].toInt();
		flags = jsonCondition["rejected"].toArray();
		// get channels of matching modality from the current reader.
		auto channels = AwSettings::getInstance()->currentReader()->infos.channels();
		channels = AwChannel::getChannelsOfType(channels, modality);
		// remove bad channels.
		AwMontageManager::instance()->removeBadChannels(channels);
		AwEpochTree *newCondition = createCondition(name, channels, label, pre, post);
		if (newCondition == NULL) {
			throw AwException(QString("Failed to build epochs for condition %1.").arg(name), origin);
			return;
		}
		// check number of epochs generated vs number of epochs in json file
		if (nEpochs != newCondition->epochs().size()) {
			throw AwException(QString("Generated epochs mismatch the number of epochs saved in json file."), origin);
			delete newCondition;
			return;
		}
		// now read rejected flags and update epochs accordingly.
		int j = 0;
		for (auto e : newCondition->epochs()) 
			e->setRejected(flags[j++].toBool());
	}
}

void AwEpochManager::saveEpochFile(const QString& path)
{
	// save to JSON format
	QFile file(path);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
		throw AwException(QString("Failed to open %1 for writing.").arg(path), QString("AwEpochManager::saveEpochFile()"));
		return;
	}
	QJsonObject root;
	QJsonArray conditions;
	for (auto condition : m_hashEpochs.values()) {
		QJsonArray  rejectedFlags;
		QJsonObject jsonCondition;
		jsonCondition["condition"] = condition->name();
		jsonCondition["modality"] = AwChannel::typeToString(condition->modality());
		jsonCondition["pre_latency"] = condition->preLatency();
		jsonCondition["post_latency"] = condition->postLatency();
		jsonCondition["marker"] = condition->markerLabel();
		jsonCondition["number of epochs"] = condition->epochs().size();
		for (auto epoch : condition->epochs()) 
			rejectedFlags.append(epoch->isRejected());
		jsonCondition["rejected"] = rejectedFlags;
		conditions.append(jsonCondition);
	}
	root["conditions"] = conditions;
	QJsonDocument doc(root);
	file.write(doc.toJson());
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
	if (conditions().isEmpty())
		return;

	AwAverageDialog dlg;
	if (dlg.exec() == QDialog::Accepted) {
		

		//AwEpochAverageWidget *widget = new AwEpochAverageWidget(dlg.selectedConditions);
		//m_avgWidgets.append(widget);
		//widget->show();

		AwWaitWidget wait("Averaging...");
		int i = 1;
		for (auto c : dlg.selectedConditions) {
			wait.setText(QString("Loading data for condition %1...").arg(c->name()));

		}
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

