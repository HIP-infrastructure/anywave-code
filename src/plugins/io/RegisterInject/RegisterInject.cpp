#include "RegisterInject.h"

RegisterInject::RegisterInject()
{
}


RegisterInjectPlugin::RegisterInjectPlugin()
{
	type = AwProcessPlugin::Background;
	name = "RegisterInject";
	version = "1.0.0";
	description = "Temporal register eeg and meg file then inject eeg data into meg file";
	setFlags(Aw::ProcessFlags::ProcessDoesntRequireData | Aw::ProcessFlags::ProcessHasInputUi | Aw::ProcessFlags::CanRunFromCommandLine);
}


bool RegisterInject::showUi()
{
	return false;
}

void RegisterInject::run()
{

}

void RegisterInject::runFromCommandLine()
{

}