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
#include <AwMarker.h>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <AwCore.h>
#include <algorithm>
#ifdef Q_OS_WIN
#include <execution>
#endif
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <QBuffer>
#include <QtConcurrent>

//
// AwMarker
//
AwMarker::AwMarker()
{
	m_type = Single;
	m_code = -1;
	m_start = m_duration = 0.;
}

AwMarker::AwMarker(const QString& label, float pos, float dur)
{
	m_type = Single;
	m_code = -1;
	m_start = pos;
	m_duration = dur;
	m_label = label;
}


AwMarker::AwMarker(AwMarker *source)
{
	m_type = source->type();
	m_code = source->value();
	m_start = source->start();
	m_duration = source->duration();
	m_label = source->label();
	m_targetChannels = source->targetChannels();
	m_color = source->color();
}

AwMarker::AwMarker(const AwMarker& source)
{
	m_type = source.m_type;
	m_code = source.m_code;
	m_start = source.m_start;
	m_duration = source.m_duration;
	m_label = source.m_label;
	m_targetChannels = source.m_targetChannels;
	m_color = source.m_color;
}

void AwMarker::setEnd(float end)
{
	// if end is below start, reverse the position of the marker
	if (end < m_start) {
		m_start = end;
		m_duration = m_start - end;
	}
	else 
		m_duration = end - m_start;
}

void AwMarker::reshape(float start, float end)
{
	float s = start, e = end;
	if (end < start) {
		s = end;
		e = start;
	}
	m_start = s;
	m_duration = e - s;
}

bool AwMarker::contains(AwMarker *m)
{
	return m->start() >= m_start && m->end() <= m_start + m_duration;
}

//
// stringToType()
//
///\return The marker type or -1 if the specified string was invalid.
///\code{.cpp}
/// int type = AwMarker::stringToType("Single");
///\endcode
int AwMarker::stringToType(const QString& s)
{
	QList<int> type;
	QStringList stringTypes;

	stringTypes << "Single" << "Selection";

	type << AwMarker::Single << AwMarker::Selection;

	return type.at(stringTypes.indexOf(s));
}

///\return a QString containing the name of the type.
///\code{.cpp}
/// QString type = AwMarker::stringToType(AwMarker::Single);
///\endcode
QString AwMarker::typeToString(int t)
{
	QList<int> types;
	QStringList stringTypes;

	stringTypes << "Single" << "Selection";

	types << AwMarker::Single << AwMarker::Selection;

	int index = types.indexOf(t);
	if (index != -1)
		return stringTypes.at(index);
	
	return QString();
}

QVariantMap AwMarker::toVariantMap(const AwSharedMarkerList& markers)
{
	QString buffer;
	QTextStream stream(&buffer);
	stream << "{ [";
	for (auto const &m : markers) {
		stream << "{";
		stream << "label:" << m->label() << ",";
		stream << "position:" << m->start() << ",";
		stream << "duration:" << m->duration() << ",";
		stream << "color:" << m->color() << ",";
		stream << "channels:" << "[";
		for (const auto& t : m->targetChannels())
			stream << t << ",";
		stream << "]" << "},";
	}
	stream << "]}";
	QJsonDocument doc;
	QJsonParseError err;
	doc = QJsonDocument::fromJson(buffer.toUtf8(), &err);
	return doc.object().toVariantMap();
}

///
/// 
///
QStringList AwMarker::markersTypeList()
{
	QStringList list;

	list << "Single" << "Selection";
	return list;
}

int AwMarker::save(const QString& path, const QList<QSharedPointer<AwMarker>>& markers)
{
	QFile file(path);
	QTextStream stream(&file);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return -1;

	stream << "// AnyWave Marker File" << endl;
	for  (auto const &m : markers) {
		QString label = m->label();
		if (label.isEmpty())
			label = "No Label";
		stream << label << '\t' << QString::number(m->value()) << '\t' << QString::number(m->start());
		stream << '\t' << QString::number(m->duration());
		if (!m->targetChannels().isEmpty()) {
			stream << '\t';
			for (auto const& t : m->targetChannels())
				stream << t << ",";
		}
		if (!m->color().isEmpty()) {
			stream << '\t';
			stream << m->color();
		}

		stream << endl;
	}
	file.close();
	return 0;
}

