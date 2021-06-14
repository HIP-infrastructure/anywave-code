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
#include <widget/AwColorMap.h>
#include <QtCore/qmath.h>

/// AwColorMapValues

double *AwColorMapValues::colorAt(int index)
{
	return &m_values.data()[index * 3];
}

void AwColorMapValues::colorAt(int index, double *colors)
{
	colors[0] = m_values.at(index * 3);
	colors[1] = m_values.at(index * 3 + 1);
	colors[2] = m_values.at(index * 3 + 2);
}

void AwColorMapValues::addColor(float r, float g, float b)
{
	m_values << r << g << b;
}

void AwColorMapValues::addColor(double r, double g, double b)
{
	m_values << r << g << b;
}

void AwColorMapValues::addColor(float *colors)
{
	m_values << colors[0] << colors[1] << colors[2];
}

void AwColorMapValues::addColor(double *colors)
{
	m_values << colors[0] << colors[1] << colors[2];
}

/// AwColorMap

QColor AwColorMap::rgbToColor(float r, float g, float b)
{
	QColor color;
	color.setBlueF(b);
	color.setRedF(r);
	color.setGreenF(g);
	return color;
}

AwCMapNamesAndTypes AwColorMap::namesAndTypes()
{
	AwCMapNamesAndTypes res;
	res.append(QPair<QString, int>("Jet", AwColorMap::Jet));
	res.append(QPair<QString, int>("Fire And Ice", AwColorMap::FireAndIce));
	res.append(QPair<QString, int>("Blue Black Red", AwColorMap::BlueBlackRed));
	res.append(QPair<QString, int>("Black and White", AwColorMap::Gray));
	res.append(QPair<QString, int>("Parula", AwColorMap::Parula));
	return res;
}	

