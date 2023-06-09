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
#include "AwFilePropertiesDialog.h"
#include <AwFileIO.h>
#include "AwFilePropertiesModel.h"
#include <QFileDialog>
#include <QTextStream>

AwFilePropertiesDialog::AwFilePropertiesDialog(AwFileIO *reader, QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);

	auto patientName = QString("%1 %2").arg(reader->infos.firstName()).arg(reader->infos.lastName());
	m_ui.lineEditPatient->setText(patientName);

	if (!reader->infos.recordingDate().isEmpty())
		m_ui.lineEditDate->setText(reader->infos.recordingDate());
	else
		m_ui.lineEditDate->setText(tr("n/a"));

	if (!reader->infos.recordingTime().isEmpty())
		m_ui.lineEditTime->setText(reader->infos.recordingTime());
	else
		m_ui.lineEditTime->setText(tr("n/a"));

	m_ui.lineEditnChannels->setText(QString::number(reader->infos.channelsCount()));

	int nMeg = 0, nRef = 0, nEeg = 0, nSeeg = 0, nEmg = 0, nEcg = 0, nTrigger = 0, nOther = 0;
	float sr = 0;
	AwChannelList channels = reader->infos.channels();
	foreach (AwChannel *c, channels)
	{
		if (c->samplingRate() > sr)
			sr = c->samplingRate();
		switch (c->type())
		{
		case AwChannel::Reference:
			nRef++;
			break;
		case AwChannel::MEG:
			nMeg++;
			break;
		case AwChannel::EEG:
			nEeg++;
			break;
		case AwChannel::SEEG:
			nSeeg++;
			break;
		case AwChannel::ECG:
			nEcg++;
			break;
		case AwChannel::EMG:
			nEmg++;
			break;
		case AwChannel::Trigger:
			nTrigger++;
			break;
		default:
			nOther++;
		}
	}

	m_ui.lineEditSamplingRate->setText(QString::number(sr) + " Hz");

	if (!nRef)
	{
		m_ui.labelRef->hide();
		m_ui.labelRef->buddy()->hide();
	}
	else m_ui.lineEditnRefChannels->setText(QString::number(nRef));
	
	if (!nMeg)
	{
		m_ui.labelMEG->hide();
		m_ui.labelMEG->buddy()->hide();
	}
	else m_ui.lineEditnMEGChannels->setText(QString::number(nMeg));

	if (!nEeg)
	{
		m_ui.labelEEG->hide();
		m_ui.labelEEG->buddy()->hide();
	}
	else m_ui.lineEditnEEGChannels->setText(QString::number(nEeg));

	if (!nSeeg)
	{
		m_ui.labelSEEG->hide();
		m_ui.labelSEEG->buddy()->hide();
	}
	else m_ui.lineEditnSEEGChannels->setText(QString::number(nSeeg));

	if (!nEmg)
	{
		m_ui.labelEMG->hide();
		m_ui.labelEMG->buddy()->hide();
	}
	else m_ui.lineEditnEMGChannels->setText(QString::number(nEmg));

	if (!nEcg)
	{
		m_ui.labelECG->hide();
		m_ui.labelECG->buddy()->hide();
	}
	else m_ui.lineEditnECGChannels->setText(QString::number(nEcg));

	if (!nTrigger)
	{
		m_ui.labelTrigger->hide();
		m_ui.labelTrigger->buddy()->hide();
	}
	else m_ui.lineEditnTriggerChannels->setText(QString::number(nTrigger));

	if (!nOther)
	{
		m_ui.labelOther->hide();
		m_ui.labelOther->buddy()->hide();
	}
	else m_ui.lineEditnOtherChannels->setText(QString::number(nOther));

	m_model = new AwFilePropertiesModel(channels);
	m_ui.tvChannelsAsRecorded->setModel(m_model);
	m_reader = reader;
}

AwFilePropertiesDialog::~AwFilePropertiesDialog()
{
	delete m_model;
}


void AwFilePropertiesDialog::changeEvent(QEvent *e)
{
	if (e)
		if (e->type() == QEvent::LanguageChange)
			m_ui.retranslateUi(this);
}




void AwFilePropertiesDialog::exportLabels()
{
	QString filename = QFileDialog::getSaveFileName(this, tr("Save labels"), "/", "*.txt");
	if (filename.isEmpty())
		return;

	QFile file(filename);
	QTextStream stream(&file);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		foreach (AwChannel *c, m_reader->infos.channels())
		{
			stream << c->name() << endl;
		}
		file.close();
	}
}