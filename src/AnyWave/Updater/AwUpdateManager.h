#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
class AwDownloadGui;

class Component
{
public:
	Component();
	Component(const Component&);

	QString name;
	int type;
	QString version, installedVersion;
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
	enum Components { Core, Plugin };

	void checkForUpdates();
	bool updatesAvailable() { return m_updatesAvailable; }
	static int compareVersion(const QString& v1, const QString& v2);
	inline QList<Component*>& components() { return m_components; }
signals:
	void downloaded();
	void newPluginLoaded(QObject*);
private slots:
	void loadJSON();
	void fileDownloaded(QNetworkReply*);
	void error(QNetworkReply::NetworkError error);
	void componentDownloaded(QNetworkReply*);
	void updateDownloadProgress(qint64, qint64);
private:
	void download(const QUrl& url);
	void clearComponents();
	bool checkForComponentsUpdates();
	void installUpdates();

	int m_currentIndex;
	QNetworkAccessManager m_networkManager;
	QNetworkReply* m_reply;
	QByteArray m_data;
	QList<Component*> m_components, m_selectedComponents;
	std::unique_ptr<AwDownloadGui> m_downloadGui;
	QString m_error;
	QFile m_file;
	bool m_updatesAvailable;
};