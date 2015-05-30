// ScanBase.cpp: implementation file for the ScanBase class
// This is a class used to define parameters needed for different types of scans and calculate position points
#include "stdafx.h"
#include "ScanBase.h"

#pragma once

//Standard constructor
ScanBase::ScanBase(void)
{
	scanName = "No selection";
	dimension = 0;
}

ScanBase::~ScanBase(void)
{
}

//virtual method to generate labeling for scan parameters
vector<CString> ScanBase::GenerateParameterLegends()
{
	vector<CString> legends;
	legends.push_back(parameter0);
	legends.push_back(parameter1);
	legends.push_back(parameter2);
	legends.push_back(parameter3);
	legends.push_back(parameter4);
	legends.push_back(parameter5);
	legends.push_back(parameter6);
	legends.push_back(parameter7);
	return legends;
}
//method to get scan name
CString ScanBase:: getScanName()
{
	return scanName;
}

// Performs scanning in increments of 'step' from 'start' to 'end'
// Parameters passed are read from the dialog box
// Returns a vector of pairs of points at which to scan.
vector<pointGroup> ScanBase::GeneratePointspace(vector<double> parameters, int aSelector)
{
	vector<pointGroup> PositionPoints;
	return PositionPoints;
}
// method to generate abstract position table of tauPlus and Xi
vector<pointGroup> ScanBase::GenerateAbstractPointspace(vector<double> abstractParameters)
{
	vector<pointGroup> AbstractPositionPoints;
	return AbstractPositionPoints;
}
//method to convert abstract position table of tauPlus and Xi to position table of tau1 and tau2
//Return arrays of both tau1 and tau2 position
vector<pointGroup> ScanBase::ConvertToDevicePosition(vector<pointGroup> abstractPositionPoints, double xiValue, double timeZeroDL2, double timeZeroDL1)
{
    pointGroup point;
	vector<pointGroup> positionPoints; //array to save MM3000 and Mercury positions
	for (int i = 0; i < abstractPositionPoints.size(); i++)
	{
	    point.tau1.position = abstractPositionPoints[i].tauPlus.position*(1-xiValue)/2; //get the t1 and t2 from tPlus and Xi
		point.tau2.position = abstractPositionPoints[i].tauPlus.position*(1+xiValue)/2;
		point.DL2.position = aMM3000.getPositionAtTau2(point.tau2.position, timeZeroDL2);//Tau2 = timeZeroDL2 - DL2.position
		point.DL1.position = aMercury.getPositionAtTau1(point.tau1.position, timeZeroDL1);//Tau1 = DL1.position - timeZeroDL1
		point.tauPlus.position = abstractPositionPoints[i].tauPlus.position;//store the tauPlus
		point.Xi.position = xiValue;//store the Xi
		positionPoints.push_back(point);
	}
    return positionPoints;
}

//method to check whether a scan is safe or not
bool ScanBase::isSafeScan(vector<double> parameters, int aSelector)
{
	vector<pointGroup> positionPoints = GeneratePointspace(parameters, aSelector);
	int last = positionPoints.size()-1;
	int lastDL1Position = positionPoints[last].DL1.position;
	int lastDL2Position = positionPoints[last].DL2.position;
	int firstDL1Position = positionPoints[0].DL1.position;
	int firstDL2Position = positionPoints[0].DL2.position;
	if(aMM3000.isSafeMove(firstDL2Position) && aMM3000.isSafeMove(lastDL2Position) && aMercury.isSafeMove(firstDL1Position) && aMercury.isSafeMove(lastDL1Position))
		return true;
	else
		return false;
}
