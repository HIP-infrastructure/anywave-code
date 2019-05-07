#include "fif_raw.h"
#include <AwException.h>

fifRaw::fifRaw()
{
	m_lastSample = m_firstSample = -1;
	m_stream = Q_NULLPTR;
}

fifRaw::~fifRaw()
{
	if (m_stream)
		delete m_stream;
}


bool fifRaw::open(const QString& path, QIODevice::OpenModeFlag mode)
{
	m_file.close();
	m_file.setFileName(path);
	if (!m_file.open(mode)) {
		throw AwException(m_file.errorString());
	}
	if (m_stream)
		delete m_stream;
	m_stream = new fifStream(&m_file);

	try {
		if (mode & QIODevice::ReadOnly) {
			m_id = m_stream->check_beginning();
			// check for raw data tags...

		}
	}

	catch (const AwException& e) {
		throw e;
		return false;
	}
	return true;
}