AwSharedMarkerList AwMarker::duplicate(const AwSharedMarkerList& markers)
{
	AwSharedMarkerList res;
	for (const auto& m : markers)
		res << AwSharedMarker(new AwMarker(m.get()));
	return res;
}

void AwMarker::sort(AwSharedMarkerList& markers)
{
	auto sortFunction = [](const QSharedPointer<AwMarker>& m1, const QSharedPointer<AwMarker>& m2) -> bool
	{ return m1->start() < m2->start();	};
#ifndef Q_OS_WIN
	std::sort(markers.begin(), markers.end(), sortFunction);
#else
	if (markers.size() <= MARKERS_THREAD_THRESHOLD)
		std::sort(markers.begin(), markers.end(), sortFunction);
	else
		std::sort(std::execution::par, markers.begin(), markers.end(), sortFunction);
#endif
}

AwSharedMarkerList AwMarker::merge(AwSharedMarkerList& markers)
{
	if (markers.isEmpty())
		return AwSharedMarkerList();
	AwMarker::sort(markers);
	auto tmp = markers;
	AwSharedMarkerList res, copiedList;

	for (auto const& m : tmp) {
		if (m->duration() > 0.)
			copiedList << m;
	}
	while (!copiedList.isEmpty()) {
		auto first = copiedList.takeFirst();
		auto itsc = AwMarker::intersect(copiedList, first->start(), first->end());
		AwMarker::sort(itsc);
		if (itsc.isEmpty()) {
			res << first;
		}
		else {
			float start = first->start();
			float end = first->end();
			for (auto &i : itsc) {
				// merge intersected markers into first and re put it in the list.
				if (i->start() < start)
					start = i->start();
				if (i->end() > end)
					end = i->end();
				copiedList.removeAll(i);
			//	toDelete << i;
			}
			// reshape marker and insert it again at first position
		//	first->reshape(start, end);
			// attention if we reshape it, duplicate it
			auto newMarker = QSharedPointer<AwMarker>(new AwMarker(first.get()));
			newMarker->reshape(start, end);
			copiedList.insert(0, newMarker);
		}
	}
	return res;
}

///
/// intersect()
/// Browse a marker list and return only markers that are contained in the specified time interval.
/// Input list must be time sorted.
/// The return list does NOT contain duplicated markers.
AwSharedMarkerList AwMarker::intersect(const AwSharedMarkerList& markers, float start, float end)
{
	AwSharedMarkerList res;
	// We assume markers are sorted before processing
	for (const auto& m : markers) {
		if (m->end() < start)
			continue;
		if (m->start() > end)
			break;
		bool startBeforeEndAfter = m->start() < start && m->end() > end;
		bool startBeforeEndWithin = m->start() < start && m->end() <= end;
		bool startWithinEndAfter = m->start() >= start && m->end() > end;
		bool isWithin = m->start() >= start && m->end() <= end;

		if (isWithin || startWithinEndAfter || startBeforeEndWithin || startBeforeEndAfter)
			res << m;
	}
	return res;
}

