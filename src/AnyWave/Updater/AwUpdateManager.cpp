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
#include <AwException.h>
//#include <QJsonDocument>
//#include <QJsonObject>

#include <QTemporaryDir>

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
	m_flags = AwUpdateManager::AllUpdates;
	connect(this, &AwUpdateManager::newPluginLoaded, pm, &AwProcessManager::addPlugin);
	connect(this, &AwUpdateManager::installComplete, this, &AwUpdateManager::showInstalledComponents);
	connect(&m_networkManager, &QNetworkAccessManager::finished, this,
		&AwUpdateManager::fileDownloaded);
	connect(this, &AwUpdateManager::downloaded, this, &AwUpdateManager::loadJSON);
}

AwUpdateManager::~AwUpdateManager()
{
	clearComponents();
}

void AwUpdateManager::clearComponents()
{
	//while (!m_components.isEmpty())
	//	delete m_components.takeLast();
}

void AwUpdateManager::checkForUpdates(int flags, bool quiet)
{
	clearComponents();
	m_quiet = quiet;
	m_flags = flags;
	//connect(&m_networkManager, &QNetworkAccessManager::finished, this,
	//	&AwUpdateManager::fileDownloaded);
	// get json file
	QString updateUrl = "https://meg.univ-amu.fr/AnyWave/updates/updates.json";
	auto aws = AwSettings::getInstance();
	if (aws->value(aws::ins_version).toBool()) {
		// get the update.json in app dir
		QString jsonFile = QString("%1/update.json").arg(aws->value(aws::app_resource_dir).toString());
		try {
			auto map = AwUtilities::json::fromJsonFileToMap(jsonFile);
			updateUrl = map.value("url").toString();
		}
		catch (const AwException& e) {
			if (!m_quiet) {
				AwMessageBox::critical(nullptr, "json error", e.errorString());
				return;
			}
			return;
		}
	}
	if (!checkConnectionToUrl(updateUrl)) {
		if (!m_quiet)
			AwMessageBox::critical(nullptr, "Connection error", "The update server could not be contacted");
		return;
	}
	download(updateUrl);
//	connect(this, &AwUpdateManager::downloaded, this, &AwUpdateManager::loadJSON);
}

