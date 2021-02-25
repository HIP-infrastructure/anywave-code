/********************************************************************
****    This software is freely distributed by Micromed s.r.l.   ****
****   to help the user have an easy interpretation of Micromed  ****
**** EEG data. Micromed cannot be held responsible for any error ****
****                  contained in this software.                ****
*********************************************************************
**** Version: 4.00                                               ****
**** Author: Cristiano Rizzo - Project Development Manager       **** 
**** Last modification: 31/01/2002                               ****
*********************************************************************
****     Any correction, appreciation or comment is welcome!     ****
****     For further informations or in case of need contact:    ****
****                                                             ****
****         Cristiano Rizzo - Micromed s.r.l.                   ****
****         I-31021 Mogliano Veneto (Treviso) - ITALY           ****
****         tel. +39.041.5937000 (district of Venice)           ****
****         fax. +39.041.5937011                                ****
****         e-mail: cristiano.rizzo@micromed-it.com             ****
********************************************************************/

#include <stdio.h>

/*******************************************************************
******************** CONSTANTS FOR ARRAY DIMENSION *****************
*******************************************************************/

#define MAX_LAB_3			256

#define LEN_MONTAGE_3		3072

#define MAX_NOTE_2			100
#define LEN_MONTAGE_2		1600


#define MAX_CAN				256		//	Maximum Number of Channels to be Acquired
#define MAX_DERIV			128		//	Maximum Number of Derivations to be Displayed
#define MAX_NOTE			200
#define MAX_SAMPLE			128
#define MAX_HISTORY			30
#define MAX_LAB				640		//	Maximum Number of Labels
#define MAX_MONT			30
#define MAX_FLAG			100
#define MAX_SEGM			100
#define LEN_MONTAGE			4096
#define MAX_SEC				40
#define	MAX_EVENT			100
#define	MAX_VIDEO_FILE		1024
#define	MAX_TRIGGER			8192


typedef struct
{
	unsigned char		NonInv;		//	This is for montages of Type 3
	unsigned char		Inv;
}
Micromed_Double_Byte;


typedef struct
{
	quint16	NonInv;		//	This is for montages of Type 4
	quint16	Inv;
}
Micromed_Double_Word;

/*******************************************************************
******* STRUCTURE NECESSARY FOR COMPATIBILITY WITH TYPE 0,1,2 ******
*******************************************************************/

typedef struct
{
	char			Surname[22];
	char			Name[20];
	unsigned char	Month;
	unsigned char	Day;
	unsigned char	Year;
	unsigned char	Reserved[3];
}
Micromed_PatientData;



typedef struct
{
	unsigned char	Month;
	unsigned char	Day;
	unsigned char	Year;
}
Micromed_Date_Type;



typedef struct
{
	char				Name[5];
	unsigned char		Reserved;
	quint16				Signal_Max;
}
Micromed_Electrode;



typedef struct
{
	quint32				Time;			
	char				Comment[40];	
}
Micromed_Old_Annotation;



typedef struct
{
	quint32				Time;        
	char				Comment[20]; 

}
Micromed_Annotation;






/*******************************************************************
******************* STRUCTURE NECESSARY FOR TYPE 3 *****************
*******************************************************************/

typedef struct
{
	char			Surname[22];
	char			Name[20];
	unsigned char	Month;
	unsigned char	Day;
	unsigned char	Year;
	unsigned char	Reserved[19];
}
Micromed_New_Patient_Data;



typedef struct 
{
	unsigned char	Day;
	unsigned char	Month;
	unsigned char	Year;
}
Micromed_New_Date_Type;



typedef struct
{
	unsigned char	Hour;
	unsigned char	Min;
	unsigned char	Sec;
}
Micromed_New_Time_Type;



#define	BQ124_INT		0
#define	MS40			1
#define	BQ132S_INT		2
#define	BQ124_BQCARD	6
#define	SAM32_BQCARD	7
#define	SAM25_BQCARD	8
#define	BQ132SR_INT		9
#define	SAM32R_BQCARD	10
#define	SAM25R_BQCARD	11
#define	SAM32_INT		12
#define	SAM25_INT		13
#define	SAM32R_INT		14
#define	SAM25R_INT		15



