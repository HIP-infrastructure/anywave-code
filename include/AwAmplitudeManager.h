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
