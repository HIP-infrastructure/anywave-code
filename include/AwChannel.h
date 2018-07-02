/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
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
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef AWCHANNEL_H
#define AWCHANNEL_H
#include <AwGlobal.h>
#include <QStringList>
#include <QUuid>
#include <QVariant>
#include <QSemaphore>
#include <QVector>
#include <AwMarker.h>
#define AW_CHANNEL_TYPES	11
/*!
 * \brief
 * This class defines the AwChannel object.
 * 
 * AwChannel is the object used to manage channels stored in a data file.
 * A channel has a type, a sampling rate and some data associated.
 * Note that data are stored using a float array. That means that each channel has its own
 * data vector.
 * Some other attributes, like the colour, the gain, are used by AnyWave for visualizing channel data.
 * 
 * \remarks
 * A channel can be set to be Virtual. Virtual channels can be used by some processing or generated by some algorithms.
 * Their data are not coming from the data file but are computed.
 * 
 */
class AW_CORE_EXPORT AwChannel
{
public:
	enum ChannelType {EEG = 0, SEEG = 0x01, MEG = 0x02, EMG = 0x03, ECG = 0x04, Reference = 0x05, Trigger = 0x06, Other = 0x07, 
	ICA = 0x08, Source = 0x09, GRAD = 0xA};
	enum SourceType { Real, Virtual };   	
	
	/** Default constructor **/
	explicit AwChannel();
	/** Copy constructor **/
	explicit AwChannel(AwChannel *chan);
	/** Destructor **/
	virtual ~AwChannel();

	/** Duplicates the actual channel object and returns a new duplicated instance **/
	virtual AwChannel* duplicate();

