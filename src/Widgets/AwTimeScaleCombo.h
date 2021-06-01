#ifndef AWTIMESCALECOMBO_H
#define AWTIMESCALECOMBO_H
#include <AwGlobal.h>
#include <QComboBox>

class AW_WIDGETS_EXPORT AwTimeScaleCombo : public QComboBox
{
	Q_OBJECT

public:
	AwTimeScaleCombo(QWidget *parent);
	~AwTimeScaleCombo();
signals:
	void valueChanged(float value);
private slots:
	void extractValue(int index);
private:

	QVector<float> m_values;
	QStringList m_items;
};

#endif // AWTIMESCALECOMBO_H
