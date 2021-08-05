#pragma once

#include <QDialog>
#include "ui_AwUpdaterGui.h"
class AwUpdateManager;
class Component;

class AwUpdaterGui : public QDialog
{
	Q_OBJECT
public:
	AwUpdaterGui(AwUpdateManager *, QWidget *parent = nullptr);
	~AwUpdaterGui();

	inline QList<Component*>& selectedComponents() { return m_selectedComponents; }
public slots:
	void accept() override;
	void selectAll();
	void unselect();

private:
	Ui::AwUpdaterGuiUi m_ui;
	AwUpdateManager* m_updateManager;
	QList<Component*> m_selectedComponents;
};
