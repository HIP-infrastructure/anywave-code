#pragma once

#include <QWidget>
#include "ui_AwBIDSGUI.h"
#include "AwBIDSSubject.h"
#include <qstandarditemmodel.h>
class AwBIDSManager;

class AwBIDSGUI : public QWidget
{
	Q_OBJECT

public:
	AwBIDSGUI(AwBIDSManager *bids, const QString& rootDir = QString(), QWidget *parent = Q_NULLPTR);
	~AwBIDSGUI();

	enum Roles { PathRole = Qt::UserRole + 1, TypeRole = Qt::UserRole + 2};
	enum ItemTypes { Subject, Session, ieeg, eeg, meg, DataFile };
	void refresh();
	void clear();
	void setRootDir(const QString& path);
public slots:
	void changeSourceFolder(int index);
signals:
	void dataFileClicked(const QString&);
protected slots:
	void handleDoubleClick(const QModelIndex& index);
protected:
	AwBIDSManager * m_bids;
	// keep a copy of models for the TreeView
	QMap<int, QStandardItemModel *> m_models;

	void initModel(QStandardItemModel *model, const AwBIDSSubjectList& subjects);
	void checkForElectrodeAndMesh(const QString& path) {}
private:
	Ui::AwBIDSGUIUi m_ui;
	QStandardItemModel *m_currentModel;
		
};
