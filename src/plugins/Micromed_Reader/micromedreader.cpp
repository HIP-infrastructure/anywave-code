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
#include "micromedreader.h"

MicromedReaderPlugin::MicromedReaderPlugin() : AwFileIOPlugin()
{
	name = QString("Micromed Reader");
	description = QString(tr("Open .trc files"));
	version = QString("1.0");
	fileExtensions << "*.trc";
	manufacturer = "Micromed";
	m_flags |=  Aw::HasExtension|Aw::CanRead;
}

MicromedReader::MicromedReader(const QString& filename) : AwFileIO(filename)
{
}

MicromedReader::~MicromedReader()
{
	cleanUpAndClose();
}


void MicromedReader::cleanUpAndClose()
{
	AwFileIO::cleanUpAndClose();
	m_file.close();
}



MicromedReader::FileStatus MicromedReader::canRead(const QString &path)
{
	m_file.setFileName(path);

	if (!m_file.open(QIODevice::ReadOnly))
		return AwFileIO::FileAccess;

	m_stream.setDevice(&m_file);
	m_stream.setByteOrder(QDataStream::LittleEndian);

	m_file.seek(175);
	char type;
	m_stream.readRawData(&type, 1);
	m_file.close();
	m_type = (int)type;
	if (m_type == 1 || m_type == 2 || m_type == 3 || m_type == 4)
		return AwFileIO::NoError;
	return AwFileIO::WrongFormat;
}

//*******************************************************************
//************** SET DESCRIPTORS FOR HEADER OF TYPE "4" *************
//*******************************************************************
void MicromedReader::setDescriptors()
{
	m_Head.Code_Area.Name[0]='O';
	m_Head.Code_Area.Name[1]='R';
	m_Head.Code_Area.Name[2]='D';
	m_Head.Code_Area.Name[3]='E';
	m_Head.Code_Area.Name[4]='R';
	m_Head.Code_Area.Name[5]=' ';
	m_Head.Code_Area.Name[6]=' ';
	m_Head.Code_Area.Name[7]=' ';
	m_Head.Code_Area.Start_Offset=sizeof(Micromed_Header_Type_4);
	m_Head.Code_Area.Length=(unsigned long int)MAX_CAN*sizeof(Micromed_New_Code);

	m_Head.Electrode_Area.Name[0]='L';
	m_Head.Electrode_Area.Name[1]='A';
	m_Head.Electrode_Area.Name[2]='B';
	m_Head.Electrode_Area.Name[3]='C';
	m_Head.Electrode_Area.Name[4]='O';
	m_Head.Electrode_Area.Name[5]='D';
	m_Head.Electrode_Area.Name[6]=' ';
	m_Head.Electrode_Area.Name[7]=' ';
	m_Head.Electrode_Area.Start_Offset=m_Head.Code_Area.Start_Offset+m_Head.Code_Area.Length;
	m_Head.Electrode_Area.Length=(unsigned long int)MAX_LAB*sizeof(Micromed_New_Electrode);

	m_Head.Note_Area.Name[0]='N';
	m_Head.Note_Area.Name[1]='O';
	m_Head.Note_Area.Name[2]='T';
	m_Head.Note_Area.Name[3]='E';
	m_Head.Note_Area.Name[4]=' ';
	m_Head.Note_Area.Name[5]=' ';
	m_Head.Note_Area.Name[6]=' ';
	m_Head.Note_Area.Name[7]=' ';
	m_Head.Note_Area.Start_Offset=m_Head.Electrode_Area.Start_Offset+m_Head.Electrode_Area.Length;
	m_Head.Note_Area.Length=(unsigned long int)MAX_NOTE*sizeof(Micromed_New_Annotation);

	m_Head.Flag_Area.Name[0]='F';
	m_Head.Flag_Area.Name[1]='L';
	m_Head.Flag_Area.Name[2]='A';
	m_Head.Flag_Area.Name[3]='G';
	m_Head.Flag_Area.Name[4]='S';
	m_Head.Flag_Area.Name[5]=' ';
	m_Head.Flag_Area.Name[6]=' ';
	m_Head.Flag_Area.Name[7]=' ';
	m_Head.Flag_Area.Start_Offset=m_Head.Note_Area.Start_Offset+m_Head.Note_Area.Length;
	m_Head.Flag_Area.Length=(unsigned long int)MAX_FLAG*sizeof(Micromed_New_Marker_Pair);

	m_Head.Segment_Area.Name[0]='T';
	m_Head.Segment_Area.Name[1]='R';
	m_Head.Segment_Area.Name[2]='O';
	m_Head.Segment_Area.Name[3]='N';
	m_Head.Segment_Area.Name[4]='C';
	m_Head.Segment_Area.Name[5]='A';
	m_Head.Segment_Area.Name[6]=' ';
	m_Head.Segment_Area.Name[7]=' ';
	m_Head.Segment_Area.Start_Offset=m_Head.Flag_Area.Start_Offset+m_Head.Flag_Area.Length;
	m_Head.Segment_Area.Length=(unsigned long int)MAX_SEGM*sizeof(Micromed_New_Segment);
	
	m_Head.B_Impedance_Area.Name[0]='I';
	m_Head.B_Impedance_Area.Name[1]='M';
	m_Head.B_Impedance_Area.Name[2]='P';
	m_Head.B_Impedance_Area.Name[3]='E';
	m_Head.B_Impedance_Area.Name[4]='D';
	m_Head.B_Impedance_Area.Name[5]='_';
	m_Head.B_Impedance_Area.Name[6]='B';
	m_Head.B_Impedance_Area.Name[7]=' ';
	m_Head.B_Impedance_Area.Start_Offset=m_Head.Segment_Area.Start_Offset+m_Head.Segment_Area.Length;
	m_Head.B_Impedance_Area.Length=(unsigned long int)MAX_CAN*sizeof(Micromed_New_Impedance);
	
	m_Head.E_Impedance_Area.Name[0]='I';
	m_Head.E_Impedance_Area.Name[1]='M';
	m_Head.E_Impedance_Area.Name[2]='P';
	m_Head.E_Impedance_Area.Name[3]='E';
	m_Head.E_Impedance_Area.Name[4]='D';
	m_Head.E_Impedance_Area.Name[5]='_';
	m_Head.E_Impedance_Area.Name[6]='E';
	m_Head.E_Impedance_Area.Name[7]=' ';
	m_Head.E_Impedance_Area.Start_Offset=m_Head.B_Impedance_Area.Start_Offset+m_Head.B_Impedance_Area.Length;
	m_Head.E_Impedance_Area.Length=(unsigned long int)MAX_CAN*sizeof(Micromed_New_Impedance);

	m_Head.Montage_Area.Name[0]='M';
	m_Head.Montage_Area.Name[1]='O';
	m_Head.Montage_Area.Name[2]='N';
	m_Head.Montage_Area.Name[3]='T';
	m_Head.Montage_Area.Name[4]='A';
	m_Head.Montage_Area.Name[5]='G';
	m_Head.Montage_Area.Name[6]='E';
	m_Head.Montage_Area.Name[7]=' ';
	m_Head.Montage_Area.Start_Offset=m_Head.E_Impedance_Area.Start_Offset+m_Head.E_Impedance_Area.Length;
	m_Head.Montage_Area.Length=(unsigned long int)MAX_MONT*sizeof(Micromed_New_Montage);

	m_Head.Compression_Area.Name[0]='C';
	m_Head.Compression_Area.Name[1]='O';
	m_Head.Compression_Area.Name[2]='M';
	m_Head.Compression_Area.Name[3]='P';
	m_Head.Compression_Area.Name[4]='R';
	m_Head.Compression_Area.Name[5]='E';
	m_Head.Compression_Area.Name[6]='S';
	m_Head.Compression_Area.Name[7]='S';
	m_Head.Compression_Area.Start_Offset=m_Head.Montage_Area.Start_Offset+m_Head.Montage_Area.Length;
	m_Head.Compression_Area.Length=(unsigned long int)sizeof(Micromed_New_Compression);
	
	m_Head.Average_Area.Name[0]='A';
	m_Head.Average_Area.Name[1]='V';
	m_Head.Average_Area.Name[2]='E';
	m_Head.Average_Area.Name[3]='R';
	m_Head.Average_Area.Name[4]='A';
	m_Head.Average_Area.Name[5]='G';
	m_Head.Average_Area.Name[6]='E';
	m_Head.Average_Area.Name[7]=' ';
	m_Head.Average_Area.Start_Offset=m_Head.Compression_Area.Start_Offset+m_Head.Compression_Area.Length;
	m_Head.Average_Area.Length=sizeof(Micromed_New_Average);

	m_Head.History_Area.Name[0]='H';
	m_Head.History_Area.Name[1]='I';
	m_Head.History_Area.Name[2]='S';
	m_Head.History_Area.Name[3]='T';
	m_Head.History_Area.Name[4]='O';
	m_Head.History_Area.Name[5]='R';
	m_Head.History_Area.Name[6]='Y';
	m_Head.History_Area.Name[7]=' ';
	m_Head.History_Area.Start_Offset=m_Head.Average_Area.Start_Offset+m_Head.Average_Area.Length;
	m_Head.History_Area.Length=(unsigned long int)MAX_SAMPLE*sizeof(Micromed_New_Sample)+(unsigned long int)MAX_HISTORY*sizeof(Micromed_New_Montage);

	m_Head.Digital_Video_Area.Name[0]='D';
	m_Head.Digital_Video_Area.Name[1]='V';
	m_Head.Digital_Video_Area.Name[2]='I';
	m_Head.Digital_Video_Area.Name[3]='D';
	m_Head.Digital_Video_Area.Name[4]='E';
	m_Head.Digital_Video_Area.Name[5]='O';
	m_Head.Digital_Video_Area.Name[6]=' ';
	m_Head.Digital_Video_Area.Name[7]=' ';
	m_Head.Digital_Video_Area.Start_Offset=m_Head.History_Area.Start_Offset+m_Head.History_Area.Length;
	m_Head.Digital_Video_Area.Length=(unsigned long int)MAX_VIDEO_FILE*sizeof(Micromed_New_Sample);

	m_Head.EventA_Area.Name[0]='E';
	m_Head.EventA_Area.Name[1]='V';
	m_Head.EventA_Area.Name[2]='E';
	m_Head.EventA_Area.Name[3]='N';
	m_Head.EventA_Area.Name[4]='T';
	m_Head.EventA_Area.Name[5]=' ';
	m_Head.EventA_Area.Name[6]='A';
	m_Head.EventA_Area.Name[7]=' ';
	m_Head.EventA_Area.Start_Offset=m_Head.Digital_Video_Area.Start_Offset+m_Head.Digital_Video_Area.Length;
	m_Head.EventA_Area.Length=(unsigned long int)sizeof(Micromed_New_Event);

	m_Head.EventB_Area.Name[0]='E';
	m_Head.EventB_Area.Name[1]='V';
	m_Head.EventB_Area.Name[2]='E';
	m_Head.EventB_Area.Name[3]='N';
	m_Head.EventB_Area.Name[4]='T';
	m_Head.EventB_Area.Name[5]=' ';
	m_Head.EventB_Area.Name[6]='B';
	m_Head.EventB_Area.Name[7]=' ';
	m_Head.EventB_Area.Start_Offset=m_Head.EventA_Area.Start_Offset+m_Head.EventA_Area.Length;
	m_Head.EventB_Area.Length=(unsigned long int)sizeof(Micromed_New_Event);
	
	m_Head.Trigger_Area.Name[0]='T';
	m_Head.Trigger_Area.Name[1]='R';
	m_Head.Trigger_Area.Name[2]='I';
	m_Head.Trigger_Area.Name[3]='G';
	m_Head.Trigger_Area.Name[4]='G';
	m_Head.Trigger_Area.Name[5]='E';
	m_Head.Trigger_Area.Name[6]='R';
	m_Head.Trigger_Area.Name[7]=' ';
	m_Head.Trigger_Area.Start_Offset=m_Head.EventB_Area.Start_Offset+m_Head.EventB_Area.Length;
	m_Head.Trigger_Area.Length=(unsigned long int)MAX_TRIGGER*sizeof(Micromed_New_Trigger);
}

