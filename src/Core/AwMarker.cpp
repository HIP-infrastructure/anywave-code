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
#include <AwMarker.h>
#include <QStringList>
#include <QFile>
#include <QTextStream>

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

///
/// 
///
QStringList AwMarker::markersTypeList()
{
	QStringList list;

	list << "Single" << "Selection";
	return list;
}

void AwMarker::save(const QString& path, const AwMarkerList& markers)
{
	QFile file(path);
	QTextStream stream(&file);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;

	stream << "// AnyWave Marker File" << endl;
	foreach (AwMarker *m, markers) {
		QString label = m->label();
		if (label.isEmpty())
			label = "No Label";
		stream << label << '\t' << QString::number(m->value()) << '\t' << QString::number(m->start());
		stream << '\t' << QString::number(m->duration());
		if (!m->targetChannels().isEmpty()) {
			stream << '\t';
			foreach (QString t, m->targetChannels())
				stream << t << ",";
		}
		if (!m->color().isEmpty()) {
			stream << '\t';
			stream << m->color();
		}

		stream << endl;
	}
	file.close();
}

AwMarkerList AwMarker::duplicate(const AwMarkerList& markers)
{
	AwMarkerList res;
	foreach (AwMarker *m, markers)
		res << new AwMarker(m);
	return res;
}

AwMarkerList& AwMarker::sort(AwMarkerList& markers)
{
	std::sort(markers.begin(), markers.end(), AwMarkerLessThan);
	return markers;
}

AwMarkerList& AwMarker::rename(AwMarkerList& markers, const QString& label)
{
	for (auto m : markers)
		m->setLabel(label);
	return markers;
}

