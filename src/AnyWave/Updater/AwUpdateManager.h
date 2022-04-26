#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
class AwDownloadGui;
#include <memory>
#include <qtemporarydir.h>

class Component
{
public:
	Component();
	Component(const Component&);

	QString name;
	QString type;
	QString version, installedVersion, runtime;
	QUrl url;
	bool updateAvailable;
	QString requirement;
	QString fileName;
};

class AwUpdateManager : public QObject
{
	Q_OBJECT
public:
	AwUpdateManager(QObject *parent = nullptr);
	~AwUpdateManager();
	enum Components { Core, Plugin, MatlabPlugin, PythonPlugin };
	enum Flags { AllUpdates, AvailablePlugins };

	void checkForUpdates(int flags = AwUpdateManager::AllUpdates, bool quiet = true);
	bool updatesAvailable() { return m_updatesAvailable; }
	static int compareVersion(const QString& v1, const QString& v2);
	QList<QSharedPointer<Component>>& components() {return m_components;	}
signals:
	void downloaded();
	void newPluginLoaded(QObject*);
	void installComplete();
private slots:
	void loadJSON();
	void fileDownloaded(QNetworkReply*);
	void error(QNetworkReply::NetworkError error);
	void componentDownloaded(QNetworkReply*);
	void updateDownloadProgress(qint64, qint64);
	void showInstalledComponents();
	void sslErrors(const QList<QSslError>&);
private:
	void download(const QUrl& url);
	void updatePlugin(QSharedPointer<Component> component);
	void clearComponents();
	bool checkForComponentsUpdates();
	void installUpdates();
	bool checkConnectionToUrl(const QUrl&);

	int m_currentIndex;
	QNetworkAccessManager m_networkManager;
	QNetworkReply* m_reply;
	QByteArray m_data;
	QTemporaryDir m_tmpDir;

	QList<QSharedPointer<Component>> m_components, m_selectedComponents;
	std::unique_ptr<AwDownloadGui> m_downloadGui;
	QString m_error;
	QFile m_file;
	int m_flags;
	bool m_updatesAvailable, m_quiet;
};