int MicromedReader::readHead0()
{
	unsigned char code_std[]={174,1,2,5,3,7,4,6,16,8,10,9,17,18,11,13,12,19,14,15,171,160,167,164};	

	int conv_label[]={0,1,2,3,4,5,6,7,8,9,10,11,12 \
		,13,14,15,16,17,18,19,28,29,30,31,27,27 \
		,27,27,27,27,27,27			     \
		,27,27,27,27,27,27,27,27,27,27,27,27,27 \
		,27,27,27,27,27,27,27,27,27,27,27,27,27 \
		,27,27,27,27,27,27			     \
		,32,34,36,38,40,42,44,46,48,50,52,54,62 \
		,62,60,62,62,62,62,62,62,62,62,62,62,62 \
		,62};

	int conv_Code[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 \
		,16,17,18,19,171,160,167,164,169,174,255,255,20,21,22,23 \
		,160,160,161,161,162,162,163,163,164,164,165,165,166,166,167,167 \
		,168,168,169,169,170,170,171,171,255,255,255,255,174,174,255,255 \
		,63,32,34,36,38,35,39,37,41,43,42,46,48,47,50,52 \
		,40,44,45,49,62,61,60,59,58,57,56,55,33,51,54,53 \
		,176,176,177,177,178,178,179,179,180,180,181,181,255,255,182,182 \
		,183,183,184,184,185,185,255,255,255,255,255,255,186,186,255,255};

	Micromed_Electrode Fix_Labels[]={{{'G','2',' ',' ',' '},0,800} \
		,{{'F','p','1',' ',' '},0,800} \
		,{{'F','p','2',' ',' '},0,800} \
		,{{'F','3',' ',' ',' '},0,800} \
		,{{'F','4',' ',' ',' '},0,800} \
		,{{'F','7',' ',' ',' '},0,800} \
		,{{'F','8',' ',' ',' '},0,800} \
		,{{'F','z',' ',' ',' '},0,800} \
		,{{'C','3',' ',' ',' '},0,800} \
		,{{'C','4',' ',' ',' '},0,800} \
		,{{'C','z',' ',' ',' '},0,800} \
		,{{'P','3',' ',' ',' '},0,800} \
		,{{'P','4',' ',' ',' '},0,800} \
		,{{'P','z',' ',' ',' '},0,800} \
		,{{'O','1',' ',' ',' '},0,800} \
		,{{'O','2',' ',' ',' '},0,800} \
		,{{'T','3',' ',' ',' '},0,800} \
		,{{'T','4',' ',' ',' '},0,800} \
		,{{'T','5',' ',' ',' '},0,800} \
		,{{'T','6',' ',' ',' '},0,800} \
		,{{'E','O','G',' ',' '},0,800} \
		,{{'E','M','G',' ',' '},0,800} \
		,{{'E','C','G',' ',' '},0,6400} \
		,{{'P','N','G',' ',' '},0,800} \
		,{{'A','U','X',' ',' '},0,4000+16384} \
		,{{'M','K','R',' ',' '},0,800} \
		,{{'G','N','D',' ',' '},0,800} \
		,{{'.','.','.','.','.'},0,800} \
		,{{'F','p','z',' ',' '},0,800} \
		,{{'O','z',' ',' ',' '},0,800} \
		,{{'A','2',' ',' ',' '},0,800} \
		,{{'A','1',' ',' ',' '},0,800} \
		,{{'E','M','G','1','+'},0,800} \
		,{{'E','M','G','1','-'},0,800} \
		,{{'E','M','G','2','+'},0,800} \
		,{{'E','M','G','2','-'},0,800} \
		,{{'E','M','G','3','+'},0,800} \
		,{{'E','M','G','3','-'},0,800} \
		,{{'E','M','G','4','+'},0,800} \
		,{{'E','M','G','4','-'},0,800} \
		,{{'P','N','G','1','+'},0,800} \
		,{{'P','N','G','1','-'},0,800} \
		,{{'P','N','G','2','+'},0,800} \
		,{{'P','N','G','2','-'},0,800} \
		,{{'P','N','G','3','+'},0,800} \
		,{{'P','N','G','3','-'},0,800} \
		,{{'E','C','G','1','+'},0,6400} \
		,{{'E','C','G','1','-'},0,6400} \
		,{{'E','C','G','2','+'},0,6400} \
		,{{'E','C','G','2','-'},0,6400} \
		,{{'D','C','1','+',' '},0,4000+16384} \
		,{{'D','C','1','+',' '},0,4000+16384} \
		,{{'D','C','2','+',' '},0,4000+16384} \
		,{{'D','C','2','+',' '},0,4000+16384} \
		,{{'E','O','G','1','+'},0,800} \
		,{{'E','O','G','1','-'},0,800} \
		,{{'.','.','.','.','.'},0,800} \
		,{{'.','.','.','.','.'},0,800} \
		,{{'.','.','.','.','.'},0,800} \
		,{{'.','.','.','.','.'},0,800} \
		,{{'M','K','R','+',' '},0,800} \
		,{{'M','K','R','-',' '},0,800} \
		,{{'.','.','.','.','.'},0,800} \
		,{{'.','.','.','.','.'},0,800}};

	Micromed_Header_Type_1 Head_Old;
	int i, j;

	m_file.seek(0);
	m_stream.skipRawData(8);
	m_stream >> Head_Old.Rate_Min;
	m_stream.skipRawData(2);
	m_stream >> Head_Old.Signal;
	m_stream.readRawData((char *)&Head_Old.Electrode_Code, 64);
	m_stream.readRawData((char *)&Head_Old.Filter, 64);
	m_stream.skipRawData(32);
	m_stream.readRawData((char *)&Head_Old.Acquisition_Unit, 1);	
	m_stream.readRawData((char *)&Head_Old.Header_Type, 1);
	m_stream >> Head_Old.Filetype >> Head_Old.LowFilterMultiplier >> Head_Old.HighFilterMultiplier;
	m_stream >> Head_Old.Data_Start_Offset >> Head_Old.Num_Chan;
	m_stream.readRawData((char *)&Head_Old.Date, 3);
	m_stream.readRawData((char *)&Head_Old.Time, 3);
	m_stream.readRawData((char *)&Head_Old.Patient_Data, 48);
	m_stream.readRawData((char *)&Head_Old.Flag, 15 * 8);
	m_stream.readRawData((char *)&Head_Old.Segment, 15 * 8);
	for (i = 0; i < 80; i++)
	{
		m_stream >> Head_Old.Note[i].Time;
		m_stream.readRawData((char *)&Head_Old.Note[i].Comment, 40);
	}
	// skip non usefull data
	m_stream.skipRawData(2656);
	// electrodes
	for (i = 0; i < 128; i++)
	{
		m_stream.readRawData((char *)&Head_Old.Labels[i].Name, 5);
		m_stream.skipRawData(1);
		m_stream >> Head_Old.Labels[i].Signal_Max;
	}
	// skip impedances
	m_stream.skipRawData(64);
	m_stream.readRawData((char *)&Head_Old.Max_Length, 32);

	for (i = 0; i < 31; i++)
		m_Head.Title[i] = ' ';
	m_Head.Title[31] = 0;

	for (i = 0; i < 31; i++)
		m_Head.Laboratory[i] = ' ';
	m_Head.Laboratory[31] = 0;

	for (i = 0; i < 22; i++)
		m_Head.Patient_Data.Surname[i] = Head_Old.Patient_Data.Surname[i];
	for (i = 0; i > 20; i++)
		m_Head.Patient_Data.Name[i] = Head_Old.Patient_Data.Name[i];

	m_Head.Patient_Data.Day = Head_Old.Patient_Data.Day;
	m_Head.Patient_Data.Month = Head_Old.Patient_Data.Month;
	m_Head.Patient_Data.Year = Head_Old.Patient_Data.Year;

	m_Head.Date.Day = Head_Old.Date.Day;
	m_Head.Date.Month = Head_Old.Date.Month;
	m_Head.Date.Year = Head_Old.Date.Year;
	m_Head.Time.Hour = Head_Old.Time.Hour;
	m_Head.Time.Min = Head_Old.Time.Min;
	m_Head.Time.Sec = Head_Old.Time.Sec;

	m_Head.Acquisition_Unit = Head_Old.Acquisition_Unit;

	m_Head.Filetype = Head_Old.Filetype;

	m_Head.Data_Start_Offset = (unsigned long int)Head_Old.Data_Start_Offset;

	m_Head.Num_Chan = Head_Old.Num_Chan;

	m_Head.Multiplexer = Head_Old.Num_Chan;

	m_Head.Rate_Min = Head_Old.Rate_Min;

	m_Head.Bytes = 1;

	m_Head.Compression = 0;

	m_Head.Montages = 0;

	m_Head.Header_Type = Head_Old.Header_Type;

	setDescriptors();

//************** Set "Electrode" Structure *************************************************
	for (i=0; i<64; i++)
	{
		m_Electrode[i].Status=0;
		m_Electrode[i].Type=0;
		m_Electrode[i].Positive_Input_Label[0]=Fix_Labels[conv_label[i]].Name[0];
		m_Electrode[i].Positive_Input_Label[1]=Fix_Labels[conv_label[i]].Name[1];
		m_Electrode[i].Positive_Input_Label[2]=Fix_Labels[conv_label[i]].Name[2];
		m_Electrode[i].Positive_Input_Label[3]=Fix_Labels[conv_label[i]].Name[3];
		m_Electrode[i].Positive_Input_Label[4]=Fix_Labels[conv_label[i]].Name[4];
		m_Electrode[i].Positive_Input_Label[5]=0;
		m_Electrode[i].Negative_Input_Label[0]='G';
		m_Electrode[i].Negative_Input_Label[1]='2';
		m_Electrode[i].Negative_Input_Label[2]=' ';
		m_Electrode[i].Negative_Input_Label[3]=' ';
		m_Electrode[i].Negative_Input_Label[4]=' ';
		m_Electrode[i].Negative_Input_Label[5]=0;
		m_Electrode[i].Logic_Minimum=(unsigned long)0;
		m_Electrode[i].Logic_Maximum=(unsigned long)255;
		m_Electrode[i].Logic_Ground=(unsigned long)128;
		m_Electrode[i].Physic_Minimum=(-1)*(long)(Fix_Labels[conv_label[i]].Signal_Max & 16383)/(2*(800/Head_Old.Signal));
		m_Electrode[i].Physic_Maximum=(long)(Fix_Labels[conv_label[i]].Signal_Max & 16383)/(2*(800/Head_Old.Signal));
		m_Electrode[i].Measurement_Unit=(unsigned short int)Fix_Labels[conv_label[i]].Signal_Max>>14;
		m_Electrode[i].Prefiltering_HiPass_Limit=333;
		m_Electrode[i].Prefiltering_HiPass_Type=1;
		m_Electrode[i].Prefiltering_LowPass_Limit=Head_Old.Rate_Min/2;
		m_Electrode[i].Prefiltering_LowPass_Type=2;
		m_Electrode[i].Rate_Coefficient=1;
		m_Electrode[i].Latitude=0.0;
		m_Electrode[i].Longitude=0.0;
		m_Electrode[i].Maps=0;
		m_Electrode[i].Average_Ref=0;
		for (j=0; j<31; j++) m_Electrode[i].Description[j]=' ';
	    m_Electrode[i].Description[31]=0;
	}
	for (i=64; i<160; i++)
	{
		m_Electrode[i].Status=0;
		m_Electrode[i].Type=0;
		m_Electrode[i].Positive_Input_Label[0]='.';
		m_Electrode[i].Positive_Input_Label[1]='.';
		m_Electrode[i].Positive_Input_Label[2]='.';
		m_Electrode[i].Positive_Input_Label[3]='.';
		m_Electrode[i].Positive_Input_Label[4]='.';
		m_Electrode[i].Positive_Input_Label[5]=0;
		m_Electrode[i].Negative_Input_Label[0]='.';
		m_Electrode[i].Negative_Input_Label[1]='.';
		m_Electrode[i].Negative_Input_Label[2]='.';
		m_Electrode[i].Negative_Input_Label[3]='.';
		m_Electrode[i].Negative_Input_Label[4]='.';
		m_Electrode[i].Negative_Input_Label[5]=0;
		m_Electrode[i].Logic_Minimum=(unsigned long)0;
		m_Electrode[i].Logic_Maximum=(unsigned long)255;
		m_Electrode[i].Logic_Ground=(unsigned long)128;
		m_Electrode[i].Physic_Minimum=-400;
		m_Electrode[i].Physic_Maximum=400;
		m_Electrode[i].Measurement_Unit=0; 
		m_Electrode[i].Prefiltering_HiPass_Limit=333;
		m_Electrode[i].Prefiltering_HiPass_Type=1;
		m_Electrode[i].Prefiltering_LowPass_Limit=Head_Old.Rate_Min/2;
		m_Electrode[i].Prefiltering_LowPass_Type=2;
		m_Electrode[i].Rate_Coefficient=1;
		m_Electrode[i].Latitude=0.0;
		m_Electrode[i].Longitude=0.0;
		m_Electrode[i].Maps=0;
		m_Electrode[i].Average_Ref=0;
		for (j=0; j<31; j++) m_Electrode[i].Description[j]=' ';
	    m_Electrode[i].Description[31]=0;
	}	

	for (i=160; i<187; i++)
	{
		m_Electrode[i].Status=0;
		m_Electrode[i].Type=1;
		m_Electrode[i].Positive_Input_Label[0]=Fix_Labels[conv_label[i-96]].Name[0];
		m_Electrode[i].Positive_Input_Label[1]=Fix_Labels[conv_label[i-96]].Name[1];
		m_Electrode[i].Positive_Input_Label[2]=Fix_Labels[conv_label[i-96]].Name[2];
		m_Electrode[i].Positive_Input_Label[3]=Fix_Labels[conv_label[i-96]].Name[3];
		m_Electrode[i].Positive_Input_Label[4]=Fix_Labels[conv_label[i-96]].Name[4];
		m_Electrode[i].Positive_Input_Label[5]=0;
		m_Electrode[i].Negative_Input_Label[0]=Fix_Labels[conv_label[i-96]+1].Name[0];
		m_Electrode[i].Negative_Input_Label[1]=Fix_Labels[conv_label[i-96]+1].Name[1];
		m_Electrode[i].Negative_Input_Label[2]=Fix_Labels[conv_label[i-96]+1].Name[2];
		m_Electrode[i].Negative_Input_Label[3]=Fix_Labels[conv_label[i-96]+1].Name[3];
		m_Electrode[i].Negative_Input_Label[4]=Fix_Labels[conv_label[i-96]+1].Name[4];
		m_Electrode[i].Negative_Input_Label[5]=0;
		m_Electrode[i].Logic_Minimum=(unsigned long)0;
		m_Electrode[i].Logic_Maximum=(unsigned long)255;
		m_Electrode[i].Logic_Ground=(unsigned long)128;
		m_Electrode[i].Physic_Minimum=(-1)*(long)(Fix_Labels[conv_label[i-96]].Signal_Max & 16383)/(2*(800/Head_Old.Signal));
		m_Electrode[i].Physic_Maximum=(long)(Fix_Labels[conv_label[i-96]].Signal_Max & 16383)/(2*(800/Head_Old.Signal));
		m_Electrode[i].Measurement_Unit=(unsigned short int)Fix_Labels[conv_label[i-96]].Signal_Max>>14;
		m_Electrode[i].Prefiltering_HiPass_Limit=333;
		m_Electrode[i].Prefiltering_HiPass_Type=1;
		m_Electrode[i].Prefiltering_LowPass_Limit=Head_Old.Rate_Min/2;
		m_Electrode[i].Prefiltering_LowPass_Type=2;
		m_Electrode[i].Rate_Coefficient=1;
		m_Electrode[i].Latitude=0.0;
		m_Electrode[i].Longitude=0.0;
		m_Electrode[i].Maps=0;
		m_Electrode[i].Average_Ref=0;
		for (j=0; j<31; j++) m_Electrode[i].Description[j]=' ';
		m_Electrode[i].Description[31]=0;
	}
	for (i=187; i<640; i++)
	{
		m_Electrode[i].Status=0;
		m_Electrode[i].Type=1;
		m_Electrode[i].Positive_Input_Label[0]='.';
		m_Electrode[i].Positive_Input_Label[1]='.';
		m_Electrode[i].Positive_Input_Label[2]='.';
		m_Electrode[i].Positive_Input_Label[3]='.';
		m_Electrode[i].Positive_Input_Label[4]='.';
		m_Electrode[i].Positive_Input_Label[5]=0;
		m_Electrode[i].Negative_Input_Label[0]='.';
		m_Electrode[i].Negative_Input_Label[1]='.';
		m_Electrode[i].Negative_Input_Label[2]='.';
		m_Electrode[i].Negative_Input_Label[3]='.';
		m_Electrode[i].Negative_Input_Label[4]='.';
		m_Electrode[i].Negative_Input_Label[5]=0;
		m_Electrode[i].Logic_Minimum=(unsigned long)0;
		m_Electrode[i].Logic_Maximum=(unsigned long)255;
		m_Electrode[i].Logic_Ground=(unsigned long)128;
		m_Electrode[i].Physic_Minimum=-400;
		m_Electrode[i].Physic_Maximum=400;
		m_Electrode[i].Measurement_Unit=0; 
		m_Electrode[i].Prefiltering_HiPass_Limit=333;
		m_Electrode[i].Prefiltering_HiPass_Type=1;
		m_Electrode[i].Prefiltering_LowPass_Limit=Head_Old.Rate_Min/2;
		m_Electrode[i].Prefiltering_LowPass_Type=2;
		m_Electrode[i].Rate_Coefficient=1;
		m_Electrode[i].Latitude=0.0;
		m_Electrode[i].Longitude=0.0;
		m_Electrode[i].Maps=0;
		m_Electrode[i].Average_Ref=0;
		for (j=0; j<31; j++) m_Electrode[i].Description[j]=' ';
	    m_Electrode[i].Description[31]=0;
	}
//************** Set "Code" Structure *****************************************************	
	for (i=0; i<MAX_CAN; i++) m_Code[i]=0;
	if (m_Head.Filetype==64)
	{
		for (i=0; i<m_Head.Num_Chan; i++) m_Code[i]=code_std[i];
	}
	else
	{
		for (i=0; i<m_Head.Num_Chan; i++)
		{
			if (((Head_Old.Electrode_Code[i]>>8)==0) || ((Head_Old.Electrode_Code[i]>>8)==26))
				m_Code[i]=conv_Code[(Head_Old.Electrode_Code[i] & 255)];
			else
			{
				if ((Head_Old.Electrode_Code[i]>>8)<32)
					m_Code[i]=conv_Code[(Head_Old.Electrode_Code[i]>>8)];
				else
					m_Code[i]=conv_Code[(Head_Old.Electrode_Code[i] & 254)];
			}
		}
	}

	//************** Set "Note" Structure *****************************************************
	for (i=0; i<80; i++)
	{
		m_Note[i].Sample=Head_Old.Note[i].Time * Head_Old.Rate_Min;
		for (j=0; j<20; j++) m_Note[i].Comment[j]=Head_Old.Note[i].Comment[j];
		for (j=20; j<40; j++) m_Note[i].Comment[j]=' ';
    }	
	for (i=80; i<MAX_NOTE; i++)
    {
		m_Note[i].Sample=0;
		for (j=0; j<40; j++) m_Note[i].Comment[j]=' ';
    }

	//************** Set "Flag" Structure *****************************************************
	for (i=0; i<15; i++)
	{
		m_Flag[i].Begin=Head_Old.Flag[i].Begin;
		m_Flag[i].End=Head_Old.Flag[i].End;
    }
	for (i=15; i<MAX_FLAG; i++)
	{
		m_Flag[i].Begin=0;
		m_Flag[i].End=0;
	}


//************** Set "EventA" and "EventB" Structure ***************************************
	m_EventA.Description[0]=0;													//
	m_EventB.Description[0]=0;													//
	for (i=0; i<MAX_EVENT; i++)													//
	{																			//	reset 
		m_EventA.Selection[i].Begin=0;											//	EventA
		m_EventB.Selection[i].Begin=0;											//	EventB
		m_EventA.Selection[i].End=0;												//
		m_EventB.Selection[i].End=0;												//
	}																			//


//************** Set "Trigger" Structure ***************************************************
	for (i=0; i<MAX_TRIGGER; i++)												//
	{																			//	reset 
		m_Trigger[i].Sample=0;													//	Trigger
		m_Trigger[i].Type=0;														//
	}		
	
	return 0;
}

