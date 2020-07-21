#include "AwBIDSManager.h"
#include <QFile>
#include <QTextStream>

int AwBIDSManager::createEventsTsv(const QString& filePath, const AwMarkerList& markers)
{
	m_errorString.clear();
	if (markers.isEmpty())
		return -1;
	QFile file(filePath);
	QTextStream stream(&file);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
		m_errorString = QString("Could not open %1 for writing.").arg(filePath);
		return -1;
	}
	// columns : onset duration trial_type
	stream << bids::tsv_event_onset << "\t" << bids::tsv_event_duration << "\t" << bids::tsv_event_trial_type << endl;
	for (auto m : markers) 
		stream << QString("%1").arg(m->start()) << "\t" << QString("%1").arg(m->duration()) << "\t" << m->label() << endl;
	file.close();
	return 0;
}