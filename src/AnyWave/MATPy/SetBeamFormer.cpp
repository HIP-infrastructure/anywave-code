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
#include "AwRequestServer.h"
#include "Source/AwSourceManager.h"
#include <QDataStream>
#include <QTcpSocket>
#include "Prefs/AwSettings.h"
#include <QFile>
#include <aw_armadillo.h>

void AwRequestServer::handleSetBeamFormer(QTcpSocket *client, AwScriptProcess *process)
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
