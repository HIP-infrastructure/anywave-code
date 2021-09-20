#ifndef APPENDFILES_H
#define APPENDFILES_H

#include "appendfiles_global.h"
#include <AwProcessInterface.h>
#include <AwFileIO.h>
#include "settings.h"

class APPENDFILES_EXPORT AppendFiles : public AwProcess
{
	Q_OBJECT
public:
	AppendFiles();
	~AppendFiles();

	bool showUi() override;
	void run() override;
private:
	settings *m_ui;
	//AwFileIO *m_writer;
	QSharedPointer<AwFileIO> m_writer;
	QMap<QString, AwFileIOPlugin*> m_writers;  // carefull: writers here are instances of plugin objects, not AwFileIO instances
	QList<QSharedPointer<AwFileIO>> m_readers;
};

class APPENDFILES_EXPORT AppendFilesPlugin : public AwProcessPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID AwProcessPlugin_IID)
	Q_INTERFACES(AwProcessPlugin)
public:
	AppendFilesPlugin();

	AW_INSTANTIATE_PROCESS(AppendFiles)

};


#endif // APPENDFILES_H
