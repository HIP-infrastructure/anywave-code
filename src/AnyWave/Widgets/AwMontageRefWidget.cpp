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
#include "AwMontageRefWidget.h"
#include <QPushButton>
#include <QGridLayout>
#include <QScrollArea>
#include <QScrollBar>

#define AW_MONTAGEREF_WIDGET_COLUMNS	7

AwMontageRefWidget::AwMontageRefWidget(int channelType, const AwChannelList& asRecorded, QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle(tr("Pick a reference channel"));
	m_type = channelType;
	m_asRecorded = asRecorded;
	QGridLayout *gridLayout = new QGridLayout(this);
	gridLayout->setContentsMargins(5, 5, 5, 5);
	gridLayout->setHorizontalSpacing(6);
	QScrollArea *scrollArea = new QScrollArea(this);
	scrollArea->setWidgetResizable(true);
	QWidget *scrollAreaWidgetContents = new QWidget();
	QGridLayout *gridLayoutArea =  new QGridLayout(scrollAreaWidgetContents);
	gridLayoutArea->setContentsMargins(5, 5, 5, 5);
	
	int row = 0, col = 0;
	QStringList channelNames;
	channelNames << QString(noref);

	foreach (AwChannel *chan, asRecorded)
	{
		if (chan->type() == channelType && !chan->isBad())
			channelNames << chan->name();	
	}
	// extract AVG channel if there is one
	int index = channelNames.indexOf("SEEG_AVG");
	if (index != -1)
	{
		channelNames.removeAll("SEEG_AVG");
		channelNames.insert(0, "SEEG_AVG");
	}
	index = channelNames.indexOf("EEG_AVG");
	if (index != -1)
	{
		channelNames.removeAll("EEG_AVG");
		channelNames.insert(0, "EEG_AVG");
	}
	index = channelNames.indexOf("MEG_AVG");
	if (index != -1)
	{
		channelNames.removeAll("MEG_AVG");
		channelNames.insert(0, "MEG_AVG");
	}

	qSort(channelNames.begin() + 2 , channelNames.end());

	foreach (QString s, channelNames)
	{
		QPushButton *button = new QPushButton(s, scrollAreaWidgetContents);
		if (col > AW_MONTAGEREF_WIDGET_COLUMNS - 1)
		{
			col = 0;
			row++;
		}
		gridLayoutArea->addWidget(button, row, col, 1, 1);
		col++;
		connect(button, SIGNAL(clicked()), this, SLOT(getSelectedChannel()));
	}
	scrollArea->setWidget(scrollAreaWidgetContents);
	gridLayout->addWidget(scrollArea, 0, 0, 1, 1);

	// resize widget's width according to layout's width
	this->setSizeGripEnabled(true);

	int lw = gridLayoutArea->sizeHint().width() + 20; // 20 is for the contents margins
	int lh = gridLayoutArea->sizeHint().height() + 20;
	if (lh > 400)
		lh = 400;
	QScrollBar *vb = scrollArea->verticalScrollBar();
	lw += vb->size().width();

	this->resize(lw, lh);
}

AwMontageRefWidget::~AwMontageRefWidget()
{

}

void AwMontageRefWidget::getSelectedChannel()
{
	QPushButton *button = (QPushButton *)sender();
	if (!button)
		return;

	m_selectedChannelName = button->text();
	accept();
}