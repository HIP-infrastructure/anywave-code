/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include <AwChannel.h>
#include <AwVirtualChannel.h>
#include <QtMath>

static QStringList ChannelTypes = { "EEG", "SEEG" , "MEG" , "EMG" , "ECG" , "REFERENCE" , "TRIGGER" , "OTHER" , "ICA" , "SOURCE" , "GRAD" , "ECOG" };
static QStringList UnitTypes = { QString::fromLatin1("µv"), QString::fromLatin1("µv") , "pT", QString::fromLatin1("µv"), QString::fromLatin1("µv") ,
	"pT", "n/a", "n/a", "unit", "unit", "pT/m", QString::fromLatin1("µv") };
static QVector<float> DefaultAmplitudeValues = { 150., 300., 4, 300, 400, 10, 10, 10, 10, 10, 150, 300. };

//
// Default constructor
//
AwChannel::AwChannel()
{
	m_type = AwChannel::EEG;
	m_unit = AwChannel::unitForType(AwChannel::EEG);
	m_sourceType = AwChannel::Real;
	m_data = NULL;
	m_dataSize = 0;
	m_gain = 100;
	m_bad = false;
	m_color = QString("black");
	m_lowFilter = -1;
	m_highFilter = -1;
	m_notch = 0;
	m_x = m_y = m_z = m_ox = m_oy = m_oz = 0.0;
	m_dataReady = false;
	m_hasCoordinates = false;
	m_parent = NULL;
	m_isSelected = false;
	m_className = "AwChannel";
}

///
/// Creates a copy of the channel passed as parameter. The copy becomes the child channel.
///
AwChannel::AwChannel(AwChannel *chan)
{
	m_type = chan->type();
	m_ID = chan->ID();
	m_sourceType = chan->sourceType();
	m_gain = chan->gain();
	m_name = chan->name();
	m_bad = chan->isBad();
	m_samplingRate = chan->samplingRate();
	m_unit = chan->unit();
	m_x = chan->x();
	m_y = chan->y();
	m_z = chan->z();
	m_ox = chan->ox();
	m_oy = chan->oy();
	m_oz = chan->oz();
	m_color = chan->color();
	m_notch = chan->notch();
	m_lowFilter = chan->lowFilter();
	m_highFilter = chan->highFilter();
	m_data = NULL; 
	m_dataSize = 0;
	m_referenceName = chan->referenceName();
	m_dataReady = false;
	m_hasCoordinates = chan->hasCoordinates();
	m_parent = chan;
	m_registeredDisplayPlugin = chan->displayPluginName();
	m_isSelected = chan->isSelected();
	m_className = "AwChannel";
}

QString AwChannel::fullName()
{
	if (m_referenceName.isEmpty())
		return m_name;
	return QString("%1-%2").arg(m_name).arg(m_referenceName);
}

///
/// Makes a copy of the current channel. Data are copied too.
/// Returns a pointer to the newly created AwChannel.
AwChannel *AwChannel::duplicate()
{
	AwChannel *newc = new AwChannel(this);
	if (this->dataSize()) { // duplicate data if any
		float *data = newc->newData(this->dataSize());
		memcpy(data, this->data(), this->dataSize() * sizeof(float));
	}

	newc->setDataReady(this->isDataReady()); 
	return newc;
}

QVector<float> AwChannel::toVector()
{
	if (m_dataSize == 0)
		return QVector<float>();

	QVector<float> vector(m_dataSize);
	memcpy(vector.data(), m_data, m_dataSize * sizeof(float));
	return vector;
}

void AwChannel::setData(float *data, quint64 size, bool copy)
{
	if (m_data)
		delete[] m_data;
	if (copy) {
		m_data = new float[size];
		memcpy(m_data, data, size * sizeof(float));
	}
	else
		m_data = data;
	m_dataSize = size;
}


void AwChannel::setClassName(const QString& name)
{
	m_className = name;
}

void AwChannel::xyz(double *x, double *y, double *z)
{
	*x = m_x;
	*y = m_y;
	*z = m_z;
}

void AwChannel::orientationXyz(double *ox, double *oy, double *oz)
{
	*ox = m_ox;
	*oy = m_oy;
	*oz = m_oz;
}

// Sets

void AwChannel::setXYZ(double x, double y, double z)
{
	m_x = x; m_y = y; m_z = z; 
	m_hasCoordinates = true;
}