#define	C128	40
#define	C84P	42
#define	C84		44
#define	C96		46
#define	C63P	48
#define	C63		50
#define	C64		52
#define	C42P	54
#define	C42		56
#define	C32		58
#define	C21P	60
#define	C21		62
#define	C19P	64
#define	C19		66
#define	C12		68
#define	C8P		70
#define	C8		72
#define	CFRE	74



typedef struct
{
	char				Name[8];
	quint32				Start_Offset;
	quint32				Length;
}
Micromed_New_Descriptor;



typedef unsigned char Micromed_Code_3;

typedef quint16 Micromed_New_Code;


typedef struct
{
	quint16	Type;
	char				Positive_Input_Label[6];
	char				Negative_Input_Label[6];
	quint16				Logic_Minimum;
	quint16				Logic_Maximum;
	quint16				Logic_Ground;
	qint32				Physic_Minimum;
	qint32				Physic_Maximum;
	quint16				Measurement_Unit;
	quint16				Prefiltering_HiPass_Limit;
	quint16				Prefiltering_HiPass_Type;
	quint16				Prefiltering_LowPass_Limit;
	quint16				Prefiltering_LowPass_Type;
	quint16				Rate_Coefficient;
	quint16				Position;
	unsigned char		Reserved_1[12];
	float				Latitude;
	float				Longitude;
	unsigned char		Maps;
	unsigned char		Average_Ref;
	char				Description[32];
	unsigned char		Reserved_2[32];
}
Micromed_Electrode_3;



typedef struct
{
	unsigned char		Status;
	unsigned char		Type;
	char				Positive_Input_Label[6];
	char				Negative_Input_Label[6];
	quint32				Logic_Minimum;
	quint32				Logic_Maximum;
	quint32				Logic_Ground;
	qint32				Physic_Minimum;
	qint32				Physic_Maximum;
	quint16				Measurement_Unit;
	quint16				Prefiltering_HiPass_Limit;
	quint16				Prefiltering_HiPass_Type;
	quint16				Prefiltering_LowPass_Limit;
	quint16				Prefiltering_LowPass_Type;
	quint16				Rate_Coefficient;
	quint16				Position;
	float				Latitude;
	float				Longitude;
	unsigned char		Maps;
	unsigned char		Average_Ref;
	char				Description[32];
	unsigned char		Reserved_2[38];
}
Micromed_New_Electrode;



typedef struct
{
	quint32	Sample;			
	char				Comment[40];	
}
Micromed_New_Annotation;



typedef struct
{
	quint32	Begin;
	quint32	End;
}
Micromed_New_Marker_Pair;



typedef struct
{
	quint32	Time;            
	quint32	Sample;          
}
Micromed_New_Segment;




typedef struct
{
	unsigned char	Positive;
	unsigned char	Negative;
}
Micromed_New_Impedance;




typedef struct
{
	quint16		Lines;
	quint16		Sectors;
	quint16		Base_Time;
	quint16		Common_Gain;
	quint16		Notch;

	unsigned char			free[502];

	unsigned char			Selection[MAX_DERIV];
	char					Description[64];
	Micromed_Double_Byte	Derivation[MAX_DERIV];
	quint16		L_Filter[MAX_DERIV];
	quint16		H_Filter[MAX_DERIV];
	unsigned char			Reference[MAX_DERIV];
}
Micromed_Montage_2;




typedef struct
{
	quint16		Lines;
	quint16		Sectors;
	quint16		Base_Time;
	quint16		Notch;
	unsigned char			Colour[MAX_DERIV];
	unsigned char			Selection[MAX_DERIV];
	char					Description[64];
	Micromed_Double_Byte	Inputs[MAX_DERIV];
	quint32		HiPass_Filter[MAX_DERIV];
	quint32		LowPass_Filter[MAX_DERIV];
	quint32		Reference[MAX_DERIV];
	unsigned char			ViewReference;
	unsigned char			Free[951];
}
Micromed_Montage_3;	




typedef struct
{
	quint16		Lines;
	quint16		Sectors;
	quint16		Base_Time;
	unsigned char			Notch;
	unsigned char			ViewReference;
	unsigned char			Colour[MAX_DERIV];
	unsigned char			Selection[MAX_DERIV];
	char					Description[64];
	Micromed_Double_Word	Inputs[MAX_DERIV];
	quint32		HiPass_Filter[MAX_DERIV];
	quint32		LowPass_Filter[MAX_DERIV];
	quint32		Reference[MAX_DERIV];

	unsigned char			Free[1720];
}
Micromed_New_Montage;




