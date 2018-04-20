#include "AnyWave.h"
#include <qtextstream.h>
#include "IO/BIDS/AwBIDSManager.h"
#include "AwException.h"
/// Methods to do NOGUI operations

void AnyWave::doSEEGToBIDS(const QString& file, const QString& subj, const QString& task, const QString& session)
{
	AwBIDSManager *bm = AwBIDSManager::instance();

	try {
		bm->seegToBIDS(file, subj, task, session);
	}
	catch (const AwException& e) {
		QTextStream(stdout) << QString("Error: %1 in %2").arg(e.errorString()).arg(e.origin()) << endl;
		return;
	}
}