AwColorMapValues AwColorMap::valuesForMap(int map)
{
	AwColorMapValues values;

	switch (map)
	{
	case AwColorMap::Parula:
		values.addColor(0.2081,    0.1663,    0.52920);
		values.addColor(0.2116,    0.1898,    0.5777);
		values.addColor(0.2123,    0.2138,    0.6270);
		values.addColor(0.2081,    0.2386,    0.6771);
		values.addColor(0.1959,    0.2645,    0.7279);
		values.addColor(0.1707,    0.2919,    0.7792);
		values.addColor(0.1253,    0.3242,    0.8303);
		values.addColor(0.0591,    0.3598,    0.8683);
		values.addColor(0.0117,    0.3875,    0.8820);
		values.addColor(0.0060,    0.4086,    0.8828);
		values.addColor(0.0165,    0.4266,    0.8786);
		values.addColor( 0.0329,    0.4430,    0.8720);
		values.addColor(0.0498,    0.4586,    0.8641);
		values.addColor(0.0629,    0.4737,    0.8554);
		values.addColor(0.0723,    0.4887,    0.8467);
		values.addColor(0.0779,    0.5040,    0.8384);
		values.addColor(0.0793,    0.5200,    0.8312);
		values.addColor(0.0749,    0.5375,    0.8263);
		values.addColor(0.0641,    0.5570,    0.8240);
		values.addColor(0.0488,    0.5772,    0.8228);
		values.addColor(0.0343,    0.5966,    0.8199);
		values.addColor(0.0265,    0.6137,    0.8135);
		values.addColor(0.0239,    0.6287,    0.8038);
		values.addColor(0.0231,    0.6418,    0.7913);
		values.addColor(0.0228,    0.6535,    0.7768);
		values.addColor(0.0267,    0.6642,    0.7607);
		values.addColor(0.0384,    0.6743,    0.7436);
		values.addColor(0.0590,    0.6838,    0.7254);
		values.addColor(0.0843,    0.6928,    0.7062);
		values.addColor(0.1133,   0.7015,    0.6859);
		values.addColor(0.1453,    0.7098,    0.6646);
		values.addColor(0.1801,    0.7177,    0.6424);
		values.addColor(0.2178,    0.7250,    0.6193);
		values.addColor(0.2586,    0.7317,    0.5954);
		values.addColor(0.3022,    0.7376,    0.5712);
		values.addColor(0.3482,    0.7424,    0.5473);
		values.addColor(0.3953,    0.7459,    0.5244);
		values.addColor(0.4420,    0.7481,    0.5033);
		values.addColor(0.4871,    0.7491,    0.4840);
		values.addColor(0.5300,    0.7491,    0.4661);
		values.addColor(0.5709,    0.7485,    0.4494);
		values.addColor(0.6099,    0.7473,    0.4337);
		values.addColor(0.6473,    0.7456,    0.4188);
		values.addColor(0.6834,    0.7435,    0.4044);
		values.addColor(0.7184,    0.7411,   0.3905);
		values.addColor(0.7525,    0.7384,    0.3768);
		values.addColor(0.7858,    0.7356,    0.3633);
		values.addColor(0.8185,    0.7327,    0.3498);
		values.addColor(0.8507,    0.7299,    0.3360);
		values.addColor(0.8824,    0.7274,    0.3217);
		values.addColor(0.9139,    0.7258,    0.3063);
		values.addColor(0.9450,    0.7261,    0.2886);
		values.addColor(0.9739,    0.7314,    0.2666);
		values.addColor(0.9938,    0.7455,    0.2403);
		values.addColor(0.9990,    0.7653,    0.2164);
		values.addColor(0.9955,    0.7861,    0.1967);
		values.addColor(0.9880,    0.8066,    0.1794);
		values.addColor(0.9789,    0.8271,    0.1633);
		values.addColor(0.9697,    0.8481,    0.1475);
		values.addColor(0.9626,    0.8705,    0.1309);
		values.addColor(0.9589,    0.8949,    0.1132);
		values.addColor(0.9598,    0.9218,    0.0948);
		values.addColor(0.9661,    0.9514,    0.0755);
		values.addColor(0.9763,    0.9831,    0.0538);	
		break;
	case AwColorMap::FireAndIce:
		values.addColor(0.75, 1, 1);
		values.addColor(0.6797, 1.0, 1.0);
		values.addColor(0.6797, 1.0, 1.0);
		values.addColor(0.6094, 1.0000, 1.0000);
		values.addColor(0.5391, 1.0000,   1.0000);
		values.addColor(0.4688,   1.0000,    1.0000);
		values.addColor(0.3984,    1.0000,    1.0000);
		values.addColor(0.3281,    1.0000,    1.0000);
		values.addColor(0.2578,    1.0000,    1.0000);
		values.addColor(0.1875,    1.0000,   1.0000);
		values.addColor(0.1172,    1.0000,    1.0000);
		values.addColor(0.0469,    1.0000,    1.0000);
		values.addColor(0,    0.9688,    1.0000);
		values.addColor(0,    0.8750,    1.0000);
		values.addColor(0,    0.7813,    1.0000);
		values.addColor(0,    0.6875,    1.0000);
		values.addColor(0,    0.5938,    1.0000);
		values.addColor(0,    0.5000,    1.0000);
		values.addColor(0,    0.4063,    1.0000);
		values.addColor(0,    0.3125,    1.0000);
		values.addColor(0,    0.2188,    1.0000);
		values.addColor(0,    0.1250,    1.0000);
		values.addColor(0,   0.0313,    1.0000);
		values.addColor(0,         0,    0.9375);
		values.addColor(0,         0,    0.8438);
		values.addColor(0,         0,    0.7500);
		values.addColor(0,         0,    0.6563);
		values.addColor(0,         0,    0.5625);
		values.addColor(0,         0,    0.4688);
		values.addColor(0,         0,    0.3750);
		values.addColor(0,         0,    0.2813);
		values.addColor(0,         0,    0.1875);
		values.addColor(0,         0,    0.0938);
		values.addColor(0.0938,         0,         0);
		values.addColor(0.1875,         0,         0);
		values.addColor(0.2813,         0,         0);
		values.addColor(0.3750,         0,         0);
		values.addColor(0.4688,         0,         0);
		values.addColor(0.5625,         0,         0);
		values.addColor(0.6563,         0,         0);
		values.addColor(0.7500,        0,        0);
		values.addColor(0.8438,         0,         0);
		values.addColor(0.9375,         0,         0);
		values.addColor(1.0000,    0.0313,         0);
		values.addColor(1.0000,    0.1250,         0);
		values.addColor(1.0000,    0.2188,         0);
		values.addColor(1.0000,    0.3125,         0);
		values.addColor(1.0000,    0.4063,         0);
		values.addColor(1.0000,    0.5000,         0);
		values.addColor(1.0000,    0.5938,         0);
		values.addColor(1.0000,   0.6875,         0);
		values.addColor(1.0000,    0.7813,         0);
		values.addColor(1.0000,    0.8750,         0);
		values.addColor(1.0000,    0.9688,         0);
		values.addColor(1.0000,    1.0000,    0.0469);
		values.addColor(1.0000,    1.0000,    0.1172);
		values.addColor(1.0000,    1.0000,    0.1875);
		values.addColor(1.0000,    1.0000,    0.2578);
		values.addColor(1.0000,    1.0000,    0.3281);
		values.addColor(1.0000,    1.0000,    0.3984);
		values.addColor(1.0000,    1.0000,    0.4688);
		values.addColor(1.0000,    1.0000,    0.5391);
		values.addColor(1.0000,    1.0000,    0.6094);
		values.addColor(1.0000,    1.0000,    0.6797);	
		values.addColor(1, 1, 0.75);
		break;
	case AwColorMap::Jet:
		values.addColor(0, 0, 0.5625);
		values.addColor(0,         0,    0.6250);
		values.addColor(0,         0,    0.6875);
		values.addColor(0,         0,    0.7500);
		values.addColor(0,         0,    0.8125);
		values.addColor(0,         0,    0.8750);
		values.addColor(0,         0,    0.9375);
		values.addColor(0,         0,    1.0000);
		values.addColor(0,    0.0625,    1.0000);
		values.addColor(0,    0.1250,    1.0000);
		values.addColor(0,    0.1875,    1.0000);
		values.addColor(0,    0.2500,    1.0000);
		values.addColor(0,    0.3125,    1.0000);
		values.addColor(0,    0.3750,    1.0000);
		values.addColor(0,    0.4375,    1.0000);
		values.addColor(0,    0.5000,    1.0000);
		values.addColor(0,    0.5625,    1.0000);
		values.addColor(0,    0.6250,    1.0000);
		values.addColor(0,    0.6875,    1.0000);
		values.addColor(0,    0.7500,    1.0000);
		values.addColor(0,    0.8125,    1.0000);
		values.addColor(0,    0.8750,    1.0000);
		values.addColor(0,    0.9375,    1.0000);
		values.addColor(0,    1.0000,    1.0000);
		values.addColor(0.0625,    1.0000,    0.9375);
		values.addColor(0.1250,    1.0000,    0.8750);
		values.addColor(0.1875,    1.0000,    0.8125);
		values.addColor(0.2500,    1.0000,    0.7500);
		values.addColor(0.3125,    1.0000,    0.6875);
		values.addColor(0.3750,    1.0000,    0.6250);
		values.addColor(0.4375,    1.0000,    0.5625);
		values.addColor(0.5000,    1.0000,   0.5000);
		values.addColor(0.5625,    1.0000,    0.4375);
		values.addColor(0.6250,    1.0000,    0.3750);
		values.addColor(0.6875,    1.0000,    0.3125);
		values.addColor(0.7500,    1.0000,    0.2500);
		values.addColor(0.8125,    1.0000,    0.1875);
		values.addColor(0.8750,    1.0000,    0.1250);
		values.addColor(0.9375,    1.0000,    0.0625);
		values.addColor(1.0000,    1.0000,         0);
		values.addColor(1.0000,    0.9375,         0);
		values.addColor(1.0000,    0.8750,         0);
		values.addColor(1.0000,    0.8125,         0);
		values.addColor(1.0000,    0.7500,         0);
		values.addColor(1.0000,    0.6875,         0);
		values.addColor(1.0000,    0.6250,         0);
		values.addColor(1.0000,    0.5625,         0);
		values.addColor(1.0000,    0.5000,         0);
		values.addColor(1.0000,    0.4375,         0);
		values.addColor(1.0000,    0.3750,         0);
		values.addColor(1.0000,    0.3125,         0);
		values.addColor(1.0000,    0.2500,         0);
		values.addColor(1.0000,    0.1875,         0);
		values.addColor(1.0000,    0.1250,         0);
		values.addColor(1.0000,    0.0625,         0);
		values.addColor(1.0000,         0,         0);
		values.addColor(0.9375,         0,         0);
		values.addColor(0.8750,         0,         0);
		values.addColor(0.8125,         0,         0);
		values.addColor(0.7500,         0,         0);
		values.addColor(0.6875,         0,         0);
		values.addColor(0.6250,         0,         0);
		values.addColor(0.5625,         0,         0);
		values.addColor(0.5, 0, 0);
		break;
	case BlueBlackRed:
		values.addColor(0., 0., 1.0000);
		values.addColor(0, 0, 0.9667);
		values.addColor(0, 0, 0.9333);
		values.addColor(0, 0, 0.9);
		values.addColor(0, 0, 0.8677);
		values.addColor(0, 0, .8333);
		values.addColor(0, 0, 0.8);
		values.addColor(0, 0, 0.7667);
		values.addColor(0, 0, 0.7333);
		values.addColor(0, 0, 0.7);
		values.addColor(0, 0, 0.6667);
		values.addColor(0, 0, 0.6333);
		values.addColor(0, 0, 0.6);
		values.addColor(0, 0, 0.5677);
		values.addColor(0, 0, 0.5333);
		values.addColor(0, 0, 0.5);
		values.addColor(0, 0, 0.4667);
		values.addColor(0, 0, 0.4333);
		values.addColor(0, 0, 0.4);
		values.addColor(0, 0, 0.3667);
		values.addColor(0, 0, 0.3333);
		values.addColor(0, 0, 0.3);
		values.addColor(0, 0, 0.2667);
		values.addColor(0, 0, 0.2333);
		values.addColor(0, 0, 0.2);
		values.addColor(0, 0, 0.1667);
		values.addColor(0, 0, 0.1333);
		values.addColor(0, 0, 0.1);
		values.addColor(0, 0, 0.0667);
		values.addColor(0, 0, 0.0333);
		values.addColor(0.,0., 0.);
		values.addColor(0.0333, 0, 0);
		values.addColor(0.0667, 0, 0);
		values.addColor(0.1, 0, 0);
		values.addColor(0.1333, 0, 0);
		values.addColor(0.1667, 0, 0);
		values.addColor(0.2, 0, 0);
		values.addColor(0.2333, 0, 0);
		values.addColor(0.2667, 0, 0);
		values.addColor(0.3, 0, 0);
		values.addColor(0.3333, 0, 0);
		values.addColor(0.3667, 0, 0);
		values.addColor(0.4, 0, 0);
		values.addColor(0.4333, 0, 0);
		values.addColor(0.4667, 0, 0);
		values.addColor(0.5, 0, 0);
		values.addColor(0.5333, 0, 0);
		values.addColor(0.5667, 0, 0);
		values.addColor(0.6, 0, 0);
		values.addColor(0.6333, 0, 0);
		values.addColor(0.6667, 0, 0);
		values.addColor(0.7, 0, 0);
		values.addColor(0.7333, 0, 0);
		values.addColor(0.7667, 0, 0);
		values.addColor(0.8, 0, 0);
		values.addColor(0.8333, 0, 0);
		values.addColor(0.86667, 0, 0);
		values.addColor(0.9, 0, 0);
		values.addColor(0.9333, 0, 0);
		values.addColor(0.9667, 0, 0);
		values.addColor(1., 0, 0);
		break;
	case Gray:
		values.addColor(0., 0., 0.);
		values.addColor(0.0159, 0.0159, 0.0159);
		values.addColor(0.0317, 0.0317, 0.0317);
		values.addColor(0.0476,  0.0476,  0.0476);
		values.addColor(0.0635, 0.0635, 0.0635);
		values.addColor(0.0794, 0.0794, 0.0794);
		values.addColor(0.0952, 0.0952, 0.0952);
		values.addColor(0.1111, 0.1111, 0.1111);
		values.addColor(0.1270, 0.1270, 0.1270);
		values.addColor(0.1429, 0.1429, 0.1429);
		values.addColor(0.1587, 0.1587, 0.1587);
		values.addColor(0.1746, 0.1746, 0.1746);
		values.addColor(0.1905, 0.1905, 0.1905);
		values.addColor(0.2063, 0.2063, 0.2063);
		values.addColor(0.2222, 0.2222, 0.2222);
		values.addColor(0.2381, 0.2381, 0.2381);
		values.addColor(0.2540, 0.2540, 0.2540);
		values.addColor(0.2698, 0.2698, 0.2698);
		values.addColor(0.2857, 0.2857, 0.2857);
		values.addColor(0.3016, 0.3016, 0.3016);
		values.addColor(0.3175, 0.3175, 0.3175);
		values.addColor(0.3333, 0.3333, 0.3333);
		values.addColor(0.3492, 0.3492, 0.3492);
		values.addColor(0.3651, 0.3651, 0.3651);
		values.addColor(0.3810, 0.3810, 0.3810);
		values.addColor(0.3968, 0.3968, 0.3968);
		values.addColor(0.4127, 0.4127, 0.4127);
		values.addColor(0.4286, 0.4286, 0.4286);
		values.addColor(0.4444, 0.4444, 0.4444);
		values.addColor(0.4603, 0.4603, 0.4603);
		values.addColor(0.4762,0.4762, 0.4762);
		values.addColor(0.4921, 0.4921, 0.4921);
		values.addColor(0.5079, 0.5079, 0.5079);
		values.addColor(0.5238, 0.5238, 0.5238);
		values.addColor(0.5397, 0.5397, 0.5397);
		values.addColor(0.5556, 0.5556, 0.5556);
		values.addColor(0.5714, 0.5714, 0.5714);
		values.addColor(0.5873, 0.5873, 0.5873);
		values.addColor(0.6032, 0.6032, 0.6032);
		values.addColor(0.6190, 0.6190, 0.6190);
		values.addColor(0.6349, 0.6349, 0.6349);
		values.addColor(0.6508, 0.6508, 0.6508);
		values.addColor(0.6667, 0.6667, 0.6667);
		values.addColor(0.6825, 0.6825, 0.6825);
		values.addColor(0.6984, 0.6984, 0.6984);
		values.addColor(0.7143, 0.7143, 0.7143);
		values.addColor(0.7302, 0.7302, 0.7302);
		values.addColor(0.7460, 0.7460, 0.7460);
		values.addColor(0.7619, 0.7619, 0.7619);
		values.addColor(0.7778, 0.7778, 0.7778);
		values.addColor(0.7937, 0.7937, 0.7937);
		values.addColor(0.8095, 0.8095, 0.8095);
		values.addColor(0.8254, 0.8254, 0.8254);
		values.addColor(0.8413, 0.8413, 0.8413);
		values.addColor(0.8571, 0.8571, 0.8571);
		values.addColor(0.8730, 0.8730, 0.8730);
		values.addColor(0.8889, 0.8889, 0.8889);
		values.addColor(0.9048, 0.9048, 0.9048);
		values.addColor(0.9206, 0.9206, 0.9206);
		values.addColor(0.9365, 0.9365, 0.9365);
		values.addColor(0.9524, 0.9524, 0.9524);
		values.addColor(0.9683, 0.9683, 0.9683);
		values.addColor(0.9841, 0.9841, 0.9841);
		values.addColor(1., 1., 1.);		
	}
	return values;
}