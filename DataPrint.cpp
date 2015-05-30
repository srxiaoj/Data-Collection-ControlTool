
#pragma once
#include "stdafx.h"
#include "DataPrint.h"
#include <ctime>

// Writes the arrays for position and intensities calculated from data acquisition, 
// as well as scan name information onto a text file LockinData.csv
// Pass it the total number of points in the pointspace, the number of delay lines
// scanned, which if only one, the name of scan, and the time the scan took in seconds
void DataPrint::WritetoFile(vector<pointGroup> PositionTable, double ValueTable[400], 
								int dimension, int auxflag, int line, CString scanName, double ScanTime)
{
	int NumberOfPoints = PositionTable.size(); 
	int seconds = fmod(ScanTime,60); //Seconds the scanning took
	int minutes = (ScanTime-seconds)/60; //Minutes the scanning took
	FILE * data_file;
    time_t now = time(0);
	struct tm tstruct;
	char date[30]; //Buffer to save date information
	tstruct = *localtime(&now); //Function to get local time
	strftime(date, sizeof(date), "%m/%d/%Y (%X)", &tstruct); // Format time into Mon/Day/Year (hour:min:sec)
    data_file = fopen ("LockinData.csv","w"); //Open/create the file where to store data
	fprintf(data_file, "Date Created: %s\n", date); //Print the time data created

	if(dimension == 2) //If two delay lines were scanned
	{
		fprintf(data_file, "Scan name: %s \n", scanName); //Print the name of scan
		fprintf(data_file, "Scanning time: %dm %ds \n\n", minutes, seconds); //Print the time it took 
		//Print the header for each column
		fprintf(data_file, "   DLN1 (%s),    DLN2 (%s),    Intensity (mV) \n\n", PositionTable[0].DL1.unit.c_str(), PositionTable[0].DL2.unit.c_str());

		for(int i=0; i < NumberOfPoints; ++i)
			//Print the respective information for each time point
			fprintf(data_file, "%12.5f, %12.5f, %14.5f \n", PositionTable[i].DL1.position, PositionTable[i].DL2.position, ValueTable[i]);
		fclose(data_file); //Close the file
	}
	else if(dimension == 1) //If only one delay line was scanned
	{
		fprintf(data_file, "Scan name: %s \n", scanName); //Print the name of scan
		//Print the delay line scanned:
		line == 1 ? fprintf(data_file, "Delay Line: MM3000 \n") : fprintf(data_file, "Delay Line: Mercury \n");

		fprintf(data_file, "Scanning time: %dm %ds \n\n", minutes, seconds); //Print the time it took 
		//Print the header for each column
		fprintf(data_file, "   DLN%d (%s),    Intensity (mV) \n\n", line, PositionTable[0].DL1.unit.c_str());

		for(int i=0; i < NumberOfPoints; ++i)
			//Print the respective information for each time point
			fprintf(data_file, "%12.5f, %14.5f \n", PositionTable[i].DL1.position, ValueTable[i]);
		fclose(data_file); //Close the file
	}
}


