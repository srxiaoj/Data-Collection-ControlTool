
#pragma once
#include "stdafx.h"
#include "afxdialogex.h"
#include "ControlTool.h"
#include "ControlToolDlg.h"
#include "DataStructure.h"


class DataPrint
{
public:
	// Writes the arrays for position and intensities calculated
	// from data acquisition onto a text file lockinData.csv
	void WritetoFile(vector<pointGroup> PositionTable, double ValueTable[400], 
						int dimension, int auxflag, int line, CString scanName, double time);
	void WritetoFile_NormalizedSignal(vector<pointGroup> PositionTable, double ValueTable[400], double AuxTable[400], double AvgNormWithAuxTable [200], double AvgNormWithAuxSquareTable [200], 
		double avgInt [200], double avgAux[200], double TwoDValueTable[200][120], double TwoDAuxTable[200][120], double TwoDNormWithAuxTable [200][120], double TwoDNormWithAuxSquareTable [200][120],
		int dimension, int auxflag, int line, CString type, double ScanTime, double totalMoveTime, double totalAverageTime, int totalReads);

};
