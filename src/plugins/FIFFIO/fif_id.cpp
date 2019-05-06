#include "fif_id.h"
#include "fif_file.h"
#include <qdatetime.h>
#include <QNetworkInterface>

fifId::fifId()
{
	clear();
}

fifId::fifId(const fifId& copy)
{
	version = copy.version;
	machid[0] = copy.machid[0];
	machid[1] = copy.machid[1];
	time.secs = copy.time.secs;
	time.usecs = copy.time.usecs;
}

fifId fifId::new_file_id()
{
	fifId id;
	id.version = FIFFC_VERSION;
	int fixed_id[2];
	get_machid(fixed_id);
	/*
	* Internet address in the first two words
	*/
	id.machid[0] = fixed_id[0];
	id.machid[1] = fixed_id[1];
	/*
	* Time in the third and fourth words
	*/
	/*
	* Time in the third and fourth words
	* Since practically no system gives times in
	* true micro seconds, the last three digits
	* are randomized to insure uniqueness.
	*/
	{
		id.time.secs = QDateTime::currentMSecsSinceEpoch() / 1000;
		id.time.usecs = rand() % 1000;
	}
	return id;
}

void fifId::clear()
{
	version = -1;
	machid[0] = machid[1] = -1;
	time.secs = time.usecs = -1;
}

bool fifId::get_machid(int *fixed_id)
{
	QList<QString> possibleHardwareAdresses;
	QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();

	fixed_id[0] = 0;
	fixed_id[1] = 0;

	if (!ifaces.isEmpty()) {
		for (int i = 0; i < ifaces.size(); ++i) {
			unsigned int flags = ifaces[i].flags();
			bool isLoopback = (bool)(flags & QNetworkInterface::IsLoopBack);
			bool isP2P = (bool)(flags & QNetworkInterface::IsPointToPoint);
			bool isRunning = (bool)(flags & QNetworkInterface::IsRunning);

			// If this interface isn't running, we don't care about it
			if (!isRunning) continue;
			// We only want valid interfaces that aren't loopback/virtual and not point to point
			if (!ifaces[i].isValid() || isLoopback || isP2P) continue;

			possibleHardwareAdresses << ifaces[i].hardwareAddress();
		}

		if (possibleHardwareAdresses.size() > 0) {
			// We take the first address as machine identifier
			QStringList hexPresentation = possibleHardwareAdresses[0].split(":");
			if (hexPresentation.size() == 6) {
				fixed_id[0] = QString(hexPresentation[0] + hexPresentation[1] + hexPresentation[2]).toInt(NULL, 16);
				fixed_id[1] = QString(hexPresentation[3] + hexPresentation[4] + hexPresentation[5]).toInt(NULL, 16);
				return true;
			}
		}
	}
	return false;
}