#include <widget/AwMarkAroundWidget.h>
#include "ui_AwMarkAroundWidget.h"

AwMarkAroundWidget::AwMarkAroundWidget(QWidget *parent)
	: QWidget(parent)
{
	m_ui = new Ui::AwMarkAroundWidgetUi();
	m_ui->setupUi(this);
	m_ui->comboColorPre->insertItem(0 ,"Default", 0);
	for (int i = 0; i < QColor::colorNames().size(); i++)	{
		m_ui->comboColorPre->insertItem(i + 1, QColor::colorNames().at(i), i);
		m_ui->comboColorPre->setItemData(i + 1, QColor(QColor::colorNames().at(i)), Qt::DecorationRole);
	}
	m_ui->comboColorPost->insertItem(0 ,"Default", 0);
	for (int i = 0; i < QColor::colorNames().size(); i++)	{
		m_ui->comboColorPost->insertItem(i + 1, QColor::colorNames().at(i), i);
		m_ui->comboColorPost->setItemData(i + 1, QColor(QColor::colorNames().at(i)), Qt::DecorationRole);
	}

	m_ui->comboColorCreateMarker->insertItem(0 ,"Default", 0);
	for (int i = 0; i < QColor::colorNames().size(); i++)	{
		m_ui->comboColorCreateMarker->insertItem(i + 1, QColor::colorNames().at(i), i);
		m_ui->comboColorCreateMarker->setItemData(i + 1, QColor(QColor::colorNames().at(i)), Qt::DecorationRole);
	}

	m_totalDuration = 0.;
}

AwMarkAroundWidget::~AwMarkAroundWidget()
{
	delete m_ui;
}

void AwMarkAroundWidget::setMarkers(const AwMarkerList& markers)
{
	m_markers = markers;
	m_ui->comboMarker->clear();
	QStringList labels;	// unique labels
	foreach(AwMarker *m, markers) {
		if (!labels.contains(m->label()))
			labels << m->label();
	}
	m_ui->comboMarker->addItems(labels);
	m_ui->comboMarker2->addItems(labels);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS

void AwMarkAroundWidget::generateAround()
{
	m_newMarkers.clear();
	float before, after;
	before = (float)m_ui->sbBefore->value();
	after = (float)m_ui->sbAfter->value();
	// check requirements for marker generation
	if (before == 0. && after == 0.)
		return;

	qint16 value = (qint16)m_ui->spinBoxValueCreate->value();
	QString create_label = m_ui->lineEditCreateMarker->text();
	QString marker_label = m_ui->comboMarker2->currentText();
	QString create_color;
	// get colors only if different from default
	if (m_ui->comboColorCreateMarker->currentIndex() > 0) {
		QColor color(m_ui->comboColorCreateMarker->currentText());
		create_color = color.name(QColor::HexRgb);
	}
	
	// create markers
	foreach (AwMarker *marker, m_markers) {
		if (marker->label() != marker_label)
			continue;

		float start = marker->start() - before;
		if (start < 0.)
			start = 0.;
		float end = marker->end() + after;
		if (end < 0.)
			end = 0.;
		if (end > m_totalDuration)
			end = m_totalDuration;
		AwMarker *res_marker = new AwMarker;
		res_marker->setLabel(create_label);
		res_marker->setStart(start);
		res_marker->setEnd(end);
		res_marker->setValue(value);
		res_marker->setColor(create_color);
		m_newMarkers << res_marker;
	}
	emit newMarkersCreated(m_newMarkers);
}

void AwMarkAroundWidget::generatePrePost()
{
	float pre_offset, post_offset;
	float pre_max, post_max;
	qint16 pre_value, post_value;
	pre_offset = (float)m_ui->sboffset1->value();
	post_offset = (float)m_ui->sboffset2->value();
	pre_max = (float)m_ui->sbmax1->value();
	post_max = (float)m_ui->sbmax2->value();
	pre_value = (qint16)m_ui->spinBoxValuePre->value();
	post_value = (qint16)m_ui->spinBoxValuePost->value();

	QString post_label, pre_label, pre_color, post_color;
	pre_label = m_ui->lineEditPreMarker->text();
	post_label= m_ui->lineEditPostMarker->text();
	QString marker_label = m_ui->comboMarker->currentText();

	// get colors only if different from default
	if (m_ui->comboColorPre->currentIndex() > 0) {
		QColor color(m_ui->comboColorPre->currentText());
		pre_color = color.name(QColor::HexRgb);
	}

	if (m_ui->comboColorPost->currentIndex() > 0) {
		QColor color(m_ui->comboColorPost->currentText());
		post_color = color.name(QColor::HexRgb);
	}

	// create markers
	foreach (AwMarker *marker, m_markers) {
		if (marker->label() != marker_label)
			continue;

		// create pre marker (if possible)
		if (pre_max != 0) {
			float start = marker->start() - pre_max;
			if (start < 0.)
				start = 0.;
			float end = marker->start() - pre_offset;
			if (end < 0.)
				end = 0.;
			AwMarker *pre_marker = new AwMarker;
			pre_marker->setLabel(pre_label);
			pre_marker->setStart(start);
			pre_marker->setEnd(end);
			pre_marker->setValue(pre_value);
			pre_marker->setColor(pre_color);
			m_newMarkers << pre_marker;
		}

		// create post marker (if possible)
		if (post_max != 0) {
			float start = marker->start() + post_offset;
			if (start < 0.)
				start = 0.;
			// if post marker is outside file => END
			if (start >= m_totalDuration) {
				emit newMarkersCreated(m_newMarkers);
				return;
			}
			float end = marker->end() + post_max;
			if (end < 0.)
				end = 0.;
			if (end > m_totalDuration)
				end = m_totalDuration;
			AwMarker *post_marker = new AwMarker;
			post_marker->setLabel(post_label);
			post_marker->setStart(start);
			post_marker->setEnd(end);
			post_marker->setValue(post_value);
			post_marker->setColor(post_color);
			m_newMarkers << post_marker;
		}
	}
	emit newMarkersCreated(m_newMarkers);
}

