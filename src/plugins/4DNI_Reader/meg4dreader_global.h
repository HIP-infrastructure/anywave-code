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
#ifndef _MEG4DREADER_GLOBAL_H
#define _MEG4DREADER_GLOBAL_H

#include <qglobal.h>

#ifdef _4DNI_READER_LIB
# define MEG4DREADER_EXPORT Q_DECL_EXPORT
#else
# define MEG4DREADER_EXPORT Q_DECL_IMPORT
#endif

#endif // _4DREADER_GLOBAL_H
