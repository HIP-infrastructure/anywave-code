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

#include "rewrite4d.h"
#include <QMessageBox>
#include <AwFileIO.h>
#include "rewriteui.h"
#include "ChooseMarker.h"
#ifdef USE_ARMADILLO
#include <math/AwMath.h>
#endif
#include <AwException.h>

void toBigEndian(const long long int size, void *value)
{
	int i;
	char result[32];
	for (i = 0; i < size; i++) {
		result[i] = ((char *)value)[size - i -1];
	}
	for (i = 0; i < size; i++)
		((char *)value)[i] = result[i];
}

inline quint32 fromBigEndian(const uchar *src)
{
	return 0
		| src[3]
		| src[2] * quint32(0x00000100)
		| src[1] * quint32(0x00010000)
		| src[0] * quint32(0x01000000);
}

#define SHORT	1
#define LONG	2
#define FLOAT	3
#define DOUBLE	4

Rewrite4DPlugin::Rewrite4DPlugin()
{
	type = AwProcessPlugin::Background;
	name = tr("Modify and copy 4DNI data file");
	description = tr("Apply filters and/or overwrite data to a new 4DNI file.");
	category = "File:(4DNI MEG File Format) Write to a new file";
	setFlags(Aw::ProcessFlags::ProcessHasInputUi);
}

Rewrite4D::Rewrite4D()
{
	setInputFlags(Aw::ProcessIO::GetAllMarkers);
}

Rewrite4D::~Rewrite4D()
{

}

bool Rewrite4D::showUi()
{
	// check for current reader
	if (pdi.input.reader()->plugin()->name != "4DNI Reader")	{
		QMessageBox::information(0, tr("Wrong File Format"), tr("The currently open file is not a 4DNI MEG file."));
		return false;
	}
	
	RewriteUi ui(pdi.input.reader()->fullPath());

	if (ui.exec() == QDialog::Accepted)	{
		m_newFilename = ui.newFilename();
		return true;
	}

	return false;
}

qint64 Rewrite4D::offsetFilePointer(const QFile& file)
{
	qint64 current = file.pos();
	qint64 offset = 0;
	if (current % 8 != 0) {
		offset = 8 - current % 8;

	}
	return offset;
}


void Rewrite4D::alignFilePointer(QFile& file)
{
	qint64 current = file.pos();
	qint64 offset = 0;
	if (current % 8 != 0) {
		offset = 8 - current % 8;
		file.seek(current + offset);
	}
}