	/** Allocates new data for length samples and return pointer to newly allocated memory. **/
	float *newData(qint64 length);
	/** Cut data marked by markers and return the new data. MAKE SURE THE MARKER LIST IS SORTED **/
	float *cutData(const AwMarkerList& markers);
	/** Clears data associated with channel. **/
	void clearData();
	inline QString& className() { return m_className; }
	/** Returns the source type for a channel. SourceType can be Real or Virtual. **/
	inline SourceType sourceType() { return m_sourceType; }
	/** Returns the gain factor of channel, used for display. This is a number of units by cm. **/
	inline float gain() { return m_gain; }
	/** Returns the sampling rate in Hz. **/
	inline float samplingRate() { return m_samplingRate; }
	/** Returns the channel's name. Most often the sensor's label. **/
	inline QString& name() {return m_name; } 
	/** Return full name which includes the reference channel **/
	QString fullName();
	inline int ID() { return m_ID; }
	/** Returns the number of samples. **/					
	virtual qint64 dataSize() { return m_dataSize; }
	/** Returns a pointer to the data array. **/
	virtual float *data() { return m_data; }	
	/** Reduce the number of samples by decimating the data **/
	void decimate(int decimation);
	/** Returns the unit string. If no unit is specified, returns an empty string.**/
	inline QString& unit() { return m_unit; }
	/** Returns the x coordinate. **/
	inline double x() { return m_x; }
	/** Returns the y coordinate.. **/
	inline double y() { return m_y; }
	/** Returns the z coordinate. **/
	inline double z() { return m_z; }
	/** Returns the x coordinate of the orientation vector. **/
	inline double ox() { return m_ox; }
	/** Returns the y coordinate of the orientation vector. **/
	inline double oy() { return m_oy; }
	/** Returns the z coordinate of the orientation vector. **/
	inline double oz() { return m_oz; }
	/** Returns x,y and z coordinates at once. **/
	void xyz(double *x, double *y, double *z);
	/** Returns x,y and z coordinates of orientation vector at once **/
	void orientationXyz(double *ox, double *oy, double *oz);
	/** Returns true if the channel is marked as Bad. Bad channels are removed from montage and display. **/
	inline bool isBad() { return m_bad; }
	/** Returns the channel's type: AwChannel::EEG, AwChannel::MEG, AwChannel::SEEG, AwChannel::ECG, etc.. **/
	inline int type() { return m_type; }
	/** Returns the channel's reference label. Could be empty if the channel has no reference. **/
	inline QString& referenceName() { return m_referenceName; }
	/** Returns the channels names list of references. Those are the channels used as references for the current channel.
	This method should not be used directly. **/
	inline QList<AwChannel *>& references() { return m_references; }		
	/** Returns the current color for the channel as a QString. **/
	inline QString& color() { return m_color; }
	/** Returns the High Pass Filter value. If no filtering is set, returns -1. **/
	inline float highFilter() { return m_highFilter; }
	/** Returns current value for notch filter. **/
	inline float notch() { return m_notch; }
	/** Returns true if channel has a reference. **/
	inline bool hasReferences() { return !m_referenceName.isEmpty(); }
	/** Returns the Low Pass Filter value. If no filtering is set, returns -1. **/
	inline float lowFilter() { return m_lowFilter; }
	inline AwChannel *parent() { return m_parent; }
	inline QString& displayPluginName() { return m_registeredDisplayPlugin; }
	/** Returns true if current channel has xyz coordinates. **/
	inline bool hasCoordinates() { return m_hasCoordinates; }
	/** Returns true if the channel is of type EEG. **/
	inline bool isEEG() { return m_type == AwChannel::EEG; }
	/** Returns true if the channel is of type MEG. **/
	inline bool isMEG() { return m_type == AwChannel::MEG; }
	/** Return true if the channel is a gradiometer. **/
	inline bool isGRAD() { return m_type == AwChannel::GRAD; }
	/** Returns true if the channel is of type Reference (MEG Reference sensors). **/
	inline bool isReference() { return m_type == AwChannel::Reference; }
	/** Returns true if the channel is of type ECG. **/
	inline bool isECG() { return m_type == AwChannel::ECG; }
	/** Returns true if the channel is of type Trigger. **/
	inline bool isTrigger() { return m_type == AwChannel::Trigger; }
	/** Returns true if the channel is of type Other. **/
	inline bool isOther() { return m_type == AwChannel::Other; }
	/** Returns true if the channel is of type EMG. **/
	inline bool isEMG() { return m_type == AwChannel::EMG; }
	/** Returns true if the channel is of type SEEG. **/
	inline bool isSEEG() { return m_type == AwChannel::SEEG; }
	inline bool isICA() { return m_type == AwChannel::ICA && m_sourceType == AwChannel::Virtual; }
	inline bool isRealICA() { return m_type == AwChannel::ICA && m_sourceType == AwChannel::Real; }
	inline bool isSource() { return m_type == AwChannel::Source; }

	/** Returns true if channel is virtual. **/
	inline bool isVirtual() { return m_sourceType == AwChannel::Virtual; }
	bool isDataReady(); 
	inline bool isSelected() { return m_isSelected; }
	inline QVariantList& customData() { return m_customData; }

	// SET
	void addRef(AwChannel *ref);
	void clearRefChannels();
	void clearRefName();
	void setParent(AwChannel *parent);

	/** Sets the name of the plugin needed to display the channel. 
	 * If no name is specified than AnyWave will displayed the channel using default channel rendering.
	 * Name must be the same of an existing Display Plugin.
	 * This is used only for custom, complex channels generated by a process. **/
	void setDisplayPluginName(const QString& name);