AwSharedMarkerList AwMarker::applyXOROperation(AwSharedMarkerList& source, AwSharedMarkerList& cutList)
{
	// We assume markers are sorted before processing
	AwSharedMarkerList res;
	if (source.isEmpty() && cutList.isEmpty())
		return res;
	if (source.isEmpty() && !cutList.isEmpty())
		return cutList;
	if (!source.isEmpty() && cutList.isEmpty())
		return source;

	auto sources = AwMarker::merge(source);
	auto cuts = AwMarker::merge(cutList);

	for (auto &cutM : cuts) {
		for (auto &m : sources) {
			bool startBeforeEndAfter = m->start() < cutM->start() && m->end() > cutM->end();
			bool startBeforeEndWithin = m->start() < cutM->start() && m->end() <= cutM->end();
			bool startWithinEndAfter = m->start() >= cutM->start() && m->end() > cutM->end();
			bool isWithin = m->start() >= cutM->start() && m->end() <= cutM->end();
			bool isBefore = m->end() < cutM->start();
			bool isAfter = m->start() > cutM->end();

			if (isBefore) {
				res << m;
				sources.removeAll(m);
				//toDelete << m;
			}
			else if (isAfter) {
				break;
			}
			else if (startBeforeEndAfter) {
				// split in two parts
				auto nm = new AwMarker(m.get());
				nm->reshape(m->start(), cutM->start());
				res << QSharedPointer<AwMarker>(nm);
				// split the current marker to be the second part and let it in the list
				m->reshape(cutM->end(), m->end());
			}
			else if (startBeforeEndWithin) {
				m->reshape(m->start(), cutM->start());
				res << QSharedPointer<AwMarker>(new AwMarker(m.get()));
				sources.removeAll(m);
				//toDelete << m;
			}
			else if (startWithinEndAfter) {
				m->reshape(cutM->end(), m->end());
			}
			else if (isWithin) {
				sources.removeAll(m);
				//toDelete << m;
			}
		}
	}
	res += sources;
	return res;
}

/** Reshape markers to keep only markers that are contained in the data markers by source markers. **/
/** Do an AND operation on overlapping markers **/
AwSharedMarkerList AwMarker::applyANDOperation(AwSharedMarkerList& sources, AwSharedMarkerList& markers)
{
	if (markers.isEmpty())
		return sources;
	if (sources.isEmpty())
		return sources;

	// We assume markers are sorted before processing
	AwSharedMarkerList res, toDelete;
	auto op1 = AwMarker::merge(sources);
	for (auto &m : op1) {
		auto intersections = AwMarker::intersect(markers, m->start(), m->end());
		for (auto &iM : intersections) {
			bool startBeforeEndAfter = iM->start() < m->start() && iM->end() > m->end();
			bool startBeforeEndWithin = iM->start() < m->start() && iM->end() <= m->end();
			bool startWithinEndAfter = iM->start() >= m->start() && iM->end() > m->end();
			bool isWithin = iM->start() >= m->start() && iM->end() <= m->end();
			if (startBeforeEndAfter) {
				auto nm = new AwMarker(iM.get());
				nm->reshape(m->start(), m->end());
				res << QSharedPointer<AwMarker>(nm);
			}
			else if (startBeforeEndWithin) {
				auto nm = new AwMarker(iM.get());
				nm->reshape(m->start(), iM->end());
				res << QSharedPointer<AwMarker>(nm);
			}
			else if (startWithinEndAfter) {
				auto nm = new AwMarker(iM.get());
				nm->reshape(iM->start(), m->end());
				res << QSharedPointer<AwMarker>(nm);
			}
			else if (isWithin) {
				//res << new AwMarker(iM);
				res << iM;
			}
		}
	}
	return res;
}

AwSharedMarkerList AwMarker::markersBefore(const AwSharedMarkerList& markers, float pos)
{
	AwSharedMarkerList res;
	for (auto const& m : markers) {
		if (m->end() < pos)
			res << m;
	}
	return res;
}

AwSharedMarkerList AwMarker::markersWithin(const AwSharedMarkerList& markers, float pos, float end)
{
	AwSharedMarkerList res;
	for (auto const& m : markers) {
		bool isWithin = m->start() >= pos && m->end() <= end;
		if (isWithin)
			res << m;
	}
	return res;
}