int MicromedReader::readHead1()
{
	int conv_label[]={0,1,2,3,4,5,6,7,8,9,10,11,12 \
		,13,14,15,16,17,18,19,28,29,30,31,27,27 \
		,27,27,27,27,27,27			     \
		,65,92,66,69,67,71,68,70,80,72,74,73,81 \
		,82,75,77,76,83,78,93,79,95,94,91,90,89 \
		,88,87,86,85,84,64			     \
		,32,34,36,38,40,42,44,46,48,50,52,54,62 \
		,62,60,62,96,98,100,102,104,106,110,112,114,116 \
		,124};

	int conv_Code[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 \
		,16,17,18,19,171,160,167,164,169,174,255,255,20,21,22,23 \
		,160,160,161,161,162,162,163,163,164,164,165,165,166,166,167,167 \
		,168,168,169,169,170,170,171,171,255,255,255,255,174,174,255,255 \
		,63,32,34,36,38,35,39,37,41,43,42,46,48,47,50,52 \
		,40,44,45,49,62,61,60,59,58,57,56,55,33,51,54,53 \
		,176,176,177,177,178,178,179,179,180,180,181,181,255,255,182,182 \
		,183,183,184,184,185,185,255,255,255,255,255,255,186,186,255,255};

	int i,j;

	Micromed_Header_Type_1 Head_Old;
	
	m_file.seek(0);
	m_stream.skipRawData(8);
	m_stream >> Head_Old.Rate_Min;
	m_stream.skipRawData(2);
	m_stream >> Head_Old.Signal;
	m_stream.readRawData((char *)&Head_Old.Electrode_Code, 64);
	m_stream.readRawData((char *)&Head_Old.Filter, 64);
	m_stream.skipRawData(32);
	m_stream.readRawData((char *)&Head_Old.Acquisition_Unit, 1);	
	m_stream.readRawData((char *)&Head_Old.Header_Type, 1);
	m_stream >> Head_Old.Filetype >> Head_Old.LowFilterMultiplier >> Head_Old.HighFilterMultiplier;
	m_stream >> Head_Old.Data_Start_Offset >> Head_Old.Num_Chan;
	m_stream.readRawData((char *)&Head_Old.Date, 3);
	m_stream.readRawData((char *)&Head_Old.Time, 3);
	m_stream.readRawData((char *)&Head_Old.Patient_Data, 48);
	m_stream.readRawData((char *)&Head_Old.Flag, 15 * 8);
	m_stream.readRawData((char *)&Head_Old.Segment, 15 * 8);
	for (i = 0; i < 80; i++)
	{
		m_stream >> Head_Old.Note[i].Time;
		m_stream.readRawData((char *)&Head_Old.Note[i].Comment, 40);
	}
	// skip non usefull data
	m_stream.skipRawData(2656);
	// electrodes
	for (i = 0; i < 128; i++)
	{
		m_stream.readRawData((char *)&Head_Old.Labels[i].Name, 5);
		m_stream.skipRawData(1);
		m_stream >> Head_Old.Labels[i].Signal_Max;
	}
	// skip impedances
	m_stream.skipRawData(64);
	m_stream.readRawData((char *)&Head_Old.Max_Length, 32);

	for (i = 0; i < 31; i++)
		m_Head.Title[i] = ' ';
	m_Head.Title[31] = 0;

	for (i = 0; i < 31; i++)
		m_Head.Laboratory[i] = ' ';
	m_Head.Laboratory[31] = 0;

	for (i = 0; i < 22; i++)
		m_Head.Patient_Data.Surname[i] = Head_Old.Patient_Data.Surname[i];
	for (i = 0; i > 20; i++)
		m_Head.Patient_Data.Name[i] = Head_Old.Patient_Data.Name[i];

	m_Head.Patient_Data.Day = Head_Old.Patient_Data.Day;
	m_Head.Patient_Data.Month = Head_Old.Patient_Data.Month;
	m_Head.Patient_Data.Year = Head_Old.Patient_Data.Year;

	m_Head.Date.Day = Head_Old.Date.Day;
	m_Head.Date.Month = Head_Old.Date.Month;
	m_Head.Date.Year = Head_Old.Date.Year;
	m_Head.Time.Hour = Head_Old.Time.Hour;
	m_Head.Time.Min = Head_Old.Time.Min;
	m_Head.Time.Sec = Head_Old.Time.Sec;

	m_Head.Acquisition_Unit = Head_Old.Acquisition_Unit;

	m_Head.Filetype = Head_Old.Filetype;

	m_Head.Data_Start_Offset = (unsigned long int)Head_Old.Data_Start_Offset;

	m_Head.Num_Chan = Head_Old.Num_Chan;

	m_Head.Multiplexer = Head_Old.Num_Chan;

	m_Head.Rate_Min = Head_Old.Rate_Min;

	m_Head.Bytes = 1;

	m_Head.Compression = 0;

	m_Head.Montages = 0;

	m_Head.Header_Type = Head_Old.Header_Type;

	setDescriptors();

//************** Set "Electrode" Structure *************************************************
	for (i=0; i<64; i++)
	{
		m_Electrode[i].Status=0;
		m_Electrode[i].Type=0;
		m_Electrode[i].Positive_Input_Label[0]=Head_Old.Labels[conv_label[i]].Name[0];
		m_Electrode[i].Positive_Input_Label[1]=Head_Old.Labels[conv_label[i]].Name[1];
		m_Electrode[i].Positive_Input_Label[2]=Head_Old.Labels[conv_label[i]].Name[2];
		m_Electrode[i].Positive_Input_Label[3]=Head_Old.Labels[conv_label[i]].Name[3];
		m_Electrode[i].Positive_Input_Label[4]=Head_Old.Labels[conv_label[i]].Name[4];
		m_Electrode[i].Positive_Input_Label[5]=0;
		m_Electrode[i].Negative_Input_Label[0]='G';
		m_Electrode[i].Negative_Input_Label[1]='2';
		m_Electrode[i].Negative_Input_Label[2]=' ';
		m_Electrode[i].Negative_Input_Label[3]=' ';
		m_Electrode[i].Negative_Input_Label[4]=' ';
		m_Electrode[i].Negative_Input_Label[5]=0;
		m_Electrode[i].Logic_Minimum=(unsigned long)0;
		m_Electrode[i].Logic_Maximum=(unsigned long)255;
		m_Electrode[i].Logic_Ground=(unsigned long)128;
		m_Electrode[i].Physic_Minimum=(-1)*(long)(Head_Old.Labels[conv_label[i]].Signal_Max & 16383)/(2*(800/Head_Old.Signal));
		m_Electrode[i].Physic_Maximum=(long)(Head_Old.Labels[conv_label[i]].Signal_Max & 16383)/(2*(800/Head_Old.Signal));
		m_Electrode[i].Measurement_Unit=(unsigned short int)Head_Old.Labels[conv_label[i]].Signal_Max>>14;
		m_Electrode[i].Prefiltering_HiPass_Limit=333;
		m_Electrode[i].Prefiltering_HiPass_Type=1;
		m_Electrode[i].Prefiltering_LowPass_Limit=Head_Old.Rate_Min/2;
		m_Electrode[i].Prefiltering_LowPass_Type=2;
		m_Electrode[i].Rate_Coefficient=1;
		m_Electrode[i].Latitude=0.0;
		m_Electrode[i].Longitude=0.0;
		m_Electrode[i].Maps=0;
		m_Electrode[i].Average_Ref=0;
		for (j=0; j<31; j++) m_Electrode[i].Description[j]=' ';
		m_Electrode[i].Description[31]=0;
	}
	for (i=64; i<160; i++)
	{
		m_Electrode[i].Status=0;
		m_Electrode[i].Type=0;
		m_Electrode[i].Positive_Input_Label[0]='.';
		m_Electrode[i].Positive_Input_Label[1]='.';
		m_Electrode[i].Positive_Input_Label[2]='.';
		m_Electrode[i].Positive_Input_Label[3]='.';
		m_Electrode[i].Positive_Input_Label[4]='.';
		m_Electrode[i].Positive_Input_Label[5]=0;
		m_Electrode[i].Negative_Input_Label[0]='.';
		m_Electrode[i].Negative_Input_Label[1]='.';
		m_Electrode[i].Negative_Input_Label[2]='.';
		m_Electrode[i].Negative_Input_Label[3]='.';
		m_Electrode[i].Negative_Input_Label[4]='.';
		m_Electrode[i].Negative_Input_Label[5]=0;
		m_Electrode[i].Logic_Minimum=(unsigned long)0;
		m_Electrode[i].Logic_Maximum=(unsigned long)255;
		m_Electrode[i].Logic_Ground=(unsigned long)128;
		m_Electrode[i].Physic_Minimum=-400;
		m_Electrode[i].Physic_Maximum=400;
		m_Electrode[i].Measurement_Unit=0; 
		m_Electrode[i].Prefiltering_HiPass_Limit=333;
		m_Electrode[i].Prefiltering_HiPass_Type=1;
		m_Electrode[i].Prefiltering_LowPass_Limit=Head_Old.Rate_Min/2;
		m_Electrode[i].Prefiltering_LowPass_Type=2;
		m_Electrode[i].Rate_Coefficient=1;
		m_Electrode[i].Latitude=0.0;
		m_Electrode[i].Longitude=0.0;
		m_Electrode[i].Maps=0;
		m_Electrode[i].Average_Ref=0;
		for (j=0; j<31; j++) m_Electrode[i].Description[j]=' ';
	    m_Electrode[i].Description[31]=0;
	}

	for (i=160; i<187; i++)
	{
		m_Electrode[i].Status=0;
		m_Electrode[i].Type=1;
		m_Electrode[i].Positive_Input_Label[0]=Head_Old.Labels[conv_label[i-96]].Name[0];
		m_Electrode[i].Positive_Input_Label[1]=Head_Old.Labels[conv_label[i-96]].Name[1];
		m_Electrode[i].Positive_Input_Label[2]=Head_Old.Labels[conv_label[i-96]].Name[2];
		m_Electrode[i].Positive_Input_Label[3]=Head_Old.Labels[conv_label[i-96]].Name[3];
		m_Electrode[i].Positive_Input_Label[4]=Head_Old.Labels[conv_label[i-96]].Name[4];
		m_Electrode[i].Positive_Input_Label[5]=0;
		m_Electrode[i].Negative_Input_Label[0]=Head_Old.Labels[conv_label[i-96]+1].Name[0];
		m_Electrode[i].Negative_Input_Label[1]=Head_Old.Labels[conv_label[i-96]+1].Name[1];
		m_Electrode[i].Negative_Input_Label[2]=Head_Old.Labels[conv_label[i-96]+1].Name[2];
		m_Electrode[i].Negative_Input_Label[3]=Head_Old.Labels[conv_label[i-96]+1].Name[3];
		m_Electrode[i].Negative_Input_Label[4]=Head_Old.Labels[conv_label[i-96]+1].Name[4];
		m_Electrode[i].Negative_Input_Label[5]=0;
		m_Electrode[i].Logic_Minimum=(unsigned long)0;
		m_Electrode[i].Logic_Maximum=(unsigned long)255;
		m_Electrode[i].Logic_Ground=(unsigned long)128;
		m_Electrode[i].Physic_Minimum=(-1)*(long)(Head_Old.Labels[conv_label[i-96]].Signal_Max & 16383)/(2*(800/Head_Old.Signal));
		m_Electrode[i].Physic_Maximum=(long)(Head_Old.Labels[conv_label[i-96]].Signal_Max & 16383)/(2*(800/Head_Old.Signal));
		m_Electrode[i].Measurement_Unit=(unsigned short int)Head_Old.Labels[conv_label[i-96]].Signal_Max>>14;
		m_Electrode[i].Prefiltering_HiPass_Limit=333;
		m_Electrode[i].Prefiltering_HiPass_Type=1;
		m_Electrode[i].Prefiltering_LowPass_Limit=Head_Old.Rate_Min/2;
		m_Electrode[i].Prefiltering_LowPass_Type=2;
		m_Electrode[i].Rate_Coefficient=1;
		m_Electrode[i].Latitude=0.0;
		m_Electrode[i].Longitude=0.0;
		m_Electrode[i].Maps=0;
		m_Electrode[i].Average_Ref=0;
		for (j=0; j<31; j++) m_Electrode[i].Description[j]=' ';
		m_Electrode[i].Description[31]=0;
	}

	for (i=187; i<640; i++)
	{
		m_Electrode[i].Status=0;
		m_Electrode[i].Type=1;
		m_Electrode[i].Positive_Input_Label[0]='.';
		m_Electrode[i].Positive_Input_Label[1]='.';
		m_Electrode[i].Positive_Input_Label[2]='.';
		m_Electrode[i].Positive_Input_Label[3]='.';
		m_Electrode[i].Positive_Input_Label[4]='.';
		m_Electrode[i].Positive_Input_Label[5]=0;
		m_Electrode[i].Negative_Input_Label[0]='.';
		m_Electrode[i].Negative_Input_Label[1]='.';
		m_Electrode[i].Negative_Input_Label[2]='.';
		m_Electrode[i].Negative_Input_Label[3]='.';
		m_Electrode[i].Negative_Input_Label[4]='.';
		m_Electrode[i].Negative_Input_Label[5]=0;
		m_Electrode[i].Logic_Minimum=(unsigned long)0;
		m_Electrode[i].Logic_Maximum=(unsigned long)255;
		m_Electrode[i].Logic_Ground=(unsigned long)128;
		m_Electrode[i].Physic_Minimum=-400;
		m_Electrode[i].Physic_Maximum=400;
		m_Electrode[i].Measurement_Unit=0; 
		m_Electrode[i].Prefiltering_HiPass_Limit=333;
		m_Electrode[i].Prefiltering_HiPass_Type=1;
		m_Electrode[i].Prefiltering_LowPass_Limit=Head_Old.Rate_Min/2;
		m_Electrode[i].Prefiltering_LowPass_Type=2;
		m_Electrode[i].Rate_Coefficient=1;
		m_Electrode[i].Latitude=0.0;
		m_Electrode[i].Longitude=0.0;
		m_Electrode[i].Maps=0;
		m_Electrode[i].Average_Ref=0;
		for (j=0; j<31; j++) m_Electrode[i].Description[j]=' ';
	    m_Electrode[i].Description[31]=0;
	}
//************** Set "Code" Structure ******************************************************	
	for (i=0; i<MAX_CAN; i++) m_Code[i]=0;
	for (i=0; i<m_Head.Num_Chan; i++)
	{
		if ((Head_Old.Electrode_Code[i]>>8)==0)
			m_Code[i]=conv_Code[(Head_Old.Electrode_Code[i] & 255)];
		else
		{
			if ((Head_Old.Electrode_Code[i]>>8)<32)
				m_Code[i]=conv_Code[(Head_Old.Electrode_Code[i]>>8)];
			else
				m_Code[i]=conv_Code[(Head_Old.Electrode_Code[i] & 254)];
		}
	}


//************** Set "Note" Structure ******************************************************	
   for (i=0; i<80; i++)
     {
       m_Note[i].Sample=Head_Old.Note[i].Time * Head_Old.Rate_Min;
	   for (j=0; j<20; j++) m_Note[i].Comment[j]=Head_Old.Note[i].Comment[j];
       for (j=20; j<40; j++) m_Note[i].Comment[j]=' ';
     }
   for (i=80; i<MAX_NOTE; i++)
     {
	   m_Note[i].Sample=0;
       for (j=0; j<40; j++) m_Note[i].Comment[j]=' ';
     }


//************** Set "Flag" Structure ******************************************************	
	for (i=0; i<15; i++)
	{
		m_Flag[i].Begin=Head_Old.Flag[i].Begin;
		m_Flag[i].End=Head_Old.Flag[i].End;
	}
	for (i=15; i<MAX_FLAG; i++)
	{
		m_Flag[i].Begin=0;
		m_Flag[i].End=0;
	}



//************** Set "EventA" and "EventB" Structure ***************************************
	m_EventA.Description[0]=0;													//
	m_EventB.Description[0]=0;													//
	for (i=0; i<MAX_EVENT; i++)													//
	{																			//	reset 
		m_EventA.Selection[i].Begin=0;											//	EventA 
		m_EventB.Selection[i].Begin=0;											//	EventB
		m_EventA.Selection[i].End=0;												//
		m_EventB.Selection[i].End=0;												//
	}																			//


//************** Set "Trigger" Structure ***************************************************
	for (i=0; i<MAX_TRIGGER; i++)												//
	{																			//	reset 
		m_Trigger[i].Sample=0;													//	Trigger
		m_Trigger[i].Type=0;														//
	}		

	return 0;
}

