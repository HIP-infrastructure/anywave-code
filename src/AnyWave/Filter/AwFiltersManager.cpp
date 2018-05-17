#include "AwFiltersManager.h"
#include <QFile>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include "AwFilterSettings.h"

// statics
AwFiltersManager *AwFiltersManager::m_instance = NULL;
AwFiltersManager *AwFiltersManager::instance()
{
	if (!m_instance)
		m_instance = new AwFiltersManager;
	return m_instance;
}

AwFiltersManager::AwFiltersManager(QObject *parent) : QObject(parent)
{
	m_ui = new AwFilterSettings;
}

void AwFiltersManager::quit()
{
	m_ui->close();
	delete m_ui;
}

void AwFiltersManager::closeFile()
{
	if (!m_filePath.isEmpty())
		save();
	m_filePath.clear();
}

void AwFiltersManager::showUi()
{
	m_ui->show();
}

void AwFiltersManager::load()
{
	if (!QFile::exists(m_filePath))
		return;
	QFile file(m_filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;
	QString data = file.readAll();
	file.close();
	QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
	if (doc.isNull())
		return;
	QJsonObject obj = doc.object();

	// get objects as in FilteringOptions
	QJsonValue value = obj.value("MEG");
	QJsonObject meg = value.toObject();
	m_fo.megHP = meg["HP"].toDouble();
	m_fo.megLP = meg["LP"].toDouble();
	m_fo.megNotch = meg["Notch"].toDouble();

	value = obj.value("EEG");
	QJsonObject eeg = value.toObject();
	m_fo.eegHP = eeg["HP"].toDouble();
	m_fo.eegLP = eeg["LP"].toDouble();
	m_fo.eegNotch = eeg["Notch"].toDouble();

	value = obj.value("EMG");
	QJsonObject emg = value.toObject();
	m_fo.emgHP = emg["HP"].toDouble();
	m_fo.emgLP = emg["LP"].toDouble();
	m_fo.emgNotch = emg["Notch"].toDouble();

	emit filtersChanged(&m_fo);
}


void AwFiltersManager::reset()
{
	m_fo.clear();
}

void AwFiltersManager::save()
{
	if (m_filePath.isEmpty())
		return;

	QFile file(m_filePath);
	if (!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))
		return;

	QJsonObject jObject, jMEG, jEEG, jEMG;
	jMEG.insert("HP", QJsonValue::fromVariant(m_fo.megHP));
	jMEG.insert("LP", QJsonValue::fromVariant(m_fo.megLP));
	jMEG.insert("Notch", QJsonValue::fromVariant(m_fo.megNotch));

	jEEG.insert("HP", QJsonValue::fromVariant(m_fo.eegHP));
	jEEG.insert("LP", QJsonValue::fromVariant(m_fo.eegLP));
	jEEG.insert("Notch", QJsonValue::fromVariant(m_fo.eegNotch));

	jEMG.insert("HP", QJsonValue::fromVariant(m_fo.emgHP));
	jEMG.insert("LP", QJsonValue::fromVariant(m_fo.emgLP));
	jEMG.insert("Notch", QJsonValue::fromVariant(m_fo.emgNotch));
	
	jObject.insert("MEG", jMEG);
	jObject.insert("EEG", jEEG);
	jObject.insert("EMG", jEMG);

	QJsonDocument doc(jObject);
	file.write(doc.toJson());
	file.close();
}

void AwFiltersManager::setFilename(const QString& filename)
{
	m_filePath = filename + ".flt";
	if (QFile::exists(m_filePath))
		load();
	else
		reset();
}