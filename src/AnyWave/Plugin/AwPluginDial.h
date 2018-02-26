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

#ifndef AW_PLUGINDIAL_H
#define AW_PLUGINDIAL_H

#include <QDialog>
#include "ui_AwPluginDial.h"

class AwFileIOPlugin;
class AwProcessPlugin;
class AwDisplayPlugin;


using namespace Ui;

class AwPluginDial : public QDialog, public PluginDialClass
{
	Q_OBJECT

public:
	AwPluginDial(const QList<AwFileIOPlugin *>& readers, const QList<AwFileIOPlugin *>&,
		const QList<AwProcessPlugin *>& processes, const QList<AwDisplayPlugin *>& displayers,
		QWidget *parent = 0);
protected:
	void changeEvent(QEvent *e) {
		if (e)
			if (e->type() == QEvent::LanguageChange)
				retranslateUi(this);
	}
private:
	QIcon m_interfaceIcon;
	QIcon m_featureIcon;
	QTreeWidgetItem *m_readerItem; // Pointeur sur item parent des intefaces disponibles
	QTreeWidgetItem *m_writerItem;  
	QTreeWidgetItem *m_processItem;  
	QTreeWidgetItem *m_displayItem;	
};

#endif // PLUGINDIAL_H
