#pragma once

#include <AwProcessInterface.h>
#include "registerinject_global.h"

class REGISTERINJECT_EXPORT RegisterInject : public AwProcess
{
    Q_OBJECT
public:
    RegisterInject();

    bool showUi() override;
    void run() override;
    void runFromCommandLine() override;
	bool init() override;
protected:
	AwBaseProcess* m_temporal, * m_eeginto4d;
};

class REGISTERINJECT_EXPORT RegisterInjectPlugin : public AwProcessPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID AwProcessPlugin_IID)
	Q_INTERFACES(AwProcessPlugin)
public:
	RegisterInjectPlugin();
	AW_INSTANTIATE_PROCESS(RegisterInject);
};
