#include <widget/AwWaitWidget.h>
#include "ui_AwWaitWidget.h"

AwWaitWidget::AwWaitWidget(const QString& title, QWidget *parent) : QDialog(parent)
{
	ui = new Ui::AwWaitWidgetUi();
	ui->setupUi(this);
	setWindowTitle(title);
	initProgress(0, 0);
	m_thread = nullptr;
}

AwWaitWidget::~AwWaitWidget()
{
	delete ui;
	if (m_thread) {
		m_thread->join();
		delete m_thread;
	}
}

void AwWaitWidget::accept()
{
	QDialog::accept();
}

int AwWaitWidget::exec()
{
	return QDialog::exec();
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