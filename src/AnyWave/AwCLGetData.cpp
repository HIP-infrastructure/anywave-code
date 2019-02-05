#include "AwCommandLineManager.h"
#include "Plugin/AwPluginManager.h"
#include "Montage/AwMontageManager.h"
#include <QFile>
#include "AwCommandLogger.h"

void AwCommandLineManager::getData(AwArguments& arguments)
{
	AwCommandLogger logger("GetData", "cl_get_data");
	auto pm = AwPluginManager::getInstance();
	auto inputFile = arguments["input_file"].toString();
	if (!QFile::exists(inputFile)) {
		logger.sendLog(QString("file %1 does not exist.").arg(inputFile));
		return;
	}
	auto *reader = pm->getReaderToOpenFile(inputFile);
	if (reader == NULL) {
		logger.sendLog(QString("No reader plugin found to open %1.").arg(inputFile));
		return;
	}
	// if skipedMarker options is set, look for marker file
	AwMarkerList markers;
	if (arguments.contains("skip_marker")) {
		auto markerFile = QString("%1.mrk").arg(inputFile);
		markers = AwMarker::load(markerFile);
		if (markers.isEmpty()) 
			logger.sendLog(QString("skip_marker was specified but no marker file could not be loaded. Continue ignoring skip_marker option..."));
		else {
			auto labels = arguments["skip_markers"].toString();

		}

	}
}