// setType()
/// Changing the type will clear the reference channel. 
void AwChannel::setType(int t)
{
	m_type = t;
	m_referenceName = "";
	m_unit = AwChannel::unitForType(t);
	m_gain = AwChannel::defaultAmplitudeForType(t);
	m_references.clear();
}
	 

void AwChannel::addRef(AwChannel *ref)
{
	m_references.append(ref);
}

/// Allocates a new data array of nSamples samples.
/// Deletes the previously allocated array of data.
float *AwChannel::newData(qint64 nSamples)
{
	m_dataReady = false;

	if (m_data)
		delete [] m_data;

	m_data = new float[nSamples];
	m_dataSize = nSamples;

	return m_data;
}

float *AwChannel::cutData(const AwMarkerList& markers)
{
	if (markers.isEmpty())
		return NULL;

	// first, sort all markers (chronologicaly)
	AwMarkerList sorted_markers = markers;
	// browse markers
	qint64 cut_size = 0;

	AwMarkerList temp;	// markers kept and to use to cut the data

	foreach(AwMarker *m, sorted_markers) {
		if (m->duration() <= 0.) {
			continue;
		}
		qint64 s = (qint64)floor(m->start() * m_samplingRate);
		if (s < 0 || s > m_dataSize) {
			continue;
		}
		qint64 end = (qint64)floor(m->end() * m_samplingRate);
		if (end > m_dataSize - 1) { // marker goes after data => reset it to end of data
			m->setEnd(m_dataSize / m_samplingRate);
			temp << m;
			break;
		}
		temp << m;
	}

	if (temp.isEmpty())
		return NULL;

	foreach(AwMarker *m, temp) 
		cut_size += (qint64)floor(m->duration() * m_samplingRate);
	
	if (cut_size >= m_dataSize)
		return NULL;

	// compute reduced size
	qint64 new_size = m_dataSize - cut_size;
	float *dest = new float[new_size];
	float *dest_buf = dest;
	float *source_buf = m_data;
	qint64 dest_s = 0, source_s = 0;
	while (!temp.isEmpty()) {
		AwMarker *m = temp.takeFirst();
		qint64 s = (qint64)floor(m->start() * m_samplingRate);
		while (source_s < s) {
			*dest_buf++ = *source_buf++;
			source_s++;
		}
		source_s = (qint64)floor(m->end() * m_samplingRate);
		source_buf = &m_data[source_s];
	}
	while (source_s < m_dataSize) {
		*dest_buf++ = *source_buf++;
		source_s++;
	}
	delete[]m_data;
	m_data = dest;
	m_dataSize = new_size;
	return m_data;
}

void AwChannel::setNotch(float val)
{
	if (val <= 0)
		m_notch = 0;
	else
		m_notch = val;
}

void AwChannel::clearRefChannels()
{
	m_references.clear();
}

void AwChannel::clearRefName()
{
	m_referenceName.clear();
}

void AwChannel::setParent(AwChannel *parent)
{
	m_parent = parent;
}

void AwChannel::setDisplayPluginName(const QString& name)
{
	m_registeredDisplayPlugin = name; 
}

void AwChannel::setUnit(const char *u)
{
	m_unit = QString::fromUtf8(u);
}

void AwChannel::setUnit(const QString& u)
{
	m_unit = u;
}

void AwChannel::setGain(float gain)
{
	m_gain = gain;
}

void AwChannel::setSamplingRate(float sr)
{
	m_samplingRate = sr;
}

void AwChannel::setName(const QString& name)
{
	m_name = name;
}

void AwChannel::setColor(const QString& color)
{
	m_color = color;
}

void AwChannel::setID(quint32 id)
{
	m_ID = id;
}

void AwChannel::setOrientationXYZ(double ox, double oy, double oz)
{
	 m_ox = ox; 
	 m_oy = oy; 
	 m_oz = oz; 
}

void AwChannel::setBad(bool f)
{
	m_bad = f;
}

void AwChannel::setReferenceName(const QString& ref)
{
	m_referenceName = ref;
}

void AwChannel::setHighFilter(float val)
{
	if (val <= 0) 
		m_highFilter = -1; 
	else m_highFilter = val;
}

void AwChannel::setLowFilter(float val)
{
	if (val <= 0) 
		m_lowFilter = -1; 
	else m_lowFilter = val;
}

