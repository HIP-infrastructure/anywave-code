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
#ifndef AW_EXPORTER_H
#define AW_EXPORTER_H
#include <AwProcessInterface.h>
#include <AwFileIO.h>
#include <QtCore>
#include <QDataStream>

class AwExporter : public AwProcess
{
	Q_OBJECT
	Q_INTERFACES(AwProcess)
public:
	AwExporter();
	~AwExporter();

	bool showUi();
	void run();
protected:
	AwChannelList m_channels;		// channels to export
	AwChannelList m_ICAChannels;	// ICA Channels to export
	AwMarkerList m_selectedMarkers;
	AwFileIOPlugin *m_plugin;
	QString m_path;
	AwFilteringOptions m_foptions;
	bool m_exportICAChannels;			// this flag indicates that the data contains ICA virtual channels
	float m_downSample;
};

class AwExporterPlugin : public AwProcessPlugin
{
	Q_OBJECT
	Q_INTERFACES(AwProcessPlugin)
public:
	AwExporterPlugin();

	AwExporter *newInstance() { return new AwExporter(); }
};

#endif