#include "AwBIDSSubject.h"

AwBIDSSubject::AwBIDSSubject(const QString& root, const QString& ID)
{
	m_ID = ID;
	m_fullPath = QString("%1/sub-%2").arg(root).arg(ID);
}


AwBIDSSubject::~AwBIDSSubject()
{
	while (!m_sessions.isEmpty())
		delete m_sessions.takeFirst();
	while (!m_items.isEmpty())
		delete m_items.takeFirst();
}

AwBIDSSession *AwBIDSSubject::addSession(const QString& label)
{
	m_sessions.append(new AwBIDSSession(this, label));
	return m_sessions.last();
}

AwFileItem *AwBIDSSubject::addItem(AwFileItem *item)
{
	m_items.append(item);
	return m_items.last();
}

QStringList AwBIDSSubject::findFile(const QString &filePath)
{
	QStringList res;
	for (auto session : m_sessions) {
		for (auto item : session->fileItems()) {
			// browse files inside a FileItem
			for (auto file : item->files()) {
				if (file == filePath || file.contains(filePath))
					res << QString("%1/%2").arg(item->fullPath()).arg(file);
			}
		}
	}
	for (auto item : m_items) {
		// browse files inside a FileItem
		for (auto file : item->files()) {
			if (file == filePath || file.contains(filePath))
				res << QString("%1/%2").arg(item->fullPath()).arg(file);
		}
	}
	return res;
}

AwFileItemList AwBIDSSubject::findFileItems(const QString &filePath)
{
	AwFileItemList res;
	for (auto session : m_sessions) {
		for (auto item : session->fileItems()) {
			// browse files inside a FileItem
			for (auto file : item->files()) {
				if (file == filePath || file.contains(filePath))
					res << item;
			}
		}
	}
	for (auto item : m_items) {
		// browse files inside a FileItem
		for (auto file : item->files()) {
			if (file == filePath || file.contains(filePath))
				res << item;
		}
	}
	return res;
}