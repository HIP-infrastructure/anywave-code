// AnyWave
// Copyright (C) 2013-2021  INS UMR 1106
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#pragma once
#include <exception>
using namespace std;
#include <QString>

class AwException : public exception
{
public:
	enum levels { warning, error };
	AwException(const QString& message, const QString& from = QString(), int level = error) throw()
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
