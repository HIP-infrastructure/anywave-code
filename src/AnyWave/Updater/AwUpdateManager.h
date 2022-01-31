#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
class AwDownloadGui;
#include <memory>

class Component
{
public:
	Component();
	Component(const Component&);

	QString name;
	int type;
	QString version, installedVersion, requirements;
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
//	inline QList<Component*>& components() { return m_components; }
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
private:
	void download(const QUrl& url);
	void clearComponents();
	bool checkForComponentsUpdates();
	void installUpdates();
	bool checkConnectionToUrl(const QUrl&);

	int m_currentIndex;
	QNetworkAccessManager m_networkManager;
	QNetworkReply* m_reply;
	QByteArray m_data;
//	QList<Component*> m_components, m_selectedComponents;

	QList<QSharedPointer<Component>> m_components, m_selectedComponents;
	std::unique_ptr<AwDownloadGui> m_downloadGui;
	QString m_error;
	QFile m_file;
	int m_flags;
	bool m_updatesAvailable, m_quiet;
};