#pragma once
#include <QDialog>
#include <AwMarker.h>

class QTableWidget;
class QPushButton;
class QLineEdit;
class AwComboColor;
class QDoubleSpinBox;

class AwEditCustomListDial : public QDialog
{
	Q_OBJECT
public:
	AwEditCustomListDial(const AwSharedMarkerList& markers, QWidget* parent = nullptr);

	inline AwSharedMarkerList& markers() { return m_markers; }
public slots:
	void accept() override;
protected slots:
	void addMarker();
	void clearMarkers();
	void removeMarkers();
protected:
	QTableWidget* m_customTableWidget;
	QPushButton* m_add;
	QPushButton* m_remove;
	QPushButton* m_clear;
	AwSharedMarkerList m_markers;

	void updateTable();
};

class AwAddCustomMarkerDial : public QDialog
{
	Q_OBJECT
public:
	AwAddCustomMarkerDial(QWidget* parent = nullptr);
	AwSharedMarker marker;
public slots:
	void accept() override;
protected:
	QLineEdit* m_editLabel;
	AwComboColor* m_comboColor;
	QDoubleSpinBox* m_spinValue;
};