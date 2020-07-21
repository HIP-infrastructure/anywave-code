#include <widget/AwSelectInputDataWidget.h>
#include "ui_AwSelectInputDataWidget.h"

AwSelectInputDataWidget::AwSelectInputDataWidget(QWidget *parent)
	: QWidget(parent)
{
	m_ui = new Ui::AwSelectInputDataWidgetUi;
	m_ui->setupUi(this);
	connect(m_ui->buttonSkip, &QPushButton::clicked, this, &AwSelectInputDataWidget::addSkipLabel);
	connect(m_ui->buttonUse, &QPushButton::clicked, this, &AwSelectInputDataWidget::addUseLabel);
	connect(m_ui->buttonClearUse, &QPushButton::clicked, this, &AwSelectInputDataWidget::clearUse);
	connect(m_ui->buttonClearSkip, &QPushButton::clicked, this, &AwSelectInputDataWidget::clearSkip);
}

AwSelectInputDataWidget::~AwSelectInputDataWidget()
{
	delete m_ui;
}


void AwSelectInputDataWidget::setMarkers(const AwMarkerList& markers)
{
	m_ui->comboSkip->setMarkers(markers);
	m_ui->comboUse->setMarkers(markers);
}

QStringList AwSelectInputDataWidget::usedMarkers()
{
	return m_useLabels;
}

QStringList AwSelectInputDataWidget::skippedMarkers()
{
	return m_skipLabels;
}

void AwSelectInputDataWidget::addSkipLabel()
{
	auto item = m_ui->comboSkip->currentText();
	if (!m_skipLabels.contains(item)) {
		m_skipLabels << item;
		m_ui->listWidgetSkip->addItem(item);
		m_ui->listWidgetSkip->update();
	}
}

void AwSelectInputDataWidget::addUseLabel()
{
	auto item = m_ui->comboUse->currentText();
	if (!m_useLabels.contains(item)) {
		m_useLabels << item;
		m_ui->listWidgetUse->addItem(item);
		m_ui->listWidgetUse->update();
	}
}

void AwSelectInputDataWidget::clearUse()
{
	m_ui->listWidgetUse->clear();
	m_useLabels.clear();
}

void AwSelectInputDataWidget::clearSkip()
{
	m_ui->listWidgetSkip->clear();
	m_skipLabels.clear();
}