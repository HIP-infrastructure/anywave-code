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
#include "Source/AwSourceManager.h"
#include <QDataStream>
#include <QTcpSocket>
#include <QBuffer>
#include <armadillo>
#include "Prefs/AwSettings.h"
#include <QFile>
using namespace arma;

void AwRequestServer::handleSetBeamFormer(QTcpSocket *client, AwScriptProcess *p)
{
	emit log("Processing aw_setbf...");
	QDataStream in(client);
	in.setVersion(QDataStream::Qt_4_4);

	QString type;
	QStringList awLabels, sourceLabels;
	in >> type >> awLabels >> sourceLabels;
	qint64 row, col;
	in >> row >> col;
	mat matrix(row, col);
	for (size_t i = 0; i < row * col; i++) {
		double value;
		in >> value;
		matrix.memptr()[i] = value;
	}

	double hp, lp;
	in >> hp >> lp;

	QString filePath =  AwSourceManager::instance()->autoSave(AwChannel::stringToType(type), matrix, awLabels, sourceLabels, lp, hp);
	if (filePath.isEmpty()) {
		emit log("setBeamformer failed to create file.");
		return;
	}
	emit beamformerAvailable(filePath);
	emit log("Done.");
}
