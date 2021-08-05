#pragma once

#include <QDialog>
#include "ui_AwDownloadGui.h"
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class Component;

class AwDownloadGui : public QDialog
{
	Q_OBJECT

public:
	AwDownloadGui(QWidget *parent = Q_NULLPTR);
	~AwDownloadGui();
public slots:
	void setText(const QString&);
	void updateDownloadProgress(qint64, qint64);

private:
	Ui::AwDownloadGuiUi m_ui;
};
