#pragma once
#include <QObject>

class AwComponents : public QObject
{
	Q_OBJECT
public:
	explicit AwComponents(QObject *parent = nullptr);
	~AwComponents();
	int init();
	void setGuiEnabled(bool flag = true);
signals:
	void log(const QString&);
protected:
	void createUserDirs();
	void initMatlab(); 
	bool checkAndCreateFolder(const QString& root, const QString& name);
	bool searchForMatlab();
	bool m_guiMode;
};