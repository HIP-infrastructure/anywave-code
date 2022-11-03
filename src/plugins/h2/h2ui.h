#ifndef H2UI_H
#define H2UI_H

#include <QDialog>
#include "ui_h2ui.h"
#include <AwMarker.h>

using namespace Ui;

typedef QPair<float, float> band;
typedef QPair<QDoubleSpinBox *, QDoubleSpinBox *> spinBox;
class H2;

class H2UI : public QDialog, public H2UIClass
{
	Q_OBJECT

public:
	H2UI(H2* h2, QWidget *parent = 0);
	~H2UI();
	QString dataFolder;
	QMap<QString, QPair<float, float> > bands;
	int method;
	int downSampling;
	float samplingRate;
	QString directory;
	AwSharedMarkerList markers;
	QStringList usedLabels, skippedLabels;
	bool saveInOneFile;
public slots:
	void accept() override;
	int exec() override;
protected:
	QStringList m_bandNames;
	QList<spinBox> m_spinBoxes;
	QList<QCheckBox *> m_checkBoxes;
	H2* m_h2;
};

#endif // H2UI_H
