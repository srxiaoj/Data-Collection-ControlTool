// ScanBase.h: header file for the ScanBase class
// This is a class used to define parameters needed for different types of scans and calculate position points

#pragma once
#include "stdafx.h"
//#include "ControlToolDlg.h"
#include "DataStructure.h"
#include "Mercury.h"
#include "MM3000.h"
#include "Lockin.h"
/*#include "ScanLinear1D.h"
#include "ScanLinear2D.h"
#include "ScanLog1D.h"
#include "ScanLog2D.h"
#include "ScanAbstract.h"*/ //don't include the head of subclass in the abstract class

class ScanBase
{
public:
	ScanBase(void);//Standard constructor
	~ScanBase(void);
	int dimension; //Variable for how many devices this scan is intended to affect
	//method to get scan name
	virtual CString getScanName();
    //method to generate position names in terms of tau1, tau2
	virtual vector<CString> GenerateParameterLegends();
    //virtual method to generate delay line position table of tau1, tau2
	virtual vector<pointGroup> GeneratePointspace(vector<double> parameters, int aSelector);
    //virtual method to generate abstract position table of tauPlus and Xi
	virtual vector<pointGroup> GenerateAbstractPointspace(vector<double> abstractParameters);
    //method to convert abstract position table of tauPlus and Xi to position table of tau1 and tau2
    virtual vector<pointGroup> ConvertToDevicePosition(vector<pointGroup> abstractPositionPoints, 
		double xiValue, double timeZeroDL2, double timeZeroDL1);
	//method to check whether a scan is safe or not
	virtual bool isSafeScan(vector<double> parameters, int aSelector);
protected:
	CString scanName; //String to describe the scanName of scan being performed
	double Xi;
	double timeZeroDL2; //timeZeroMM3000
	double timeZeroDL1; // timeZeroMercury
	Mercury aMercury;//instance for dl1
	MM3000 aMM3000;//instance for dl2
	//Strings to give relevant names to generic parameters:
	CString parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6, parameter7;
	//Initialize scanning label
	vector<pointGroup> pointspace; //Vector where the scanning pointspace will be written
};