/** Cut data marked by cutMarkers. Reshape markers and return a new list with markers after the cut. **/
AwSharedMarkerList AwMarker::cutAroundMarkers(AwSharedMarkerList& markers, AwSharedMarkerList& cutMarkers)
{
	// We assume markers are sorted before processing
	if (cutMarkers.isEmpty() || markers.isEmpty())
		return AwSharedMarkerList();
	auto tmp = markers;
	auto cutList = AwMarker::merge(cutMarkers);
	// remove markers that may be present on both list from the source list
	QStringList labels = AwMarker::getAllLabels(cutMarkers);
	AwSharedMarkerList res, copiedList;
	for (auto const& m : tmp) {
		if (!labels.contains(m->label()))
			copiedList << m;
	} 
	copiedList = AwMarker::duplicate(copiedList);  // duplicate instances of markers as we possibly modify some of them

	float shiftedTimePos = 0.;
	for (auto const& cutM : cutList) {
		foreach (auto const& m , copiedList) {  // for each is IMPORTANT HERE to allow removing item while browsing list
			bool startBeforeEndAfter = m->start() < cutM->start() && m->end() > cutM->end();
			bool startBeforeEndWithin = m->start() < cutM->start() && m->end() <= cutM->end();
			bool startWithinEndAfter = m->start() >= cutM->start() && m->end() > cutM->end();
			bool isBefore = m->end() < cutM->start();
			bool isAfter = m->start() > cutM->end();
			bool isWithin = m->start() >= cutM->start() && m->end() <= cutM->end();
			// tmp here will be used as to delete from copied list temp list
			if (isBefore) {
				res << m;
				copiedList.removeOne(m);
			}
			else if (isAfter) {
				float start = m->start() - cutM->duration();
				if (start < 0.)
					start = 0.;
				m->setStart(start);
			}
			else if (startBeforeEndAfter) {
				m->reshape(m->start(), m->end() - cutM->duration());
			}
			else if (startBeforeEndWithin) {
				m->reshape(m->start(), cutM->start());
			}
			else if (startWithinEndAfter) {
				auto offset = cutM->end() - m->start();
				m->reshape(cutM->start(), cutM->start() + offset);
			}
			else if (isWithin)
				copiedList.removeOne(m);
		}
	}
	res += copiedList;
	return res;
}


AwSharedMarkerList AwMarker::getMarkersWithDuration(const AwSharedMarkerList& markers)
{
	AwSharedMarkerList res;
	for (auto const& m : markers)
		if (m->duration())
			res << m;
	return res;
}

AwSharedMarkerList AwMarker::filterMarkersLabels(AwSharedMarkerList& markers, const QStringList& labels)
{
	AwSharedMarkerList res;
	for (auto const& m : markers) {
		if (labels.contains(m->label()))
			res << m;
	}
	return res;
}

