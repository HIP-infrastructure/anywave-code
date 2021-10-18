#include "ICAAlgorithm.h"

ICAAlgorithm::ICAAlgorithm(ICA *process, QObject *parent) : QObject(parent)
{
	m_nComp = 0;
	m_process = process;
}