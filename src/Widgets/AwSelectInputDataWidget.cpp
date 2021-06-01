#include <widget/AwSelectInputDataWidget.h>
#include "ui_AwSelectInputDataWidget.h"
#include <QMessageBox>

AwSelectInputDataWidget::AwSelectInputDataWidget(QWidget* parent)
	: QWidget(parent)
{
	m_ui = new Ui::AwSelectInputDataWidgetUi;
	m_ui->setupUi(this);
	connect(m_ui->buttonSkip, &QPushButton::clicked, this, &AwSelectInputDataWidget::addSkipLabel);
	connect(m_ui->buttonUse, &QPushButton::clicked, this, &AwSelectInputDataWidget::addUseLabel);
	connect(m_ui->buttonClearUse, &QPushButton::clicked, this, &AwSelectInputDataWidget::clearUse);
	connect(m_ui->buttonClearSkip, &QPushButton::clicked, this, &AwSelectInputDataWidget::clearSkip);
	m_ui->listWidgetSkip->hide();
	m_ui->listWidgetUse->hide();
	m_ui->comboSkip->hide();
	m_ui->comboUse->hide();
	m_ui->buttonClearSkip->hide();
	m_ui->buttonClearUse->hide();
	m_ui->buttonSkip->hide();
	m_ui->buttonUse->hide();
	m_ui->labelUse->hide();
	m_ui->labelSkip->hide();
	connect(m_ui->checkSkip, SIGNAL(clicked(bool)), this, SLOT(changeSkipStatus(bool)));
	connect(m_ui->checkUse, SIGNAL(clicked(bool)), this, SLOT(changeUseStatus(bool)));
}

AwSelectInputDataWidget::~AwSelectInputDataWidget()
{
	delete m_ui;
}


void AwSelectInputDataWidget::setMarkers(const AwMarkerList& markers, int filters)
{
	// check special case where Process Manager has set only one global marker as input.
	// this happens when launching a plugin which requires time selections and no time selections exists.
	// So Process Manager will create a global time selection using a marker labeled "global".
	bool active = !markers.isEmpty();

	if (markers.size() == 1)
		if (markers.first()->label().toLower() == "global")
			active = false;
	if (!active) {
		m_ui->checkSkip->setChecked(false);
		m_ui->checkSkip->setEnabled(false);
		m_ui->checkUse->setEnabled(false);
		m_ui->checkUse->setChecked(false);
		return;
	}

	AwMarkerList tmp = markers;
	switch (filters) {
	case AwSelectInputDataWidget::WithDuration:
		tmp = AwMarker::getMarkersWithDuration(markers);
		break;
	case AwSelectInputDataWidget::UniqueLabels:
		tmp = AwMarker::getMarkersWithUniqueLabels(markers);
		break;
	case AwSelectInputDataWidget::WithDurationAndUniqueLabels:
		tmp = AwMarker::getMarkersWithDuration(markers);
		tmp = AwMarker::getMarkersWithUniqueLabels(tmp);
		break;
	default:
		break;
	}

	// enable the use of the widget only if markers are available
	if (!tmp.isEmpty()) {
		m_ui->checkSkip->setEnabled(true);
		m_ui->checkUse->setEnabled(true);
		m_ui->comboSkip->setMarkers(tmp);
		m_ui->comboUse->setMarkers(tmp);
	}
	else {
		m_ui->checkSkip->setChecked(false);
		m_ui->checkSkip->setEnabled(false);
		m_ui->checkUse->setEnabled(false);
		m_ui->checkUse->setChecked(false);
	}
}

QStringList AwSelectInputDataWidget::usedMarkers()
{
	return m_useLabels;
}

QStringList AwSelectInputDataWidget::skippedMarkers()
{
	return m_skipLabels;
}

void AwSelectInputDataWidget::changeSkipStatus(bool flag)
{
	if (!flag)
		clearSkip();
}

void AwSelectInputDataWidget::changeUseStatus(bool flag)
{
	if (!flag)
		clearUse();
}

void AwSelectInputDataWidget::addSkipLabel()
{
	auto item = m_ui->comboSkip->currentText();
	// check if labels is NOT on the other list
	if (m_useLabels.contains(item)) {
		QMessageBox::information(this, "Markers", "This marker is already set in the other list.");
		return;
	}
	if (!m_skipLabels.contains(item)) {
		m_skipLabels << item;
		m_ui->listWidgetSkip->addItem(item);
		m_ui->listWidgetSkip->show();
		m_ui->listWidgetSkip->update();
	}
}

void AwSelectInputDataWidget::addUseLabel()
{
	auto item = m_ui->comboUse->currentText();
	// check if labels is NOT on the other list
	if (m_skipLabels.contains(item)) {
		QMessageBox::information(this, "Markers", "This marker is already set in the other list.");
		return;
	}
	if (!m_useLabels.contains(item)) {
		m_useLabels << item;
		m_ui->listWidgetUse->addItem(item);
		m_ui->listWidgetUse->show();
		m_ui->listWidgetUse->update();
	}
}

void AwSelectInputDataWidget::clearUse()
{
	m_ui->listWidgetUse->clear();
//	m_ui->listWidgetUse->hide();
	m_useLabels.clear();
}

void AwSelectInputDataWidget::clearSkip()
{
	m_ui->listWidgetSkip->clear();
//	m_ui->listWidgetSkip->hide();
	m_skipLabels.clear();
}