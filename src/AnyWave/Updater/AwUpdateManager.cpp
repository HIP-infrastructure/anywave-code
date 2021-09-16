#include "AwUpdateManager.h"
#include "Prefs/AwSettings.h"
#include <utils/json.h>
#include "AwVersion.h"
#include "Plugin/AwPluginManager.h"
#include <AwProcessInterface.h>
#include <widget/AwMessageBox.h>
#include "AwUpdaterGui.h"
#include "AwDownloadGui.h"
#include <QStandardPaths>
#include <QProcess>
#include "Process/AwProcessManager.h"
#include <cmath>


Component::Component()
{
	updateAvailable = false;
	type = AwUpdateManager::Core;
}

Component::Component(const Component& copy)
{
	name = copy.name;
	version = copy.version;
	updateAvailable = copy.updateAvailable;
	type = copy.type;
	url = copy.url;
	installedVersion = copy.installedVersion;
	requirement = copy.requirement;
}

AwUpdateManager::AwUpdateManager(QObject *parent) : QObject(parent)
{
	// check for updates only if the option is enabled in the global settings
	auto aws = AwSettings::getInstance();
	auto pm = AwProcessManager::instance();
	m_updatesAvailable = false;
	m_quiet = true;
	connect(this, &AwUpdateManager::newPluginLoaded, pm, &AwProcessManager::addPlugin);
	connect(this, &AwUpdateManager::installComplete, this, &AwUpdateManager::showInstalledComponents);
}

AwUpdateManager::~AwUpdateManager()
{
	clearComponents();
}

void AwUpdateManager::clearComponents()
{
	while (!m_components.isEmpty())
		delete m_components.takeLast();
}

void AwUpdateManager::checkForUpdates(bool quiet)
{
	clearComponents();
	m_quiet = quiet;
	connect(&m_networkManager, &QNetworkAccessManager::finished, this,
		&AwUpdateManager::fileDownloaded);
	// get json file
	QString updateUrl = "https://meg.univ-amu.fr/AnyWave/updates/updates.json";
	auto aws = AwSettings::getInstance();
	if (aws->value(aws::ins_version).toBool()) {
		// get the update.json in app dir
		QString jsonFile = QString("%1/update.json").arg(aws->value(aws::app_resource_dir).toString());
		auto map = AwUtilities::json::fromJsonFileToMap(jsonFile);
		updateUrl = map.value("url").toString();
	}
	connect(this, &AwUpdateManager::downloaded, this, &AwUpdateManager::loadJSON);
}

void AwUpdateManager::download(const QUrl& url)
{
	QNetworkRequest request(url);
	m_networkManager.get(request);
}

void AwUpdateManager::fileDownloaded(QNetworkReply* reply)
{
	m_data = reply->readAll();
	reply->deleteLater();
	emit downloaded();
}

void AwUpdateManager::error(QNetworkReply::NetworkError error)
{
	if (error == QNetworkReply::NoError)
		return;

	switch (error)
	{
	case QNetworkReply::ConnectionRefusedError:
		m_error = "Connection refused.";
		break;

	}
}

void AwUpdateManager::loadJSON()
{
	disconnect(this, &AwUpdateManager::downloaded, nullptr, nullptr);
	QString jsonString(m_data), error;
	QVariantMap map = AwUtilities::json::mapFromJsonString(jsonString, error);
	if (error.isEmpty()) {
		QVariantMap platform;
#ifdef Q_OS_WIN
		if (map.contains("windows"))
			platform = map.value("windows").toMap();
#endif
#ifdef Q_OS_MAC
		if (map.contains("macos"))
			platform = map.value("macos").toMap();
#endif
#ifdef Q_OS_LINUX
		if (map.contains("linux"))
			platform = map.value("linux").toMap();
#endif
		auto components = platform.value("components").toList();
		QStringList names;
		for (auto const& c : components) {
			auto map = c.toMap();
			Component* comp = new Component;
			comp->name = map.value("name").toString();
			comp->version = map.value("version").toString();
			comp->type = map.value("type").toString() == "core" ? AwUpdateManager::Core : AwUpdateManager::Plugin;
			comp->url = QUrl(map.value("url").toString());
			if (map.contains("requirement")) 
				comp->requirement = map.value("requirement").toString();
			comp->fileName = map.value("filename").toString();

			m_components << comp;
		}
	}
	if (checkForComponentsUpdates()) {

		AwUpdaterGui gui(this);
		if (gui.exec() == QDialog::Accepted) {
			m_selectedComponents = gui.selectedComponents();
			m_currentIndex = 0;
			disconnect(&m_networkManager, &QNetworkAccessManager::finished, nullptr, nullptr);
			connect(&m_networkManager, &QNetworkAccessManager::finished, this,
				&AwUpdateManager::componentDownloaded);
			m_downloadGui = std::make_unique<AwDownloadGui>();
			m_downloadGui.get()->show();
			installUpdates();
		//	m_updatesAvailable = false;
		}
	}
	else {
		//m_updatesAvailable = false;
		if (!m_quiet)
			AwMessageBox::information(nullptr, "AnyWave Updates", "Everything is up to date.");
	}
}