/// <summary>
/// getInputMarkers()
/// based on skip labels and used labels, update the markers list (passed as reference) and return the input list to use to request data
/// </summary>
/// <param name="markers"></param>
/// <param name="skipLabels"></param>
/// <param name="useLabels"></param>
/// <param name="totalDuration"></param>
/// <returns></returns>
AwSharedMarkerList AwMarker::getInputMarkers(AwSharedMarkerList& markers, const QStringList& skipLabels, const QStringList& useLabels, float totalDuration)
{
	bool skip = !skipLabels.isEmpty();
	bool use = !useLabels.isEmpty();
	AwSharedMarkerList inputMarkers;
	AwSharedMarkerList skippedMarkers, usedMarkers;
	if (skip) {
		skippedMarkers = AwMarker::getMarkersWithLabels(markers, skipLabels);
		if (skippedMarkers.isEmpty())
			skip = false;
	}
	if (use) {
		usedMarkers = AwMarker::getMarkersWithLabels(markers, useLabels);
		if (usedMarkers.isEmpty())
			use = false;
	}


	if (!use && !skip) { // do not modify markers and return ALL DATA as input.
		inputMarkers << QSharedPointer<AwMarker>(new AwMarker("All Data", 0, totalDuration));
	}
	else if (use && !skip) { // just use some markers as input =>  reshape markers and return used markers as input.
		//inputMarkers = AwMarker::getMarkersWithLabels(markers, useLabels);
		inputMarkers = usedMarkers;
		// remove used markers from the list
		for (auto &m : inputMarkers)
			markers.removeAll(m);

		float newPosition = 0.;
		AwSharedMarkerList updatedMarkers;
		for (auto &m : inputMarkers) {
			auto intersection = AwMarker::intersect(markers, m->start(), m->end());
			if (intersection.size()) {
				AwSharedMarkerList modifiedMarkers;
				for (auto &modified : intersection) {
					// reshape intersected markers if  necessary
					// if markers starts BEFORE, realign it to be in the input marker
					auto newM = new AwMarker(modified.get());
					if (newM->start() < m->start()) {
						if (newM->duration()) 
							newM->setDuration(m->start() - newM->start());
						newM->setStart(m->start());
					}
					// be sure the marker don't expand after the input marker
					newM->setEnd(std::min(m->end(), newM->end()));
					modifiedMarkers << QSharedPointer<AwMarker>(newM);
				}
				updatedMarkers += modifiedMarkers;
			}
		}
		markers = updatedMarkers;
	}
	else if (skip && !use) { // skip sections of data => reshape all the markers and set the inverted selection as input.
		inputMarkers = AwMarker::invertMarkerSelection(skippedMarkers, "Selection", totalDuration);
		// remove skipped markers from the list
		for (auto &m : skippedMarkers)
			markers.removeAll(m);
		auto reshaped = AwMarker::applyANDOperation(inputMarkers, markers);
		// now reshape all the markers after removing skipped ones.
		auto cutMarkers = AwMarker::cutAroundMarkers(reshaped, skippedMarkers);
		markers = cutMarkers;
	}
	else if (skip && use) {
		// remove artefact markers from the marker list
		for (auto const &m : skippedMarkers)
			markers.removeAll(m);
		// first => cut used markers with skipmarkers using XOR 
		auto usedCut = AwMarker::applyXOROperation(usedMarkers, skippedMarkers);
		// revert selection using usedCut markers
		auto revertSelection = AwMarker::invertMarkerSelection(usedCut, "Selection", totalDuration);
		// reshape intersected markers using XOR
		auto reshaped = AwMarker::applyANDOperation(usedCut, markers);
		// now cut all the markers.
		auto cutMarkers = AwMarker::cutAroundMarkers(reshaped, revertSelection);
		markers = cutMarkers;
		inputMarkers = usedCut;
	}
	return inputMarkers;
}

AwSharedMarkerList AwMarker::invertMarkerSelection(const AwSharedMarkerList& markers, const QString& label, float end)
{
	AwSharedMarkerList list = markers;
	AwMarker::sort(list);
	AwSharedMarkerList res;
	// get first maker to handle special case
	auto first = list.takeFirst();
	float pos = std::min(first->end(), end);

	if (first->start() > 0.) {
		auto newMarker = new AwMarker(label);
		newMarker->setStart(0.);
		newMarker->setEnd(first->start());
		res << QSharedPointer<AwMarker>(newMarker);
	}

	while (!list.isEmpty()) {
		auto m = list.takeFirst();
		auto nm = new AwMarker(label);
		nm->setStart(pos);
		nm->setEnd(m->start());
		res << QSharedPointer<AwMarker>(nm);
		pos = std::min(m->end(), end);
	}
	if (pos < end) {
		auto newMarker = new AwMarker(label);
		newMarker->setStart(pos);
		newMarker->setEnd(end);
		res << QSharedPointer<AwMarker>(newMarker);
	}
	return res;
}

AwSharedMarkerList AwMarker::getMarkersWithLabels(const AwSharedMarkerList& markers, const QStringList& labels)
{
	AwSharedMarkerList list;
	for (auto const& m : markers) {
		if (labels.contains(m->label()))
			list << m;
	}
	return list;
}

AwSharedMarkerList AwMarker::getMarkersWithLabel(const AwSharedMarkerList& markers, const QString& label)
{
	AwSharedMarkerList list;
	for (auto const& m : markers) {
		if (m->label() == label)
			list << m;
	}
	return list;
}