int MicromedReader::readHead2()
{
	int i, j;
	Micromed_Code_3			Code_3[MAX_CAN];
	Micromed_Electrode_3	Electrode_3[MAX_LAB_3];	

	m_file.seek(0);
	m_stream.readRawData((char *)&m_Head.Title, 32);
	m_stream.readRawData((char *)&m_Head.Laboratory, 32);
	m_stream.readRawData((char *)&m_Head.Patient_Data, sizeof(Micromed_New_Patient_Data));
	m_stream.readRawData((char *)&m_Head.Date, sizeof(Micromed_New_Date_Type));
	m_stream.readRawData((char *)&m_Head.Time, sizeof(Micromed_New_Time_Type));
	m_stream >> m_Head.Acquisition_Unit >> m_Head.Filetype >> m_Head.Data_Start_Offset;
	m_stream >> m_Head.Num_Chan >> m_Head.Multiplexer >> m_Head.Rate_Min;
	m_stream >> m_Head.Bytes >> m_Head.Compression >> m_Head.Montages;
	m_stream >> m_Head.Dvideo_Begin >> m_Head.MPEG_Difference;
	m_stream.skipRawData(15);
	m_stream.readRawData((char *)&m_Head.Header_Type, 1);
	m_stream.readRawData((char *)&m_Head.Code_Area.Name, 8);
	m_stream >> m_Head.Code_Area.Start_Offset >> m_Head.Code_Area.Length;
	m_stream.readRawData((char *)&m_Head.Electrode_Area.Name, 8);
	m_stream >> m_Head.Electrode_Area.Start_Offset >> m_Head.Electrode_Area.Length;
	m_stream.readRawData((char *)&m_Head.Note_Area.Name, 8);
	m_stream >> m_Head.Note_Area.Start_Offset >> m_Head.Note_Area.Length;
	m_stream.readRawData((char *)&m_Head.Flag_Area.Name, 8);
	m_stream >> m_Head.Flag_Area.Start_Offset >> m_Head.Flag_Area.Length;
	m_stream.readRawData((char *)&m_Head.Segment_Area.Name, 8);
	m_stream >> m_Head.Segment_Area.Start_Offset >> m_Head.Segment_Area.Length;
	m_stream.readRawData((char *)&m_Head.B_Impedance_Area.Name, 8);
	m_stream >> m_Head.B_Impedance_Area.Start_Offset >> m_Head.B_Impedance_Area.Length;
	m_stream.readRawData((char *)&m_Head.E_Impedance_Area.Name, 8);
	m_stream >> m_Head.E_Impedance_Area.Start_Offset >> m_Head.E_Impedance_Area.Length;
	m_stream.readRawData((char *)&m_Head.Montage_Area.Name, 8);
	m_stream >> m_Head.Montage_Area.Start_Offset >> m_Head.Montage_Area.Length;
	m_stream.readRawData((char *)&m_Head.Compression_Area.Name, 8);
	m_stream >> m_Head.Compression_Area.Start_Offset >> m_Head.Compression_Area.Length;
	m_stream.readRawData((char *)&m_Head.Average_Area.Name, 8);
	m_stream >> m_Head.Average_Area.Start_Offset >> m_Head.Average_Area.Length;
	m_stream.readRawData((char *)&m_Head.History_Area.Name, 8);
	m_stream >> m_Head.History_Area.Start_Offset >> m_Head.History_Area.Length;
	m_stream.readRawData((char *)&m_Head.Digital_Video_Area.Name, 8);
	m_stream >> m_Head.Digital_Video_Area.Start_Offset >> m_Head.Digital_Video_Area.Length;
	m_stream.readRawData((char *)&m_Head.EventA_Area.Name, 8);
	m_stream >> m_Head.EventA_Area.Start_Offset >> m_Head.EventA_Area.Length;
	m_stream.readRawData((char *)&m_Head.EventB_Area.Name, 8);
	m_stream >> m_Head.EventB_Area.Start_Offset >> m_Head.EventB_Area.Length;
	m_stream.readRawData((char *)&m_Head.Trigger_Area.Name, 8);
	m_stream >> m_Head.Trigger_Area.Start_Offset >> m_Head.Trigger_Area.Length;

	if (checkHeader())
		return -1;


//************** Set "Code" Structure *****************************************************
	m_file.seek(m_Head.Code_Area.Start_Offset);
	m_stream.readRawData((char *)Code_3, m_Head.Code_Area.Length);
	
	for (i = 0; i < m_Head.Num_Chan; i++) 
		m_Code[i] = (unsigned short int)Code_3[i];

	for (i = m_Head.Num_Chan; i < MAX_CAN; i++) 
		m_Code[i] = 0;		


//************** Set "Electrode" Structure ************************************************
	for (i = 0; i < MAX_LAB_3; i++)
	{
		m_file.seek(m_Head.Electrode_Area.Start_Offset + i * 128); // 128 bytes for Electrode3 struct
		m_stream >> Electrode_3[i].Type;
		m_stream.readRawData((char *)&Electrode_3[i].Positive_Input_Label, 6);
		m_stream.readRawData((char *)&Electrode_3[i].Negative_Input_Label, 6);
		m_stream >> Electrode_3[i].Logic_Minimum >> Electrode_3[i].Logic_Maximum;
		m_stream >> Electrode_3[i].Logic_Ground >> Electrode_3[i].Physic_Minimum;
		m_stream >> Electrode_3[i].Physic_Maximum >> Electrode_3[i].Measurement_Unit;
		m_stream >> Electrode_3[i].Prefiltering_HiPass_Limit >>  Electrode_3[i].Prefiltering_HiPass_Type;
		m_stream >> Electrode_3[i].Prefiltering_LowPass_Limit >>  Electrode_3[i].Prefiltering_LowPass_Type;
		m_stream >> Electrode_3[i].Rate_Coefficient >> Electrode_3[i].Position;
		m_stream.skipRawData(12);
		m_stream >> Electrode_3[i].Latitude >> Electrode_3[i].Longitude;
		m_stream.readRawData((char *)&Electrode_3[i].Maps, 1);
		m_stream.readRawData((char *)&Electrode_3[i].Average_Ref, 1);
		m_stream.readRawData((char *)&Electrode_3[i].Description, 32);
		// don't read last reserved bytes
	}
	
	for (i=0; i<240; i++)
	{
		m_Electrode[i].Status=0;
		m_Electrode[i].Type=(unsigned char)Electrode_3[i].Type;
		m_Electrode[i].Positive_Input_Label[0]=Electrode_3[i].Positive_Input_Label[0];
		m_Electrode[i].Positive_Input_Label[1]=Electrode_3[i].Positive_Input_Label[1];
		m_Electrode[i].Positive_Input_Label[2]=Electrode_3[i].Positive_Input_Label[2];
		m_Electrode[i].Positive_Input_Label[3]=Electrode_3[i].Positive_Input_Label[3];
		m_Electrode[i].Positive_Input_Label[4]=Electrode_3[i].Positive_Input_Label[4];
		m_Electrode[i].Positive_Input_Label[5]=Electrode_3[i].Positive_Input_Label[5];
		m_Electrode[i].Negative_Input_Label[0]=Electrode_3[i].Negative_Input_Label[0];
		m_Electrode[i].Negative_Input_Label[1]=Electrode_3[i].Negative_Input_Label[1];
		m_Electrode[i].Negative_Input_Label[2]=Electrode_3[i].Negative_Input_Label[2];
		m_Electrode[i].Negative_Input_Label[3]=Electrode_3[i].Negative_Input_Label[3];
		m_Electrode[i].Negative_Input_Label[4]=Electrode_3[i].Negative_Input_Label[4];
		m_Electrode[i].Negative_Input_Label[5]=Electrode_3[i].Negative_Input_Label[5];
		m_Electrode[i].Logic_Minimum=(unsigned long)Electrode_3[i].Logic_Minimum;
		m_Electrode[i].Logic_Maximum=(unsigned long)Electrode_3[i].Logic_Maximum;
		m_Electrode[i].Logic_Ground=(unsigned long)Electrode_3[i].Logic_Ground;
		m_Electrode[i].Physic_Minimum=Electrode_3[i].Physic_Minimum;
		m_Electrode[i].Physic_Maximum=Electrode_3[i].Physic_Maximum;
		m_Electrode[i].Measurement_Unit=Electrode_3[i].Measurement_Unit;
		m_Electrode[i].Prefiltering_HiPass_Limit=Electrode_3[i].Prefiltering_HiPass_Limit;
		m_Electrode[i].Prefiltering_HiPass_Type=Electrode_3[i].Prefiltering_HiPass_Type;
		m_Electrode[i].Prefiltering_LowPass_Limit=Electrode_3[i].Prefiltering_LowPass_Limit;
		m_Electrode[i].Prefiltering_LowPass_Type=Electrode_3[i].Prefiltering_LowPass_Type;
		m_Electrode[i].Rate_Coefficient=Electrode_3[i].Rate_Coefficient;
		m_Electrode[i].Latitude=Electrode_3[i].Latitude;
		m_Electrode[i].Longitude=Electrode_3[i].Longitude;
		m_Electrode[i].Maps=Electrode_3[i].Maps;
		m_Electrode[i].Average_Ref=Electrode_3[i].Average_Ref;
		for (j=0; j<32; j++) m_Electrode[i].Description[j]= Electrode_3[i].Description[j];
	}

	for (i=240; i<640; i++)
	{
		m_Electrode[i].Status=0;
		m_Electrode[i].Type=0;
		m_Electrode[i].Positive_Input_Label[0]='.';
		m_Electrode[i].Positive_Input_Label[1]='.';
		m_Electrode[i].Positive_Input_Label[2]='.';
		m_Electrode[i].Positive_Input_Label[3]='.';
		m_Electrode[i].Positive_Input_Label[4]='.';
		m_Electrode[i].Positive_Input_Label[5]=0;
		m_Electrode[i].Negative_Input_Label[0]='.';
		m_Electrode[i].Negative_Input_Label[1]='.';
		m_Electrode[i].Negative_Input_Label[2]='.';
		m_Electrode[i].Negative_Input_Label[3]='.';
		m_Electrode[i].Negative_Input_Label[4]='.';
		m_Electrode[i].Negative_Input_Label[5]=0;
		m_Electrode[i].Logic_Minimum=(unsigned long)Electrode_3[1].Logic_Minimum;
		m_Electrode[i].Logic_Maximum=(unsigned long)Electrode_3[1].Logic_Maximum;
		m_Electrode[i].Logic_Ground=(unsigned long)Electrode_3[1].Logic_Ground;
		m_Electrode[i].Physic_Minimum=Electrode_3[1].Physic_Minimum;
		m_Electrode[i].Physic_Maximum=Electrode_3[1].Physic_Maximum;
		m_Electrode[i].Measurement_Unit=Electrode_3[1].Measurement_Unit;
		m_Electrode[i].Prefiltering_HiPass_Limit=Electrode_3[1].Prefiltering_HiPass_Limit;
		m_Electrode[i].Prefiltering_HiPass_Type=Electrode_3[1].Prefiltering_HiPass_Type;
		m_Electrode[i].Prefiltering_LowPass_Limit=Electrode_3[1].Prefiltering_LowPass_Limit;
		m_Electrode[i].Prefiltering_LowPass_Type=Electrode_3[1].Prefiltering_LowPass_Type;
		m_Electrode[i].Rate_Coefficient=Electrode_3[1].Rate_Coefficient;
		m_Electrode[i].Latitude=0.0;
		m_Electrode[i].Longitude=0.0;
		m_Electrode[i].Maps=0;
		m_Electrode[i].Average_Ref=0;
		for (j=0; j<32; j++) m_Electrode[i].Description[j]=Electrode_3[1].Description[j];
	}
//************** Convert "Note" Structure ***********************************************
	for (int i = 0; i < MAX_NOTE; i++)
	{
		m_file.seek(m_Head.Note_Area.Start_Offset + i * 44);
		m_stream >> m_Note[i].Sample;
		m_stream.readRawData((char *)&m_Note[i].Comment, 40);
	}
	
	for (i = 0; i < MAX_NOTE_2; i++)
		m_Note[i].Sample *= m_Head.Rate_Min;				//
	for (i = MAX_NOTE_2; i < MAX_NOTE; i++)											//
    {																			//	converts 
	   m_Note[i].Sample = 0;														//	Format
	   for (j=0; j<40; j++)
		   m_Note[i].Comment[j] = ' ';								//
    }	

//************** Set "Flag" Structure ******************************************************
	for (int i = 0; i < MAX_FLAG; i++)
	{
		m_file.seek(m_Head.Flag_Area.Start_Offset + i * 8);								
		m_stream.readRawData((char *)&m_Flag[i], 8);
	}


//************** Set "EventA" and "EventB" Structure ***************************************
	m_EventA.Description[0]=0;													//
	m_EventB.Description[0]=0;													//
	for (i=0; i<MAX_EVENT; i++)													//
	{																			//	reset 
		m_EventA.Selection[i].Begin=0;											//	EventA
		m_EventB.Selection[i].Begin=0;											//	EventB
		m_EventA.Selection[i].End=0;												//
		m_EventB.Selection[i].End=0;												//
	}																			//


//************** Set "Trigger" Structure ***************************************************
	for (i=0; i<MAX_TRIGGER; i++)												//
	{																			//	reset 
		m_Trigger[i].Sample=0;													//	Trigger
		m_Trigger[i].Type=0;														//
	}																			//
	
//************** Set "Descriptor" Structure ************************************************
	setDescriptors();
	return 0;
}

