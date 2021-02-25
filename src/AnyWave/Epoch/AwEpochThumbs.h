#pragma once
class AwEpoch;
#include <QPixmap>

typedef struct {
	AwEpoch *epoch;
	QPixmap pixmap;
} thumb;

typedef QList<thumb *> thumbList;