// ScanTauPlusLinear.cpp: implemetation file for the ScanTauPlusLinear class
// This is a class used to calculate the parameters needed for an abstract scan
// tauPlus and Xi values are from user input, an abstract position table is generated
// and then converted to delay line position table
// This is a child class derived from the ScanBase class

#include "stdafx.h"
#include "ScanTauPlusLinear.h"
#pragma once

//Standard constructor
ScanTauPlusLinear::ScanTauPlusLinear(CWnd* pParent /*=NULL*/)
{
	scanName = "TauPlus Linear Scan";
	parameter0 = "Start (t+)";
	parameter1 = "End (t+)";
	parameter2 = "Step (t+)";
	parameter3 = "Xi";
	parameter4 = "t0_DL1(mercury)";
	parameter5 = "t0_DL2(mm3000)";
	parameter6 = "";
	parameter7 = "";
	dimension = 1;
}

ScanTauPlusLinear::~ScanTauPlusLinear()
{
}

//virtual method to generate labeling for scan parameters
vector<CString> ScanTauPlusLinear::GenerateParameterLegends()
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
// Parameters passed are read from user input of tauPlus range and step
// Returns array of abstract positions for tauPlus
vector<pointGroup> ScanTauPlusLinear::GenerateAbstractPointspace(vector<double> abstractParameters)
{
    start = abstractParameters[0];
	end   = abstractParameters[1];
	step  = abstractParameters[2];

	pointGroup points; //Create object 'points' of type pointGroup
	vector<pointGroup> scanPoints; //Create a vector of 'points' objects

	int i; //Placeholder used to iterate through array
	double numPts = fabs(end-start)/step; //Number of time points in the array
	double value = start; //Current value of the item in iteration

	if(start > end) step = -step; //Condition if the movement is backwards

	for(i=0; i <= numPts; ++i) //Iterates through the amount of points
	{
		points.tauPlus.position = value; //Saves the current delay line position on the first delay line element
		scanPoints.push_back(points); //Adds the pair of points to the vector scanPoints
		value += step; //Increments by step
	}
	return scanPoints; //Returns the vector containing pairs of calculated delay line position points 
}
// Parameters passed are read from user input of tauPlus and Xi
// convert to device position points
// Returns array of position points
vector<pointGroup> ScanTauPlusLinear::GeneratePointspace(vector<double> abstractParameters, int aSelector)
{
	Xi = abstractParameters[3];
	timeZeroDL1 = abstractParameters[4];
	timeZeroDL2 = abstractParameters[5];
	vector<pointGroup> abstractPositionPoints = GenerateAbstractPointspace(abstractParameters);// generate array of abstract positions
	vector<pointGroup> positionPoints = ConvertToDevicePosition(abstractPositionPoints, Xi, timeZeroDL2, timeZeroDL1);// convert to device position points
	return positionPoints;
}

//method to get scan name
CString ScanTauPlusLinear::getScanName()
{
	return scanName;
}
