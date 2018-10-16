#pragma once

class MFFResourceType
{
public:
	enum Types {
		kMFF_RT_Unknown = -1, kMFF_RT_Any = 0, kMFF_RT_MFFFile = 1, kMFF_RT_Signal = 2, kMFF_RT_EventTrack = 3,
		kMFF_RT_Epochs = 4, kMFF_RT_Subject = 5, kMFF_RT_History = 6, kMFF_RT_Info = 7, kMFF_RT_InfoN = 8,
		kMFF_RT_Categories = 9, kMFF_RT_JTFCategories = 10, kMFF_RT_SensorLayout = 11, kMFF_RT_Coordinates = 12, kMFF_RT_Photogrammetry = 13,
		kMFF_RT_PNSSet = 14, kMFF_RT_MovieSyncs = 14, kMFF_RT_Fields = 14, kMFF_RT_Notes = 14, kMFF_RT_Montage = 14, kMFF_RT_DioleSet = 14
	};
	MFFResourceType(int type) { m_resourceType = type; }
	inline int getResourceType() { return m_resourceType; }
	inline void setResourceType(int type) { m_resourceType = type; }
protected:
	int m_resourceType;
};