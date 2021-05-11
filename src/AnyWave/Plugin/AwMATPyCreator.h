#pragma once

#include <QDialog>
#include "ui_AwMATPyCreator.h"

class AwMATPyCreator : public QDialog
{
	Q_OBJECT

public:
	AwMATPyCreator(QWidget *parent = Q_NULLPTR);
	~AwMATPyCreator();

	enum Flags { Ok, AlreadyExists, FailedtoCreate};
public slots:
	void accept() override;
private:
	Ui::AwMATPyCreatorUi m_ui;
	int makePlugin(const QVariantMap&);
};
