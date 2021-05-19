#ifndef AW_COMBOCMAP_H
#define AW_COMBOCMAP_H
#include <QComboBox>
#include <AwGlobal.h>
#include <graphics/AwColorMap.h>

class AW_WIDGETS_EXPORT AwComboCMap : public QComboBox
{
	Q_OBJECT
public:
	AwComboCMap(QWidget *parent);
	void initToColorMap(int cmap);
	int selectedMap();
protected:
	AwCMapNamesAndTypes m_names_types;
};

#endif
