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
#include "AwAddVenvDial.h"
#include "AwPython.h"

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

	// MATLAB
	auto matlabs = aws->detectedMATLABApplications();
	if (matlabs.isEmpty()) {
	//	groupMATLAB->hide();
		auto model = new QStandardItemModel(1, 1, this);
		model->setHorizontalHeaderLabels({ "Status" });
		tableViewMATLAB->setModel(model);
		auto item = new QStandardItem("No MATLAB application detected.");
		item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		model->setItem(0, 0, item);
		buttonDefault->hide();
	}
	else {

		auto model = new QStandardItemModel(0, 3, this);
		tableViewMATLAB->setModel(model);
#ifdef Q_OS_WIN
		tableViewMATLAB->setToolTip(QString("The MATLAB default version is the last one installed on the computer.\n") +
			QString("Changing this may have no real impact until you register it as the default COM server."));
#endif
		model->setHorizontalHeaderLabels({ "Status", "MATLAB Version", "Path" });
		tableViewMATLAB->setEditTriggers(QAbstractItemView::NoEditTriggers);
		int row = 0;
		auto defaultMatlab = aws->value(aws::default_matlab).toString();
		QMenu* menu = new QMenu(this);
		for (auto const& k : matlabs.keys()) {
			auto item = new QStandardItem;
			item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
			if (k == defaultMatlab) 
				item->setText("Default");
			model->setItem(row, 0, item);
			item = new QStandardItem(k);
			item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
			model->setItem(row, 1, item);
			auto action = menu->addAction(k);
			action->setData(k);
			connect(action, &QAction::triggered, this, &AwPrefsDial::changeDefaultMATLAB);
			model->setItem(row++, 2, new QStandardItem(matlabs.value(k)));
		}
		buttonDefault->setMenu(menu);
		if (model->rowCount() == 1) 
			buttonDefault->hide();
	}
	auto runtimes = aws->detectedMATLABRuntimes();
	if (runtimes.isEmpty()) {
		//groupRuntime->hide();
		auto model = new QStandardItemModel(1, 1, this);
		model->setHorizontalHeaderLabels({ "Status" });
		tableViewRuntime->setModel(model);
		auto item = new QStandardItem("No MATLAB Runtime detected.");
		item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		model->setItem(0, 0, item);
		buttonRuntimeDefault->hide();
	}
	else {
		auto model = new QStandardItemModel(0, 3, this);

		tableViewRuntime->setModel(model);
		// on windows the default Runtime is the one in first position in the PATH (the last one installed)
		// so we don't show the Status column
		model->setHorizontalHeaderLabels({ "Status", "Runtime Version", "Path" });
		int row = 0;
		auto defaultRuntime = aws->value(aws::default_runtime).toString();
		QMenu* menu = new QMenu(this);
		for (auto const& k : runtimes.keys()) {
			auto item = new QStandardItem;
			item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
			if (k == defaultRuntime) 
				item->setText("Default");
			model->setItem(row, 0, item);
			item = new QStandardItem(k);
			item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
			model->setItem(row, 1, item);
			auto action = menu->addAction(k);
			action->setData(k);
			connect(action, &QAction::triggered, this, &AwPrefsDial::changeDefaultRuntime);
			model->setItem(row++, 2, new QStandardItem(runtimes.value(k)));
		}
		buttonRuntimeDefault->setMenu(menu);
		if (model->rowCount() == 1) 
			buttonRuntimeDefault->hide();
	}

    editGARDEL->setText(qsettings.value("GARDEL/path").toString());
	editITK->setText(qsettings.value("ITK-SNAP/path").toString());
	radioTriggerParserOn->setChecked(aws->value(aws::auto_trigger_parsing).toBool());

	int markerMode = AwSettings::getInstance()->value(aws::markerbar_mode_default).toInt();
	if (markerMode == 0)
		radioMarkerGlobal->setChecked(true);
	else
		radioMarkerClassic->setChecked(true);

	bool def = aws->value(aws::python_use_default).toBool();
	if (def) {
		radioDefault->setChecked(true);
		radioDefault->clicked(true);
	}
	else {
		radioVenv->setChecked(true);
		radioVenv->clicked(true);
	}

	// init venvs table view
	m_venvsModel = new QStandardItemModel(0, 3, this);
	m_venvsModel->setHorizontalHeaderLabels({ "Status", "Alias", "Path" });
	tableVenvs->setModel(m_venvsModel);
	connect(buttonAddVenv, &QPushButton::clicked, this, &AwPrefsDial::addPythonVenv);
	connect(buttonSetAsDefault, &QPushButton::clicked, this, &AwPrefsDial::setSelectedVenvAsDefault);
	connect(buttonRemove, &QPushButton::clicked, this, &AwPrefsDial::removeSelectedVenv);

	auto header = tableVenvs->horizontalHeader();
	header->setDefaultAlignment(Qt::AlignHCenter);
	initVenvTable();
	setDefaultVenv(aws->value(aws::python_venv_alias).toString());
	// select tab
	tabwidget_Prefs->setCurrentIndex(tab);
}

