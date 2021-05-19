#ifndef AW_COMBO_COLOR_H
#define AW_COMBO_COLOR_H
#include <QComboBox>
#include <AwGlobal.h>
#include <QColor>
#include <QStringList>

class AW_WIDGETS_EXPORT AwComboColor : public QComboBox
{
	Q_OBJECT
public:
	AwComboColor(QWidget *parent);
	void initToColor(const QString& color);
	QString selectedColor();
protected:
	QStringList m_names, m_codes;
};

#endif