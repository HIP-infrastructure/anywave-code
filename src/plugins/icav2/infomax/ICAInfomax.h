#pragma once
#include "ICAAlgorithm.h"
#include <QCheckBox>
class ICA;

#ifdef Q_OS_MAC
class BlasInterface : public QObject
{
public:
	BlasInterface(QObject* parent = nullptr) : QObject(parent) {}

	double *runica(double* data, double* weights, int chans, int samples, double* bias, int* signs);
signals:
	void progressChanged(const QString&);
};
#endif

class ICAInfomax : public ICAAlgorithm
{
	Q_OBJECT
public:
	ICAInfomax(ICA *process);

	void run() override;
	QWidget* getGUI() override;
	QVariantMap getExtraSettings() override;

protected slots:
	void setExtendedMode(bool);
protected:
	QWidget* m_widget;
	QCheckBox* m_checkExtended;
	QVariantMap m_settings;

	void infomax(int m, int n, int nc);

#ifdef Q_OS_MAC
	BlasInterface blas;
#else
	void runica(double* data, double* weights, int chans, int samples, double* bias, int* signs);
#endif
};