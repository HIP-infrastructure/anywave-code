#include "AwFiltersManager.h"
#include <QFile>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include "AwFilterSettings.h"
#include <widget/AwMessageBox.h>

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
	closeFile();
	m_ui->close();
	delete m_ui;
}

void AwFiltersManager::closeFile()
{
	if (!m_filePath.isEmpty())
		save();
	m_filePath.clear();
	m_sourceSettings.clear();
	m_icaSettings.clear();
	m_ui->closeFile();
	m_fo.clear();
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

	m_ui->updateFilters();
	emit filtersChanged(&m_fo);
}


void AwFiltersManager::reset()
{
	m_fo.clear();
	m_ui->reset();
}

///
/// switch filter on or off (if flag is true = on)
void AwFiltersManager::switchFilters(bool flag)
{
	if (flag) {
		m_fo = m_copiedFo;
		m_ui->updateFilters();
	}
	else {
		m_copiedFo = m_fo;
		m_fo.clear();
		//m_ui->disableFilters();
	}
	emit filtersChanged(&m_fo);
}

void AwFiltersManager::update()
{
	// check for ica or source settings
	bool sourceWarning = false, icaWarning = false;
	for (auto t : m_sourceSettings.keys()) {
		QPair<float, float> values = m_sourceSettings.value(t);
		switch (t) {
		case AwChannel::EEG:
		case AwChannel::SEEG:
			if (m_fo.eegHP < values.first) {
				sourceWarning = true;
				break;
			}
			if (m_fo.eegLP > values.second) {
				sourceWarning = true;
				break;
			}
			break;
		case AwChannel::MEG:
		case AwChannel::GRAD:
			if (m_fo.eegHP < values.first) {
				sourceWarning = true;
				break;
			}
			if (m_fo.eegLP > values.second) {
				sourceWarning = true;
				break;
			}
			break;
		case AwChannel::EMG:
		case AwChannel::ECG:
			if (m_fo.emgHP < values.first) {
				sourceWarning = true;
				break;
			}
			if (m_fo.emgLP > values.second) {
				sourceWarning = true;
				break;
			}
			break;
		}
		if (sourceWarning)
			break;
	}
	if (sourceWarning) {
		AwMessageBox::information(0, "Source Warning", "The filters applied on signals differ from the settings applied when the Source channels were computed.");
	}
	for (auto t : m_icaSettings.keys()) {
		QPair<float, float> values = m_icaSettings.value(t);
		switch (t) {
		case AwChannel::EEG:
		case AwChannel::SEEG:
			if (m_fo.eegHP < values.first) {
				icaWarning = true;
				break;
			}
			if (m_fo.eegLP > values.second) {
				icaWarning = true;
				break;
			}
			break;
		case AwChannel::MEG:
		case AwChannel::GRAD:
			if (m_fo.megHP < values.first) {
				icaWarning = true;
				break;
			}
			if (m_fo.megLP > values.second) {
				icaWarning = true;
				break;
			}
			break;
		case AwChannel::EMG:
		case AwChannel::ECG:
			if (m_fo.emgHP < values.first) {
				icaWarning = true;
				break;
			}
			if (m_fo.emgLP > values.second) {
				icaWarning = true;
				break;
			}
			break;
		}
		if (icaWarning)
			break;
	}
	if (icaWarning) {
		AwMessageBox::information(0, "ICA Warning", "The filters applied on signals differ from the settings applied when the ICA channels were computed.");
	}

	emit filtersChanged(&m_fo);
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


void AwFiltersManager::setSourceSettings(int type, float hp, float lp)
{
	if (m_sourceSettings.contains(type))
		m_sourceSettings.remove(type);
	m_sourceSettings.insert(type, QPair<float, float>(hp, lp));
	// force current filter to suit Source settings
	switch (type) {
	case AwChannel::EEG:
		m_fo.eegHP = hp;
		m_fo.eegLP = lp;
		break;
	case AwChannel::MEG:
		m_fo.megHP = hp;
		m_fo.megLP = lp;
		break;
	case AwChannel::EMG:
		m_fo.emgHP = hp;
		m_fo.emgLP = lp;
		break;
	}
	m_ui->setSourceSettings(type, hp, lp);
	m_ui->updateFilters();
	emit filtersChanged(&m_fo);
}

void AwFiltersManager::setICASettings(int type, float hp, float lp)
{
	if (m_icaSettings.contains(type))
		m_icaSettings.remove(type);
	m_icaSettings.insert(type, QPair<float, float>(hp, lp));
	// force current filter to suit ICA settings
	switch (type) {
	case AwChannel::EEG:
		m_fo.eegHP = hp;
		m_fo.eegLP = lp;
		break;
	case AwChannel::MEG:
		m_fo.megHP = hp;
		m_fo.megLP = lp;
		break;
	case AwChannel::EMG:
		m_fo.emgHP = hp;
		m_fo.emgLP = lp;
		break;
	}
	m_ui->setICASettings(type, hp, lp);
	m_ui->updateFilters();
	emit filtersChanged(&m_fo);
}