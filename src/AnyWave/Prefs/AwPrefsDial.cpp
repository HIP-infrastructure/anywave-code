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
	auto aws = AwSettings::getInstance();
	QSettings qsettings;

	int sh = qsettings.value("Preferences/screenCalibration/heightMM", 0).toInt();
	int sw = qsettings.value("Preferences/screenCalibration/widthMM", 0).toInt();
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
	bool autoTrigger = aws->value(aws::auto_trigger_parsing).toBool();
	if (autoTrigger)
		radioTriggerParserOn->setChecked(true);
	else
		radioTriggerParserOff->setChecked(true);

	// Time representation
	if ((isTimeHMS()))
		radioHMSOn->setChecked(true);
	else
		radioHMSOff->setChecked(true);

	// CPU Cores
	auto totalCPUCores = aws->value(aws::total_cpu_cores).toInt();
	int cores = aws->value(aws::max_cpu_cores).toInt();
	sliderCPU->setRange(2, totalCPUCores);
	sliderCPU->setValue(cores);

	// check for updates when starting
	checkBoxUpdates->setChecked(aws->value(aws::check_updates).toBool());

	plainTextEdit->setVisible(false);
	// MATLAB
	bool isMatlabDetected = qsettings.value("matlab/detected", false).toBool();

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
	lineEditMatlabPath->setText(qsettings.value("matlab/path").toString());
	labelMCR->hide();
	lineEditMCR->hide();
	buttonSelectMCR->hide();
    
    editGARDEL->setText(qsettings.value("GARDEL/path").toString());
	editITK->setText(qsettings.value("ITK-SNAP/path").toString());
	radioTriggerParserOn->setChecked(aws->value(aws::auto_trigger_parsing).toBool());

	// COMPILED PLUGIN. Windows do not required environments variables to be set but Linux and Mac OS X do.
#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
	labelMCR->show();
	lineEditMCR->show();
	buttonSelectMCR->show();
	lineEditMCR->setText(qsettings.value("matlab/mcr_path").toString());
#endif

	QString python = qsettings.value("py/interpreter", QString()).toString();
//	if (python.isEmpty()) { // set default path to the interpreter name
//#ifdef Q_OS_WIN
//		python = "python.exe";
//#endif
//#if defined Q_OS_MAC || defined Q_OS_LINUX
//		python = "python";
//#endif
//		qsettings.setValue("py/interpreter", python);
//	}
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
	QSettings qsettings;
	int w = spinBoxH->value();
	int h = spinBoxV->value();
	float xPixPerCm = 0., yPixPerCm = 0.;
	
	if (h > 0)	{
		qsettings.setValue("Preferences/screenCalibration/heightMM", h);
		h /= 10;
		yPixPerCm = AW_CALIBRATION_WIDTH / h;
		qsettings.setValue("Preferences/screenCalibration/yPixPerCm", yPixPerCm);
	}
	if (w > 0)	{
		qsettings.setValue("Preferences/screenCalibration/widthMM", w);
		w /= 10;
		xPixPerCm = AW_CALIBRATION_WIDTH / w;
		qsettings.setValue("Preferences/screenCalibration/xPixPerCm", xPixPerCm);
	}

	// calibration has been done
	qsettings.setValue("Preferences/screenCalibration/calibrationDone", true);
	emit screenCalibrationChanged(xPixPerCm, yPixPerCm);

	// CPU CORES
	qsettings.setValue("general/cpu_cores", spinCPU->value());
	
	//// misc. parameters
	AwSettings *aws = AwSettings::getInstance();
	
	saveTimeHMS(radioHMSOn->isChecked());
	aws->setValue(aws::auto_trigger_parsing, radioTriggerParserOn->isChecked());

	QString pythonPath = lineEditPythonPath->text();
	if (!pythonPath.isEmpty()) {
		if (detectPython(pythonPath))
			qsettings.setValue("py/interpreter", pythonPath);
	}
//qsettings.setValue("py/interpreter", lineEditPythonPath->text());

	// if matlab path is empty => consider not using MATLAB plugins anymore.
	// On Mac and Linux that will be done by deleting the matlab.sh script
	if (lineEditMatlabPath->text().isEmpty()) {
		qsettings.setValue("matlab/detected", false);

#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
		QString scriptPath = QString("%1/AnyWave/matlab.sh").arg(AwSettings::getInstance()->value(aws::home_dir).toString());
		if (QFile::exists(scriptPath))
			QFile::remove(scriptPath);
#endif
	}
	else {
		// check if location exists
		QDir dir(lineEditMatlabPath->text());
		if (!dir.exists()) {
			QMessageBox::critical(this, tr("MATLAB"), tr("Invalid MATLAB location"));
			qsettings.setValue("matlab/detected", false);
			return;
		}
		qsettings.setValue("matlab/detected", true);
		qsettings.setValue("matlab/path", lineEditMatlabPath->text());
#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
		AwSettings::getInstance()->createMatlabShellScript(lineEditMatlabPath->text());
#endif
	}
	// MATLAB MCR on Linux and Mac
