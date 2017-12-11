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
#ifndef AW_GRAPHIC_INTERFACE_H_
#define AW_GRAPHIC_INTERFACE_H_
#include <AwGlobal.h>
#include <graphics/AwGraphicsObjects.h>

class AW_GRAPHICS_EXPORT AwDisplayPlugin : public QObject
{
	// DO NOT INSERT Q_OBJECT MACRO
public:
	QString name;
	QString version;
	QString description;

	virtual AwBaseGraphicsSignalItem *newInstance(AwChannel *chan, AwDisplayPhysics *phys) = 0;
//	virtual void deleteInstance(AwBaseGraphicsSignalItem *item) { delete item; item = NULL; }
//	virtual void deleteInstance(AwBaseGraphicsSignalItem *item) = 0;
};

class AW_GRAPHICS_EXPORT AwDisplayPlugin2DPlot : public AwGraphics2DPlotItem
{
public:
	enum ScaleType { AnyWaveXScale, AnyWaveYScale, CustomScale };
	AwDisplayPlugin2DPlot(AwChannel *chan, AwDisplayPhysics *phys) : AwGraphics2DPlotItem(chan, phys) { m_xScaleType = AnyWaveXScale; m_yScaleType = CustomScale;}
protected:
	int m_xScaleType;
	int m_yScaleType;
};

#define AwDisplayPlugin_IID "AnyWave.DisplayPluginInterface"
#define AwGraphicsSignalItem_IID "AnyWave.GraphicsSignalItem"
#define AwDisplayPlugin2DPlot_IID "AnyWave.DisplauPlugin2DPlot"
#define AwGraphicsWidgetItem_IID "AnyWave.GraphicsWidgetItem"

Q_DECLARE_INTERFACE(AwDisplayPlugin, AwDisplayPlugin_IID)
Q_DECLARE_INTERFACE(AwGraphicsSignalItem, AwGraphicsSignalItem_IID)
Q_DECLARE_INTERFACE(AwDisplayPlugin2DPlot, AwDisplayPlugin2DPlot_IID)
Q_DECLARE_INTERFACE(AwGraphicsWidgetItem, AwGraphicsWidgetItem_IID)
#endif