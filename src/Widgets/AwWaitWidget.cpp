// AnyWave
// Copyright (C) 2013-2021  INS UMR 1106
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
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