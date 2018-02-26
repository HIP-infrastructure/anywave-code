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
#ifndef AW_2D_GRID_H
#define AW_2D_GRID_H

#include <AwSensor.h>

class Aw2DGrid
{
public:
	Aw2DGrid();
	Aw2DGrid(int res, const AwSensorList& sensors);
	~Aw2DGrid();

	/** compute a totally new grid, do interpolation. **/
	void computeGrid(int res, const AwSensorList& sensors);
	/** update the current grid, just doing the interpolation **/
	void updateGrid(const AwSensorList& sensors);
	/** clear the current grid. All values are reset and interpolated values are deleted. */
	void clear();
	enum Status { Ok, InterpolationError };
	double *xi;
	double *yi;
	double *zi;
	double *x;
	double *y;
	double *z;
	double min_x, min_y, min_z;
	double max_x, max_y, max_z;
	double max_zi, min_zi;
	int np;
	int ni;		// total number of interpolated values
	int res;	// resolution used to interpolate.
	int status;
};

#endif