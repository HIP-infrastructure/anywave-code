/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "AwPrefsDial.h"
#include <QSettings>
#include "Prefs/AwSettings.h"
#include <AwMarker.h>
#include <QColorDialog>
#include <QFontDialog>
#include <QDir>
#include <widget/AwMessageBox.h>
#include <QFileDialog>
#include "Display/AwDisplay.h"
#include <utils/gui.h>
#include <utils/time.h>
using namespace AwUtilities::time;
using namespace AwUtilities::gui;

AwPrefsDial::AwPrefsDial(int tab, QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
	AwSettings *aws = AwSettings::getInstance();


	QSettings settings;

	int sh = settings.value("Preferences/screenCalibration/heightMM", 0).toInt();
	int sw = settings.value("Preferences/screenCalibration/widthMM", 0).toInt();


	spinBoxV->setValue(sh);
	spinBoxH->setValue(sw);

	// GENERAL
	// COLORS
	m_colorsChanged = false;
	m_markerColors << markerColor(AwMarker::Single) << markerColor(AwMarker::Selection);
	m_markerTypes << "Single" << "Selection";
	comboType->insertItems(0, m_markerTypes);
	comboType->setCurrentIndex(0);
	m_markerIndex = 0;
	buttonColor->setStyleSheet("background: " + m_markerColors.at(0) + ";");

	changeCursorColor(cursorColor());
	changeMappingCursorColor(mappingCursorColor());
	changeCursorFontText(cursorFont());
	changeMappingCursorFontText(mappingCursorFont());
	changeMarkerFontText(markerFont(AwMarker::Single));

	// Auto trigger parsing
	if (aws->getBool("isAutoTriggerParsingOn"))
		radioTriggerParserOn->setChecked(true);
	else
		radioTriggerParserOff->setChecked(true);

	// Time representation
	if ((isTimeHMS()))
		radioHMSOn->setChecked(true);
	else
		radioHMSOff->setChecked(true);

	// CPU Cores
	auto totalCPUCores = aws->getInt("totalCPUCores");
	int cores = aws->getInt("maxCPUCores");
	sliderCPU->setRange(2, totalCPUCores);
	sliderCPU->setValue(cores);

	// check for updates when starting
	bool checkForUpdates = aws->getBool("checkForUpdates");
	checkBoxUpdates->setChecked(checkForUpdates);

	// language
	QDir dir(aws->langPath);
	QStringList files = dir.entryList(QStringList("anywave_*.qm"));
	if (files.isEmpty()) // if no language files are found, disable the language option
		comboLanguage->setDisabled(true);
	QString savedLocale = settings.value("general/locale", QString("en")).toString();
	for (int i = 0; i < files.size(); i++) {
		QString locale;
		locale = files[i];
		locale.truncate(locale.lastIndexOf('.'));
		locale.remove(0, locale.indexOf('_') + 1);
		QString lang = QLocale::languageToString(QLocale(locale).language());
		comboLanguage->addItem(lang, locale);
		if (locale == savedLocale)
			comboLanguage->setCurrentIndex(i);
	}

	plainTextEdit->setVisible(false);
	// MATLAB
	bool isMatlabDetected = settings.value("matlab/detected", false).toBool();

	if (!isMatlabDetected) {
		plainTextEdit->setVisible(true);
		plainTextEdit->appendPlainText(tr("MATLAB was not detected. Please install and/or select the folder where MATLAB is installed."));
#ifdef Q_OS_WIN
		plainTextEdit->appendPlainText("Location could be C:\\Program Files\\MATLAB\\R2015a");
#endif
#ifdef Q_OS_LINUX
		plainTextEdit->appendPlainText("Location could be /usr/local/MATLAB/R2015a");
#endif
#ifdef Q_OS_MAC
		plainTextEdit->appendPlainText("Location could be /Applications/MATLAB_R2015a.app");
#endif
	}
	lineEditMatlabPath->setText(settings.value("matlab/path").toString());
	labelMCR->hide();
	lineEditMCR->hide();
	buttonSelectMCR->hide();
    
    lineEditGARDEL->setText(settings.value("GARDEL/path").toString());

	// COMPILED PLUGIN. Windows do not required environments variables to be set but Linux and Mac OS X do.
#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
	labelMCR->show();
	lineEditMCR->show();
	buttonSelectMCR->show();
	lineEditMCR->setText(settings.value("matlab/mcr_path").toString());
#endif

	QString python = settings.value("py/interpreter", QString()).toString();
	if (python.isEmpty()) { // set default path to the interpreter name
#ifdef Q_OS_WIN
		python = "python.exe";
#endif
#if defined Q_OS_MAC || defined Q_OS_LINUX
		python = "python";
#endif
		settings.setValue("py/interpreter", python);
	}
	lineEditPythonPath->setText(python);

	// select tab
	tabwidget_Prefs->setCurrentIndex(tab);
}


