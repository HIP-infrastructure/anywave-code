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
#include "AwUtilities.h"
#include <QTime>
#include <QtCore/qmath.h>
#include <QSettings>
#include <AwMarker.h>


QDebugStream::QDebugStream(int streamType)
{
	if (streamType == QDebugStream::StdOut)	{
		m_outBufferBackup = std::cout.rdbuf();
		std::cout.rdbuf(m_stream.rdbuf());
	}
	else	{
		m_outBufferBackup = std::cerr.rdbuf();
		std::cerr.rdbuf(m_stream.rdbuf());
	}
	m_type = streamType;
}

QDebugStream::~QDebugStream()
{
	// restore buffers
	if (m_type == QDebugStream::StdOut)
		std::cout.rdbuf(m_outBufferBackup);
	else
		std::cerr.rdbuf(m_outBufferBackup);
}

QString QDebugStream::text()
{
	QString out(m_stream.str().c_str());
	return out;
}

QString AwUtilities::timeToString(float seconds)
{
	float tmp = seconds;
	int h = seconds / 3600;
	tmp -= h * 3600;
	int m = tmp / 60;
	tmp -= m * 60;
	int s = (int)tmp;
	tmp -= s;
	int ms = tmp * 1000;
	QString sh = QString("%1h ").arg(h);
	QString sm = QString("%1min ").arg(m);
	QString ss = QString("%1s ").arg(s);
	QString sms = QString("%1ms").arg(ms);

	QString res;
	
	if (h)
		res += sh;
	if (m)
		res += sm;
	if (s) 
		res += ss;
	if (ms)
		res += sms;

	return res;
}

// Time representations
float AwUtilities::roundedTime(float time)
{
	QString round;
	round.setNum(time, 'g', 3);
	return (float)round.toDouble();
}

QTime AwUtilities::timeFromString(const QString& string)
{
	return QTime::fromString(string, "hh':'mm':'ss");
}

QDate AwUtilities::dateFromString(const QString& string)
{
	return QDate::fromString(string, "dd'.'MM'.'yyyy");
}


QString AwUtilities::hmsTime(float seconds, bool showMs)
{
	qint64 ms = (qint64)qFloor(seconds * 1000);
	return AwUtilities::hmsTime(ms, showMs);
}

QString AwUtilities::hmsTime(qint64 milliseconds, bool showMs)
{
	if (milliseconds == 0)
		if (showMs)
			return QString("00:00:00.000");
		else
			return QString("00:00:00");

	QTime time(0, 0, 0, 0);  // Qt5 requires an initialized QTime object before adding secs or msecs.

	qint64 tmp = milliseconds;
	int h = tmp / (1000 * 3600);
	tmp -= h * 1000 * 3600;
	int m = tmp / (1000 * 60);
	tmp -= m * 1000 * 60;

	int s = tmp / 1000;
	tmp -= s * 1000;
	
	if (h)
		time = time.addSecs(h * 3600);
	if (m)
		time = time.addSecs(m * 60);
	if (s) 
		time = time.addSecs(s);
	if (tmp)
		time = time.addMSecs(tmp);

	if (!showMs)
		return time.toString("hh:mm:ss");
	
	return time.toString("hh:mm:ss.zzz");
}

void AwUtilities::saveTimeHMS(bool flag)
{
	QSettings settings;
	settings.setValue("Preferences/time/HMS", flag);
}

bool AwUtilities::isTimeHMS()
{
	QSettings settings;
	return settings.value("Preferences/time/HMS", true).toBool();
}

QString AwUtilities::cursorColor()
{
	QSettings settings;
	return settings.value("Preferences/colors/cursor", "darkGreen").toString();
}

QString AwUtilities::defaultCursorColor()
{
	return QString("darkGreen");
}

void AwUtilities::saveCursorColor(const QString& color)
{
	QSettings settings;
	settings.setValue("Preferences/colors/cursor", color);
}

QString AwUtilities::mappingCursorColor()
{
	QSettings settings;
	return settings.value("Preferences/colors/mapping_cursor", "#ff8000").toString();
}

QString AwUtilities::defaultMappingCursorColor()
{
	return QString("#ff8000");
}

void AwUtilities::saveMappingCursorColor(const QString& color)
{
	QSettings settings;
	settings.setValue("Preferences/colors/mapping_cursor", color);
}