typedef struct
{
	unsigned char	Undefined[10];	
}
Micromed_New_Compression;



#define AVERAGE_FREE	82
typedef struct
{
	quint32		Mean_Trace;					//	Number of Averaged Traces (Triggers)
	quint32		Mean_File;					//	Number of Averaged Files
	quint32		Mean_Prestim;				//	Pre Stim (msec.)
	quint32		Mean_PostStim;				//	Post Stim (msec.)
	quint32		Mean_Type;					//	0 = Not Weighted, 1 = Weighted
	quint16		Correct_Answers;			//	Number of Sweep with Correct Answers
	quint16      Wrong_Answers;				//	Number of Sweep with Wrong Answers
	quint16		No_Answers;					//	Number of Sweep with No Answer
	float					Corr_Mean_Time;				//
	float					Corr_SD;					//	
	float					Corr_Max_Time;				//
	float					Corr_Min_Time;				//
	float					Corr_Ratio;					//
	unsigned char			Free[AVERAGE_FREE];			//
}
Micromed_New_Average;



typedef struct
{
	unsigned char	Undefined[10];			
}
Micromed_New_Free;



typedef unsigned long int Micromed_New_Sample;



typedef struct
{
	char						Description[64];
	Micromed_New_Marker_Pair	Selection[MAX_EVENT];
}
Micromed_New_Event;


typedef struct
{
	qint32						Sample;
	qint16						Type;
}
Micromed_New_Trigger;


/*******************************************************************
*************** STRUCTURE OF TYPE "0" and "1" FILES ****************
*******************************************************************/

typedef struct
{
	quint16						reserved1[4];
	quint16			Rate_Min;
	quint16			reserved2;
	quint16			Signal;
	quint16			Electrode_Code[32];
	quint16			Filter[32];
	unsigned char				reserved3[32];
	unsigned char				Acquisition_Unit;
	unsigned char				Header_Type;
	quint16			Filetype;
	quint16			LowFilterMultiplier;
	quint16			HighFilterMultiplier;
	quint16			Data_Start_Offset;
	quint16			Num_Chan;
	Micromed_Date_Type			Date;
	Micromed_New_Time_Type		Time;
	Micromed_PatientData		Patient_Data;
	Micromed_New_Marker_Pair	Flag[15];
	Micromed_New_Segment		Segment[15];
	Micromed_Annotation			Note[80];

	unsigned char				free_at_all[2600];
	unsigned char				reserved4[40];
	quint16			Fre_Punt;
	quint16			Fre_Len;
	quint16			Evo_Punt;
	quint16			Evo_Len;
	quint16			Com_Punt;
	quint16			Com_Len;
	quint16			FFT_Punt;
	quint16			FFT_Len;
	Micromed_Electrode			Labels[128];
	quint16			Impedance[32];
	unsigned char				Max_Length[32];
}
Micromed_Header_Type_1;


/*******************************************************************
******************* STRUCTURE OF TYPE "2" FILES ********************
*******************************************************************/

typedef struct
{
	char						Title[32];
	char						Laboratory[32];
	Micromed_New_Patient_Data	Patient_Data;
	Micromed_New_Date_Type		Date;
	Micromed_New_Time_Type		Time;
	quint16						Acquisition_Unit;
	quint16						Filetype;
	quint32						Data_Start_Offset;
	quint16						Num_Chan;
	quint16						Multiplexer;
	quint16						Rate_Min;
	quint16						Bytes;
	quint16						Compression;
	quint16						Montages;
	unsigned char			    Reserved_1[21];

	unsigned char				Header_Type;

	Micromed_New_Descriptor		Code_Area;
	Micromed_New_Descriptor		Electrode_Area;
	Micromed_New_Descriptor		Note_Area;
	Micromed_New_Descriptor		Flag_Area;
	Micromed_New_Descriptor		Segment_Area;
	Micromed_New_Descriptor		B_Impedance_Area;
	Micromed_New_Descriptor		E_Impedance_Area;
	Micromed_New_Descriptor		Montage_Area;
	Micromed_New_Descriptor		Compression_Area;

	unsigned char				Reserved_2[320];
}
Micromed_Header_Type_2;


