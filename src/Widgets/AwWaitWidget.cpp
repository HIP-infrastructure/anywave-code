#include <widget/AwWaitWidget.h>
#include "ui_AwWaitWidget.h"
#include <QtConcurrent>

AwWaitWidget::AwWaitWidget(const QString& title, QWidget *parent) : QDialog(parent)
{
	ui = new Ui::AwWaitWidgetUi();
	ui->setupUi(this);
	setWindowTitle(title);
	initProgress(0, 0);
}

AwWaitWidget::~AwWaitWidget()
{
	delete ui;
}

template<typename F>
void AwWaitWidget::run(F function)
{
	show();
	QFutureWatcher<void> watcher;
	QFuture<void> future = QtConcurrent::run(function);
	watcher.setFuture(future);
	connect(&watcher, SIGNAL(finished()), this, SLOT(close()));
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
