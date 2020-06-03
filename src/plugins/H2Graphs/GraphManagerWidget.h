#pragma once

#include <QWidget>
#include "ui_GraphManagerWidget.h"
#include "GraphSet.h"
#include <process/AwProcessGUIWidget.h>


class GraphManagerWidget : public AwProcessGUIWidget
{
	Q_OBJECT

public:
	GraphManagerWidget(AwGUIProcess *process, QWidget *parent = Q_NULLPTR);
	~GraphManagerWidget();

public slots:
	void addGraphSet(const QString& matlabFile);
	void loadFile();
	void visualize();
private:
	Ui::GraphManagerWidgetUi m_ui;
	QList<GraphSet *> m_graphSets;
	void updateTableView();
};
