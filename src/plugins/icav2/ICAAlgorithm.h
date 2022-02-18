#pragma once
#include <QObject>
#include <QWidget>
#include <AwChannel.h>
#ifdef Q_OS_MAC
#define ARMA_DONT_USE_WRAPPER
#endif
#include <aw_armadillo.h>
#include "ica.h"

class ICAAlgorithm : public QObject
{
	Q_OBJECT
public:
	ICAAlgorithm(ICA* process, QObject* parent = nullptr);
	ICAAlgorithm(const ICAAlgorithm&) = delete;
	enum Flags { CanDoPCA = 1, HaveGUI = 2 };

	inline int flags() { return m_flags; }
	QString& name() { return m_name; }
	inline void setChannels(const AwChannelList& channels) { m_channels = channels; }
	inline void setNumberOfComponents(int n) { m_nComp = n; }
	virtual void run() = 0;
	virtual QWidget* getGUI() { return nullptr; }
	mat& mixing() { return m_mixing; }
	mat& unmixing() { return m_unmixing; }
	/** override this to provide extra key/values for your algorithm **/
	virtual QVariantMap getExtraSettings() { return QVariantMap(); }
signals: 
	void progressChanged(const QString&);

protected:
	QString m_name;
	int m_flags, m_nComp;
	AwChannelList m_channels;
	mat m_mixing, m_unmixing;
	ICA* m_process;

};