void AwPrefsDial::changeEvent(QEvent *e)
{
	if (e)
		if (e->type() == QEvent::LanguageChange)
			retranslateUi(this);
}

void AwPrefsDial::pickMatlabFolder()
{
#ifdef Q_OS_MAC
	QString folder = QFileDialog::getOpenFileName(this, "Select MATLAB Application", "/Applications", "Application (*.app)");
#else
	QString folder = QFileDialog::getExistingDirectory(this, "Select MATLAB Directory", "/");
#endif
	if (!folder.isEmpty()) {
		lineEditMatlabPath->setText(folder);
	}
}


void AwPrefsDial::accept()
{
	// SCREEN CALIBRATION
	QSettings settings;
	int w = spinBoxH->value();
	int h = spinBoxV->value();
	float xPixPerCm, yPixPerCm;
	
	if (h > 0)	{
		settings.setValue("Preferences/screenCalibration/heightMM", h);
		h /= 10;
		yPixPerCm = AW_CALIBRATION_WIDTH / h;
		settings.setValue("Preferences/screenCalibration/yPixPerCm", yPixPerCm);
	}
	if (w > 0)	{
		settings.setValue("Preferences/screenCalibration/widthMM", w);
		w /= 10;
		xPixPerCm = AW_CALIBRATION_WIDTH / w;
		settings.setValue("Preferences/screenCalibration/xPixPerCm", xPixPerCm);
	}

	// calibration has been done
	settings.setValue("Preferences/screenCalibration/calibrationDone", true);

	emit screenCalibrationChanged(xPixPerCm, yPixPerCm);

	// CPU CORES
	settings.setValue("general/cpu_cores", spinCPU->value());

	//// misc. parameters
	AwSettings *aws = AwSettings::getInstance();
	saveTimeHMS(radioHMSOn->isChecked());

	settings.setValue("py/interpreter", lineEditPythonPath->text());

	// if matlab path is empty => consider not using MATLAB plugins anymore.
	// On Mac and Linux that will be done by deleting the matlab.sh script
	if (lineEditMatlabPath->text().isEmpty()) {
		settings.setValue("matlab/detected", false);

#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
		QString scriptPath = QString("%1/AnyWave/matlab.sh").arg(AwSettings::getInstance()->getString("homeDir"));
		if (QFile::exists(scriptPath))
			QFile::remove(scriptPath);
#endif
	}
	else {
		// check if location exists
		QDir dir(lineEditMatlabPath->text());
		if (!dir.exists()) {
			QMessageBox::critical(this, tr("MATLAB"), tr("Invalid MATLAB location"));
			settings.setValue("matlab/detected", false);
			return;
		}
		settings.setValue("matlab/detected", true);
		settings.setValue("matlab/path", lineEditMatlabPath->text());
#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
		AwSettings::getInstance()->createMatlabShellScript(lineEditMatlabPath->text());
#endif
	}
	// MATLAB MCR on Linux and Mac
#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
	if (!lineEditMCR->text().isEmpty())
		settings.setValue("matlab/mcr_path", lineEditMCR->text());
#endif
	QString Gardel = lineEditGARDEL->text();
	if (QFile::exists(Gardel))
		settings.setValue("GARDEL/path", Gardel);

	// change language
	QString lang = comboLanguage->itemData(comboLanguage->currentIndex()).toString();
	aws->loadLanguage(lang);

	// CPU CORES
	auto maxCPUCores = sliderCPU->value();
	aws->setSettings("maxCPUCores", maxCPUCores);
    QThreadPool::globalInstance()->setMaxThreadCount(maxCPUCores);
	// Check for updates
	auto check = checkBoxUpdates->isChecked();
	aws->setSettings("checkForUpdates", check);
	settings.setValue("general/checkForUpdates", check);

	QDialog::accept();
}


void AwPrefsDial::restoreDefaultMarkersColors()
{
	comboType->setCurrentIndex(0);
	m_markerIndex = 0;
	m_markerColors.clear();
	m_markerColors << defaultMarkerColor(AwMarker::Single) <<  defaultMarkerColor(AwMarker::Selection);
	buttonColor->setStyleSheet("background: " + m_markerColors.at(0) + ";");
	saveMarkerFont(QFont("Helvetica", 10, QFont::Normal), AwMarker::Single);
	changeMarkerFontText(markerFont(AwMarker::Single));
}

void AwPrefsDial::restoreDefaultCursorColors()
{
	buttonCursorColor->setStyleSheet("background: " + defaultCursorColor() + ";");
	saveCursorFont(QFont("Helvetica", 10, QFont::Normal));
	changeCursorFontText(cursorFont());
}

void AwPrefsDial::restoreDefaultMappingColors()
{
	buttonMappingColor->setStyleSheet("background: " + mappingCursorColor() + ";");
	saveMappingCursorFont(QFont("Helvetica", 10, QFont::Normal));
	changeMappingCursorFontText(mappingCursorFont());
}

