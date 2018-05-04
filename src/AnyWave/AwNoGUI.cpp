#include "AnyWave.h"
#include <qtextstream.h>
#include "IO/BIDS/AwBIDSManager.h"
#include "AwException.h"
/// Methods to do NOGUI operations

int AnyWave::doSEEGToBIDS(const QString& file, const QString& destDir, const QString& format, const QString& subj, const QString& task, const QString& session, 
	const QString& run)
{
	AwBIDSManager *bm = AwBIDSManager::instance();

	try {
		bm->seegToBIDS(file, destDir, format, subj, task, session, run);
	}
	catch (const AwException& e) {
		QTextStream(stdout) << QString("Error: %1 in %2").arg(e.errorString()).arg(e.origin()) << endl;
		return -1;
	}
	return 0;
}