QHash<QString, int> AwMarker::computeHistogram(const AwSharedMarkerList& markers)
{
	QHash<QString, int> res;
	if (markers.isEmpty())
		return res;
	AwSharedMarkerList tmp = markers;
	while (!tmp.isEmpty()) {
		QString label = tmp.first()->label();
		AwSharedMarkerList::iterator it;
#ifndef Q_OS_WIN
		it = std::remove_if(tmp.begin(), tmp.end(), [label](AwSharedMarker& m1) { return m1->label() == label;  });
#else
		if (tmp.size() <= MARKERS_THREAD_THRESHOLD)
			it = std::remove_if(tmp.begin(), tmp.end(), [label](AwSharedMarker& m1) { return m1->label() == label;  });
		else
			it = std::remove_if(std::execution::par, tmp.begin(), tmp.end(), [label](AwSharedMarker& m1) { return m1->label() == label;  });
#endif
		int count = 0;
		for (AwSharedMarkerList::iterator i = it; i < tmp.end(); i++)
			count++;
		tmp.erase(it, tmp.end());
		res.insert(label, count);
	}
	return res;
}

AwSharedMarkerList AwMarker::getMarkersWithUniqueLabels(const AwSharedMarkerList& markers)
{
	AwSharedMarkerList res, tmp;
	if (markers.isEmpty())
		return res;
	tmp = markers;
	while (!tmp.isEmpty()) {
		QString label = tmp.first()->label();
		res << tmp.first();
#ifndef Q_OS_WIN
		tmp.erase(std::remove_if(tmp.begin(), tmp.end(), [label](AwSharedMarker& m1) { return m1->label() == label;  }), tmp.end());
#else
		if (tmp.size() <= MARKERS_THREAD_THRESHOLD)
			tmp.erase(std::remove_if(tmp.begin(), tmp.end(), [label](AwSharedMarker& m1) { return m1->label() == label;  }), tmp.end());
		else
			tmp.erase(std::remove_if(std::execution::par, tmp.begin(), tmp.end(), [label](AwSharedMarker& m1) { return m1->label() == label;  }), tmp.end());
#endif
	}
	return res;
}

///
/// getUniqueLabels()
/// 
QStringList AwMarker::getUniqueLabels(const QList<QSharedPointer<AwMarker>>& markers)
{
	QStringList res;
	if (markers.isEmpty())
		return res;

	QStringList labels = AwMarker::getAllLabels(markers);
	
	while (!labels.isEmpty()) {
		QString label = labels.first();
#ifndef Q_OS_WIN
		labels.erase(std::remove_if(labels.begin(), labels.end(), [label](const QString& l) { return l == label;  }), labels.end());
#else
		if (labels.size() <= MARKERS_THREAD_THRESHOLD)
			labels.erase(std::remove_if(labels.begin(), labels.end(), [label](const QString& l) { return l == label;  }), labels.end());
		else
			labels.erase(std::remove_if(std::execution::par, labels.begin(), labels.end(), [label](const QString& l) { return l == label;  }), labels.end());
#endif
		res << label;
	}
	return res;
}

///
/// getAllLabels()
/// 
QStringList AwMarker::getAllLabels(const QList<QSharedPointer<AwMarker>>& markers)
{
	QStringList res;
	for (auto const& m : markers)
		res << m->label();
	return res;
}

AwSharedMarkerList AwMarker::loadShrdFaster(const QString& path)
{
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return AwSharedMarkerList();
	QByteArray ba = file.readAll();
	file.close();
	QBuffer buffer(&ba);
	buffer.open(QIODevice::ReadOnly);
	QTextStream stream(&buffer);
	AwSharedMarkerList markers;

	while (!stream.atEnd()) {
		QString line = stream.readLine();
		line = line.trimmed();

		// processing line and skip line starting with //
		if (!line.startsWith("//")) {
			QString label = line.section('\t', 0, 0);
			if (label.isEmpty()) // no label => skip line
				continue;
			QString value = line.section('\t', 1, 1);
			if (value.isEmpty())
				continue;
			QString position = line.section('\t', 2, 2);
			if (position.isEmpty())
				continue;
			QString duration = line.section('\t', 3, 3);

			// check for target OR color
			QString guess = line.section('\t', 4, 4);
			QString color;
			QStringList targets;
			if (!guess.isEmpty()) {
				if (guess.startsWith("#")) { // got the color colum
					color = guess;
				}
				else { // try for targets
					targets = guess.split(",", QString::SkipEmptyParts);
				}
			}
			// check for other colum
			guess = line.section('\t', 5, -1);
			if (!guess.isEmpty()) {
				if (guess.startsWith("#")) { // got the color colum
					color = guess;
				}
				else { // try for targets
					targets = guess.split(",", QString::SkipEmptyParts);
				}
			}

			AwMarker* m = new AwMarker;
			m->setLabel(label);
			m->setValue((float)value.toDouble());
			m->setStart(position.toFloat());
			if (!duration.isEmpty())
				m->setDuration(duration.toFloat());
			if (!color.isEmpty())
				m->setColor(color);
			if (!targets.isEmpty())
				for (auto const &t : targets)
					m->addTargetChannel(t.trimmed());
			markers << AwSharedMarker(m);
		}
	}
	return markers;

}

