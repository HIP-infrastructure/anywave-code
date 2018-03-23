#pragma once
#include <exception>
using namespace std;
#include <QString>

class AwException : public exception
{
public:
	enum levels { warning, error };
	AwException(const QString& message, const QString& from, int level = error) throw()
		: m_level(level), m_message(message), m_from(from) 
	{}

	const QString& errorString() const throw()
	{
		return m_message;
	}

	const char *c_errorString() const throw()
	{
		return m_message.toStdString().c_str();
	}

	const int level() const throw()
	{
		return m_level;
	}

	const QString& origin() const throw()
	{
		return m_from;
	}
private:
	int m_level;
	QString m_message, m_from;
};
