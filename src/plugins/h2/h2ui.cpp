/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "h2ui.h"
#include <QFileDialog>
#include "h2.h"

H2UI::H2UI(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
	samplingRate = 1;


	QSettings settings(QSettings::SystemScope, "INSERM U1106", "AnyWave");
	m_bandNames = QStringList{ QString("delta"), QString("theta"), QString("beta"), QString("alpha"), QString("gamma"),
		QString("gamma2"), QString("ripple"), QString("fast ripple"), QString("broad") };
	m_checkBoxes = { checkDelta, checkTheta, checkBeta, checkAlpha, checkGamma, checkGamma2,
		checkRipple, checkFastRipple, checkBroad };

	m_spinBoxes = { spinBox(spinDeltaHP, spinDeltaLP), spinBox(spinThetaHP, spinThetaLP), spinBox(spinBetaHP, spinBetaLP),
		spinBox(spinAlphaHP, spinAlphaLP), spinBox(spinGammaHP, spinGammaLP), spinBox(spinGamma2HP, spinGamma2LP), 
		spinBox(spinRippleHP, spinRippleLP), spinBox(spinFastRippleHP, spinFastRippleLP), spinBox(spinBroadHP, spinBroadLP) };
	
}

H2UI::~H2UI()
{
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS

int H2UI::exec()
{
	comboDS->setItemData(0, QString("x1 (%1Hz)").arg(samplingRate), Qt::DisplayRole);
	comboDS->setItemData(1, QString("x2 (%1Hz->%2Hz)").arg(samplingRate).arg(samplingRate / 2), Qt::DisplayRole);
	comboDS->setItemData(2, QString("x4 (%1Hz->%2Hz)").arg(samplingRate).arg(samplingRate / 4), Qt::DisplayRole);
	comboDS->setItemData(3, QString("x8 (%1Hz->%2Hz)").arg(samplingRate).arg(samplingRate / 8), Qt::DisplayRole);
	// check for settings file in working directory
	if (!directory.isEmpty()) {
		QString fileName = QString("%1/settings.json").arg(directory);
		QJsonDocument doc;
		if (QFile::exists(fileName)) {
			QFile file(fileName);
			if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
				QString data = file.readAll();
				file.close();
				doc = QJsonDocument::fromJson(data.toUtf8());
				QJsonObject obj = doc.object();
				// get general settings
				QJsonValue mainSettings = obj.value("computation");
				if (!mainSettings.isNull()) {
					QJsonObject main = mainSettings.toObject();
					sbTimeWindow->setValue(main["timeWindow"].toDouble());
					sbStep->setValue(main["step"].toDouble());
					sbMaxLag->setValue(main["maxLag"].toDouble());
					comboMethod->setCurrentIndex(main["method"].toInt());
					comboDS->setCurrentIndex(main["downSampling"].toInt());
				}
				for (int i = 0; i < m_bandNames.size(); i++) {
					QString b = m_bandNames.at(i);
					QCheckBox *checkBox = m_checkBoxes.at(i);
					spinBox sp = m_spinBoxes.at(i);
					QJsonValue value = obj.value(b);
					if (!value.isNull()) {
						QJsonObject item = value.toObject();
						sp.first->setValue(item["hp"].toDouble());
						sp.first->setEnabled(item["checked"].toBool());
						sp.second->setValue(item["lp"].toDouble());
						sp.second->setEnabled(item["checked"].toBool());
						checkBox->setChecked(item["checked"].toBool());
					}
				}
			}
		}
	}
	widgetInputData->setMarkers(markers);
	// if markers contains only one marker called whole data => disabled
	if (markers.isEmpty()) 
		widgetInputData->setDisabled(true);
	
	if (markers.size() == 1) {
		if (markers.first()->label() == "whole data")
			widgetInputData->setDisabled(true);	
	}
	return QDialog::exec();
}

void H2UI::accept()
{ 
	// prepare input data depending on widgetInputData
	skippedLabels = widgetInputData->skippedMarkers();
	usedLabels = widgetInputData->usedMarkers();

	if (checkAnyWave->isChecked()) {
		bands.insert("AnyWave", QPair<float, float>(0, 0));
	}
	if (checkDelta->isChecked()) {
		bands.insert("delta", QPair<float, float>(spinDeltaHP->value(), spinDeltaLP->value()));
	}
	if (checkTheta->isChecked()) {
		bands.insert("theta", QPair<float, float>(spinThetaHP->value(), spinThetaLP->value()));
	}
	if (checkBeta->isChecked()) {
		bands.insert("beta", QPair<float, float>(spinBetaHP->value(), spinBetaLP->value()));

	}
	if (checkGamma->isChecked()) {
		bands.insert("gamma", QPair<float, float>(spinGammaHP->value(), spinGammaLP->value()));
	}
	if (checkGamma2->isChecked()) {
		bands.insert("gamma2", QPair<float, float>(spinGamma2HP->value(), spinGamma2LP->value()));
	}
	if (checkAlpha->isChecked()) {
		bands.insert("alpha", QPair<float, float>(spinAlphaHP->value(), spinAlphaLP->value()));
	}
	if (checkRipple->isChecked()) {
		bands.insert("ripple", QPair<float, float>(spinRippleHP->value(), spinRippleLP->value()));
	}
	if (checkFastRipple->isChecked()) {
		bands.insert("fast ripple", QPair<float, float>(spinFastRippleHP->value(), spinFastRippleLP->value()));
	}
	if (checkBroad->isChecked()) {
		bands.insert("broad", QPair<float, float>(spinBroadHP->value(), spinBroadLP->value()));
	}

	method = comboMethod->currentIndex() == 0 ? Global::Method::h2 : Global::Method::r2;

	switch (comboDS->currentIndex()) {
	case 0:
		downSampling = 1;
		break;
	case 1:
		downSampling = 2;
		break;
	case 2:
		downSampling = 4;
		break;
	case 3:
		downSampling = 8;
		break;
	default:
		downSampling = 1;
		break;
	}

	saveInOneFile = radioSaveInOneFile->isChecked();

	// check for settings file in working directory
	if (!directory.isEmpty()) {
		QString fileName = QString("%1/settings.json").arg(directory);
		QFile file(fileName);
		if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QJsonObject obj;
			// save general settings
			QJsonObject computation;
			computation.insert("timeWindow", sbTimeWindow->value());
			computation.insert("step", sbStep->value());
			computation.insert("maxLag", sbMaxLag->value());
			computation.insert("method", comboMethod->currentIndex());
			computation.insert("downSampling", comboDS->currentIndex());
			obj.insert("computation", computation);
			for (int i = 0; i < m_bandNames.size(); i++) {
				QString b = m_bandNames.at(i);
				QCheckBox *checkBox = m_checkBoxes.at(i);
				spinBox sp = m_spinBoxes.at(i);
				QJsonObject band;
				band.insert("hp", sp.first->value());
				band.insert("lp", sp.second->value());
				band.insert("checked", checkBox->isChecked());
				obj.insert(b, band);
			}
			QJsonDocument doc(obj);
			file.write(doc.toJson());
			file.close();
		}
	}
	QDialog::accept();
}