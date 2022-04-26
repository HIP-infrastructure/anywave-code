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
#include <widget/AwGainLevelsWidget.h>
#include <QGridLayout>
#include <AwChannel.h>
#include "AwGainLevels.h"
#include <QLabel>
#include <QSlider>
#include <QLineEdit>
#include <QPushButton>

AwGainLevelsWidget::AwGainLevelsWidget(AwGainLevels *gl, QWidget* parent) : QWidget(parent)
{
	m_gainLevels = gl;
	Qt::WindowFlags flags = 0;
	
	flags  |= Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint;
	setWindowFlags(flags);

	if (parent) {
		QPoint pos = parent->pos();
		if (pos.x() < 0)
			pos.setX(0);
		if (pos.y() < 0)
			pos.setY(0);
	}
	buildLayout();
}

void AwGainLevelsWidget::buildLayout()
{
	QGridLayout* layout = new QGridLayout;
	// first create all the widgets for every gain levels
	int iteration = 0;
	QPushButton* button;
	for (auto gl : m_gainLevels->gainLevels()) {
		// label
		auto text = AwChannel::typeToString(gl->type);
		auto label = new QLabel(text);
		m_labels << label;
		layout->addWidget(label, iteration, 0);
		// plus 
		auto buttonUp = new QPushButton;
		buttonUp->setMinimumSize(QSize(32, 32));
		buttonUp->setMaximumSize(QSize(32, 32));
		buttonUp->setIcon(QIcon(":/images/plus_sign_24x24.png"));
		buttonUp->setIconSize(QSize(32, 32));
		buttonUp->setFlat(true);
		connect(buttonUp, &QPushButton::clicked, this, &AwGainLevelsWidget::updateGUI);
		m_buttonsUp << buttonUp;
		m_widgetsToTypes.insert(buttonUp, gl->type);
		//m_widgets.insert(gl->type, buttonUp);
		layout->addWidget(buttonUp, iteration, 1);
		// slider
		auto slider = new QSlider(Qt::Horizontal);
		slider->setMaximum(gl->values().n_elem - 1);
		slider->setMinimum(0);
		int index = gl->getIndexOfValue(gl->value());
		slider->setValue(0);
		if (index != -1)
			slider->setValue(index);
		connect(slider, &QSlider::valueChanged, this, &AwGainLevelsWidget::updateGUI);
		layout->addWidget(slider, iteration , 2);
		m_sliders << slider;
		m_widgetsToTypes.insert(slider, gl->type);
		m_typeToSlider.insert(gl->type, slider);
		//m_widgets.insert(gl->type, slider);
		// minus
		auto buttonDown = new QPushButton;
		buttonDown->setMinimumSize(QSize(32, 32));
		buttonDown->setMaximumSize(QSize(32, 32));
		buttonDown->setIcon(QIcon(":/images/minus_sign_24x24.png"));
		buttonDown->setIconSize(QSize(32, 32));
		buttonDown->setFlat(true);
		m_buttonsDown << buttonDown;
		m_widgetsToTypes.insert(buttonDown, gl->type);
		//m_widgets.insert(gl->type, buttonDown);
		connect(buttonDown, &QPushButton::clicked, this, &AwGainLevelsWidget::updateGUI);
		layout->addWidget(buttonDown, iteration, 3);
		// Edit box
		//auto edit = new AwGainLineEdit();
		QLineEdit *edit = new QLineEdit();
		edit->setText(QString("%1").arg(gl->value()));
		//connect(edit, &AwGainLineEdit::editingFinished, this, &AwGainLevelsWidget::getEditValue);
		connect(edit, &QLineEdit::editingFinished, this, &AwGainLevelsWidget::updateGUI);
		layout->addWidget(edit, iteration , 4);
		m_values << edit;
		m_widgetsToTypes.insert(edit, gl->type);
		m_typeToEdit.insert(gl->type, edit);
		//m_widgets.insert(gl->type, edit);
		auto unit = new QLabel(gl->unit());
		layout->addWidget(unit, iteration, 5);
		iteration++;

		// this will be triggered each time gl->value is changed
		connect(gl, &AwGainLevel::valueChanged, this, &AwGainLevelsWidget::updateWidgets);
	}
	setLayout(layout);
	setWindowTitle("Gain Levels");
}

void AwGainLevelsWidget::updateGUI()
{
	// get sender type
	// could be pushbutton slider or lineedit
	QLineEdit* edit = qobject_cast<QLineEdit*>(sender());
	QSlider* slider = qobject_cast<QSlider*>(sender());
	QPushButton* button = qobject_cast<QPushButton*>(sender());

	QWidget* widget = qobject_cast<QWidget*>(sender());
	if (widget == nullptr)
		return;

	auto gl = m_gainLevels->getGainLevelFor(m_widgetsToTypes.value(widget));

	if (edit) {
		//gl->setValue(edit->text().toFloat());
		gl->insertNewValue(edit->text().toFloat());
	}
	else if (slider) {
		gl->insertNewValue(gl->values()(slider->value()));
	}
	else { // button
		// which button? up or down?
		bool up = m_buttonsUp.indexOf(button) != -1;
		if (up)
			gl->down();
		else
			gl->up();
	}
	emit amplitudeChanged(gl->type, gl->value());
}

void AwGainLevelsWidget::updateWidgets(int type, float value)
{
	// sender if a gain level
	AwGainLevel* gl = qobject_cast<AwGainLevel*>(sender());

	if (gl == nullptr)
		return;

	// update slider and edit line
	QSlider* slider = m_typeToSlider.value(type);
	disconnect(slider, nullptr, this, nullptr);
	slider->setMaximum(gl->values().size() - 1);
	slider->setValue(gl->index());
	connect(slider, &QSlider::valueChanged, this, &AwGainLevelsWidget::updateGUI);
	QLineEdit* edit = m_typeToEdit.value(type);
	disconnect(edit, nullptr, this, nullptr);
	edit->setText(QString("%1").arg(value));
	connect(edit, &QLineEdit::editingFinished, this, &AwGainLevelsWidget::updateGUI);
}