void Rewrite4D::run()
{
	AwFileIO *reader = pdi.input.reader();
	if (reader->plugin()->name != "4DNI Reader") {
		sendMessage("Error: the current file is not a 4DNI MEG File.");
		return;
	}

	QFile newFile(m_newFilename);
	QDataStream stream(&newFile);
	stream.setVersion(QDataStream::Qt_4_4);

	QFile srcFile(reader->fullPath());
	if (!srcFile.open(QIODevice::ReadOnly)) {
		sendMessage(tr("Error opening source file for reading."));
		return;
	}
    
    if (!newFile.open(QIODevice::WriteOnly)) {
        sendMessage(tr("Error opening destination file for writing."));
        return;
    }
    sendMessage(QString("File created: %1").arg(m_newFilename));

	// try to find the .bad file
	//QString flt_file = QString("%1.flt").arg(reader->infos.fileName());
	QString mrk_file = QString("%1.mrk").arg(reader->fullPath());
	QString mtg_file = QString("%1.mtg").arg(reader->fullPath());
	QString lvl_file = QString("%1.levels").arg(reader->fullPath());
	QString disp_file = QString("%1.display").arg(reader->fullPath());

	m_badFile = QString("%1.bad").arg(reader->fullPath());
	if (QFile::exists(m_badFile)) {
		QFile bad(m_badFile);
		QTextStream stream(&bad);
		if (bad.open(QIODevice::ReadOnly|QIODevice::Text)) {
			while (!stream.atEnd()) {
				m_badLabels << stream.readLine();
			}
		}
		QFile::copy(m_badFile, QString("%1.bad").arg(m_newFilename));
	}
	//if (QFile::exists(flt_file))
	//	QFile::copy(flt_file, QString("%1.flt").arg(m_newFilename));
	if (QFile::exists(mrk_file))
		QFile::copy(mrk_file, QString("%1.mrk").arg(m_newFilename));
	if (QFile::exists(mtg_file))
		QFile::copy(mtg_file, QString("%1.mtg").arg(m_newFilename));
	if (QFile::exists(lvl_file))
		QFile::copy(lvl_file, QString("%1.levels").arg(m_newFilename));
	if (QFile::exists(disp_file))
		QFile::copy(disp_file, QString("%1.display").arg(m_newFilename));

	QDataStream src_stream(&srcFile);
	src_stream.setVersion(QDataStream::Qt_4_4);
	qint64 srcHeaderPos; 
	qint64 srcHeaderSize;
		
	srcFile.seek(srcFile.size() - 8);
	src_stream >> srcHeaderPos;
	srcHeaderSize = (srcFile.size() - 8) - srcHeaderPos;

	AwChannelList channels;
	for (auto c : reader->infos.channels())
		channels << c->duplicate();
	
	float totalDuration = reader->infos.totalDuration(); 
    sendMessage(QString("Loading %1 seconds of data...").arg(totalDuration));
	pdi.input.filterSettings.apply(channels);
	requestData(&channels, 0., totalDuration);
    if (hasErrorOccured()) {
        sendMessage(errorString());
        srcFile.close();
        newFile.close();
        while (!channels.isEmpty())
           delete channels.takeFirst();
        return;
    }
    if (endOfData()) {
        sendMessage(QString("endOfDataflag: %1").arg(errorString()));
        srcFile.close();
        newFile.close();
        while (!channels.isEmpty())
            delete channels.takeFirst();
        return;
    }
    sendMessage(QString(tr("Done.")));
	if (!endOfData()) {
#ifdef USE_ARMADILLO
		sendMessage(QString(tr("Preparing data for output...")));
		uword cols = channels.first()->dataSize();
		uword rows = channels.size();
		uword totalSamples = rows * cols;
		fmat arma_matrix;
		try {
			arma_matrix = fmat(rows, cols);
		}
		catch (std::bad_alloc&) {
			sendMessage(tr("Memory error allocating whole data matrix."));
			srcFile.close();
			newFile.close();
			while (!channels.isEmpty())
				delete channels.takeFirst();
			emit progressChanged(tr("Aborted."));
			return;
		}

		// insert source MEG channels data into matrix (Channels are not only MEG type)
		for (int i = 0; i < channels.size(); i++) {
			AwChannel *chan = channels.at(i);
            float factor = 1.;
            if (chan->isMEG() || chan->isReference())
                factor = 1e-12;
            else if (chan->isEEG() || chan->isECG() || chan->isEMG())
                factor = 1e-6;
			for (uword c = 0; c < cols; c++) {
				arma_matrix(i, c) = chan->data()[c] * factor;
			}
			channels.at(i)->clearData();
		}

		sendMessage(tr("Converting to BIG ENDIAN..."));
		// convert matrix to BIG ENDIAN
		float *data = arma_matrix.memptr();

		for (qint64 i = 0; i < totalSamples; i++) {
			float val = data[i];
			quint32 be = fromBigEndian((uchar *)&val);
			memcpy(&data[i], &be, 4);
		}
		sendMessage(tr("Done."));
		// create a new file converting short values to real float values
		emit progressChanged(tr("Writing to the new MEG file..."));
        qint64 bytesLeft = totalSamples * sizeof(float);
        qint64 chunkSize = std::min((qint64)(500 * 1024 * 1024), (qint64)(totalSamples * sizeof(float)));
        qint64 written = 0;
        char *matrix_data = (char *)arma_matrix.memptr();
        while (bytesLeft) {
            written = newFile.write(matrix_data, chunkSize);
            if (written == -1)
                break;
            bytesLeft -= written;
            chunkSize = std::min(qint64(500 * 1024 * 1024), bytesLeft);
            matrix_data += written;
        }
        if (written == -1) {
            sendMessage("Error writing data.");
            srcFile.close();
            newFile.close();
            while (!channels.isEmpty())
                delete channels.takeFirst();
            return;
        }
		emit progressChanged(tr("Done."));
#else // MAC Specific code
		sendMessage(QString(tr("Saving samples...")));
		
		for (qint64 j = 0; j < channels.first()->dataSize(); j++) {
			for (int i = 0; i < channels.size(); i++) {
				AwChannel *chan = channels.at(i);
				float val = chan->data()[j];
				if (chan->isMEG() || chan->isReference())
					val *= 1e-12;
				else if (chan->isEEG() || chan->isECG() || chan->isEMG())
					val *= 1e-6;
				stream << val;
			}
		}
		sendMessage("Done."); 
#endif
	}    
	// position the header on a 8 bytes alignment
	qint64 bytes = offsetFilePointer(newFile);
	if (bytes)	{
		char *dummy = new char[bytes];
		memset(dummy, 0, bytes);
		stream.writeRawData(dummy, bytes);
		delete[] dummy;
	}

	// copy old header
	qint64 newHeaderPos = newFile.pos();
	char *header = new char[srcHeaderSize];
	srcFile.seek(srcHeaderPos);
	alignFilePointer(srcFile);
	src_stream.readRawData(header, srcHeaderSize);
	stream.writeRawData(header, srcHeaderSize);
	// write offset to header
	stream << newHeaderPos;
	delete[] header;

	// update header info
	newFile.seek(newHeaderPos);
	alignFilePointer(newFile);
	newFile.seek(newFile.pos() + 8);
	//stream.skipRawData(8);
	// modify format :
	stream << (qint16)3; // 3 is float data

	srcFile.close();
	newFile.close();
	while (!channels.isEmpty())
		delete channels.takeFirst();
}