int MicromedReader::readHead3()
{
	int i, j;
	Micromed_Code_3			Code_3[MAX_CAN];
	Micromed_Electrode_3	Electrode_3[MAX_LAB_3];	

	m_file.seek(0);
	m_stream.readRawData((char *)&m_Head.Title, 32);
	m_stream.readRawData((char *)&m_Head.Laboratory, 32);
	m_stream.readRawData((char *)&m_Head.Patient_Data, sizeof(Micromed_New_Patient_Data));
	m_stream.readRawData((char *)&m_Head.Date, sizeof(Micromed_New_Date_Type));
	m_stream.readRawData((char *)&m_Head.Time, sizeof(Micromed_New_Time_Type));
	m_stream >> m_Head.Acquisition_Unit >> m_Head.Filetype >> m_Head.Data_Start_Offset;
	m_stream >> m_Head.Num_Chan >> m_Head.Multiplexer >> m_Head.Rate_Min;
	m_stream >> m_Head.Bytes >> m_Head.Compression >> m_Head.Montages;
	m_stream >> m_Head.Dvideo_Begin >> m_Head.MPEG_Difference;
	m_stream.skipRawData(15);
	m_stream.readRawData((char *)&m_Head.Header_Type, 1);
	m_stream.readRawData((char *)&m_Head.Code_Area.Name, 8);
	m_stream >> m_Head.Code_Area.Start_Offset >> m_Head.Code_Area.Length;
	m_stream.readRawData((char *)&m_Head.Electrode_Area.Name, 8);
	m_stream >> m_Head.Electrode_Area.Start_Offset >> m_Head.Electrode_Area.Length;
	m_stream.readRawData((char *)&m_Head.Note_Area.Name, 8);
	m_stream >> m_Head.Note_Area.Start_Offset >> m_Head.Note_Area.Length;
	m_stream.readRawData((char *)&m_Head.Flag_Area.Name, 8);
	m_stream >> m_Head.Flag_Area.Start_Offset >> m_Head.Flag_Area.Length;
	m_stream.readRawData((char *)&m_Head.Segment_Area.Name, 8);
	m_stream >> m_Head.Segment_Area.Start_Offset >> m_Head.Segment_Area.Length;
	m_stream.readRawData((char *)&m_Head.B_Impedance_Area.Name, 8);
	m_stream >> m_Head.B_Impedance_Area.Start_Offset >> m_Head.B_Impedance_Area.Length;
	m_stream.readRawData((char *)&m_Head.E_Impedance_Area.Name, 8);
	m_stream >> m_Head.E_Impedance_Area.Start_Offset >> m_Head.E_Impedance_Area.Length;
	m_stream.readRawData((char *)&m_Head.Montage_Area.Name, 8);
	m_stream >> m_Head.Montage_Area.Start_Offset >> m_Head.Montage_Area.Length;
	m_stream.readRawData((char *)&m_Head.Compression_Area.Name, 8);
	m_stream >> m_Head.Compression_Area.Start_Offset >> m_Head.Compression_Area.Length;
	m_stream.readRawData((char *)&m_Head.Average_Area.Name, 8);
	m_stream >> m_Head.Average_Area.Start_Offset >> m_Head.Average_Area.Length;
	m_stream.readRawData((char *)&m_Head.History_Area.Name, 8);
	m_stream >> m_Head.History_Area.Start_Offset >> m_Head.History_Area.Length;
	m_stream.readRawData((char *)&m_Head.Digital_Video_Area.Name, 8);
	m_stream >> m_Head.Digital_Video_Area.Start_Offset >> m_Head.Digital_Video_Area.Length;
	m_stream.readRawData((char *)&m_Head.EventA_Area.Name, 8);
	m_stream >> m_Head.EventA_Area.Start_Offset >> m_Head.EventA_Area.Length;
	m_stream.readRawData((char *)&m_Head.EventB_Area.Name, 8);
	m_stream >> m_Head.EventB_Area.Start_Offset >> m_Head.EventB_Area.Length;
	m_stream.readRawData((char *)&m_Head.Trigger_Area.Name, 8);
	m_stream >> m_Head.Trigger_Area.Start_Offset >> m_Head.Trigger_Area.Length;

	if (checkHeader())
		return -1;

	m_file.seek(m_Head.Code_Area.Start_Offset);
	m_stream.readRawData((char *)Code_3, m_Head.Code_Area.Length);
	
	for (i = 0; i < m_Head.Num_Chan; i++) 
		m_Code[i] = (unsigned short int)Code_3[i];

	for (i = m_Head.Num_Chan; i < MAX_CAN; i++) 
		m_Code[i] = 0;							
	

//************** Set "Electrode" Structure ************************************************
			
	for (i = 0; i < MAX_LAB_3; i++)
	{
		m_file.seek(m_Head.Electrode_Area.Start_Offset + i * 128); // 128 bytes for Electrode3 struct
		m_stream >> Electrode_3[i].Type;
		m_stream.readRawData((char *)&Electrode_3[i].Positive_Input_Label, 6);
		m_stream.readRawData((char *)&Electrode_3[i].Negative_Input_Label, 6);
		m_stream >> Electrode_3[i].Logic_Minimum >> Electrode_3[i].Logic_Maximum;
		m_stream >> Electrode_3[i].Logic_Ground >> Electrode_3[i].Physic_Minimum;
		m_stream >> Electrode_3[i].Physic_Maximum >> Electrode_3[i].Measurement_Unit;
		m_stream >> Electrode_3[i].Prefiltering_HiPass_Limit >>  Electrode_3[i].Prefiltering_HiPass_Type;
		m_stream >> Electrode_3[i].Prefiltering_LowPass_Limit >>  Electrode_3[i].Prefiltering_LowPass_Type;
		m_stream >> Electrode_3[i].Rate_Coefficient >> Electrode_3[i].Position;
		m_stream.skipRawData(12);
		m_stream >> Electrode_3[i].Latitude >> Electrode_3[i].Longitude;
		m_stream.readRawData((char *)&Electrode_3[i].Maps, 1);
		m_stream.readRawData((char *)&Electrode_3[i].Average_Ref, 1);
		m_stream.readRawData((char *)&Electrode_3[i].Description, 32);
		// don't read last reserved bytes
	}

	for (i=0; i<240; i++)
	{
		m_Electrode[i].Status=0;
		m_Electrode[i].Type=(unsigned char)Electrode_3[i].Type;
		m_Electrode[i].Positive_Input_Label[0]=Electrode_3[i].Positive_Input_Label[0];
		m_Electrode[i].Positive_Input_Label[1]=Electrode_3[i].Positive_Input_Label[1];
		m_Electrode[i].Positive_Input_Label[2]=Electrode_3[i].Positive_Input_Label[2];
		m_Electrode[i].Positive_Input_Label[3]=Electrode_3[i].Positive_Input_Label[3];
		m_Electrode[i].Positive_Input_Label[4]=Electrode_3[i].Positive_Input_Label[4];
		m_Electrode[i].Positive_Input_Label[5]=Electrode_3[i].Positive_Input_Label[5];
		m_Electrode[i].Negative_Input_Label[0]=Electrode_3[i].Negative_Input_Label[0];
		m_Electrode[i].Negative_Input_Label[1]=Electrode_3[i].Negative_Input_Label[1];
		m_Electrode[i].Negative_Input_Label[2]=Electrode_3[i].Negative_Input_Label[2];
		m_Electrode[i].Negative_Input_Label[3]=Electrode_3[i].Negative_Input_Label[3];
		m_Electrode[i].Negative_Input_Label[4]=Electrode_3[i].Negative_Input_Label[4];
		m_Electrode[i].Negative_Input_Label[5]=Electrode_3[i].Negative_Input_Label[5];
		m_Electrode[i].Logic_Minimum=(unsigned long)Electrode_3[i].Logic_Minimum;
		m_Electrode[i].Logic_Maximum=(unsigned long)Electrode_3[i].Logic_Maximum;
		m_Electrode[i].Logic_Ground=(unsigned long)Electrode_3[i].Logic_Ground;
		m_Electrode[i].Physic_Minimum=Electrode_3[i].Physic_Minimum;
		m_Electrode[i].Physic_Maximum=Electrode_3[i].Physic_Maximum;
		m_Electrode[i].Measurement_Unit=Electrode_3[i].Measurement_Unit;
		m_Electrode[i].Prefiltering_HiPass_Limit=Electrode_3[i].Prefiltering_HiPass_Limit;
		m_Electrode[i].Prefiltering_HiPass_Type=Electrode_3[i].Prefiltering_HiPass_Type;
		m_Electrode[i].Prefiltering_LowPass_Limit=Electrode_3[i].Prefiltering_LowPass_Limit;
		m_Electrode[i].Prefiltering_LowPass_Type=Electrode_3[i].Prefiltering_LowPass_Type;
		m_Electrode[i].Rate_Coefficient=Electrode_3[i].Rate_Coefficient;
		m_Electrode[i].Latitude=Electrode_3[i].Latitude;
		m_Electrode[i].Longitude=Electrode_3[i].Longitude;
		m_Electrode[i].Maps=Electrode_3[i].Maps;
		m_Electrode[i].Average_Ref=Electrode_3[i].Average_Ref;
		for (j=0; j<32; j++) m_Electrode[i].Description[j]=Electrode_3[i].Description[j];
	}

	for (i=240; i<640; i++)
	{
		m_Electrode[i].Status=0;
		m_Electrode[i].Type=0;
		m_Electrode[i].Positive_Input_Label[0]='.';
		m_Electrode[i].Positive_Input_Label[1]='.';
		m_Electrode[i].Positive_Input_Label[2]='.';
		m_Electrode[i].Positive_Input_Label[3]='.';
		m_Electrode[i].Positive_Input_Label[4]='.';
		m_Electrode[i].Positive_Input_Label[5]=0;
		m_Electrode[i].Negative_Input_Label[0]='.';
		m_Electrode[i].Negative_Input_Label[1]='.';
		m_Electrode[i].Negative_Input_Label[2]='.';
		m_Electrode[i].Negative_Input_Label[3]='.';
		m_Electrode[i].Negative_Input_Label[4]='.';
		m_Electrode[i].Negative_Input_Label[5]=0;
		m_Electrode[i].Logic_Minimum=(unsigned long)Electrode_3[1].Logic_Minimum;
		m_Electrode[i].Logic_Maximum=(unsigned long)Electrode_3[1].Logic_Maximum;
		m_Electrode[i].Logic_Ground=(unsigned long)Electrode_3[1].Logic_Ground;
		m_Electrode[i].Physic_Minimum=Electrode_3[1].Physic_Minimum;
		m_Electrode[i].Physic_Maximum=Electrode_3[1].Physic_Maximum;
		m_Electrode[i].Measurement_Unit=Electrode_3[1].Measurement_Unit;
		m_Electrode[i].Prefiltering_HiPass_Limit=Electrode_3[1].Prefiltering_HiPass_Limit;
		m_Electrode[i].Prefiltering_HiPass_Type=Electrode_3[1].Prefiltering_HiPass_Type;
		m_Electrode[i].Prefiltering_LowPass_Limit=Electrode_3[1].Prefiltering_LowPass_Limit;
		m_Electrode[i].Prefiltering_LowPass_Type=Electrode_3[1].Prefiltering_LowPass_Type;
		m_Electrode[i].Rate_Coefficient=Electrode_3[1].Rate_Coefficient;
		m_Electrode[i].Latitude=0.0;
		m_Electrode[i].Longitude=0.0;
		m_Electrode[i].Maps=0;
		m_Electrode[i].Average_Ref=0;
		for (j=0; j<32; j++) m_Electrode[i].Description[j]=Electrode_3[1].Description[j];
	}

//************** Set "Note" Structure ******************************************************
	for (int i = 0; i < MAX_NOTE; i++)
	{
		m_file.seek(m_Head.Note_Area.Start_Offset + i * 44);
		m_stream >> m_Note[i].Sample;
		m_stream.readRawData((char *)&m_Note[i].Comment, 40);
	}

//************** Set "Flag" Structure ******************************************************
	for (int i = 0; i < MAX_FLAG; i++)
	{
		m_file.seek(m_Head.Flag_Area.Start_Offset + i * 8);								
		m_stream.readRawData((char *)&m_Flag[i], 8);
	}

	
//************** Set "Event A" Structure ***************************************************
	if (QString(m_Head.EventA_Area.Name).contains("EVENT A"))
	{																			//
		m_file.seek(m_Head.EventA_Area.Start_Offset);						//	if defined
		m_stream.readRawData((char *)&m_EventA.Description, 64);
		for (i = 0; i < MAX_EVENT; i++)
			m_stream.readRawData((char *)&m_EventA.Selection[i], 8);
	}																			//	EventA
	else																		//
	{																			//
		m_EventA.Description[0]=0;												//
		for (i=0; i<MAX_EVENT; i++)												//	
		{																		//	otherwise
			m_EventA.Selection[i].Begin=0;										//	reset
			m_EventA.Selection[i].End=0;											//	EventA
		}																		//
	}


//************** Set "Event B" Structure ***************************************************
	if (QString(m_Head.EventB_Area.Name).contains("EVENT B"))
	{																			//
		m_file.seek(m_Head.EventB_Area.Start_Offset);						//	if defined
		m_stream.readRawData((char *)&m_EventB.Description, 64);
		for (i = 0; i < MAX_EVENT; i++)
			m_stream.readRawData((char *)&m_EventB.Selection[i], 8);
	}																			//	EventA
	else																		//
	{																			//
		m_EventB.Description[0]=0;												//
		for (i=0; i<MAX_EVENT; i++)												//	
		{																		//	otherwise
			m_EventB.Selection[i].Begin=0;										//	reset
			m_EventB.Selection[i].End=0;											//	EventA
		}																		//
	}
	
//************** Set "Trigger" Structure ***************************************************
	for (i = 0; i< MAX_TRIGGER; i++)												//
	{																			//	reset
		m_Trigger[i].Sample=0;													//	Trigger
		m_Trigger[i].Type=0;														//
	}																			//


//************** Set "Descriptor" Structure ************************************************
	setDescriptors();
	return 0;
}

