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