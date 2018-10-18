#pragma once

#include <QString>

struct Event {
	QString beginTime;
	quint64 duration;
	QString label;
};

typedef QList<Event *> Events;