void AwPrefsDial::addPythonVenv()
{
	AwAddVenvDial dlg;

	if (dlg.exec() == QDialog::Accepted) 
		addVenv(dlg.m_alias, dlg.m_directory);
}

void AwPrefsDial::setDefaultVenv(const QString& alias)
{
	for (int r = 0; r < m_venvsModel->rowCount(); r++) {
		auto item = m_venvsModel->item(r, 1); 
		auto checkItem = m_venvsModel->item(r, 0);
		if (item->text() == alias) {
			checkItem->setCheckState(Qt::Checked);
			AwSettings::getInstance()->setValue(aws::python_venv_alias, alias);
			AwSettings::getInstance()->setValue(aws::python_venv_dir, m_venvsMap.value(alias));
		}
		else {
			checkItem->setCheckState(Qt::Unchecked);
		}
	}
}

void AwPrefsDial::addVenv(const QString& alias, const QString& dir)
{
	if (m_venvsMap.contains(alias)) {
		QMessageBox::information(this, "venv Alias", "an alias with the same name already exists.");
		return;
	}
	// we assume here dir is really a venv dir (not checking that here)
	m_venvsMap.insert(alias, dir);
	auto checkItem = new QStandardItem("active");
	checkItem->setCheckable(true);
	checkItem->setCheckState(Qt::Checked);
	int row = m_venvsModel->rowCount();
	m_venvsModel->setItem(row, 0, checkItem);
	m_venvsModel->setItem(row, 1, new QStandardItem(alias));
	m_venvsModel->setItem(row, 2, new QStandardItem(dir));

	setDefaultVenv(alias);
}


void AwPrefsDial::initVenvTable()
{
	m_venvsModel->removeRows(0, m_venvsModel->rowCount());
	QStringList venvs = AwSettings::getInstance()->value(aws::python_venv_list).toStringList();

	// detect if we are running a version containing anywave venv
	AwPython python;
	auto pySettings = python.detect(AwSettings::getInstance()->value(aws::python_embeded_venv_dir).toString());
	if (pySettings.isDetected()) {
		// always set the default venv
		auto checkItem = new QStandardItem("active");
		checkItem->setCheckable(true);
		checkItem->setCheckState(Qt::Checked);
		m_venvsModel->setItem(0, 0, checkItem);
		m_venvsModel->setItem(0, 1, new QStandardItem("anywave"));
		m_venvsModel->setItem(0, 2, new QStandardItem("embedded"));
	}

	if (!venvs.isEmpty()) {
		int row = 1, col = 0;
		for (auto const& v : venvs) {
			auto splitted = v.split("::");
			if (splitted.size() == 2) {
				m_venvsMap.insert(splitted.first(), splitted.last());
				auto check = new QStandardItem("active");
				check->setCheckable(true);
				check->setCheckState(Qt::Checked);
				m_venvsModel->setItem(row, col++, check);
				m_venvsModel->setItem(row, col++, new QStandardItem(splitted.first()));
				m_venvsModel->setItem(row++, col, new QStandardItem(splitted.last()));
				col = 0;
			}
		}
	}
}

void AwPrefsDial::setSelectedVenvAsDefault()
{
	QModelIndexList selection = tableVenvs->selectionModel()->selectedRows();
	if (selection.isEmpty())
		return;
	auto const& selected = selection.first();
	setDefaultVenv(m_venvsModel->item(selected.row(), 1)->text());
}

