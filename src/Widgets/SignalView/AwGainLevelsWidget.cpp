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


AwGainLevelsWidget::AwGainLevelsWidget(AwGainLevels *gl, QWidget* parent) : QWidget(parent)
{
	m_gainLevels = gl;
	Qt::WindowFlags flags = 0;
//	m_ui.setupUi(this);
	
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
	for (auto gl : m_gainLevels->gainLevels()) {
		// label
		auto text = AwChannel::typeToString(gl->type);
		auto label = new QLabel(text);
		m_labels << label;
		layout->addWidget(label, iteration, 0);
		// slider
		auto slider = new QSlider(Qt::Horizontal);
		slider->setMaximum(gl->values.n_elem - 1);
		slider->setMinimum(0);
		uvec indexes = arma::find(gl->values == gl->value);
		slider->setValue(0);
		if (!indexes.is_empty()) 
			slider->setValue(indexes(0));
		connect(slider, &QSlider::valueChanged, this, &AwGainLevelsWidget::getSliderValue);
		layout->addWidget(slider, iteration , 1);
		m_sliders << slider;
		m_widgetsToTypes.insert(slider, gl->type);
		// Edit box
		auto edit = new QLineEdit();
		edit->setText(QString("%1").arg(gl->value));
		connect(edit, &QLineEdit::editingFinished, this, &AwGainLevelsWidget::getEditValue);
		layout->addWidget(edit, iteration , 2);
		m_values << edit;
		m_widgetsToTypes.insert(edit, gl->type);
		auto unit = new QLabel(gl->unit);
		layout->addWidget(unit, iteration, 3);
		iteration++;
	}
	setLayout(layout);
	setWindowTitle("Gain Levels");
}

void AwGainLevelsWidget::getSliderValue(int v)
{
	// get the slider
	QSlider* slider = qobject_cast<QSlider*>(sender());
	if (slider == nullptr)
		return;
	int type = m_widgetsToTypes.value(slider);
	auto gl = m_gainLevels->getGainLevelFor(type);
	if (gl == nullptr)
		return;
	if (v >= 0 && v < gl->values.n_elem)
		gl->value = gl->values(v);

	// update the buddy EditLine
	QLineEdit* edit = qobject_cast<QLineEdit *>(m_values.at(m_sliders.indexOf(slider)));
	disconnect(edit, nullptr, this, nullptr);
	edit->setText(QString("%1").arg(gl->value));
	connect(edit, &QLineEdit::editingFinished, this, &AwGainLevelsWidget::getEditValue);
	// SEND A SIGNAL !!!
	emit amplitudeChanged(gl->type, gl->value);
}

void AwGainLevelsWidget::getEditValue()
{
	QLineEdit *edit = qobject_cast<QLineEdit*>(sender());
	if (edit == nullptr)
		return;
	int type = m_widgetsToTypes.value(edit);
	auto gl = m_gainLevels->getGainLevelFor(type);
	if (gl == nullptr)
		return;
	float v = edit->text().toFloat();
	// find value in scale
	int index = gl->getIndexOfValue(v);
	if (index == -1)
		index = gl->insertNewValue(v);
	gl->value = v;

	// set the slider buddy
	QSlider *slider = qobject_cast<QSlider*>(m_sliders.at(m_values.indexOf(edit)));
	disconnect(slider, nullptr, this, nullptr);
	slider->setValue(index);
	connect(slider, &QSlider::valueChanged, this, &AwGainLevelsWidget::getSliderValue);
	emit amplitudeChanged(gl->type, gl->value);
}