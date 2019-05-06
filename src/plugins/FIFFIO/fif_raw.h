#pragma once



#include <QString>
#include <QFile>
#include "fif_info.h"
#include "fif_stream.h"
#include <Eigen/Core>
using namespace Eigen;
class fifRawDir;

class fifRaw
{
public:
	fifRaw(const QString& filePath);
	~fifRaw();

	bool open(QIODevice::OpenModeFlag mode);
protected:
	QFile m_file;
	int m_firstSample, m_lastSample;
	fifInfo m_info;
	MatrixXd m_proj;
	RowVectorXd m_cals;
	fifStream *m_stream;
	QList<fifRawDir *> m_rawDirs;
};