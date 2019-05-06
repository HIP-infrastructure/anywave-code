#include "fif_raw.h"
#include <AwException.h>

fifRaw::fifRaw(const QString& filePath)
{
	m_file.setFileName(filePath);
	m_lastSample = m_firstSample = -1;
	m_stream = Q_NULLPTR;
}

fifRaw::~fifRaw()
{
	if (m_stream)
		delete m_stream;
}


bool fifRaw::open(QIODevice::OpenModeFlag mode)
{
	if (!m_file.open(mode)) {
		throw AwException(m_file.errorString());
	}
	if (m_stream)
		delete m_stream;
	m_stream = new fifStream(&m_file);

	try {
		if (mode & QIODevice::ReadOnly) {
			m_stream->check_beginning();
		}
	}

	catch (const AwException& e) {
		throw e;
		return;
	}
}