void AwChannel::setSourceType(AwChannel::SourceType type)
{
	m_sourceType = type;
}

void AwChannel::setSelected(bool flag)
{
	m_isSelected = flag;
}

void AwChannel::decimate(int factor)
{
	if (m_data == NULL)
		return;
	qint64 newSize = m_dataSize / factor;
	float *data = new float[newSize];
	
	for (qint64 i = 0; i < newSize; i++)
		data[i] = m_data[i * factor];

	m_dataSize =  newSize;
	delete[] m_data;
	m_data = data;
}

void AwChannel::clearData()
{
	if (m_data)
		delete [] m_data;
	m_data = NULL;
	m_dataReady = false;
	m_dataSize = 0;
}


void AwChannel::setDataReady(bool flag)
{
	m_dataReady = flag;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// STATIC METHODS


int AwChannel::stringToType(const QString& s)
{
	auto index = ChannelTypes.indexOf(s.toUpper());
	return index;
}

QStringList AwChannel::types()
{
	QStringList stringTypes = { "EEG", "SEEG" , "MEG" , "EMG" , "ECG" , "REFERENCE" , "TRIGGER" , "OTHER" , "ICA" , "SOURCE" , "GRAD" , "ECOG" };
	return stringTypes;
}


QString& AwChannel::unitForType(int type)
{
	return UnitTypes.value(type);
}

float AwChannel::defaultAmplitudeForType(int type)
{
	return DefaultAmplitudeValues.value(type);
}
///
///
///
QString AwChannel::typeToString(int t)
{
	if (t >= AW_CHANNEL_TYPES)
		return QString();
	return ChannelTypes.at(t);
}


QList<int> AwChannel::getTypesAsInt(const QList<AwChannel *>& list)
{
	QList<int>  res;
	
	for (auto c : list) {
		if (res.contains(c->type()))
			continue;
		res << c->type();
	}
	return res;
}

QStringList AwChannel::getTypesAsString(const QList<AwChannel *>& list)
{
	QStringList res;
	QList<int> types = AwChannel::getTypesAsInt(list);
	for (auto t : types)
		res << AwChannel::typeToString(t);
	return res;
}

//
// Get a copy of channels depending on type from a list.
// 
QList<AwChannel *> AwChannel::extractChannelsOfType(const QList<AwChannel *>& list, int type)
{
	AwChannelList res;
	foreach(AwChannel *c, list)
		if (c->type() == type)
			res << new AwChannel(c);

	return res;
}

//
// Get a sub list containing only the channels of the specified type
// 
QList<AwChannel *> AwChannel::getChannelsOfType(const QList<AwChannel *>& list, int type)
{
	AwChannelList res;
	foreach(AwChannel *c, list)
		if (c->type() == type)
			res << c;
	return res;
}

QList<AwChannel *> AwChannel::getChannelsWithLabels(const QList<AwChannel *>& list, const QStringList& labels)
{
	AwChannelList res, tmp;
	foreach(AwChannel *c, list) {
		if (labels.contains(c->name()))
			tmp << c;
	}
	// reorder list to match the order of labels in labels
	for (auto l : labels) {
		foreach(AwChannel *c, tmp) {
			if (c->name() == l) {
				res << c;
				tmp.removeAll(c);
			}
		}
	}
 	return res;
}

//
// Get a copy of channels depending on type from a list.
// 
QList<AwChannel *> AwChannel::duplicateChannels(const QList<AwChannel *>& list)
{
	AwChannelList res;
	foreach(AwChannel *c, list)  {
		if (c->isVirtual()) {
			AwVirtualChannel *vc = static_cast<AwVirtualChannel *>(c);
			res << c->duplicate();
		}
		else 
			res << c->duplicate();
	}

	return res;
}

QList<AwChannel *> AwChannel::cloneList(const QList<AwChannel *>& list, bool cloneData)
{
	AwChannelList res;
	foreach(AwChannel *c, list) {
		AwChannel *newChan = c->duplicate();
		if (cloneData && c->dataSize())  {
			newChan->newData(c->dataSize());
			memcpy(newChan->data(), c->data(), c->dataSize() * sizeof(float));
		}
	}
	return res;
}

QList<AwChannel *> AwChannel::removeDoublons(const QList<AwChannel *>& list)
{
	QStringList labels;
	AwChannelList res;
	for (auto c : list) {
		if (!labels.contains(c->name())) {
			labels << c->name();
			res << c;
		}
	}
	return res;
}

QStringList AwChannel::getLabels(const QList<AwChannel *>& list, bool fullName)
{
	QStringList res;
	foreach (AwChannel *c, list)
		fullName ? res << c->fullName() : res << c->name();
	return res;
}

/// 
/// Compute the mean range value for the channels.
/// Channels must be of the same type and have data.
float AwChannel::meanRangeValue(const QList<AwChannel *>& list)
{
	float mean = 0.;
	for (auto c : list) {
		float min = 0, max = 0;
		for (qint64 i = 0; i < c->dataSize(); i++) {
			if (c->data()[i] < min)
				min = c->data()[i];
			else if (c->data()[i] > max)
				max = c->data()[i];
		}
		mean += std::abs(max - min);		
	}
	return mean / list.size();
}


void AwChannel::clearData(const QList<AwChannel *>& list)
{
	for (auto c : list)
		c->clearData();
}

void AwChannel::clearFilters(const QList<AwChannel *>& list)
{
	for (auto c : list) {
		c->setLowFilter(0);
		c->setHighFilter(0);
		c->setNotch(0);
	}
}

void AwChannel::setFilters(const QList<AwChannel *>& list, float hp, float lp, float notch)
{
	for (auto c : list) {
		c->setLowFilter(lp);
		c->setHighFilter(hp);
		c->setNotch(notch);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// END OF STATIC METHODS


bool AwChannel::isDataReady()
{
	return m_dataReady;
}



AwChannel::~AwChannel()
{
	if (m_data)	{
		delete [] m_data;
		m_data = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// AwVirtualChannel
///
AwVirtualChannel::~AwVirtualChannel()
{
	clearConnectedChannels();
}

AwVirtualChannel::AwVirtualChannel(AwVirtualChannel *c) : AwChannel(static_cast<AwChannel *>(c))
{
	m_sourceType = AwChannel::Virtual;
	m_className = "AwVirtual";
	foreach (AwChannel *cc, c->connectedChannels())
		m_connectedChannels << new AwChannel(cc);
}

AwVirtualChannel *AwVirtualChannel::duplicate()
{
	AwVirtualChannel *newc = new AwVirtualChannel();

	newc->setSourceType(this->sourceType());
	newc->setClassName(this->className());
	newc->setID(this->ID());
	newc->setSamplingRate(this->samplingRate());
	newc->setBad(this->isBad());
	newc->setColor(this->color());
	newc->setGain(this->gain());
	newc->setType(this->type());
	newc->setName(this->name());
	newc->setUnit(this->unit());
	newc->setXYZ(this->x(), this->y(), this->z());
	newc->setOrientationXYZ(this->ox(), this->oy(), this->oz());
	newc->setNotch(this->notch());
	newc->setLowFilter(this->lowFilter());
	newc->setHighFilter(this->highFilter());
	newc->setReferenceName(this->referenceName());
	newc->setDisplayPluginName(this->displayPluginName());

	// check for connected channels
	if (!m_connectedChannels.isEmpty()) {
		AwChannelList list;
		foreach (AwChannel *c, m_connectedChannels) {
			list << new AwChannel(c);
		}
		newc->connectChannels(list);
	}

	if (m_dataSize) {
		float *data = newc->newData(this->dataSize());
		memcpy(data, this->data(), this->dataSize() * sizeof(float));
		newc->setDataReady();
	}

	return newc;
}

void AwVirtualChannel::connectChannels(const AwChannelList& channels)
{
	clearConnectedChannels();
	m_connectedChannels = channels;
	foreach(AwChannel *c, channels)
		m_channelsFromName.insert(c->name(), c);
}

void AwVirtualChannel::connectChannel(AwChannel *channel)
{
	clearConnectedChannels();
	m_connectedChannels << channel;
	m_channelsFromName.insert(channel->name(), channel);
}

AwChannel *AwVirtualChannel::getConnectedChannel(const QString& name)
{
	return m_channelsFromName.value(name);
}


void AwVirtualChannel::clearConnectedChannels()
{
	while (!m_connectedChannels.isEmpty())
		delete m_connectedChannels.takeFirst();
	m_connectedChannels.clear();
	m_channelsFromName.clear();
}
