#include "Aw2DGrid.h"
#include <math/AwSpline2D.h>

Aw2DGrid::Aw2DGrid()
{
	status = Aw2DGrid::Ok;
	xi = yi = zi = x = y =  z = NULL;
	res = 0;
}

Aw2DGrid::Aw2DGrid(int r, const AwSensorList& sensors)
{
	status = Aw2DGrid::Ok;
	xi = yi = zi = x = y =  z = NULL;
	res = r;
	computeGrid(res, sensors);
}

Aw2DGrid::~Aw2DGrid()
{
	clear();
}


void Aw2DGrid::clear()
{
	np = ni = 0;
	if (xi)	{
		delete[] xi;
		xi = NULL;
	}
	if (yi)	{
		delete[] yi;
		yi = NULL;
	}
	if (zi)	{
		delete[] zi;
		zi = NULL;
	}
	if (x)	{
		delete[] x;
		x = NULL;
	}
	if (y)	{
		delete[] y;
		y = NULL;
	}
	if (z)	{
		delete[] z;
		z = NULL;
	}
}

void Aw2DGrid::updateGrid(const AwSensorList& sensors)
{
	// ASSUME THAT SENSORS ARE  THE SAME.
	// Only the scalar value has changed;

	// check that sensors matches the current sensor list size
	if (sensors.size() != np) {
		status = Aw2DGrid::InterpolationError;
		return;
	}

	// re compute z values and interpolate.

	min_z = 1e3;
	max_z = -1e3;
	for (int i = 0; i < np; i++)	{
		z[i] = sensors.at(i)->value();

		if (min_z > z[i]) min_z = z[i];
		if (max_z < z[i]) max_z = z[i];
	}

	if (AwSpline2D::interpolate(np, 1, ni, x, y, z, xi, yi, zi) != 0)
		status = Aw2DGrid::InterpolationError;
}



void Aw2DGrid::computeGrid(int r, const AwSensorList& sensors)
{
	clear();

	np = sensors.size();
	ni = r * r;
	res = r;
	x = new double[np];
	y = new double[np];
	z = new double[np];
	xi = new double[ni];
	yi = new double[ni];
	zi = new double[ni];

	min_x = min_y = min_z = 1e3;
	max_x = max_y = max_z = -1e3;
	double *xy;

	for (int i=0; i< np; i++) {
		xy = sensors.at(i)->coordinates();
		z[i] = sensors.at(i)->value();
		x[i] = xy[0];
		y[i] = xy[1];

		if (min_x > x[i]) min_x = x[i];
		if (min_y > y[i]) min_y = y[i];
		if (min_z > z[i]) min_z = z[i];
		if (max_x < x[i]) max_x = x[i];
		if (max_y < y[i]) max_y = y[i];
		if (max_z < z[i]) max_z = z[i];
	}
	double mx = (max_x - min_x)/2
	    , my = (max_y - min_y)/2
	    , dz = ((-min_z) > max_z) ? -min_z : max_z;

	/* inc min/max to effectively shrink sensors on interpolation grid */
	float shrink=1.0;
	min_x = (min_x - mx)*shrink + mx;
	max_x = (max_x - mx)*shrink + mx;
	min_y = (min_y - my)*shrink + my;
	max_y = (max_y - my)*shrink + my;

	for (int i=0; i<res; i++)
		for (int j=0; j<res; j++) {
			xi[i*res+j] = (max_x - min_x) / (res - 1) * j + min_x;
			yi[i*res+j] = max_y - (max_y - min_y) / (res - 1) * i;
		}

	if (AwSpline2D::interpolate(np, 1, ni, x, y, z, xi, yi, zi) != 0)
		status = Aw2DGrid::InterpolationError;

	//// tranpose zi 
	//double *zit = new double[ni];
	//for(int i = 0; i < res; i++)
	//	for(int j = 0; j < res; j++)
	//		zit[i] = zi[i * res + j];
	//delete[] zi;
	//zi = zit;

	// recompute min and max for interpolated values
	max_zi = min_zi = zi[0];
	for (int i = 1; i < ni; i++) {
		if (zi[i] > max_zi)
			max_zi = zi[i];
		if (zi[i] < min_zi)
			min_zi = zi[i];
	}
}