void AwPrefsDial::removeSelectedVenv()
{
	QModelIndexList selection = tableVenvs->selectionModel()->selectedRows();
	if (selection.isEmpty())
		return;
	auto const& selected = selection.first();
	// DO NOT REMOVE anywave embedded venv
	if (selected.row() == 0)
		return;
	m_venvsMap.remove(m_venvsModel->item(selected.row(), 1)->text());
	m_venvsModel->removeRow(selected.row());
	// fall back to default anywave venv as the active one
	setDefaultVenv("anywave");
}

void AwPrefsDial::changeEvent(QEvent *e)
{
	if (e)
		if (e->type() == QEvent::LanguageChange)
			retranslateUi(this);
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

	if (aws->MATLABDetected()) {
		// get MATLAB model
		QStandardItemModel* model = static_cast<QStandardItemModel*>(tableViewMATLAB->model());
		bool ok = false;

		for (auto i = 0; i < model->rowCount(); i++) {
			auto status = model->item(i, 0);
			auto release = model->item(i, 1)->text();
			auto path = model->item(i, 2)->text();
			if (status->text() == "Default") {
				auto matlabPath = qsettings.value("matlab/path", QString()).toString();
				if (matlabPath != path) {
					qsettings.setValue("matlab/path", path);
					qsettings.setValue("matlab/default", release);
				}
				aws->setValue(aws::default_matlab, release);
			}
		}
	}
	if (aws->MATLABRuntimeDetected()) {
		// get MATLAB Runtime model
		QStandardItemModel* model = static_cast<QStandardItemModel*>(tableViewRuntime->model());
		for (auto i = 0; i < model->rowCount(); i++) {
			auto status = model->item(i, 0)->text();
			auto release = model->item(i, 1)->text();
			auto path = model->item(i, 2)->text();
			if (status == "Default") {
				auto matlabPath = qsettings.value("matlab/mcr_path", QString()).toString();
				if (matlabPath != path) {
					qsettings.setValue("matlab/mcr_path", path);
					qsettings.setValue("matlab/mcr_default", release);
				}
				aws->setValue(aws::default_runtime, release);
			}
		}
	}

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

	int markerMode = 0;
	if (!radioMarkerGlobal->isChecked()) 
		markerMode = 1;
	qsettings.setValue("general/markerbar_mode", markerMode);
	aws->setValue(aws::markerbar_mode_default, markerMode);

	// save python settings
	qsettings.setValue("python/use_default", radioDefault->isChecked());
	qsettings.setValue("python/venv_alias", AwSettings::getInstance()->value(aws::python_venv_alias));
	qsettings.setValue("python/venv_dir", AwSettings::getInstance()->value(aws::python_venv_dir));
	if (!m_venvsMap.isEmpty()) {
		qsettings.beginWriteArray("venvs");
		auto const& keys = m_venvsMap.keys();
		for (int i = 0; i < keys.size(); i++) {
			qsettings.setArrayIndex(i);
			QString value = QString("%1::%2").arg(keys.at(i)).arg(m_venvsMap.value(keys.at(i)));
			qsettings.setValue("venv", value);
		}
		qsettings.endArray();
	}
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

void AwPrefsDial::changeDefaultRuntime()
{
	QAction* action = static_cast<QAction*>(sender());
	if (action == nullptr)
		return;
	auto model = static_cast<QStandardItemModel*>(tableViewRuntime->model());
	auto release = action->data().toString();
	for (auto i = 0; i < model->rowCount(); i++) {
		auto r = model->item(i, 1)->text();
		if (r.contains(release)) 
			model->item(i, 0)->setText("Default");
		else
			model->item(i, 0)->setText("");
	}
}

void AwPrefsDial::changeDefaultMATLAB()
{
	QAction* action = qobject_cast<QAction*>(sender());
	if (action == nullptr)
		return;
	auto model = static_cast<QStandardItemModel*>(tableViewMATLAB->model());
	auto release = action->data().toString();
	for (auto i = 0; i < model->rowCount(); i++) {
		auto r = model->item(i, 1)->text();
		if (r.contains(release)) 
			model->item(i, 0)->setText("Default");
		else
			model->item(i, 0)->setText("");
	}
}