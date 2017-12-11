#ifndef AW_EXPORTER_TYPES_H
#define AW_EXPORTER_TYPES_H

#include <QString>

struct MarkerExport {
	MarkerExport(const QString& l, int t = 0, float pr = 0., float po = 0.) {
		label = l; pre = pr; post = po; type = t; selected = false;}
	QString label;
	bool selected;
	int type;	// type == 0 for Single   != 0  for Selection
	float pre;
	float post;
};

typedef QList<MarkerExport *> ExportMarkerList;

#endif