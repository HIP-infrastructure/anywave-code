/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
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
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef AWAMPLITUDEBUTTON_H
#define AWAMPLITUDEBUTTON_H
#include <AwGlobal.h>
#include <QPushButton>
#include <QActionGroup>
#include <AwChannel.h>
class QMenu;
class AwAmplitudeManager;

class AW_WIDGETS_EXPORT AwAmplitudeButton : public QPushButton
{
	Q_OBJECT

public:
	AwAmplitudeButton(QWidget *parent);
	~AwAmplitudeButton();
signals:
	void amplitudeChanged(int, float);
	void amplitudesChanged();
private slots:
	void changeAmplitude(bool checked);
public slots:
	void setAmplitudes();
	void setAmplitude(int, float);
	void up();			// raise signal amplitudes (reduces the amplitude scale).
	void down();		// lower signal amplitudes (raises the amplitude scale).
	void switchGainLevelToChannel(int type);
private:
	void upActions(QActionGroup *group);
	void downActions(QActionGroup *group);
	void createActionGroup(QMenu *menu, int type);

	AwAmplitudeManager *m_am;
	QMenu *m_menu;
	QActionGroup *m_groups[AW_CHANNEL_TYPES];
	int m_gainLevelForChannel;	
};

#endif // AWAMPLITUDEBUTTON_H
