#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>


class Component
{
public:
	Component();
	Component(const Component&);

	QString name;
	int type;
	QString version;
	QUrl url;
	bool updateAvailable;
};

class AwUpdateManager : public QObject
{
	Q_OBJECT
public:
	AwUpdateManager(QObject *parent = nullptr);
	~AwUpdateManager();
	enum Components { Core, Plugin };

	void checkForUpdates();
	static int compareVersion(const QString& v1, const QString& v2);
signals:
	void updatesAvailable();
	void downloaded();
private slots:
	void loadJSON();
	void fileDownloaded(QNetworkReply*);
private:
	void download(const QUrl& url);
	void clearComponents();
	bool checkForComponentsUpdates();

	QNetworkAccessManager m_networkManager;
	QByteArray m_data;
	QList<Component*> m_components, m_installedComponents;
};