#include "AwSourceManager.h"
#include <hdf5/AwHDF5.h>
#include <widget/AwMessageBox.h>
#include "Montage/AwMontageManager.h"
#include "Prefs/AwSettings.h"
#include <QFile>
#include <math/AwMath.h>
#include <QFileDialog>
#include "Filter/AwFiltersManager.h"
// statics init and definitions
AwSourceManager *AwSourceManager::m_instance = 0;

AwSourceManager *AwSourceManager::instance()
{
	if (!m_instance) {
		m_instance = new AwSourceManager;
	}
	return m_instance;
}

//end of statics

AwSourceManager::AwSourceManager(QObject *parent)
	: QObject(parent)
{
		m_lpf[0] = m_hpf[0] = m_lpf[1] = m_hpf[1] = 0.;
}

AwSourceManager::~AwSourceManager()
{
	closeFile();
}

void AwSourceManager::computeSources(AwSourceChannelList& channels)
{
	int source_type = channels.first()->subType() == AwChannel::MEG ? AwSourceManager::MEG : AwSourceManager::EEG;
	mat data = AwMath::channelsToMat(m_realChannels[source_type]);
	mat sources = m_matrices[source_type] * data;
	fmat res = conv_to<fmat>::from(sources).t();

	// apply data to source channels
	foreach(AwSourceChannel *c, channels) {
		float *dest = c->newData((qint64)data.n_cols);
		fcolvec col = res.col(c->index());
		memcpy(dest, col.memptr(), col.n_elem * sizeof(float));
	}
}

float AwSourceManager::lp(int type)
{
	int source_type = type == AwChannel::MEG ? AwSourceManager::MEG : AwSourceManager::EEG;
	return m_lpf[source_type];
}

float AwSourceManager::hp(int type)
{
	int source_type = type == AwChannel::MEG ? AwSourceManager::MEG : AwSourceManager::EEG;
	return m_hpf[source_type];
}

AwSourceChannelList& AwSourceManager::sources(int type)
{
	int source_type = type == AwChannel::MEG ? AwSourceManager::MEG : AwSourceManager::EEG;
	return m_channels[source_type];
}

AwChannelList& AwSourceManager::realChannels(int type)
{
	int source_type = type == AwChannel::MEG ? AwSourceManager::MEG : AwSourceManager::EEG;
	return m_realChannels[source_type];
}

void AwSourceManager::closeFile()
{
	// reset realChannels
	while (!m_realChannels[0].isEmpty())
		delete m_realChannels[0].takeFirst();
	while (!m_realChannels[1].isEmpty())
		delete m_realChannels[1].takeFirst();

	// reset source channels
	while (!m_channels[0].isEmpty())
		delete m_channels[0].takeFirst();
	while (!m_channels[1].isEmpty())
		delete m_channels[1].takeFirst();
}

QString AwSourceManager::autoSave(int type, mat& matrix, const QStringList& awLabels, const QStringList& labels, float lp, float hp)
{
	QMutexLocker lock(&m_mutex);
	QString dir = AwSettings::getInstance()->currentFileDir();
	QString path = QString("%1/sources_%2_lp%3_hp%4.bf").arg(dir).arg(AwChannel::typeToString(type)).arg(QString::number(lp)).arg(QString::number(hp));
	QFile file(path);
	if (!file.open(QIODevice::ReadWrite)) 
		return QString();
	
	QDataStream stream(&file);
	stream.setByteOrder(QDataStream::LittleEndian);
	stream.setVersion(QDataStream::Qt_4_4);
	stream << type << awLabels << labels << lp << hp;
	stream << matrix.n_rows << matrix.n_cols;
	for (int i = 0; i < matrix.n_elem; i++)
		stream << matrix.memptr()[i];
	file.close();
	return path;
}

void AwSourceManager::load(QString path)
{
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly)) {
		AwMessageBox::critical(0, "Source Manager", "Could not load source file.");
		return;
	}
	QDataStream stream(&file);
	stream.setByteOrder(QDataStream::LittleEndian);
	stream.setVersion(QDataStream::Qt_4_4);
	int type;
	QStringList awLabels, labels;
	float lp, hp;
	stream >> type >> awLabels >> labels >> lp >> hp;
	arma::uword nrow, ncol, index = 0;
	stream >> nrow >> ncol;
	mat matrix(nrow, ncol);

	for (arma::uword i = 0; i < matrix.n_elem; i++) {
		double value;
		stream >> value;
		matrix.memptr()[index++] = value;
	}
	file.close();
	setBeamformer(type, matrix, awLabels, labels, lp, hp);
}

///
/// setBeamFormer()
/// - type specifies the channels used to compute sources. (MEG or EEG).
/// Bad channels are not used.
/// - matrix is the transfert matrix from channels in file to sources
/// - awLabels labels of channels in AnyWave used to compute sources
/// - labels is the names of sources channels.
/// the number of labels must match the matrix number of columns
void AwSourceManager::setBeamformer(int type, mat& matrix, const QStringList& awLabels, const QStringList &labels, float lp, float hp)
{
	int source_type = type == AwChannel::MEG ? AwSourceManager::MEG : AwSourceManager::EEG;
	AwChannelList asRecordedChannels = AwChannel::getChannelsOfType(AwMontageManager::instance()->asRecordedChannels(), type);
	QStringList asRecordedLabels = AwChannel::getLabels(asRecordedChannels);

	// reset realChannels
	while (!m_realChannels[source_type].isEmpty())
		delete m_realChannels[source_type].takeFirst();
	// take only channels in awLabels
	foreach(QString s, awLabels) {
		int index = asRecordedLabels.indexOf(s);
		if (index == -1) {
			AwMessageBox::critical(0, "Source Manager", "Beamformer was computed using channels that don't exist in the current file.");
			return;
		}
		m_realChannels[source_type].append(new AwChannel(asRecordedChannels.at(index)));
	}
	float sr = asRecordedChannels.first()->samplingRate();
	m_lpf[source_type] = lp;
	m_hpf[source_type] = hp;
	m_matrices[source_type] = matrix;

	// reset source channels
	while (!m_channels[source_type].isEmpty())
		delete m_channels[source_type].takeFirst();

	int index = 0;
	foreach (QString s, labels) {
		AwSourceChannel *schan = new AwSourceChannel();
		schan->setIndex(index++);
		schan->setName(s);
		schan->setSubType(type);
		schan->setSamplingRate(sr);
		schan->setUnit("unit/cm");
		schan->setLowFilter(lp);
		schan->setHighFilter(hp);
		schan->setNotch(0);	// NO NOTCH !!!
		m_channels[source_type].append(schan);
	}
	emit newSourcesCreated(type);
}