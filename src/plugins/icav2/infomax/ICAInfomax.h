#pragma once
#include "ICAAlgorithm.h"
#include <QCheckBox>
class ICA;

class ICAInfomax : public ICAAlgorithm
{
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
	void runica(double* data, double* weights, int chans, int samples, double* bias, int* signs);
};