QString AwUtilities::markerColor(int type)
{
	QSettings settings;
	if (type == AwMarker::Single)
		return settings.value("Preferences/colors/marker_single", "darkBlue").toString();
	// Selection
	return settings.value("Preferences/colors/marker_selection", "magenta").toString();
}

QString AwUtilities::defaultMarkerColor(int type)
{
	if (type == AwMarker::Single)
		return QString("darkBlue");
	
	return QString("magenta");
}

void AwUtilities::saveMarkerColor(const QString& color, int type)
{
	QSettings settings;
	if (type == AwMarker::Single)
		settings.setValue("Preferences/colors/marker_single", color);
	else
		settings.setValue("Preferences/colors/marker_selection", color);
}


QFont AwUtilities::cursorFont()
{
	QSettings settings;
	QString family = settings.value("Preferences/fonts/cursor/family", "Helvetica").toString();
	int weight = settings.value("Preferences/fonts/cursor/weight", QFont::Normal).toInt();
	int size = settings.value("Preferences/fonts/cursor/size", 10).toInt();
	bool italic = settings.value("Preferences/fonts/cursor/italic", false).toBool();

	return QFont(family, size, weight, italic);
}

void AwUtilities::saveCursorFont(const QFont& font)
{
	QSettings settings;
	settings.setValue("Preferences/fonts/cursor/family", font.family());
	settings.setValue("Preferences/fonts/cursor/size", font.pointSize());
	settings.setValue("Preferences/fonts/cursor/weight", font.weight());
	settings.setValue("Preferences/fonts/cursor/italic", font.italic());
}

QFont AwUtilities::mappingCursorFont()
{
	QSettings settings;
	QString family = settings.value("Preferences/fonts/mapping_cursor/family", "Helvetica").toString();
	int weight = settings.value("Preferences/fonts/mapping_cursor/weight", QFont::Normal).toInt();
	int size = settings.value("Preferences/fonts/mapping_cursor/size", 10).toInt();
	bool italic = settings.value("Preferences/fonts/mapping_cursor/italic", false).toBool();

	return QFont(family, size, weight, italic);
}

void AwUtilities::saveMappingCursorFont(const QFont& font)
{
	QSettings settings;
	settings.setValue("Preferences/fonts/mapping_cursor/family", font.family());
	settings.setValue("Preferences/fonts/mapping_cursor/size", font.pointSize());
	settings.setValue("Preferences/fonts/mapping_cursor/weight", font.weight());
	settings.setValue("Preferences/fonts/mapping_cursor/italic", font.italic());
}

QFont AwUtilities::markerFont(int type)
{
	QSettings settings;
	QString family;
	int weight, size;
	bool italic;

	if (type == AwMarker::Single) {
		family = settings.value("Preferences/fonts/marker_single/family", "Helvetica").toString();
		weight = settings.value("Preferences/fonts/marker_single/weight", QFont::Normal).toInt();
		size = settings.value("Preferences/fonts/marker_single/size", 10).toInt();
		italic = settings.value("Preferences/fonts/marker_single/italic", false).toBool();
	}
	else {
		family = settings.value("Preferences/fonts/marker_selection/family", "Helvetica").toString();
		weight = settings.value("Preferences/fonts/marker_selection/weight", QFont::Normal).toInt();
		size = settings.value("Preferences/fonts/marker_selection/size", 10).toInt();
		italic = settings.value("Preferences/fonts/marker_selection/italic", false).toBool();
	}
	return QFont(family, size, weight, italic);
}

void AwUtilities::saveMarkerFont(const QFont& font, int type)
{
	QSettings settings;

	if (type == AwMarker::Single) {
		settings.setValue("Preferences/fonts/marker_single/family", font.family());
		settings.setValue("Preferences/fonts/marker_single/size", font.pointSize());
		settings.setValue("Preferences/fonts/marker_single/weight", font.weight());
		settings.setValue("Preferences/fonts/marker_single/italic", font.italic());
	}
	else {
		settings.setValue("Preferences/fonts/marker_selection/family", font.family());
		settings.setValue("Preferences/fonts/marker_selection/size", font.pointSize());
		settings.setValue("Preferences/fonts/marker_selection/weight", font.weight());
		settings.setValue("Preferences/fonts/marker_selection/italic", font.italic());
	}
}


char *AwUtilities::QStringToChar(const QString& str)
{
	char *res = new char[str.size()];
	memset(res, 0, str.size());
	QByteArray ba = str.toLatin1();
	memcpy(res, ba.data(), str.size());
	res[str.size() - 1] = '\0';
	return res;
}