//
// check if Header is correct:
// important information must be correct to avoid crash later.
//
int MicromedReader::checkHeader()
{
	if (m_Head.Num_Chan > MAX_CAN)
		return -1;
	if (m_Head.Bytes < 1 || m_Head.Bytes > 2)
		return -1;
	if (m_Head.Rate_Min <= 0)
		return -1;

	return 0;
}

int MicromedReader::readHead4()
{
	char pad[256];

	m_file.seek(0);
	m_stream.readRawData((char *)&m_Head.Title, 32);
	m_stream.readRawData((char *)&m_Head.Laboratory, 32);
	m_stream.readRawData((char *)&m_Head.Patient_Data, sizeof(Micromed_New_Patient_Data));
	m_stream.readRawData((char *)&m_Head.Date, sizeof(Micromed_New_Date_Type));
	m_stream.readRawData((char *)&m_Head.Time, sizeof(Micromed_New_Time_Type));
	m_stream >> m_Head.Acquisition_Unit >> m_Head.Filetype >> m_Head.Data_Start_Offset;
	m_stream >> m_Head.Num_Chan >> m_Head.Multiplexer >> m_Head.Rate_Min;
	m_stream >> m_Head.Bytes >> m_Head.Compression >> m_Head.Montages;
	m_stream >> m_Head.Dvideo_Begin >> m_Head.MPEG_Difference;
	m_stream.readRawData(pad, 15);
	m_stream.readRawData((char *)&m_Head.Header_Type, 1);
	m_stream.readRawData((char *)&m_Head.Code_Area.Name, 8);
	m_stream >> m_Head.Code_Area.Start_Offset >> m_Head.Code_Area.Length;
	m_stream.readRawData((char *)&m_Head.Electrode_Area.Name, 8);
	m_stream >> m_Head.Electrode_Area.Start_Offset >> m_Head.Electrode_Area.Length;
	m_stream.readRawData((char *)&m_Head.Note_Area.Name, 8);
	m_stream >> m_Head.Note_Area.Start_Offset >> m_Head.Note_Area.Length;
	m_stream.readRawData((char *)&m_Head.Flag_Area.Name, 8);
	m_stream >> m_Head.Flag_Area.Start_Offset >> m_Head.Flag_Area.Length;
	m_stream.readRawData((char *)&m_Head.Segment_Area.Name, 8);
	m_stream >> m_Head.Segment_Area.Start_Offset >> m_Head.Segment_Area.Length;
	m_stream.readRawData((char *)&m_Head.B_Impedance_Area.Name, 8);
	m_stream >> m_Head.B_Impedance_Area.Start_Offset >> m_Head.B_Impedance_Area.Length;
	m_stream.readRawData((char *)&m_Head.E_Impedance_Area.Name, 8);
	m_stream >> m_Head.E_Impedance_Area.Start_Offset >> m_Head.E_Impedance_Area.Length;
	m_stream.readRawData((char *)&m_Head.Montage_Area.Name, 8);
	m_stream >> m_Head.Montage_Area.Start_Offset >> m_Head.Montage_Area.Length;
	m_stream.readRawData((char *)&m_Head.Compression_Area.Name, 8);
	m_stream >> m_Head.Compression_Area.Start_Offset >> m_Head.Compression_Area.Length;
	m_stream.readRawData((char *)&m_Head.Average_Area.Name, 8);
	m_stream >> m_Head.Average_Area.Start_Offset >> m_Head.Average_Area.Length;
	m_stream.readRawData((char *)&m_Head.History_Area.Name, 8);
	m_stream >> m_Head.History_Area.Start_Offset >> m_Head.History_Area.Length;
	m_stream.readRawData((char *)&m_Head.Digital_Video_Area.Name, 8);
	m_stream >> m_Head.Digital_Video_Area.Start_Offset >> m_Head.Digital_Video_Area.Length;
	m_stream.readRawData((char *)&m_Head.EventA_Area.Name, 8);
	m_stream >> m_Head.EventA_Area.Start_Offset >> m_Head.EventA_Area.Length;
	m_stream.readRawData((char *)&m_Head.EventB_Area.Name, 8);
	m_stream >> m_Head.EventB_Area.Start_Offset >> m_Head.EventB_Area.Length;
	m_stream.readRawData((char *)&m_Head.Trigger_Area.Name, 8);
	m_stream >> m_Head.Trigger_Area.Start_Offset >> m_Head.Trigger_Area.Length;

	if (checkHeader())
		return -1;


//************** Set "Code" Structure ******************************************************
	m_file.seek(m_Head.Code_Area.Start_Offset);								//	reads
	m_stream.readRawData((char *)m_Code, m_Head.Code_Area.Length);						//	Code


//************** Set "Electrode" Structure *************************************************
	m_file.seek(m_Head.Electrode_Area.Start_Offset);	
	for (int i = 0; i < MAX_LAB; i++)
	{
		m_file.seek(m_Head.Electrode_Area.Start_Offset + i * 128);
		m_stream.readRawData((char *)&m_Electrode[i].Status, 1);
		m_stream.readRawData((char *)&m_Electrode[i].Type, 1);
		m_stream.readRawData((char *)&m_Electrode[i].Positive_Input_Label, 6);
		m_stream.readRawData((char *)&m_Electrode[i].Negative_Input_Label, 6);
		m_stream >> m_Electrode[i].Logic_Minimum >> m_Electrode[i].Logic_Maximum >> m_Electrode[i].Logic_Ground;
		m_stream >> m_Electrode[i].Physic_Minimum >>  m_Electrode[i].Physic_Maximum;
		m_stream >> m_Electrode[i].Measurement_Unit >> m_Electrode[i].Prefiltering_HiPass_Limit;
		m_stream >> m_Electrode[i].Prefiltering_HiPass_Type >> m_Electrode[i].Prefiltering_LowPass_Limit;
		m_stream >> m_Electrode[i].Prefiltering_LowPass_Type >> m_Electrode[i].Rate_Coefficient;
		m_stream >> m_Electrode[i].Position >> m_Electrode[i].Latitude >> m_Electrode[i].Longitude;
		m_stream.readRawData((char *)&m_Electrode[i].Maps, 1);
		m_stream.readRawData((char *)&m_Electrode[i].Average_Ref, 1);
		m_stream.readRawData((char *)&m_Electrode[i].Description, 32);
		m_stream.readRawData((char *)pad, 38);
	}


//************** Set "Note" Structure ******************************************************
	for (int i = 0; i < MAX_NOTE; i++)
	{
		m_file.seek(m_Head.Note_Area.Start_Offset + i * 44);
		m_stream >> m_Note[i].Sample;
		m_stream.readRawData((char *)&m_Note[i].Comment, 40);
	}
	

//************** Set "Flag" Structure ******************************************************
	for (int i = 0; i < MAX_FLAG; i++)
	{
		m_file.seek(m_Head.Flag_Area.Start_Offset + i * 8);								
		m_stream.readRawData((char *)&m_Flag[i], 8);
	}


//************** Set "Event_A" Structure ***************************************************	
	m_file.seek(m_Head.EventA_Area.Start_Offset);								//	reads 
	m_stream.readRawData((char *)&m_EventA.Description, 64);
	for (int i = 0; i < MAX_EVENT; i++)
		m_stream.readRawData((char *)&m_EventA.Selection[i], 8);



//************** Set "Event_B" Structure ***************************************************	
	m_file.seek(m_Head.EventB_Area.Start_Offset);								//	reads 
	m_stream.readRawData((char *)&m_EventB.Description, 64);
	for (int i = 0; i < MAX_EVENT; i++)
		m_stream.readRawData((char *)&m_EventB.Selection[i], 8);


//************** Set "Trigger" Structure ***************************************************	
	for (int i = 0; i < MAX_TRIGGER; i++)
	{
		m_file.seek(m_Head.Trigger_Area.Start_Offset + i * 6);								//	reads 
		m_stream.readRawData((char *)&m_Trigger[i], 6);
	}

	return 0;
}