	/** Sets the unit string for the channel. i.e. "�V" for EEG channels. **/
	void setUnit(const char *u);
	/** Sets unit QString for the channel. i.e. "pT" for MEG channels. **/
    void setUnit(const QString& u);
	/** Sets the gain for the channel, in units by cm. **/
	void setGain(float factor);
	/** Sets the data sampling rate. **/
	void setSamplingRate(float rate);
	/** Sets the channel's name. **/
    void setName(const QString& Name);
	/** Sets the current color. **/
    void setColor(const QString& col); 
	/** Set the ID for the channel. Most often used as an index to identify the channel in the as recorded list of channels. **/
	void setID(quint32 id);
	/** Sets x, y and z coordinates. **/
	void setXYZ(double x, double y, double z);
	/** Sets x, y and z coordinates for orientation vector. **/
	void setOrientationXYZ(double ox, double oy, double oz);
	/** Marks the channel as Bad. **/
	void setBad(bool flag);
	/** Sets the channel's type. **/
	void setType(int t);	
	/** Changes the reference for the channel. **/
	void setReferenceName(const QString& ref);	
	/** Sets the High Pass Filtering value. **/
	void setHighFilter(float val);
	/** Sets the Low Pass Filtering value. **/
	void setLowFilter(float val);
	void setNotch(float val);		
	void setCustomData(const QVariantList& data) { m_customData = data; }
	/** Sets the source type which is Real or Virtual. By defaut all channels created are Real. **/
	void setSourceType(SourceType stype);						
	/** Sets flag that indicates that new data are loaded or computed for the channel. **/
	/** Sets class name **/
	void setClassName(const QString& name);
	void setDataReady(bool flag = true);
	/** Sets the current channel as selected in display. **/
	void setSelected(bool flag);
	/** Set new data vector for the channel **/
	void setData(float *data, quint64 dataSize, bool copy = false);


	/** Returns the type of channel based on the QString passed as parameter. If the string does not match a valid type,
	returns -1. **/
	static int stringToType(const QString& s);
	/** Returns the type of the channelStatic method that converts channel's type from an int value to a QString. **/
	static QString typeToString(int t);
	/** Returns the list of channel type present in the list passed as parameter. **/
	static QStringList getTypesAsString(const QList<AwChannel *>& list);
	static QList<int> getTypesAsInt(const QList<AwChannel *>& list);
	/** Get a sub list containing only channels of specified type. The channels are copied. The list can be empty. **/
	static QList<AwChannel *> extractChannelsOfType(const QList<AwChannel *>& list, int type);
	/** Get a sub list containing only channels of specified type. The list can be empty. **/
	static QList<AwChannel *> getChannelsOfType(const QList<AwChannel *>& list, int type);
	static QList<AwChannel *> duplicateChannels(const QList<AwChannel *>& list);
	static QList<AwChannel *> getChannelWithLabels(const QList<AwChannel *>& list, const QStringList& labels);
	/** Get the labels as a list from the channels. Fullname flag will return the full labels (ie. with the reference channel) **/
	static QStringList getLabels(const QList<AwChannel *>& list, bool fullName = false);
	static QList<AwChannel *> cloneList(const QList<AwChannel *>& list, bool cloneData = false);
	/* Release memory for data of all the channels in a list. */
	static void clearData(const  QList<AwChannel *>& list);
	/* Cut all the filters on channels. */
	static void clearFilters(const  QList<AwChannel *>& list);
	/* Defines the filters for all channels. */
	static void setFilters(const  QList<AwChannel *>& list, float hp, float lp, float notch = 0.);
	/** Compute the abs(max-min) for each channels and return the mean value **/
	static float meanRangeValue(const QList<AwChannel *>& list);
	/** Build a string list containing all the available types of channels **/
	static QStringList types();
	QVector<float> toVector();
protected:
	quint32 m_ID;					
	bool m_dataReady;
	float m_gain;					
	float m_samplingRate;	
	QString m_name; 
	float *m_data;
	qint64 m_dataSize;
	QString m_unit;			// Unit� du signal (�V, fT, etc.)
	// xyz coordinates
	double m_x;				
	double m_y;
	double m_z;
	// orientation vector
	double m_ox;				
	double m_oy;
	double m_oz;
	bool m_bad;				
	bool m_isSelected;
	int m_type;
	bool m_hasCoordinates;	
	SourceType m_sourceType;
	QString m_registeredDisplayPlugin;		// Optional name of a Display Plugin that will display the channel. 
											// If empty, AnyWave will use the default plugin to render signals.
	AwChannel *m_parent;
	QString m_referenceName;					
	QList<AwChannel *> m_references;	
	QString m_color;
	float m_lowFilter;	 
	float m_highFilter;   
	float m_notch;
	QString m_className;
	QVariantList m_customData;				// Custom data associated with the channel.
};

typedef QList<AwChannel *> AwChannelList;  ///< AwChannelList defines a list of AwChannel *
Q_DECLARE_METATYPE(AwChannelList)

#endif