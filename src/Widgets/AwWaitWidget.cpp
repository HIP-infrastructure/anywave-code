#include <widget/AwWaitWidget.h>
#include "ui_AwWaitWidget.h"

//AwWaitWidget::AwWaitWidget(const QString& title, const QString& text, QWidget *parent)
//	: QWidget(parent)
//{
//	ui = new Ui::AwWaitWidget();
//	ui->setupUi(this);
//	ui->label->setText(text);
//	setWindowTitle(title);
//	ui->progressBar->hide();
////	show();
//}

AwWaitWidget::AwWaitWidget(const QString& title, QWidget *parent) : QDialog(parent)
{
	ui = new Ui::AwWaitWidgetUi();
	ui->setupUi(this);
	setWindowTitle(title);
}

AwWaitWidget::~AwWaitWidget()
{
	delete ui;
}

void AwWaitWidget::initProgress(int min, int max)
{
	ui->progressBar->setRange(min, max);
	ui->progressBar->show();
}

void AwWaitWidget::setCurrentProgress(int value) 
{ 
	ui->progressBar->setValue(value); 
	repaint(); 
}


void AwWaitWidget::setText(const QString& text)
{
	ui->label->setText(text);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// EVENTS
void AwWaitWidget::changeEvent(QEvent *e)
{
	if (e) {
		if (e->type() == QEvent::LanguageChange) 
			ui->retranslateUi(this);
	}
}
