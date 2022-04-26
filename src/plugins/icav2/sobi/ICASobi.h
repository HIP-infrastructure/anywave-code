#pragma once
#include "ICAAlgorithm.h"
class ICA;

class ICASobi : public ICAAlgorithm
{
	Q_OBJECT
public:
	ICASobi(ICA* process, QObject *parent);

	void run() override;
protected:

};