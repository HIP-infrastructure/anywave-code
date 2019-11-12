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

#include <utils/AwUtilities.h>
#include <utils/gui.h>
#include <utils/time.h>
#include <utils/endian.h>
#include <QTime>
#include <QtCore/qmath.h>
#include <QSettings>
#include <AwMarker.h>
#include <QFile>
#include <qjsondocument.h>

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

QString AwUtilities::time::timeToString(float seconds)
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
float AwUtilities::time::roundedTime(float time)
{
	QString round;
	round.setNum(time, 'g', 3);
	return (float)round.toDouble();
}

QTime AwUtilities::time::timeFromString(const QString& string)
{
	return QTime::fromString(string, "hh':'mm':'ss");
}

QDate AwUtilities::time::dateFromString(const QString& string)
{
	return QDate::fromString(string, "dd'.'MM'.'yyyy");
}


QString AwUtilities::time::hmsTime(float seconds, bool showMs)
{
	qint64 ms = (qint64)qFloor(seconds * 1000);
	return AwUtilities::time::hmsTime(ms, showMs);
}

QString AwUtilities::time::hmsTime(qint64 milliseconds, bool showMs)
{
	if (milliseconds == 0)
		if (showMs)
			return QString("00:00:00.000");
		else
			return QString("00:00:00");

	bool negative = milliseconds < 0;
	auto abs = std::abs(milliseconds);

	QTime time(0, 0, 0, 0);  // Qt5 requires an initialized QTime object before adding secs or msecs.

	qint64 tmp = abs;
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

	QString prefix = "";
	if (negative)
		prefix = "-";

	if (!showMs)
		return time.toString(QString("%1hh:mm:ss").arg(prefix));
	
	return time.toString(QString("%1hh:mm:ss.zzz").arg(prefix));
}

void AwUtilities::time::saveTimeHMS(bool flag)
{
	QSettings settings;
	settings.setValue("Preferences/time/HMS", flag);
}

bool AwUtilities::time::isTimeHMS()
{
	QSettings settings;
	return settings.value("Preferences/time/HMS", true).toBool();
}

QString AwUtilities::gui::cursorColor()
{
	QSettings settings;
	return settings.value("Preferences/colors/cursor", "darkGreen").toString();
}

QString AwUtilities::gui::defaultCursorColor()
{
	return QString("darkGreen");
}

void AwUtilities::gui::saveCursorColor(const QString& color)
{
	QSettings settings;
	settings.setValue("Preferences/colors/cursor", color);
}

QString AwUtilities::gui::mappingCursorColor()
{
	QSettings settings;
	return settings.value("Preferences/colors/mapping_cursor", "#ff8000").toString();
}

QString AwUtilities::gui::defaultMappingCursorColor()
{
	return QString("#ff8000");
}

void AwUtilities::gui::saveMappingCursorColor(const QString& color)
{
	QSettings settings;
	settings.setValue("Preferences/colors/mapping_cursor", color);
}

QString AwUtilities::gui::markerColor(int type)
{
	QSettings settings;
	if (type == AwMarker::Single)
		return settings.value("Preferences/colors/marker_single", "darkBlue").toString();
	// Selection
	return settings.value("Preferences/colors/marker_selection", "magenta").toString();
}

QString AwUtilities::gui::defaultMarkerColor(int type)
{
	if (type == AwMarker::Single)
		return QString("darkBlue");
	
	return QString("magenta");
}

void AwUtilities::gui::saveMarkerColor(const QString& color, int type)
{
	QSettings settings;
	if (type == AwMarker::Single)
		settings.setValue("Preferences/colors/marker_single", color);
	else
		settings.setValue("Preferences/colors/marker_selection", color);
}


QFont AwUtilities::gui::cursorFont()
{
	QSettings settings;
	QString family = settings.value("Preferences/fonts/cursor/family", "Helvetica").toString();
	int weight = settings.value("Preferences/fonts/cursor/weight", QFont::Normal).toInt();
	int size = settings.value("Preferences/fonts/cursor/size", 10).toInt();
	bool italic = settings.value("Preferences/fonts/cursor/italic", false).toBool();

	return QFont(family, size, weight, italic);
}

void AwUtilities::gui::saveCursorFont(const QFont& font)
{
	QSettings settings;
	settings.setValue("Preferences/fonts/cursor/family", font.family());
	settings.setValue("Preferences/fonts/cursor/size", font.pointSize());
	settings.setValue("Preferences/fonts/cursor/weight", font.weight());
	settings.setValue("Preferences/fonts/cursor/italic", font.italic());
}

QFont AwUtilities::gui::mappingCursorFont()
{
	QSettings settings;
	QString family = settings.value("Preferences/fonts/mapping_cursor/family", "Helvetica").toString();
	int weight = settings.value("Preferences/fonts/mapping_cursor/weight", QFont::Normal).toInt();
	int size = settings.value("Preferences/fonts/mapping_cursor/size", 10).toInt();
	bool italic = settings.value("Preferences/fonts/mapping_cursor/italic", false).toBool();

	return QFont(family, size, weight, italic);
}

void AwUtilities::gui::saveMappingCursorFont(const QFont& font)
{
	QSettings settings;
	settings.setValue("Preferences/fonts/mapping_cursor/family", font.family());
	settings.setValue("Preferences/fonts/mapping_cursor/size", font.pointSize());
	settings.setValue("Preferences/fonts/mapping_cursor/weight", font.weight());
	settings.setValue("Preferences/fonts/mapping_cursor/italic", font.italic());
}

QFont AwUtilities::gui::markerFont(int type)
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

void AwUtilities::gui::saveMarkerFont(const QFont& font, int type)
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


QJsonDocument AwUtilities::readJsonFile(const QString& filePath)
{
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return QJsonDocument();
	}
	QJsonParseError error;
	QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
	file.close();
	if (doc.isNull() || doc.isEmpty() || error.error != QJsonParseError::NoError) {
		return QJsonDocument();
	}
	return doc;
}

quint32 AwUtilities::endianness::fromBigEndian(const uchar *src)
{
	return 0
		| src[3]
		| src[2] * quint32(0x00000100)
		| src[1] * quint32(0x00010000)
		| src[0] * quint32(0x01000000);
}

quint16 AwUtilities::endianness::fromBigEndian16(const uchar *src)
{
	return 0
		| src[1]
		| src[0] * 0x0100;
}

quint64 AwUtilities::endianness::fromBigEndian64(const uchar *src)
{
	return 0
		| src[7]
		| src[6] * Q_UINT64_C(0x0000000000000100)
		| src[5] * Q_UINT64_C(0x0000000000010000)
		| src[4] * Q_UINT64_C(0x0000000001000000)
		| src[3] * Q_UINT64_C(0x0000000100000000)
		| src[2] * Q_UINT64_C(0x0000010000000000)
		| src[1] * Q_UINT64_C(0x0001000000000000)
		| src[0] * Q_UINT64_C(0x0100000000000000);
}
