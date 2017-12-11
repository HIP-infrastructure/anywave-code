/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
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
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "AwRequestServer.h"
#include "Process/AwScriptPlugin.h"
#include <QDataStream>
#include <QTcpSocket>
#include <QBuffer>
#include "Montage/AwMontageManager.h"
#include "Plugin/AwPluginManager.h"
#include "Data/AwDataServer.h"
#include "Display/AwDisplay.h"
#include <AwFiltering.h>

#include "AwResponse.h"

void AwRequestServer::handleGetData3(QTcpSocket *client, AwScriptProcess *p)
{
	emit log("Processing aw_getdata...");

	AwResponse response(client);

	// get parameters from client
	QByteArray data;
	QDataStream out(&data, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_4);
	QDataStream in(client);
	in.setVersion(QDataStream::Qt_4_4);

	float start, duration;
	int decim;
	QString file, montage;
	QStringList labels, types;
	AwFileIO *reader = NULL;

	in >> file >> montage >> start >> duration >> decim >> labels >> types;
	int flag;
	in >> flag;
	AwFilteringOptions *fo = NULL;
	if (flag == 1) {
		fo = new AwFilteringOptions; 
		float eegH, eegL, megH, megL, emgH, emgL;
		in >> eegH >> eegL >> megH >> megL >> emgH >> emgL;
		fo->eegLP = eegL;
		fo->eegHP = eegH;
		fo->megLP = megL;
		fo->megHP = megH;
		fo->emgLP = emgL;
		fo->emgHP = emgH;
	}
	else if (flag == 2)
		fo = new AwFilteringOptions; 

	// checking parameters
	if (start <= 0)
		start = 0;
	if (duration <= 0)
		duration = -1;

	AwChannelList requestedChannels;
	response.begin();
	QBuffer *buf = response.buffer();
	QDataStream stream(buf);
	bool usingFile = false;

	if (!file.isEmpty()) { // got a file
		// get the plugin to open the file
		reader = AwPluginManager::getInstance()->getReaderToOpenFile(file);
		if (reader) 
			usingFile = reader->openFile(file)  == AwFileIO::NoError;
		if (!usingFile) { // no plugin to open
			stream << (qint64)0;
			response.send();
			if (fo)
				delete fo;
			return;
		}
	}

	bool usingLabels = !labels.isEmpty();
	bool usingTypes = !types.isEmpty();
	if (labels.isEmpty()) { // no labels specified, so use the input channels for the process
		if (usingTypes) { // using types without labels => request channels from montage or file
			if (usingFile)
				//requestedChannels = AwChannel::duplicateChannels(reader->infos.channels());
				requestedChannels = reader->infos.channels(); 
			else 
				requestedChannels = AwChannel::duplicateChannels(AwMontageManager::instance()->channels());

			AwChannelList types_list;
			foreach (QString type, types) {
				int t = AwChannel::stringToType(type);
				foreach(AwChannel *c, requestedChannels) {
					if (c->type() == t) 
						types_list << c->duplicate();
				}
			}
			requestedChannels = types_list;

		}
		else { // NOT USING LABELS NOR TYPES
			if (usingFile)
				requestedChannels = AwChannel::duplicateChannels(reader->infos.channels());
			else
				requestedChannels = p->pdi.input.channels;
		}
	}
	else {  // USING LABELS
		if (usingFile) {
			AwChannelList temp = AwChannel::getChannelWithLabels(reader->infos.channels(), labels);
			if (usingTypes) {
				AwChannelList types_list;
				foreach (QString type, types) {
					int t = AwChannel::stringToType(type);
					foreach(AwChannel *c, temp) {
						if (c->type() == t) 
							types_list << c;
					}
				}
				requestedChannels = AwChannel::duplicateChannels(types_list);
			}
			else
				requestedChannels = AwChannel::duplicateChannels(temp);
		}
		else {
			AwChannelList temp = AwMontageManager::instance()->channelsWithLabels(labels);
			if (usingTypes) {
				AwChannelList types_list;
				foreach (QString type, types) {
					int t = AwChannel::stringToType(type);
					foreach(AwChannel *c, temp) {
						if (c->type() == t) 
							types_list << c;
					}
				}
				requestedChannels = AwChannel::duplicateChannels(types_list);
			}
			else 
				requestedChannels = AwChannel::duplicateChannels(temp);
		}
	}
	
	// check if requested channels are empty
	if (requestedChannels.isEmpty()) {
		emit log("No channels found using labels and/or types criterias");
		stream << (qint64)0;
		response.send();
		if (fo)
			delete fo;
		return;
	}

	if (usingFile) {
		AwDataServer::getInstance()->openConnection(this, reader);
		// ATTENTION:  opening a new connection with the same client, will close the previous one (i.e. the connection AwRequestServer made when starting)
		// Do not forget to open again a connection.
		if (montage.toUpper() == "SEEG BIPOLAR") 
			requestedChannels = AwMontageManager::makeSEEGBipolar(requestedChannels);
	}

	// requesting data
#ifndef NDEBUG
	qDebug() << Q_FUNC_INFO << "requesting data..." << endl;
#endif
	if (fo)
		fo->setFilters(requestedChannels);

	if (decim > 1) {
		requestData(&requestedChannels, start, duration, true);
		AwFiltering::decimate(requestedChannels, decim);
		AwFiltering::filter(&requestedChannels);
	}
	else 
		requestData(&requestedChannels, start, duration);
#ifndef NDEBUG
	qDebug() << Q_FUNC_INFO << "data ready." << endl;
#endif
	stream << requestedChannels.size();
	response.send();
	if (fo)
		delete fo;

	QStringList selected = AwChannel::getLabels(AwDisplay::instance()->selectedChannels());
	foreach (AwChannel *c, requestedChannels) {
		// get selected channels from Display if not using file
		if (!usingFile) 			
			c->setSelected(selected.contains(c->name()));
		response.begin();
		stream.setDevice(response.buffer());
		stream.setVersion(QDataStream::Qt_4_4);
		int selected = c->isSelected() ? (int)1 : (int)0;

		stream << c->name();
		stream << AwChannel::typeToString(c->type());
		stream << c->referenceName();
		stream << c->samplingRate();
		stream << c->highFilter();
		stream << c->lowFilter();
		stream << c->dataSize();
		stream << selected;
		response.send();

		if (c->dataSize()) { // if there are data, send them
			bool finished = false;
			qint64 chunkSize = 200000;	// chunk of 200000 samples
			qint64 nSamplesSent = 0;
			qint64 nSamplesLeft = c->dataSize();

			while (!finished) {
				response.begin();
				stream.setDevice(response.buffer());
				stream.setVersion(QDataStream::Qt_4_4);
				if (nSamplesLeft == 0) {
					chunkSize = 0;
					stream << chunkSize;
					response.send();
					finished = true;
					continue;
				}
				if (nSamplesLeft < chunkSize) 
					chunkSize = nSamplesLeft;
				stream << chunkSize;
				emit log("Sending chunk of data (size is " + QString::number(chunkSize) + " samples)");
				for (int i = 0; i < chunkSize; i++) 
					stream << c->data()[i + nSamplesSent];
				nSamplesSent += chunkSize;
				response.send();
				nSamplesLeft -= chunkSize;
			} // end of while
		}
	}

	// finished sending data
	if (usingLabels || usingFile) {
		while (!requestedChannels.isEmpty())
			delete requestedChannels.takeFirst();
	}
	else
		AwChannel::clearData(requestedChannels);

	emit log("Done.");
}
