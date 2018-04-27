#pragma once

#include <QWidget>
#include "ui_AwBIDSGUI.h"
#include "AwBIDSSubject.h"

class AwBIDSGUI : public QWidget
{
	Q_OBJECT

public:
	AwBIDSGUI(const QString& rootDir = QString(), QWidget *parent = Q_NULLPTR);
	~AwBIDSGUI();

	void setSubjects(const AwBIDSSubjectList& subject);
private:
	Ui::AwBIDSGUIUi m_ui;
};
