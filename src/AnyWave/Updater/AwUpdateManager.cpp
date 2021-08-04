#include "AwUpdateManager.h"
#include "Prefs/AwSettings.h"
#include <utils/json.h>
#include "AwVersion.h"
#include "Plugin/AwPluginManager.h"
#include <AwProcessInterface.h>
#include <widget/AwMessageBox.h>

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
}

AwUpdateManager::AwUpdateManager(QObject *parent) : QObject(parent)
{
	connect(&m_networkManager, &QNetworkAccessManager::finished, this,
		&AwUpdateManager::fileDownloaded);
	// check for updates only if the option is enabled in the global settings
	auto aws = AwSettings::getInstance();
	if (aws->value(aws::check_updates).toBool())
		checkForUpdates();
}

AwUpdateManager::~AwUpdateManager()
{
	clearComponents();
}

void AwUpdateManager::clearComponents()
{
	while (!m_components.isEmpty())
		delete m_components.takeLast();
	while (!m_installedComponents.isEmpty())
		delete m_installedComponents.takeLast();
}

void AwUpdateManager::checkForUpdates()
{
	clearComponents();
	// get json file
	QString updateUrl = "https://meg.univ-amu.fr/AnyWave/updates/updates.json";
	QUrl url(updateUrl);
	download(url);
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
			m_components << comp;
		}
	}
	if (checkForComponentsUpdates()) {
		int ans = AwMessageBox::question(nullptr, "AnyWave Updates", "Do you want to see available updates?", QMessageBox::Yes | QMessageBox::No);
		if (ans == QMessageBox::Yes) {

		}
	}
}

bool AwUpdateManager::checkForComponentsUpdates()
{
	bool updates = false;
	for (auto c : m_components) {
		if (c->name == "AnyWave") {
			QString version = QString("%1.%2.%3").arg(AW_MAJOR_VERSION).arg(AW_MINOR_VERSION).arg(AW_FIX_VERSION);
			Component* installed = new Component(*c);
			installed->version = version;
			m_installedComponents << installed;
			if (AwUpdateManager::compareVersion(c->version, version) > 0) 
				updates = true;
		}
		else {
			// get the plugin matching the name
			auto plugin = AwPluginManager::getInstance()->getProcessPluginByName(c->name);
			if (plugin) {
				Component* installed = new Component(*c);
				installed->version = plugin->version;
				m_installedComponents << installed;
				if (AwUpdateManager::compareVersion(c->version, plugin->version) > 0) {
					updates = true;
				}
			}
		}
	}
	return false;
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