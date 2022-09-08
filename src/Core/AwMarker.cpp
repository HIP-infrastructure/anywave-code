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

QVariantMap AwMarker::toVariantMap(const AwMarkerList& markers)
{
	QString buffer;
	QTextStream stream(&buffer);
	stream << "{ [";
	for (auto m : markers) {
		stream << "{";
		stream << "label:" << m->label() << ",";
		stream << "position:" << m->start() << ",";
		stream << "duration:" << m->duration() << ",";
		stream << "color:" << m->color() << ",";
		stream << "channels:" << "[";
		for (const auto& t: m->targetChannels()) 
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
	return save(path, AwMarker::toMarkerList(markers));
}

int AwMarker::save(const QString& path, const AwMarkerList& markers)
{
	QFile file(path);
	QTextStream stream(&file);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return -1;

	stream << "// AnyWave Marker File" << endl;
	for  (AwMarker *m : markers) {
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

AwMarkerList AwMarker::duplicate(const AwMarkerList& markers)
{
	AwMarkerList res;
	for (AwMarker *m : markers)
		res << new AwMarker(m);
	return res;
}

AwSharedMarkerList AwMarker::duplicate(const AwSharedMarkerList& markers)
{
	AwSharedMarkerList res;
	for (const auto& m : markers)
		res << QSharedPointer<AwMarker>(new AwMarker(m.get()));
	return res;
}

void AwMarker::sort(AwMarkerList& markers)
{
#ifndef Q_OS_WIN
	std::sort(markers.begin(), markers.end(), AwMarkerLessThan);
#else
	if (markers.size() <= MARKERS_THREAD_THRESHOLD)
		std::sort(markers.begin(), markers.end(), AwMarkerLessThan);
	else
		std::sort(std::execution::par, markers.begin(), markers.end(), AwMarkerLessThan);
#endif
}

AwMarkerList& AwMarker::rename(AwMarkerList& markers, const QString& label)
{
	for (auto m : markers)
		m->setLabel(label);
	return markers;
}


AwMarkerList AwMarker::merge(AwMarkerList& markers)
{
	if (markers.isEmpty())
		return AwMarkerList();
	AwMarker::sort(markers);
	auto copiedList = AwMarker::duplicate(markers);
	AwMarkerList res, toDelete;

	// do not process single markers
	foreach(AwMarker *m, copiedList) {
		if (m->duration() <= 0.) {
			res << m;
			copiedList.removeAll(m);
		}
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
			for (auto i : itsc) {
				// merge intersected markers into first and re put it in the list.
				if (i->start() < start)
					start = i->start();
				if (i->end() > end)
					end = i->end();
				copiedList.removeAll(i);
				toDelete << i;
			}
			// reshape marker and insert it again at first position
			first->reshape(start, end);
			copiedList.insert(0, first);
		}
	}
	AW_DESTROY_LIST(toDelete);
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

///
/// intersect()
/// Browse a marker list and return only markers that are contained in the specified time interval.
/// Input list must be time sorted.
/// The return list does NOT contain duplicated markers.
AwMarkerList AwMarker::intersect(const AwMarkerList& markers, float start, float end)
{
	AwMarkerList res;
	// We assume markers are sorted before processing
	for (auto m : markers) {
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

AwMarkerList AwMarker::applyXOROperation(AwMarkerList& source, AwMarkerList& cutList)
{
	// We assume markers are sorted before processing
	AwMarkerList res, toDelete;
	if (source.isEmpty() && cutList.isEmpty())
		return res;
	if (source.isEmpty() && !cutList.isEmpty())
		return AwMarker::duplicate(cutList);
	if (!source.isEmpty() && cutList.isEmpty())
		return AwMarker::duplicate(source);

	auto sources = AwMarker::merge(source);
	auto cuts = AwMarker::merge(cutList);

	for (auto cutM : cuts) {
		foreach(AwMarker *m, sources) {
			bool startBeforeEndAfter = m->start() < cutM->start() && m->end() > cutM->end();
			bool startBeforeEndWithin = m->start() < cutM->start() && m->end() <= cutM->end();
			bool startWithinEndAfter = m->start() >= cutM->start() && m->end() > cutM->end();
			bool isWithin = m->start() >= cutM->start() && m->end() <= cutM->end();
			bool isBefore = m->end() < cutM->start();
			bool isAfter = m->start() > cutM->end();

			if (isBefore) {
				res << new AwMarker(m);
				sources.removeAll(m);
				toDelete << m;
			}
			else if (isAfter) {
				break;
			}
			else if (startBeforeEndAfter) {
				// split in two parts
				auto nm = new AwMarker(m);
				nm->reshape(m->start(), cutM->start());
				res << nm;
				// split the current marker to be the second part and let it in the list
				m->reshape(cutM->end(), m->end());
			}
			else if (startBeforeEndWithin) {
				m->reshape(m->start(), cutM->start());
				res << new AwMarker(m);
				sources.removeAll(m);
				toDelete << m;
			}
			else if (startWithinEndAfter) {
				m->reshape(cutM->end(), m->end());
			}
			else if (isWithin) {
				sources.removeAll(m);
				toDelete << m;
			}
		}
	}
	// duplicates the rest of copiedList
	for (auto m : sources) {
		res << new AwMarker(m);
	}

	AW_DESTROY_LIST(sources);
	AW_DESTROY_LIST(toDelete);
	AW_DESTROY_LIST(cuts);
	return res;
}

/** Reshape markers to keep only markers that are contained in the data markers by source markers. **/
/** Do an AND operation on overlapping markers **/
AwMarkerList AwMarker::applyANDOperation(AwMarkerList& sources, AwMarkerList& markers)
{
	if (markers.isEmpty())
		return AwMarker::duplicate(sources);
	if (sources.isEmpty())
		return sources;

	// We assume markers are sorted before processing
	AwMarkerList res, toDelete;

	auto op1 = AwMarker::merge(sources);
	auto op2 = AwMarker::duplicate(markers);

	for (auto m : op1) {
		auto intersections = AwMarker::intersect(op2, m->start(), m->end());
		for (auto iM : intersections) {
			bool startBeforeEndAfter = iM->start() < m->start() && iM->end() > m->end();
			bool startBeforeEndWithin = iM->start() < m->start() && iM->end() <= m->end();
			bool startWithinEndAfter = iM->start() >= m->start() && iM->end() > m->end();
			bool isWithin = iM->start() >= m->start() && iM->end() <= m->end();
			if (startBeforeEndAfter) {
				auto nm = new AwMarker(iM);
				nm->reshape(m->start(), m->end());
				res << nm;
			}
			else if (startBeforeEndWithin) {
				auto nm = new AwMarker(iM);
				nm->reshape(m->start(), iM->end());
				res << nm;
			}
			else if (startWithinEndAfter) {
				auto nm = new AwMarker(iM);
				nm->reshape(iM->start(), m->end());
				res << nm;
			}
			else if (isWithin) {
				res << new AwMarker(iM);
			}
		}
	}
	AW_DESTROY_LIST(op1)
	AW_DESTROY_LIST(op2)
	return res;
}

/** Cut data marked by cutMarkers. Reshape markers and return a new list with markers after the cut. **/
AwMarkerList AwMarker::cutAroundMarkers(AwMarkerList& markers, AwMarkerList& cutMarkers)
{
	// We assume markers are sorted before processing
	if (cutMarkers.isEmpty() || markers.isEmpty())
		return AwMarkerList();

	auto copiedList = AwMarker::duplicate(markers);
	auto cutList = AwMarker::merge(cutMarkers);

	// remove markers that may be present on both list from the source list
	QStringList labels = AwMarker::getAllLabels(cutMarkers);
	for (int i = 0; i < copiedList.size(); i++) {
		AwMarker *m = copiedList.at(i);
		if (labels.contains(m->label()))
			copiedList.removeAll(m);
	}

	AwMarkerList res, toDelete;
	float shiftedTimePos = 0.;
	
	for (auto cutM : cutList) {
		foreach(AwMarker *m, copiedList) {
			bool startBeforeEndAfter = m->start() < cutM->start() && m->end() > cutM->end();
			bool startBeforeEndWithin = m->start() < cutM->start() && m->end() <= cutM->end();
			bool startWithinEndAfter = m->start() >= cutM->start() && m->end() > cutM->end();
			bool isWithin = m->start() >= cutM->start() && m->end() <= cutM->end();
			bool isBefore = m->end() < cutM->start();
			bool isAfter = m->start() > cutM->end();

			// markers before are copied to res and removed from the list
			if (isBefore) {
				res << new AwMarker(m);
				copiedList.removeAll(m); // remove then from the current list.
				toDelete << m;
			}
			// markers after are time shifted by the cutm duration
			else if (isAfter) {
				float start = m->start() - cutM->duration();
				if (start < 0.)
					start = 0.;
				m->setStart(start);
			}
			else if (startBeforeEndAfter) { // cut the part inside Im and reshape it
				m->reshape(m->start(), m->end() - cutM->duration());
				//copiedList.removeAll(iM); // remove it from the current list.
			}
			else if (startBeforeEndWithin) {
				m->reshape(m->start(), cutM->start());
			}
			else if (startWithinEndAfter) {
				auto offset = cutM->end() - m->start();
				m->reshape(cutM->start(), cutM->start() + offset);
			}
			else if (isWithin) { // remove the marker
				copiedList.removeAll(m); // remove then from the current list.
				toDelete << m;
			}
		}
	}

	// duplicates the rest of copiedList
	for (auto m : copiedList) {
		res << new AwMarker(m);
	}

	AW_DESTROY_LIST(copiedList);
	AW_DESTROY_LIST(toDelete);
	AW_DESTROY_LIST(cutList);

	return res;
}

AwMarkerList AwMarker::getMarkersWithDuration(const AwMarkerList& markers)
{
	AwMarkerList res;
	for (auto m : markers)
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

AwMarkerList AwMarker::getInputMarkers(AwMarkerList& markers, const QStringList& skipLabels, const QStringList& useLabels, float totalDuration)
{
	bool skip = !skipLabels.isEmpty();
	bool use = !useLabels.isEmpty();
	AwMarkerList inputMarkers;

	if (!use && !skip) { // do not modify markers and return ALL DATA as input.
		inputMarkers << new AwMarker("All Data", 0, totalDuration);
	}
	else if (use && !skip) { // just use some markers as input =>  reshape markers and return used markers as input.
		inputMarkers = AwMarker::getMarkersWithLabels(markers, useLabels);
		if (inputMarkers.isEmpty())
			return AwMarker::duplicate(markers);
		// remove used markers from the list
		for (auto m : inputMarkers)
			markers.removeAll(m);

		float newPosition = 0.;
		AwMarkerList updatedMarkers;
		for (auto m : inputMarkers) {
			auto intersection = AwMarker::intersect(markers, m->start(), m->end());
		//	m->setStart(newPosition);
			if (intersection.size()) {
				auto modifiedMarkers = AwMarker::duplicate(intersection);
				for (auto modified : modifiedMarkers) {
					// reshape intersected markers if  necessary
					// if markers starts BEFORE, realign it to be in the input marker
					if (modified->start() < m->start()) {
						if (modified->duration())
							modified->setDuration(m->start() - modified->start());
						modified->setStart(m->start());
					}
					// be sure the marker don't expand after the input marker
					modified->setEnd(std::min(m->end(), modified->end()));
				}
				updatedMarkers += modifiedMarkers;
			}
		}
		AW_DESTROY_LIST(markers);
		markers = updatedMarkers;
	}
	else if (skip && !use) { // skip sections of data => reshape all the markers and set the inverted selection as input.
		auto skippedMarkers = AwMarker::getMarkersWithLabels(markers, skipLabels);
		if (skippedMarkers.isEmpty())
			return AwMarker::duplicate(markers);
		inputMarkers = AwMarker::invertMarkerSelection(skippedMarkers, "Selection", totalDuration);
		// remove skipped markers from the list
		for (auto m : skippedMarkers)
			markers.removeAll(m);
		auto reshaped = AwMarker::applyANDOperation(inputMarkers, markers);
		// now reshape all the markers after removing skipped ones.
		auto cutMarkers = AwMarker::cutAroundMarkers(reshaped, skippedMarkers);
		AW_DESTROY_LIST(markers);
		markers = cutMarkers;
		AW_DESTROY_LIST(reshaped);
		AW_DESTROY_LIST(skippedMarkers);
	}
	else if (skip && use) {
		auto usedMarkers = AwMarker::getMarkersWithLabels(markers, useLabels);
		auto skippedMarkers = AwMarker::getMarkersWithLabels(markers, skipLabels);
		if (usedMarkers.isEmpty() || skippedMarkers.isEmpty())
			return AwMarker::duplicate(markers);
		// remove artefact markers from the marker list
		for (auto m : skippedMarkers)
			markers.removeAll(m);
		// first => cut used markers with skipmarkers using XOR 
		auto usedCut = AwMarker::applyXOROperation(usedMarkers, skippedMarkers);
		// revert selection using usedCut markers
		auto revertSelection = AwMarker::invertMarkerSelection(usedCut, "Selection", totalDuration);
		// reshape intersected markers using XOR
		auto reshaped = AwMarker::applyANDOperation(usedCut, markers);
		// now cut all the markers.
		auto cutMarkers = AwMarker::cutAroundMarkers(reshaped, revertSelection);
		AW_DESTROY_LIST(markers);
		markers = cutMarkers;
		inputMarkers = usedCut;
		AW_DESTROY_LIST(revertSelection)
		AW_DESTROY_LIST(skippedMarkers)
		AW_DESTROY_LIST(reshaped)
	}

	return inputMarkers;
}

///
/// applySelectionFilter
/// Based on labels to skip and/or select, construct a new list with updated markers.
/// The skipped markers are used to cut around the other markers.
/// the used markers are used to select markers with corresponding labels from the original list.
/// The returned list is ALWAYS duplicated.

AwMarkerList AwMarker::applySelectionFilter(const AwMarkerList& markers, const QStringList& skipped, const QStringList& used, float totalDuration)
{
    AwMarkerList list = markers, skipList, tmpList;
	bool skip = !skipped.isEmpty();
	bool use = !used.isEmpty();
	AwMarkerList res, skippedMarkers, usedMarkers;

	if (skip && !use) {
        auto tmp = AwMarker::getMarkersWithLabels(markers, skipped);
        skippedMarkers = AwMarker::merge(tmp);
		if (!skippedMarkers.isEmpty())
			res = invertMarkerSelection(skippedMarkers, "selection", totalDuration);
	}
	else if (!skip && use) {
        //auto tmp = AwMarker::getMarkersWithLabels(markers, used);
       // res = AwMarker::merge(tmp);  // don't merge input markers is specified by use_markers
		res = AwMarker::duplicate(AwMarker::getMarkersWithLabels(markers, used));
	}
	else if (skip && use) {
        auto tmp = AwMarker::getMarkersWithLabels(markers, skipped);
        skippedMarkers = AwMarker::merge(tmp);
        tmp = AwMarker::getMarkersWithLabels(markers, used);
        usedMarkers = AwMarker::merge(tmp);
		if (skippedMarkers.isEmpty() && !usedMarkers.isEmpty())
			return usedMarkers;
		if (!skippedMarkers.isEmpty() && usedMarkers.isEmpty())
			return skippedMarkers;
		if (skippedMarkers.isEmpty() && usedMarkers.isEmpty())
			return res;
		// browse used markers and test if they overlap rejected/skipped ones.
		for (int i = 0; i < skippedMarkers.size(); i++) {
			auto m = skippedMarkers.at(i);
			auto inter = AwMarker::intersect(usedMarkers, m->start(), m->end());
			if (!inter.isEmpty()) {
				for (auto u : inter) {
					bool startBefore = u->start() < m->start();
					bool endBefore = u->end() < m->start();
					bool endWithin = u->end() <= m->end();
					bool startWithin = u->start() >= m->start() && u->start() <= m->end();
					bool endAfter = u->end() > m->end();
					bool included = u->start() >= m->start() && u->end() <= m->end();

					if (startBefore && endWithin) { 
						auto nm = new AwMarker(u);
						nm->reshape(u->start(), m->start());
						usedMarkers.removeAll(u);
						delete u;
						usedMarkers << nm;
					}
					else if (startWithin && endBefore) {
						usedMarkers.removeAll(u);
						delete u;
					}
					else if (startBefore && endAfter) {
						auto nm1 = new AwMarker(u);
						nm1->reshape(nm1->start(), m->start());
						usedMarkers << nm1;
						auto nm2 = new AwMarker(u);
						nm2->reshape(m->end(), u->end());
						usedMarkers << nm2;
						usedMarkers.removeAll(u);
						delete u;
					}
					else if (startWithin && endAfter) {
						auto nm = new AwMarker(u);
						nm->reshape(m->end(), u->end());
						usedMarkers << nm;
						usedMarkers.removeAll(u);
						delete u;
					}
				}
			}

		}
		AwMarker::sort(usedMarkers);
		res = AwMarker::duplicate(usedMarkers);
	}
	else
		res = markers;

	while (!usedMarkers.isEmpty())
		delete usedMarkers.takeFirst();
	while (!skippedMarkers.isEmpty())
		delete skippedMarkers.takeFirst();

	return res;
}


AwMarkerList AwMarker::invertMarkerSelection(const AwMarkerList& markers, const QString& label, float end)
{
	AwMarkerList list = markers;
	AwMarker::sort(list);
	AwMarkerList res;
	// get first maker to handle special case
	auto first = list.takeFirst();
	float pos = std::min(first->end(), end);

	if (first->start() > 0.) {
		auto newMarker = new AwMarker(label);
		newMarker->setStart(0.);
		newMarker->setEnd(first->start());
		res << newMarker;
	}

	while (!list.isEmpty()) {
		auto m = list.takeFirst();
		auto nm = new AwMarker(label);
		nm->setStart(pos);
		nm->setEnd(m->start());
		res << nm;
		pos = std::min(m->end(), end);
	}
	if (pos < end) {
		auto newMarker = new AwMarker(label);
		newMarker->setStart(pos);
		newMarker->setEnd(end);
		res << newMarker;
	}
	return res;
}

AwMarkerList AwMarker::getMarkersWithLabels(const AwMarkerList& markers, const QStringList& labels)
{
	AwMarkerList list;
	for (auto m : markers) {
		if (labels.contains(m->label()))
			list << m; 
	}
	return list;
}


AwMarkerList AwMarker::getMarkersWithLabel(const AwMarkerList& markers, const QString& label)
{
	AwMarkerList list;
	for (auto m : markers) {
		if (m->label() == label)
			list << m;
	}
	return list;
}


AwMarkerList AwMarker::getMarkersBetween(const AwMarkerList& markers, float pos1, float pos2)
{
	AwMarkerList _markers = markers;
	AwMarker::sort(_markers);
	return AwMarker::intersect(_markers, pos1, pos2);
}

QHash<QString, int> AwMarker::computeHistogram(const AwMarkerList& markers)
{
	QHash<QString, int> res;
	if (markers.isEmpty())
		return res;
	AwMarkerList tmp = markers;
	while (!tmp.isEmpty()) {
		QString label = tmp.first()->label();
		AwMarkerList::iterator it;
#ifndef Q_OS_WIN
		it = std::remove_if(tmp.begin(), tmp.end(), [label](AwMarker* m1) { return m1->label() == label;  });
#else
		if (tmp.size() <= MARKERS_THREAD_THRESHOLD)
			it = std::remove_if(tmp.begin(), tmp.end(), [label](AwMarker* m1) { return m1->label() == label;  });
		else
			it = std::remove_if(std::execution::par, tmp.begin(), tmp.end(), [label](AwMarker* m1) { return m1->label() == label;  });
#endif
		int count = 0;
		for (AwMarkerList::iterator i = it; i < tmp.end(); i++)
			count++;
		tmp.erase(it, tmp.end());
		res.insert(label, count);
	}
	return res;
}

AwMarkerList AwMarker::getMarkersWithUniqueLabels(const AwMarkerList& markers)
{
	AwMarkerList res, tmp;
	if (markers.isEmpty())
		return res;
	tmp = markers;
	while (!tmp.isEmpty()) {
		QString label = tmp.first()->label();
		res << tmp.first();
#ifndef Q_OS_WIN
		tmp.erase(std::remove_if(tmp.begin(), tmp.end(), [label](AwMarker* m1) { return m1->label() == label;  }), tmp.end());
#else
		if (tmp.size() <= MARKERS_THREAD_THRESHOLD)
			tmp.erase(std::remove_if(tmp.begin(), tmp.end(), [label](AwMarker* m1) { return m1->label() == label;  }), tmp.end());
		else
			tmp.erase(std::remove_if(std::execution::par, tmp.begin(), tmp.end(), [label](AwMarker* m1) { return m1->label() == label;  }), tmp.end());
#endif
	}
	return res;
}

///
/// getUniqueLabels()
/// 
QStringList AwMarker::getUniqueLabels(const QList<AwMarker *>& markers)
{
	QStringList res;
	if (markers.isEmpty())
		return res;

	AwMarkerList l_markers = markers;
	while (!l_markers.isEmpty()) {
		QString label = l_markers.first()->label();
#ifndef Q_OS_WIN
		l_markers.erase(std::remove_if(l_markers.begin(), l_markers.end(), [label] (AwMarker* m1) { return m1->label() == label;  }), l_markers.end());
#else
		if (l_markers.size() <= MARKERS_THREAD_THRESHOLD)
			l_markers.erase(std::remove_if(l_markers.begin(), l_markers.end(), [label] (AwMarker* m1) { return m1->label() == label;  }), l_markers.end());
		else
			l_markers.erase(std::remove_if(std::execution::par, l_markers.begin(), l_markers.end(), [label](AwMarker* m1) { return m1->label() == label;  }), l_markers.end());
#endif
		res << label;
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

QList<QPair<QString, int> > AwMarker::count(const AwMarkerList& markers)
{
	QStringList labels;
	QList<int> counts;
	for (auto m : markers) {
		if (!labels.contains(m->label())) {
			labels << m->label();
			counts << 1;
		}
		else {
			int index = labels.indexOf(m->label());
			int value = counts.at(index);
			counts.replace(index, ++value);
		}
	}
	QList<QPair<QString, int> > list;
	for (int i = 0; i < labels.size(); i++) 
		list.append(QPair<QString, int>(labels.at(i), counts.at(i)));
	return list;
}

///
/// getAllLabels()
/// 
QStringList AwMarker::getAllLabels(const QList<AwMarker *>& markers)
{
	QStringList res;
	for (auto m : markers)
		res << m->label();
	return res;
}

QStringList AwMarker::getAllLabels(const QList<QSharedPointer<AwMarker>>& markers)
{
	QStringList res;
	for (auto const& m : markers)
		res << m->label();
	return res;
}


bool AwMarkerLessThan(AwMarker *m1, AwMarker *m2)
{
	return m1->start() < m2->start();
}

AwSharedMarkerList AwMarker::loadShrdFaster(const QString& path)
{
	auto markers = loadFaster(path);
	return AwMarker::toSharedPointerList(markers);
}

AwMarkerList AwMarker::loadFaster(const QString& path)
{
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return AwMarkerList();
	QByteArray ba = file.readAll();
	file.close();
	QBuffer buffer(&ba);
	buffer.open(QIODevice::ReadOnly);
	QTextStream stream(&buffer);
	AwMarkerList markers;

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
			markers << m;
		}
	}
	return markers;

}

AwMarkerList AwMarker::load(const QString& path)
{
	QFile file(path);
	QTextStream stream(&file);

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return AwMarkerList();

	AwMarkerList markers;

	while (!stream.atEnd())	{
		QString line = stream.readLine();
		line = line.trimmed();

		// processing line and skip line starting with //
		if (!line.startsWith("//")) 	{
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
		
			AwMarker *m = new AwMarker;
			m->setLabel(label);
			//m->setValue((qint16)value.toInt());
			m->setValue((float)value.toDouble());
			m->setStart(position.toFloat());
			if (!duration.isEmpty())
				m->setDuration(duration.toFloat());
			if (!color.isEmpty())
				m->setColor(color);
			if (!targets.isEmpty()) 
				foreach (QString t, targets)
					m->addTargetChannel(t.trimmed());
			markers << m;
		}
	}
	file.close();
	return markers;
}

int AwMarker::removeDoublons(QList<AwMarker*>& markers, bool sortList)
{
	AwMarkerList res, removed;
	if (markers.isEmpty())
		return 0;
	if (sortList)
		//		std::sort(markers.begin(), markers.end(), AwMarkerLessThan);
		AwMarker::sort(markers);
	// cluster markers by their positions
	int current = 0;
	const float tol = 0.001;  // 1 ms tolerance
	QList<AwMarkerList> clusters;
	do {
		AwMarkerList cluster;
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

	auto compareMarkers = [](AwMarker* m, AwMarker* m1) {
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
	std::function<AwMarkerList(AwMarkerList&)> clustering = [compareMarkers](AwMarkerList& markers) -> AwMarkerList {
		AwMarkerList res;
		AwMarker *first = markers.takeFirst();
		while (markers.size()) {
			for (auto m : markers) {
				if (compareMarkers(first, m)) {
					res << first;
					break;
				}
			}
			first = markers.takeFirst();
		}
		return res;
	};

	QList<AwMarkerList> results = QtConcurrent::blockingMapped<QList<AwMarkerList>>(clusters.begin(), clusters.end(), clustering);
	AwMarkerList overall;
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
