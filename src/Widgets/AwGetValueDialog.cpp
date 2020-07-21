#include <widget/AwGetValueDialog.h>

#include "ui_AwGetValueDialog.h"

AwGetValueDialog::AwGetValueDialog(const QString& title, QWidget *parent)
	: QDialog(parent)
{
	m_ui = new Ui::AwGetValueDialogUi;
	m_ui->setupUi(this);
	setWindowTitle(title);
}

AwGetValueDialog::~AwGetValueDialog()
{
	delete m_ui;
}


QString AwGetValueDialog::value()
{
	return m_ui->lineEdit->text();
}