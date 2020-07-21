#include "AwDataSet.h"
#include <AwFileIO.h>
#include "Data/AwDataServer.h"
//#include "Montage/AwMontageManager.h"
#include <montage/AwMontage.h>


AwDataSet::AwDataSet(AwFileIO *reader, QObject *parent)
	: QObject(parent)
{
	m_reader = reader;
	m_ds = AwDataServer::getInstance()->duplicate(m_reader);
	m_ds->setParent(this);
	m_montage = new AwMontage(reader);
}

AwChannelList& AwDataSet::montage()
{
	return m_montage->channels();
}

AwDataSet::~AwDataSet()
{
	delete m_montage;
}

void AwDataSet::connect(AwDataClient *client)
{
	m_ds->openConnection(client);
}
