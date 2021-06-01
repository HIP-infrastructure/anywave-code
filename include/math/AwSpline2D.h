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
#pragma once

#include <AwGlobal.h>

class AW_MATH_EXPORT AwSpline2D
{
public:
	static int interpolate(int np, int nz, int ni, double *x, double *y, double *z,
		double *xi, double *yi, double *zi);

protected:
	AwSpline2D() {}
	static int find_weights(int np, int nz, double *x, double *y, double *z, double *dist);
	static int solve_coef_sys(int np, int nz, double *g, double *z);
};