///
/// merge()
/// Check if selection markers overlap. If so, merge them.
/// Note : single marker are not processed.
/// The return list contains duplicated markers.
AwMarkerList AwMarker::merge(AwMarkerList& markers)
{
	if (markers.isEmpty()) 
		return AwMarkerList();

	AwMarkerList sorted = AwMarker::sort(markers);
	AwMarkerList res;

	// do not process single markers => copy them
	for (int i = 0; i < sorted.size(); i++) {
		AwMarker *m = sorted.at(i);
		if (m->duration() == 0.) {
			res << new AwMarker(m);
			sorted.removeAll(m);
			i--;
		}
	}

	if (sorted.isEmpty())
		return res;

	if (sorted.size() == 1) {
		res << new AwMarker(sorted.first());
		return res;
	}

	// At least two selection markers remaining.
	do {
		AwMarker *first = sorted.first();
		AwMarker *m = sorted.at(1);
		bool startAfter = m->start() > first->end();
		bool startInsideEndAfter = m->start() <= first->end() && m->end() > first->end();
		bool isWithin = m->start() >= first->start() && m->end() <= first->end();
		if (startAfter) { // just copy the previous marker to the result list.
			res << new AwMarker(first);
			sorted.removeAll(first);
		}
		else if (startInsideEndAfter) { // merge the two markers.
			float end = m->end();
			m->setStart(first->start());
			m->setEnd(end);
			sorted.removeAll(first);
		}
		else if (isWithin) {
			sorted.removeAll(m);
		}
	} while (sorted.size() > 1);

	for (auto m : sorted)
		res << new AwMarker(m);

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


AwMarkerList AwMarker::applyANDOperation(AwMarkerList& source, AwMarkerList& dest)
{
	AwMarkerList res;
	if (source.isEmpty() && dest.isEmpty())
		return res;
	if (source.isEmpty() && !dest.isEmpty())
		return AwMarker::duplicate(dest);
	if (!source.isEmpty() && dest.isEmpty())
		return AwMarker::duplicate(source);

	auto sources = AwMarker::merge(AwMarker::sort(source));
	auto destinations = AwMarker::merge(AwMarker::sort(dest));

	for (auto m : sources) {
		auto intersection = AwMarker::intersect(destinations, m->start(), m->end());
		if (intersection.isEmpty())
			continue;
		for (auto i : intersection) {
			bool startBeforeEndAfter = i->start() < m->start() && i->end() > m->end();
			bool startBeforeEndWithin = i->start() < m->start() && i->end() <= m->end();
			bool startWithinEndAfter = i->start() >= m->start() && i->end() > m->end();
			bool isWithin = i->start() >= m->start() && i->end() <= m->end();

			if (startBeforeEndAfter) {
				auto nm = new AwMarker(i);
				nm->reshape(m->start(), m->end());
				res << nm;
			}
			if (startBeforeEndWithin) {
				auto nm = new AwMarker(i);
				nm->reshape(m->start(), i->end());
				res << nm;
			}
			if (startWithinEndAfter) {
				auto nm = new AwMarker(i);
				nm->reshape(i->start(), m->end());
				res << nm;
			}
			if (isWithin)
				res << new AwMarker(i);
		}
	}
	while (!sources.isEmpty())
		delete sources.takeFirst();
	while (!destinations.isEmpty())
		delete destinations.takeFirst();
	return res;
}

AwMarkerList AwMarker::cutAroundMarkers(AwMarkerList& markers, AwMarkerList& cutMarkers)
{
	if (cutMarkers.isEmpty() || markers.isEmpty())
		return markers;
	AwMarkerList copiedList = AwMarker::sort(markers);

	// remove markers that may be present on both list from the source list
	QStringList labels = AwMarker::getAllLabels(cutMarkers);
	for (int i = 0; i < copiedList.size(); i++) {
		AwMarker *m = copiedList.at(i);
		if (labels.contains(m->label()))
			copiedList.removeAll(m);
	}

	AwMarkerList res, toRemove;
	for (auto sk : AwMarker::merge(cutMarkers)) {
		for (int i = 0; i < copiedList.size(); i++) {
			AwMarker *m = copiedList.at(i);
			// copy markers that ends before the first cut and remove it from copied list.
			if (m->end() < sk->start()) {
				copiedList.removeAll(m);
				res << new AwMarker(m);
				i--;
				continue;
			}
			if (m->start() > sk->end())  // skip markers that are after the current cut.
				break;

			// 
			bool startBeforeEndWithin = m->start() < sk->start() && m->end() <= sk->end();
			bool startBeforeEndAfter = m->start() < sk->start() && m->end() > sk->end();
			bool startWhitinEndAfter = m->start() >= sk->start() && m->end() > sk->end();
			bool isWithin = m->start() >= sk->start() && m->end() <= sk->end();

			if (isWithin) {
				copiedList.removeAll(m);
				i--;
				continue;
			}
			if (startBeforeEndWithin) {
				AwMarker *nm = new AwMarker(m);
				nm->setEnd(sk->start());
				res << nm;
				copiedList.removeAll(m);
				i--;
				continue;
			}

			if (startWhitinEndAfter) {
				AwMarker *nm = new AwMarker(m);
				nm->setStart(sk->end());
				nm->setEnd(m->end());
				res << nm;
				copiedList.removeAll(m);
				i--;
				continue;
			}

			if (startBeforeEndAfter) {
				AwMarker *nm1 = new AwMarker(m);
				nm1->setEnd(sk->start());
				res << nm1;
				AwMarker *nm2 = new AwMarker(m);
				nm2->setStart(sk->end());
				nm2->setEnd(m->end());
				res << nm2;
				copiedList.removeAll(m);
				i--;
				continue;
			}
		}
	}

	// duplicates the rest of copiedList
	for (auto m : copiedList)
		res << new AwMarker(m);

	return res;
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
		res = invertMarkerSelection(skippedMarkers, "selection", totalDuration);
	}
	else if (!skip && use) {
        auto tmp = AwMarker::getMarkersWithLabels(markers, used);
        res = AwMarker::merge(tmp);
	}
	else if (skip && use) {
        auto tmp = AwMarker::getMarkersWithLabels(markers, skipped);
        skippedMarkers = AwMarker::merge(tmp);
        tmp = AwMarker::getMarkersWithLabels(markers, used);
        usedMarkers = AwMarker::merge(tmp);
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

					if (startBefore && endWithin) { // crop marker from begining
						auto nm = new AwMarker(u);
						nm->reshape(m->end(), u->end());
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
		res = AwMarker::duplicate(AwMarker::sort(usedMarkers));
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
	list = AwMarker::sort(list);
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



///
/// getUniqueLabels()
/// 
QStringList AwMarker::getUniqueLabels(const QList<AwMarker *>& markers)
{
	QStringList res;

	foreach (AwMarker *m, markers)	{
		if (!res.contains(m->label()))
			res << m->label();
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

bool AwMarkerLessThan(AwMarker *m1, AwMarker *m2)
{
	return m1->start() < m2->start();
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
