#pragma once

// class designed to represent a filtering set 
// contains HP,LP,Notch filtering settings and associated color

#include <QString>

class MFVFilterSet
{
public:
	explicit MFVFilterSet() { m_hp = m_lp = m_notch = 0.; }

	inline QString& color() { return m_color; }
	inline float hp() { return m_hp; }
	inline float lp() { return m_lp; }
	inline float notch() { return m_notch; }
	void setHP(float hp) { m_hp = hp; }
	void setLP(float lp) { m_lp = lp; }
	void setNotch(float notch) { m_notch = notch; }
	void setColor(const QString& color) { m_color = color; }
protected:
	float m_lp, m_hp, m_notch;
	QString m_color;
};

using MFVFilterSets = QList<MFVFilterSet *>;