void AwPrefsDial::changeMarkerType(int index)
{
	m_markerIndex = index;
	buttonColor->setStyleSheet("background: " + m_markerColors.at(index) + ";");
}


void AwPrefsDial::chooseCustomColor()
{
	QColor color = QColorDialog::getColor(QColor(m_markerColors.at(m_markerIndex)), this, tr("Select Color"));
	
	if (color.isValid())
		changeMarkerColor(color.name());
}

void AwPrefsDial::changeMarkerColor(const QString& color)
{
	buttonColor->setStyleSheet("background: " + color + ";");
	if (color != m_markerColors.at(m_markerIndex))
		m_markerColors.replace(m_markerIndex, color);

	if (m_markerIndex == 0)
		saveMarkerColor(color, AwMarker::Single);
	else
		saveMarkerColor(color, AwMarker::Selection);
}


void AwPrefsDial::selectCursorColor()
{
	QColor color = QColorDialog::getColor(QColor(cursorColor()), this, tr("Edit cursor's color"));
	
	if (color.isValid())	{
		saveCursorColor(color.name());
		changeCursorColor(color.name());
	}
}

void AwPrefsDial::changeCursorColor(const QString& color)
{
	buttonCursorColor->setStyleSheet("background: " + color + ";");
	saveCursorColor(color);
}

void AwPrefsDial::changeMappingCursorColor(const QString& color)
{
	buttonMappingColor->setStyleSheet("background: " + color + ";");
	saveMappingCursorColor(color);
}

void AwPrefsDial::selectMappingColor()
{
	QColor color = QColorDialog::getColor(QColor(mappingCursorColor()), this, tr("Edit mapping cursor's color"));
	
	if (color.isValid())	{
		saveMappingCursorColor(color.name());
		changeMappingCursorColor(color.name());
	}
}

void AwPrefsDial::editMappingFont()
{
	bool ok;
	QFont font = QFontDialog::getFont(&ok, mappingCursorFont(), this, tr("Choose Mapping Cursor Font"));
	if (ok)	{
		saveMappingCursorFont(font);
		changeMappingCursorFontText(font);
	}
}

void AwPrefsDial::editCursorFont()
{
	bool ok;
	QFont font = QFontDialog::getFont(&ok, cursorFont(), this, tr("Choose Cursor Font"));
	if (ok)	{
		saveCursorFont(font);
		changeCursorFontText(font);
	}
}

void AwPrefsDial::editMarkerFont()
{
	bool ok;
	QFont font = QFontDialog::getFont(&ok,  markerFont(AwMarker::Single), this, tr("Choose Marker Font"));
	if (ok)	{
		saveMarkerFont(font, AwMarker::Single);
		changeMarkerFontText(font);
	}
}

QString AwPrefsDial::fontToLabel(const QFont &font)
{
	QFontInfo fi(font);
	QString style;
	QString res;
	switch (fi.weight())
	{
	case QFont::Light:
		style = tr("Light");
		break;
	case QFont::Normal:
		style = tr("Normal");
		break;
	case QFont::DemiBold:
		style = tr("DemiBold");
		break;
	case QFont::Bold:
		style = tr("Bold");
		break;
	case QFont::Black:
		style = tr("Black");
		break;
	}

	res = fi.family() + " " + QString::number(fi.pointSize()) + " " + style;
	return res;
}

void AwPrefsDial::changeCursorFontText(const QFont &font)
{
	QString label = fontToLabel(font);
	labelCursorFont->setText(label);
}

void AwPrefsDial::changeMappingCursorFontText(const QFont &font)
{
	QString label = fontToLabel(font);
	labelMappingFont->setText(label);
}

void AwPrefsDial::changeMarkerFontText(const QFont &font)
{
	QString label = fontToLabel(font);
	labelMarkerFont->setText(label);
}


void AwPrefsDial::pickGARDEL()
{
	QString path = QFileDialog::getOpenFileName(this, tr("GARDEL"), "/");
	if (path.isEmpty())
		return;
	lineEditGARDEL->setText(path);
}


void AwPrefsDial::pickPython()
{
	QFileDialog dlg;
	dlg.setFileMode(QFileDialog::ExistingFile);
	if (dlg.exec() == QFileDialog::Accepted) {
		lineEditPythonPath->setText(dlg.selectedFiles().at(0));
		QSettings settings;
		settings.setValue("py/interpreter", lineEditPythonPath->text());
	}
}


void AwPrefsDial::pickMCRFolder()
{
	QFileDialog dlg;
	dlg.setFileMode(QFileDialog::Directory);
	dlg.setOption(QFileDialog::ShowDirsOnly);
	if (dlg.exec() == QFileDialog::Accepted)
		lineEditMCR->setText(dlg.selectedFiles().at(0));
}

