#pragma once

#include <QWidget>
#include "ui_AwBIDSGUI.h"
#include "AwBIDSSubject.h"
class AwBIDSManager;

class AwBIDSGUI : public QWidget
{
	Q_OBJECT

public:
	AwBIDSGUI(AwBIDSManager *bids, const QString& rootDir = QString(), QWidget *parent = Q_NULLPTR);
	~AwBIDSGUI();

	void refresh();
protected:
	AwBIDSManager * m_bids;
private:
	Ui::AwBIDSGUIUi m_ui;
		
};