bool AwUpdateManager::checkForComponentsUpdates()
{
	bool updates = false;
	QString anywaveVersion = QString("%1.%2.%3").arg(AW_MAJOR_VERSION).arg(AW_MINOR_VERSION).arg(AW_FIX_VERSION);
	for (auto c : m_components) {
		if (c->name == "AnyWave") {
			c->installedVersion = anywaveVersion;
			if (AwUpdateManager::compareVersion(c->version, anywaveVersion) > 0) {
				updates = true;
				c->updateAvailable = true;
			}
		}
		else {
			// get the plugin matching the name
			auto plugin = AwPluginManager::getInstance()->getProcessPluginByName(c->name);
			if (plugin) {
				// check requirement for plugin
				if (AwUpdateManager::compareVersion(c->requirement, anywaveVersion) <= 0) {  // anywave version is ok for the plugin
					c->installedVersion = plugin->version;
					if (AwUpdateManager::compareVersion(c->version, plugin->version) > 0) {
						updates = true;
						c->updateAvailable = true;
					}
				}
			}
			else { // the plugin is not installed yet
				c->installedVersion = "Not installed";
				c->updateAvailable = true;
			}
		}
	}
	return updates;
}

int AwUpdateManager::compareVersion(const QString& v1, const QString& v2)
{
	QStringList t1 = v1.split('.');
	QStringList t2 = v2.split('.');

	Q_ASSERT(t1.size() ==  t2.size());
	
	for (int i = 0; i < t1.size(); i++) {
		int value1 = t1.at(i).toInt();
		int value2 = t2.at(i).toInt();
		if (value1 > value2) 
			return 1;
		if (value1 < value2)
			return -1;
	}
	return 0;
}


void AwUpdateManager::installUpdates()
{
	if (m_currentIndex == m_selectedComponents.size()) {
		m_downloadGui.get()->close();
		emit installComplete();
		return;
	}
	auto c = m_selectedComponents.at(m_currentIndex);
	QString downloadsFolder = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
	Q_ASSERT(!downloadsFolder.isEmpty());
	QString filePath = QString("%1/%2").arg(downloadsFolder).arg(c->fileName);
	m_file.setFileName(filePath);
	if (!m_file.open(QIODevice::WriteOnly)) {
		AwMessageBox::critical(nullptr, "Error", QString("Fatal error: could not create file %1").arg(filePath));
		return;
	}

	m_downloadGui->setText(QString("Downloading %1...").arg(c->name));
	QNetworkRequest request(c->url);
	m_reply = m_networkManager.get(request);
#if QT_VERSION >= QT_VERSION_CHECK(5,15,0)
	connect(m_reply, &QNetworkReply::errorOccurred, this, &AwUpdateManager::error);
#endif
	connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)), m_downloadGui.get(), SLOT(updateDownloadProgress(qint64, qint64)));
	connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(updateDownloadProgress(qint64, qint64)));

}


void AwUpdateManager::componentDownloaded(QNetworkReply *reply)
{
	reply->deleteLater();
	m_file.close();
	// install the update
	auto c = m_selectedComponents.at(m_currentIndex);
	if (c->name == "AnyWave") {
	#ifdef Q_OS_WIN
		QProcess::startDetached(m_file.fileName());
		exit(0);
	#endif
	#ifdef Q_OS_MAC
		QProcess::startDetached("open", {m_file.fileName()});
		exit(0);
	#endif
	#ifdef Q_OS_LINUX
		QProcess::startDetached("nautilus",  {m_file.fileName()});
		exit(0);
	#endif
	}
	auto aws = AwSettings::getInstance();
	// plugin
	auto pm = AwPluginManager::getInstance();
	QString pluginPath = QString("%1/%2").arg(aws->value(aws::app_plugins_dir).toString()).arg(c->fileName);
	bool exists = pm->unloadPlugin(pluginPath, c->name) == 0;
	QFile::remove(pluginPath);
	QFile::copy(m_file.fileName(), pluginPath);
	auto plugin = pm->loadPlugin(pluginPath);
	if (!exists)
		emit newPluginLoaded(plugin);
	m_currentIndex++;
	installUpdates();
}

void AwUpdateManager::updateDownloadProgress(qint64, qint64)
{
	m_file.write(m_reply->readAll());
}

void AwUpdateManager::showInstalledComponents()
{
	AwMessageBox::information(nullptr, "Updates", "Components updated successfully");
}
