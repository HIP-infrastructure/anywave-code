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
#ifndef AWAMPLITUDEMANAGER_H
#define AWAMPLITUDEMANAGER_H
#include <AwGlobal.h>
#include <QObject>
#include <QVector>
#include <QHash>
#include <AwChannel.h>

class AW_GRAPHICS_EXPORT AwAmplitudeManager : public QObject
{
	Q_OBJECT
public:
	static AwAmplitudeManager *instance();
	inline QVector<float>& getScale(int type) { return m_amplitudes[type]; }
	/** Add a new value to an existing gain scale. if the value is already present in the scale, do nothing. **/ 
	void insertValueInScale(int type, float value);
	inline float amplitude(int type) { return m_amplitude[type]; }
	float middleValueForScale(int scale);
	inline QString unitOfChannel(int type) { return m_units[type]; }
	void closeFile();
	void quit();
	void setFilename(const QString& path);
public slots:
	void setAmplitude(int type, float value);
	void reset();

signals:
	/** Sent when all the amplitude scales have changed. When loading a previously saved .gain file for example. **/
	void amplitudesChanged();

protected:
	AwAmplitudeManager(QObject *parent = 0);
private:
	static AwAmplitudeManager *m_instance;

	void save();
	void load();
	void defaults();
	float m_amplitude[AW_CHANNEL_TYPES];
	QVector<float> m_amplitudes[AW_CHANNEL_TYPES];
	QString m_units[AW_CHANNEL_TYPES];
	QString m_filePath;
};


#endif // AWAMPLITUDEMANAGER_H
