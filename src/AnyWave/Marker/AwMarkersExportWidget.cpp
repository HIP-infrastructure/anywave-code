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
#include "AwMarkersExportWidget.h"
#include "Prefs/AwSettings.h"
#include <qfiledialog.h>
#include <widget/AwMessageBox.h>
#include "Montage/AwMontageManager.h"
#include <AwFileIO.h>
#include "Data/AwDataManager.h"

AwMarkersExportWidget::AwMarkersExportWidget(const AwMarkerList& selection, const AwMarkerList& markers, QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	m_all = markers;
	m_selected = selection;
	if (!selection.isEmpty())
		m_ui.radioSelected->setChecked(true);
	else
		m_ui.radioAll->setChecked(true);

	m_ui.lineEditFile->setText(QString("%1/%2").arg(AwDataManager::instance()->dataDir()).arg("Export"));

	connect(m_ui.buttonSelectFile, &QPushButton::clicked, this, &AwMarkersExportWidget::selectFile);
}

AwMarkersExportWidget::~AwMarkersExportWidget()
{
}


void AwMarkersExportWidget::selectFile()
{
	QString filter = m_ui.radioMATLAB->isChecked() ? "*.mat" : "*.ades";
	QString file = QFileDialog::getSaveFileName(this, "Export to file", AwDataManager::instance()->dataDir(), filter);
	if (file.isEmpty())
		return;
	m_ui.lineEditFile->setText(file);
}

bool AwMarkersExportWidget::checkFile()
{
	m_file = m_ui.lineEditFile->text();
	if (m_file.isEmpty())
		return false;

	if (m_ui.radioADES->isChecked()) {
		if (!m_file.contains(".ades")) {
			m_file = QString("%1%2").arg(m_file).arg(".ades");
			m_ui.lineEditFile->setText(m_file);
		}
		return true;
	}
	if (m_ui.radioMATLAB->isChecked()) {
		if (!m_file.contains(".mat")) {
			m_file = QString("%1%2").arg(m_file).arg(".mat");
			m_ui.lineEditFile->setText(m_file);
		}
		return true;
	}
	return false;
}

void AwMarkersExportWidget::accept()
{
	// prepare markers to be used by the exporter process
	AwMarkerList source = m_ui.radioAll->isChecked() ? m_all : m_selected;

	for (auto m : source) {
		if (m->duration() == 0.) {
			float start = m->start() - (float)m_ui.spinBefore->value() / 1000;
			if (start < 0.)
				start = 0.;
			float end = m->start() + (float)m_ui.spinAfter->value() / 1000;
			m_markers << new AwMarker("export", start, end - start);
		}
		else {
			float start = m->start();
			float end = m->end();
			if (m_ui.checkApplyToSelection->isChecked()) {
				float start = m->start() - (float)m_ui.spinBefore->value() / 1000;
				if (start < 0.)
					start = 0.;
				float end = m->start() + (float)m_ui.spinAfter->value() / 1000;
			//	m_markers << new AwMarker("export", start, end - start);
			}
			m_markers << new AwMarker("export", start, end - start);
		}
	}
	// check if dest contains markers with duration
	bool ok = true;
	for (auto m : m_markers) {
		if (m->duration() == 0.) {
			ok = false;
			break;
		}
	}
	if (!ok) {
		AwMessageBox::critical(this, tr("Exported Markers"), tr("Some markers are invalid.\nCheck latencies settings."));
		while (!m_markers.isEmpty())
			delete m_markers.takeFirst();
		return;
	}

	// build channels
	if (m_ui.radioCurrentMontage->isChecked())
		m_channels = AwChannel::duplicateChannels(AwMontageManager::instance()->channels());
	else
		m_channels = AwChannel::duplicateChannels(AwDataManager::instance()->rawChannels());

	if (m_ui.radioRawData->isChecked()) {
		for (auto c : m_channels) {
			c->setLowFilter(0.);
			c->setHighFilter(0.);
			c->setNotch(0.);
		}
	}
	if (!checkFile()) {
		AwMessageBox::critical(this, tr("Exported Markers"), tr("Invalid Output file."));
		return;
	}
	QDialog::accept();
}