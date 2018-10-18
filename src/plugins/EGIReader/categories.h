#pragma once

#include <qlist.h>

class Segment
{
public:
	Segment() {
		beginTime = endTime = 0; evtBegin = evtEnd = 0;
	}

	quint64 beginTime, endTime;
	int evtBegin, evtEnd;
};

class Categorie
{
public:
	Categorie() {}
	~Categorie() {
		while (!segments.isEmpty())
			delete segments.takeFirst();
	}

	QString name;
	QList<Segment *> segments;
};

typedef QList<Categorie *> Categories;
