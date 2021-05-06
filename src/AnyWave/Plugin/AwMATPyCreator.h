#pragma once

#include <QDialog>
#include "ui_AwMATPyCreator.h"
//class QRadioButton;
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
	Ui::AwMATPyCreator m_ui;
//	QList<QRadioButton*> m_buttons;
	int makePlugin(const QVariantMap&);
};
