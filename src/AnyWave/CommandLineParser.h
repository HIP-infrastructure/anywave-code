#pragma once

#include <QCommandLineParser>
#include "AwCommandLine.h"
namespace aw {
	namespace commandLine {
		enum Commands { BIDS_SEEG, ICA };
		QMap<int, AwArguments> doParsing();
		int doCommandLineOperations(QMap<int, AwArguments>& operations);
	}
}