int AwMarker::removeDoublons(AwSharedMarkerList& markers, bool sortList)
{
	if (markers.isEmpty())
		return 0;
	if (sortList)
		//		std::sort(markers.begin(), markers.end(), AwMarkerLessThan);
		AwMarker::sort(markers);
	int current = 0;
	const float tol = 0.001;  // 1 ms tolerance
	QList<AwSharedMarkerList> clusters;
	do {
		AwSharedMarkerList cluster;
		cluster.append(markers.at(current));
		float pos = markers.at(current++)->start();
		while (current < markers.size()) {
			auto m = markers.at(current);
			float delta = std::abs(m->start() - pos);
			if (delta <= tol) {
				cluster.append(m);
				current++;
			}
			else
				break;
		}
		if (cluster.size() > 1)
			clusters << cluster;
		if (current == markers.size())
			break;
	} while (true);

	auto compareMarkers = [](QSharedPointer<AwMarker>&m, QSharedPointer<AwMarker>& m1) {
		if (m->label() != m1->label())
			return false;
		const float tol = 0.001;
		float pos = std::abs(m1->start() - m->start());
		float dur = std::abs(m1->duration() - m->duration());

		bool res = pos <= tol && dur <= tol;
		if (res) { // check that targets are similar
			QStringList t1 = m1->targetChannels();
			QStringList t2 = m->targetChannels();
			if (t1.isEmpty() && t2.isEmpty())
				return true;
			if (t1.size() != t2.size())
				return false;
			for (int i = 0; i < t1.size(); i++) {
				QString s1 = t1.at(i);
				QString s2 = t2.at(i);
				if (s1.compare(s2, Qt::CaseSensitive) != 0)
					return false;
			}
			return true;
		}
		return res;
	};
	std::function<AwSharedMarkerList(AwSharedMarkerList&)> clustering = [compareMarkers](AwSharedMarkerList& markers) -> AwSharedMarkerList {
		AwSharedMarkerList res;
		auto first = markers.takeFirst();
		while (markers.size()) {
			for (auto &m : markers) {
				if (compareMarkers(first, m)) {
					res << first;
					break;
				}
			}
			first = markers.takeFirst();
		}
		return res;
	};

	QList<AwSharedMarkerList> results = QtConcurrent::blockingMapped<QList<AwSharedMarkerList>>(clusters.begin(), clusters.end(), clustering);
	AwSharedMarkerList overall;
	for (const auto& l : results)
		overall += l;
	for (auto m : overall)
#ifndef Q_OS_WIN
		markers.erase(std::find(markers.begin(), markers.end(), m));
#else
		markers.erase(std::find(std::execution::par, markers.begin(), markers.end(), m));
#endif
	return overall.size();
}

QList<QSharedPointer<AwMarker>> AwMarker::toSharedPointerList(const QList<AwMarker*>& list)
{
	AwSharedMarkerList res;
	for (auto marker : list)
		res << QSharedPointer<AwMarker>(marker);
	return res;
}

QList<AwMarker*> AwMarker::toMarkerList(const QList<QSharedPointer<AwMarker>>& list)
{
	AwMarkerList res;
	for (const auto& marker : list)
		res << marker.get();
	return res;
}