MicromedReader::FileStatus MicromedReader::openFile(const QString& path)
{
	m_file.setFileName(path);
	if (!m_file.open(QIODevice::ReadOnly)) {
		m_error = "Cannot open the file.";
		return AwFileIO::FileAccess;
	}

	m_stream.setDevice(&m_file);
	m_stream.setByteOrder(QDataStream::LittleEndian);
	
	m_file.seek(175);
	quint8 discriminant;
	m_stream >> discriminant;
	int status = 0;
	switch (discriminant)
	{
	case 0:
		status = readHead0();
		break;
	case 1:
		status = readHead1();
		break;
	case 2:
		status = readHead2();
		break;
	case 3:
		status = readHead3();
		break;
	case 4:
		status = readHead4();
		break;
	}

	if (status)
	{
		m_file.close();
		return AwFileIO::WrongFormat;
	}

	// set date and time correctly 

	QDate date((int)m_Head.Date.Year + 1900, (int)m_Head.Date.Month, (int)m_Head.Date.Day);
	QTime time((int)m_Head.Time.Hour, (int)m_Head.Time.Min, (int)m_Head.Time.Sec);
	infos.setTime(time.toString(Qt::ISODate));
	infos.setDate(date.toString(Qt::ISODate));
	infos.setISODate(QDateTime(date, time).toString(Qt::ISODate));
	infos.setFirstName(QString(m_Head.Patient_Data.Surname).trimmed());
	infos.setLastName(QString(m_Head.Patient_Data.Name).trimmed());

	// add a block
	AwBlock *block = infos.newBlock();
	qint64 nSamples = (m_file.size() - m_Head.Data_Start_Offset) / (m_Head.Bytes * m_Head.Num_Chan);
	block->setDuration((float)(nSamples / m_Head.Rate_Min));
	block->setSamples((quint32)nSamples);


	for (int i = 0; i < m_Head.Num_Chan; i++)
	{
		AwChannel chan;
		Micromed_New_Electrode electrode = m_Electrode[m_Code[i]];

		// remove spaces after the electrode name.
		QString name(electrode.Positive_Input_Label);
		chan.setName(name.trimmed());

		if (chan.name() == "ECG")
			chan.setType(AwChannel::ECG);
		
		switch (electrode.Measurement_Unit)
		{
		case -1:
			chan.setUnit(QString::fromLatin1("nV"));
			break;
		case 0:
			chan.setUnit(QString::fromLatin1("µV"));
			break;
		case 1:
			chan.setUnit(QString::fromLatin1("mV"));
			break;
		case 100:
			chan.setUnit(QString::fromLatin1("%"));
			chan.setType(AwChannel::Other);
			break;
		case 101:
			chan.setUnit(QString::fromLatin1("bpm"));
			chan.setType(AwChannel::ECG);
			break;
		case 102:
			chan.setUnit(QString::fromLatin1("Adim"));
			chan.setType(AwChannel::Other);
			break;
		default:
			chan.setUnit(QString::fromLatin1("µV"));
			break;
		}
		if (electrode.Rate_Coefficient <= 0)
			electrode.Rate_Coefficient = 1;
		chan.setSamplingRate(electrode.Rate_Coefficient * m_Head.Rate_Min);
		infos.addChannel(&chan);
	}

	// parse Notes
	for (int i = 0; i < MAX_NOTE; i++)
	{
		if (m_Note[i].Sample > 0)
		{
			AwMarker marker;
			marker.setStart(m_Note[i].Sample / m_Head.Rate_Min);
			marker.setLabel(QString(m_Note[i].Comment));
			infos.blocks().at(0)->addMarker(marker);
		}
	}
	for (int i = 0; i < MAX_TRIGGER; i++)
	{
		if (m_Trigger[i].Sample > 0)
		{
			AwMarker marker;
			marker.setStart(m_Trigger[i].Sample / m_Head.Rate_Min);
			marker.setLabel("Trigger");
			marker.setValue(m_Trigger[i].Type);
			infos.blocks().at(0)->addMarker(marker);
		}
	}
	for (int i = 0; i < MAX_FLAG; i++)
	{
		if (m_Flag[i].Begin > 0)
		{
			AwMarker marker;
			marker.setStart(m_Flag[i].Begin / m_Head.Rate_Min);
			marker.setLabel("Flag");
			marker.setDuration((m_Flag[i].End - m_Flag[i].Begin) /  m_Head.Rate_Min);
			infos.blocks().at(0)->addMarker(marker);
		}
	}
	return AwFileIO::NoError;
}