#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
	if (!lineEditMCR->text().isEmpty())
		qsettings.setValue("matlab/mcr_path", lineEditMCR->text());
#endif
	QString Gardel = editGARDEL->text();
	if (QFile::exists(Gardel))
		qsettings.setValue("GARDEL/path", Gardel);
	QString itk = editITK->text();
	if (QFile::exists(itk))
		qsettings.setValue("ITK-SNAP/path", itk);

	aws->setValue(aws::itk_snap, itk);
	aws->setValue(aws::gardel, Gardel);
	// CPU CORES
	aws->setValue(aws::max_cpu_cores, sliderCPU->value());
    QThreadPool::globalInstance()->setMaxThreadCount(sliderCPU->value());
	// Check for updates
	aws->setValue(aws::check_updates, checkBoxUpdates->isChecked());
	qsettings.setValue("general/checkForUpdates", checkBoxUpdates->isChecked());
	// auto detect triggers
	qsettings.setValue("Preferences/autoTriggerParsing", radioTriggerParserOn->isChecked());
	aws->setValue(aws::auto_trigger_parsing, radioTriggerParserOn->isChecked());
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

bool AwPrefsDial::detectPython(const QString& path)
{
#ifdef Q_OS_WIN
	QString venvDir = "Scripts";
#else
	QString venvDir = "bin";
#endif
	QDir dir(path);
	// search for a virtual env
	QString tmp = path + "/";
	QStringList dirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
	if (dirs.contains(venvDir)) {
		tmp += venvDir;
		// find activate script and python exe
		QDir subDir(tmp);
		QStringList files = subDir.entryList(QDir::Files);
		bool ok = false;
#ifdef Q_OS_WIN
		ok = files.contains("activate.bat") && files.contains("python.exe");
#else
		ok = files.contains("activate") && files.contains("python");
#endif
		if (ok) {
			// found a venv
			QSettings qsettings;
			qsettings.setValue("general/python_venv", path);
			AwSettings::getInstance()->setValue(aws::python_venv_dir, path);
			QString pyExec;
#ifdef Q_OS_WIN
			pyExec = subDir.absoluteFilePath("python.exe");
#else
			pyExec = subDir.absoluteFilePath("python");
#endif
			qsettings.setValue("general/python_interpreter", pyExec);
			AwSettings::getInstance()->setValue(aws::python_exe, pyExec);
			return true;
		}
	}
	else {  // venv not found, check for interpreter path
		QStringList files = dir.entryList(QDir::Files);
		QSettings qsettings;
#ifdef Q_OS_WIN
		if (files.contains("python.exe")) {
			qsettings.setValue("general/python_venv", QString());
			qsettings.setValue("general/python_interpreter", dir.absoluteFilePath("python.exe"));
			return true;
		}
#else
		if (files.contains("python")) {
			qsettings.setValue("general/python_venv", QString());
			qsettings.setValue("general/python_interpreter", dir.absoluteFilePath("python"));
			return true;
		}
#endif
	}

	return false;
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


void AwPrefsDial::pickITK()
{
	QString path = QFileDialog::getOpenFileName(this, tr("ITK-SNAP"), "/");
	if (path.isEmpty())
		return;
	editITK->setText(path);
}

void AwPrefsDial::pickGARDEL()
{
	QString path = QFileDialog::getOpenFileName(this, tr("GARDEL"), "/");
	if (path.isEmpty())
		return;
	editGARDEL->setText(path);
}


void AwPrefsDial::pickPython()
{
	QFileDialog dlg;
	auto dir = dlg.getExistingDirectory(this, "Select the virtual env folder or your Python installation folder");
	if (dir.isEmpty())
		return;
	if (!detectPython(dir)) {
		QMessageBox::information(this, "Python", "No python environment found.");
		return;
	}
	lineEditPythonPath->setText(dir);
}


void AwPrefsDial::pickMCRFolder()
{
	QFileDialog dlg;
	dlg.setFileMode(QFileDialog::Directory);
	dlg.setOption(QFileDialog::ShowDirsOnly);
	if (dlg.exec() == QFileDialog::Accepted)
		lineEditMCR->setText(dlg.selectedFiles().at(0));
}