bool AwUpdateManager::checkConnectionToUrl(const QUrl& url)
{
	QTcpSocket socket;
	auto host = url.host();
	socket.connectToHost(host, url.port());
	if (socket.waitForConnected()) 
		return true;
	return false;
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
//	disconnect(this, &AwUpdateManager::downloaded, nullptr, nullptr);
	QString jsonString(m_data), error;
	QVariantMap map = AwUtilities::json::mapFromJsonString(jsonString, error);

//	QJsonDocument document = QJsonDocument::fromJson(m_data);
//	QVariantMap map = document.object().toVariantMap();
	if (map.isEmpty()) {
		if (!m_quiet)
			AwMessageBox::information(nullptr, "AnyWave Updates", "Could not connect to the update server.");
		return;
	}

	//QString jsonString(m_data), error;
	//QVariantMap map = AwUtilities::json::mapFromJsonString(jsonString, error);
//	if (error.isEmpty()) {
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
		//QMap<QString, int> types = { { "core", AwUpdateManager::Core }, { "plugin", AwUpdateManager::Plugin},
		//	{ "matlab plugin", AwUpdateManager::MatlabPlugin},  { "python plugin", AwUpdateManager::PythonPlugin} };
		QStringList types = { "core", "plugin", "matlab plugin", "python plugin" };
		for (auto const& c : components) {
			auto map = c.toMap();
			if (map.contains("name") && map.contains("type") && map.contains("version") && map.contains("filename")) {
				QString type = map.value("type").toString().toLower();
				if (!types.contains(type))
					continue;

				QSharedPointer<Component> comp = QSharedPointer<Component>(new Component);
				comp->name = map.value("name").toString();
				comp->version = map.value("version").toString();
				comp->type = type;

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
//	}
//	else { // error occured
//		AwMessageBox::critical(nullptr, "Error", error);
//		return;
//	}
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
		//	auto plugin = AwPluginManager::getInstance()->getProcessPluginByName(c->name);
			auto plugin = AwPluginManager::getInstance()->getPlugin(c->name);
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
				updates = true;
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
	try {
		updatePlugin(c);
	}
	catch (const AwException& e) 
	{
		AwMessageBox::critical(nullptr, "Plugin update", e.errorString());
		return;
	}

	//auto pm = AwPluginManager::getInstance();
	//QString pluginPath = QString("%1/%2").arg(aws->value(aws::app_plugins_dir).toString()).arg(c->fileName);
	//bool exists = pm->unloadPlugin(pluginPath, c->name) == 0;
	//QFile::remove(pluginPath);
	//QFile::copy(m_file.fileName(), pluginPath);
	//auto plugin = pm->loadPlugin(pluginPath);
	//if (!exists)
	//	emit newPluginLoaded(plugin);
	m_currentIndex++;
	installUpdates();
}

void AwUpdateManager::updatePlugin(QSharedPointer<Component> c)
{
	auto pm = AwPluginManager::getInstance();
	auto aws = AwSettings::getInstance();
	QString pluginPath;
	QString unzipCommand, unzipArgs;
	QString destPluginPath;
#ifdef Q_OS_WIN
	unzipCommand = "powershell";
	unzipArgs = "-Command";
#endif
	QString filePath = m_file.fileName(); // full path to downloaded file (a zip file)
	// create a temp dir to unzip file
	QTemporaryDir tmpDir;
	if (!tmpDir.isValid())
		throw AwException("Could not create a temporary dir to unzip file.");

	auto plugin = pm->getPlugin(c->name);
	bool exists = plugin != nullptr;
	
	if (exists) {
		pluginPath = plugin->pluginDir;
		pm->unloadPlugin(plugin->pluginDir, c->name);
	}
	QProcess process;
	//QStringList args = { unzipArgs, m_file.fileName(), tmpDir.path() };
#ifdef Q_OS_WIN
	QStringList args = { unzipArgs, QString("Expand-Archive -Path '%1' -DestinationPath '%2'").arg(m_file.fileName()).arg(tmpDir.path())};
#endif
	int status = process.execute(unzipCommand, args);
	if (status != 0) 
		throw AwException("error starting unzip command.");
	QString destPath;
	if (exists)
		destPath = plugin->pluginDir;
	if (c->type == "plugin") {
		// expect one file .so .dylib .dll
		if (!exists) 
			destPath = aws->value(aws::app_plugins_dir).toString();
		auto list = QDir(tmpDir.path()).entryInfoList(QDir::Files);
		QString cppPluginExt;
#ifdef Q_OS_WIN
		cppPluginExt = ".dll";
#endif
#ifdef Q_OS_MAC
		cppPluginExt = ".dylib";
#endif
#ifdef Q_OS_LINUX
		cppPluginExt = ".so";
#endif
		for (auto const& file : list) {
			QString destFile = QString("%1/2").arg(destPath).arg(file.fileName());
			if (file.fileName().endsWith(cppPluginExt)) {
				if (QFile::exists(destFile))
					QFile::remove(destFile);
				QFile::copy(file.absoluteFilePath(), destFile);
				pm->loadPlugin(destFile);
				break;
			}
		}
		
	}
	else if (c->type == "matlab plugin") {
		// expect a dir containing several files
		if (!exists)
			destPath = aws->value(aws::matlab_plugins_dir).toString();
		auto list = QDir(tmpDir.path()).entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot);
		const auto& dir = list.first();
		QString destDir = QString("%1/%2").arg(destPath).arg(dir.fileName());
		if (QDir().exists(destDir)) {
			QDir tmp(destDir);
			tmp.removeRecursively();
		}
#ifdef Q_OS_WIN
		int status = process.execute("powershell", { "-Command", QString("Copy-Item -Path '%1' -Destination '%2' -recurse -force").arg(dir.fileName()).arg(destPath)});
#endif
		if (status != 0) {
			throw AwException("error while copying folder to plugin dir");
		}
	}

	if (!exists)
		emit newPluginLoaded(plugin);

	//	QDir dir(plugin->pluginDir);
	//	dir.removeRecursively();
	//}
	//else {
	//	if (c->type == "plugin")
	//		pluginPath = QString("%1/%1").arg(aws->value(aws::app_plugins_dir).toString().arg(pluginName));
	//	else if (c->type == "matlab plugin") 
	//		pluginPath = QString("%1/%1").arg(aws->value(aws::matlab_plugins_dir).toString().arg(c->fileName));
	//}


}

void AwUpdateManager::updateDownloadProgress(qint64, qint64)
{
	m_file.write(m_reply->readAll());
}

void AwUpdateManager::showInstalledComponents()
{
	AwMessageBox::information(nullptr, "Updates", "Components updated successfully");
}