qint64 MicromedReader::readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList)
{
	if (channelList.isEmpty())
		return 0;

	// number of samples to read
	qint64 nSamples = (qint64)floor(duration * m_Head.Rate_Min);
	// starting sample in channel.
	qint64 nStart = (qint64)floor(start * m_Head.Rate_Min);
	// total number of channels in file.
	quint32 nbChannels = infos.channelsCount();
	// starting sample in file.
	qint64 startSample = nStart * nbChannels;

	if (nSamples <= 0) 
		return 0;

	if (nStart > infos.totalSamples())
		return 0;

	if (nStart + nSamples > infos.totalSamples())
		nSamples = infos.totalSamples() - nStart;

	qint64 totalSize = nSamples * nbChannels;

	m_file.seek(m_Head.Data_Start_Offset + startSample * m_Head.Bytes);
	qint64 read = 0;
	quint16 *sbuf;
	quint8 *cbuf;
	Micromed_New_Electrode elec;

	switch (m_Head.Bytes)
	{
	case 1:
		cbuf = new quint8[totalSize];
		//read = m_stream.readRawData((char *)cbuf, totalSize);
		read = m_file.read((char *)cbuf, totalSize);
		if (read <= 0)	{
			delete [] cbuf;
			return 0;
		}
		read /= nbChannels;
		foreach (AwChannel *c, channelList)	{
			int index = infos.indexOfChannel(c->name());
			elec = m_Electrode[m_Code[index]];
			float *data = c->newData(read);
			qint64 count = 0;
			while (count < read) {
				float value = (float)(cbuf[index + count * nbChannels]);
				*data++ = getRealValue(value, index);
				count++;
			}
		}
		delete [] cbuf;
		break;
	case 2:	
		sbuf = new quint16[totalSize];
		//read = m_stream.readRawData((char *)sbuf, totalSize * 2);
		read = m_file.read((char *)sbuf, totalSize * 2);
		read /= 2;
		if (read <= 0) 	{
			delete [] sbuf;
			return 0;
		}
		read /= nbChannels;
		foreach (AwChannel *c, channelList)
		{
			int index = infos.indexOfChannel(c->name());
			elec = m_Electrode[m_Code[index]];
			float *data = c->newData(read);
			qint64 count = 0;
			while (count < read) {
				float value = (float)(sbuf[index + count * nbChannels]);
				*data++ = getRealValue(value, index);
				count++;
			}
		}
		delete [] sbuf;
		break;
	}
    return read;
}

float MicromedReader::getRealValue(float value, int index)
{
	Micromed_New_Electrode e = m_Electrode[m_Code[index]];
	// AnyWave expects values in µV.
	// So we must rescale the factor accordingly.
	float factor = 1;
	if (e.Measurement_Unit != 0)
	{
		switch (e.Measurement_Unit)
		{
		case -1: // nV
			factor = 1E-6 / 1E-9;
			break;
		case 1: // mV
			factor =  1E-6 / 1E-3;
			break;
		}
	}

	float res = (value - e.Logic_Ground) / (e.Logic_Maximum - e.Logic_Minimum + 1);
	return res * (e.Physic_Maximum - e.Physic_Minimum) * factor;
}