/*******************************************************************
******************* STRUCTURE OF TYPE "3" FILES ********************
*******************************************************************/

typedef struct
{
	char			   			Title[32];
	char						Laboratory[32];
	Micromed_New_Patient_Data	Patient_Data;
	Micromed_New_Date_Type		Date;
	Micromed_New_Time_Type		Time;
	quint16						Acquisition_Unit;
	quint16	    				Filetype;
	quint32						Data_Start_Offset;
	quint16						Num_Chan;
	quint16		   				Multiplexer;
	quint16		   				Rate_Min;
	quint16						Bytes;
	quint16						Compression;
	quint16		   				Montages;
	quint32						Dvideo_Begin;
	unsigned char			    Reserved_1[17];

	unsigned char			    Header_Type;

	Micromed_New_Descriptor		Code_Area;
	Micromed_New_Descriptor		Electrode_Area;
	Micromed_New_Descriptor		Note_Area;
	Micromed_New_Descriptor		Flag_Area;
	Micromed_New_Descriptor		Segment_Area;
	Micromed_New_Descriptor		B_Impedance_Area;
	Micromed_New_Descriptor		E_Impedance_Area;
	Micromed_New_Descriptor		Montage_Area;
	Micromed_New_Descriptor		Compression_Area;
	Micromed_New_Descriptor		Average_Area;			
	Micromed_New_Descriptor		History_Area;
	Micromed_New_Descriptor		Reserved2;
	Micromed_New_Descriptor		EventA_Area;
	Micromed_New_Descriptor		EventB_Area;

	unsigned char			    Reserved3[240];
}
Micromed_Header_Type_3;


/*******************************************************************
******************* STRUCTURE OF TYPE "4" FILES ********************
*******************************************************************/

typedef struct
{
	char			   			Title[32];
	char						Laboratory[32];
	Micromed_New_Patient_Data	Patient_Data;
	Micromed_New_Date_Type		Date;
	Micromed_New_Time_Type		Time;
	quint16						Acquisition_Unit;
	quint16	   					Filetype;
	quint32						Data_Start_Offset;
	quint16						Num_Chan;
	quint16		   				Multiplexer;
	quint16		   				Rate_Min;
	quint16						Bytes;
	quint16						Compression;
	quint16		   				Montages;
	quint32						Dvideo_Begin;
	quint16						MPEG_Difference;
	unsigned char			    Reserved_1[15];

	unsigned char			    Header_Type;

	Micromed_New_Descriptor		Code_Area;
	Micromed_New_Descriptor		Electrode_Area;
	Micromed_New_Descriptor		Note_Area;
	Micromed_New_Descriptor		Flag_Area;
	Micromed_New_Descriptor		Segment_Area;
	Micromed_New_Descriptor		B_Impedance_Area;
	Micromed_New_Descriptor		E_Impedance_Area;
	Micromed_New_Descriptor		Montage_Area;
	Micromed_New_Descriptor		Compression_Area;
	Micromed_New_Descriptor		Average_Area;			
	Micromed_New_Descriptor		History_Area;
	Micromed_New_Descriptor		Digital_Video_Area;
	Micromed_New_Descriptor		EventA_Area;
	Micromed_New_Descriptor		EventB_Area;
	Micromed_New_Descriptor		Trigger_Area;

	unsigned char			    Reserved_2[224];
}
Micromed_Header_Type_4;



/*******************************************************************
*********** STRUCTURE NECESSARY FOR OTHER MICROMED FILES ***********
*******************************************************************/

typedef struct
{
	unsigned short int		FileType;
	unsigned char			Header_Type;
	unsigned char			Bytes;
	unsigned char			Number_TRC;
	unsigned char			free1;
	unsigned short int		Rate_Min;
	unsigned short int		Signal;
	Micromed_New_Date_Type	Date;
	Micromed_New_Time_Type	Time;
	unsigned char			Segments;
	unsigned long int		Duration;
	unsigned short int		Analyse;
	unsigned short int		Optical_Disk;
}
Micromed_Pazrec;