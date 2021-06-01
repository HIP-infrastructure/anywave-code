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
#ifndef AWMATLABSUPPORT_H
#define AWMATLABSUPPORT_H

#include "awmatlabsupport_global.h"
#include <AwMatlabInterface.h>


class AWMATLABSUPPORT_EXPORT AwMatlabSupport : public AwMatlabInterface
{
	Q_OBJECT
	Q_INTERFACES(AwMatlabInterface)
	Q_PLUGIN_METADATA(IID AwMatlabInterface_IID)
public:
	/** run a matlab script **/
	void run(const QVariantMap& settings) override;
signals:
	void progressChanged(const QString&);

};

#endif // AWMATLABSUPPORT_H
