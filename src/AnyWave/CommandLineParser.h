#pragma once

#include <QCommandLineParser>
#include "AwCommandLine.h"
namespace aw {
	namespace commandLine {
		enum Commands { BIDS_SEEG, ICA };
		QMap<int, AwArguments> doParsing();
		int doCommandLineOperations(QMap<int, AwArguments>& operations);

		class AwCommandLogger : public QObject
		{
			Q_OBJECT
		public:
			AwCommandLogger(const QString& componentName);
			~AwCommandLogger();

			void sendLog(const QString& message);
		signals:
			void log(const QString& message);
		};
	}
}