// If Read AUX box is checked, write both signal
void DataPrint::WritetoFile_NormalizedSignal(vector<pointGroup> PositionTable, double ValueTable[400], double AuxTable[400], double AvgNormWithAuxTable [200], double AvgNormWithAuxSquareTable [200], 
		double avgInt [200], double avgAux[200], double TwoDValueTable[200][120], double TwoDAuxTable[200][120], double TwoDNormWithAuxTable [200][120], double TwoDNormWithAuxSquareTable [200][120],
		int dimension, int auxflag, int line, CString type, double ScanTime, double totalMoveTime, double totalAverageTime, int totalReads)
{
	
	int NumberOfPoints = PositionTable.size(); 
	int seconds = fmod(ScanTime,60); //Seconds the scanning took
	int minutes = (ScanTime-seconds)/60; //Minutes the scanning took
	int moveSeconds = fmod(totalMoveTime,60); //Seconds the move took
	int moveMinutes = (totalMoveTime-moveSeconds)/60; //Minutes the move took
	int totalAverageSeconds = fmod(totalAverageTime, 60);//Seconds the read took
	int totalAverageMinutes = (totalAverageTime-totalAverageSeconds)/60;//Minutes the read took

	FILE * data_file;
	time_t now = time(0);
	struct tm tstruct;//a variable to save the date and time when scanning start
	char date[30]; //Buffer to save date information
	tstruct = *localtime(&now); //Function to get local time
	strftime(date, sizeof(date), "%m/%d/%Y (%H:%M:%S)", &tstruct); // Format time into Mon/Day/Year (hour:min:sec)
    data_file = fopen ("LockinData.csv","w"); //Open/create the file where to store data
	fprintf(data_file, "<Head Line> \n"); // Note to file user, below are the head lines, total of 10 lines
	fprintf(data_file, "Date Created: %s\n", date); //Print the time data created
	int k; //number of points taken at each delay position

	if(dimension == 1) //If only one delay line was scanned
	{
		
		fprintf(data_file, "Scan type: %s \n", type); //Print the type of scan
		fprintf(data_file, "Total move time: %dm %ds \n", moveMinutes, moveSeconds);
		fprintf(data_file, "Total average read time: %dm %ds \n", totalAverageMinutes, totalAverageSeconds);
		fprintf(data_file, "Scanning time: %dm %ds \n\n\n\n\n", minutes, seconds); //Print the time it took 

		//Print the header for each column
		fprintf(data_file, "<Subhead Line> \n"); // Note to file user, below are the subhead lines, total of 3 lines
		fprintf(data_file, "  DLN1(Mercury),    DLN2(MM3000),	tau1,	tau2,	tau+,	Xi, ");
		for (k = 1; k <= totalReads; ++k)
			{
				fprintf(data_file, "   Int %d ,    Aux %d,   Int/Aux %d,	Int/Aux^2 %d,   ", k, k, k, k);
				
			}
		fprintf(data_file, "<Int/Aux>,   <Int/Aux^2>,	<Int>,   <Aux>,   ");
		fprintf(data_file, "\n");
		fprintf(data_file, "   (%s),   (%s),   (%s),   (%s),   (%s),   %s,", PositionTable[0].DL1.unit.c_str(), PositionTable[0].DL2.unit.c_str(), PositionTable[0].tau1.unit.c_str(), PositionTable[0].tau2.unit.c_str(),PositionTable[0].tau1.unit.c_str(), "");
		for (k = 1; k <= totalReads+1; ++k)
			{
				fprintf(data_file, "   (mV),    (mV),   (mV),   (mV),   ");
			}
		fprintf(data_file, "\n");
		for(int i=0; i < NumberOfPoints; ++i)
			{		
				fprintf(data_file, "%12.5f,   %12.5f, %12.5f,   %12.5f,   %12.5f,   %12.3f,   ", PositionTable[i].DL1.position, PositionTable[i].DL2.position, PositionTable[i].tau1.position, PositionTable[i].tau2.position, PositionTable[i].tauPlus.position, PositionTable[i].Xi.position);
				//Print the respective information for each time point
				for (k = 1; k <= totalReads; ++k)
				{
				fprintf(data_file, "%14.5f,  %14.5f,  %14.5e,  %14.5e,	",  TwoDValueTable[i][k], TwoDAuxTable[i][k], TwoDNormWithAuxTable[i][k], TwoDNormWithAuxSquareTable[i][k]);
				}	
				fprintf(data_file, "%14.5e,     %14.5e,      %14.5f,     %14.5f,     ", AvgNormWithAuxTable[i], AvgNormWithAuxSquareTable[i], avgInt[i], avgAux[i]);
				fprintf(data_file, "\n");
			}
		fclose(data_file); //Close the file
	}
	else if(dimension == 2) //If two delay lines were scanned
	{
		fprintf(data_file, "Scan type: %s \n", type); //Print the type of scan
		fprintf(data_file, "Scanning time: %dm %ds \n\n", minutes, seconds); //Print the time it took 
		//Print the header for each column
		fprintf(data_file, "   DLN1 (%s),    DLN2 (%s),    Intensity (mV) \n", PositionTable[0].DL1.unit.c_str(), PositionTable[0].DL2.unit.c_str());

		for(int i=0; i < NumberOfPoints; ++i)
			//Print the respective information for each time point
			fprintf(data_file, "%12.8f, %12.8f, %14.8f \n", PositionTable[i].DL1.position, PositionTable[i].DL2.position, ValueTable[i]);
		fclose(data_file); //Close the file
	}
}
