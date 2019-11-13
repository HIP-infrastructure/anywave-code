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
#include "AwConverter.h"
#define AW_CONVERTER_BLOCK	30		// 30 seconds block length

AwConverterPlugin::AwConverterPlugin()
{
	name = QString("File Format Converter");
	description = QString(tr("Export data to file"));
	version = "1.0";
	type = AwProcessPlugin::Background;
}

AwConverter::AwConverter() : AwProcess()
{
	pdi.addInputChannel(-1, 1, 0);
}

AwConverter::~AwConverter()
{
}

void AwConverter::run()
{
	auto list = pdi.input.channels();	    // Les canaux a convertir/exporter
	auto markers = pdi.input.markers();		// Les marqueurs à convertir/exporter
	m_writer->infos.setChannels(list);
	
	// markers should contain one and only one marker
	if (markers.isEmpty())	{
		m_writer->plugin()->deleteInstance(m_writer);
		return;
	}

	// see which marker from global list of markers is present in the range of local markers
	AwMarker *m = markers.first();
	// add at least one block
	AwBlock *block = m_writer->infos.newBlock();
	block->setDuration(m->duration());
	block->setSamples(pdi.input.channels().first()->samplingRate() * m->duration());

	QList<AwMarker *> markersToWrite;

	foreach (AwMarker *mm, m_markers) {
		if (mm->start() >= m->start() && (mm->start() + mm->duration()) <= (m->start() + m->duration())) {
			// beware to duplicate marker otherwise writer plugin will delete it...
			AwMarker *newMarker = new AwMarker(mm);
			// re set offset of marker. start is given relative to the whole file but not relative to marker passed as input
			newMarker->setStart(newMarker->start() - m->start());
			markersToWrite << newMarker;
		}
	}

	block->setMarkers(markersToWrite);

	// creating file
	if (m_writer->createFile(m_path) != AwFileIO::NoError)	{
		m_writer->plugin()->deleteInstance(m_writer);
		return;
	}

	emit progressChanged(tr("Loading and filtering data..."));
	requestData(&list, m->start(), m->duration());
	if (!endOfData())	{
		emit progressChanged(tr("Saving data to file..."));
		m_writer->writeData(&list);
	}
	foreach(AwChannel *c, list)
		c->clearData();

	m_writer->plugin()->deleteInstance(m_writer);
}