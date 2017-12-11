#ifndef AWNEWVIEWDIAL_H
#define AWNEWVIEWDIAL_H

#include <QDialog>
#include "ui_AwNewViewDial.h"

class AwNewViewDial : public QDialog
{
	Q_OBJECT

public:
	AwNewViewDial(QWidget *parent = 0);
	~AwNewViewDial();
	inline QList<int>& filters() { return m_filters; }
protected:
	void changeEvent(QEvent*);
public slots:
	void accept();
	void all();
	void none();
private:
	Ui::AwNewViewDialUi m_ui;

	QList<QCheckBox *> m_checkBoxes;	// all check boxes for channel types.
	QVector<int> m_types;				// corresponding channel types
	QList<int> m_filters;				// selected filters.
};

#endif // AWNEWVIEWDIAL_H
