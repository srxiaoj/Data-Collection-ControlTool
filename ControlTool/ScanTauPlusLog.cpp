// ScanTauPlusLog.cpp: implemetation file for the ScanTauPlusLog class
// This is a class used to calculate the parameters needed for an abstract scan
// tauPlus and Xi values are from user input, an abstract position table is generated
// and then converted to delay line position table
// This is a child class derived from the ScanBase class

#include "stdafx.h"
#include "ScanTauPlusLog.h"
#pragma once

//Standard constructor
ScanTauPlusLog::ScanTauPlusLog(CWnd* pParent /*=NULL*/)
{
	scanName = "TauPlus Log Scan";
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

ScanTauPlusLog::~ScanTauPlusLog()
{
}

//virtual method to generate labeling for scan parameters
vector<CString> ScanTauPlusLog::GenerateParameterLegends()
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
vector<pointGroup> ScanTauPlusLog::GenerateAbstractPointspace(vector<double> abstractParameters)
{
    startTauPlus = abstractParameters[0];
	endTauPlus   = abstractParameters[1];
	stepTauPlus  = abstractParameters[2];
	

	pointGroup points; //Declares object 'points' of type pointGroup
	vector<pointGroup> scanPoints; //Create a vector of 'points' objects

	double b = 20;
	double a = 1/(exp(1/b)-exp(-1/b));
	int x = -40;
	
	double value = startTauPlus; //Current value of the item in iteration
	double logStep; //Logarithmic step to be taken through routine
	double numPts = 0; //Number of time points in the array (to be calculated)

//////////// Routine to calculate the total number of time points in the array ///////////
	double fvalue = startTauPlus; //Dummy variable 
	int fx = -40; //Dummy variable
	if(startTauPlus < endTauPlus) 
		for(fvalue; fvalue<=endTauPlus; fvalue=startTauPlus+stepTauPlus*a*(exp(fx/b)-exp(-fx/b)),++fx) ++numPts;
    else 
		for(fvalue; fvalue>=endTauPlus; fvalue=startTauPlus-stepTauPlus*a*(exp(fx/b)-exp(-fx/b)),++fx) ++numPts;
/////////////////////////////////////////////////////////////////////////////////////////

	for(int i=0; i < numPts; ++i)
	{
		logStep = stepTauPlus*a*(exp(x/b)-exp(-x/b)); //Calculates current log step
		if(startTauPlus < endTauPlus) 
			value = startTauPlus + logStep; //If scanning forward, increase by log step
		else 
			value = startTauPlus - logStep; //If scanning backward, decrease by log step
		points.tauPlus.position = value; //Saves the current delay line position on the delay line element
		scanPoints.push_back(points); //Adds the pair of points to the vector scanPoints
		++x;
	}
	return scanPoints; //Returns the vector containing pairs of calculated delay line position 
}
// Parameters passed are read from user input of tauPlus and Xi
// convert to device position points
// Returns array of position points
vector<pointGroup> ScanTauPlusLog::GeneratePointspace(vector<double> abstractParameters, int aSelector)
{
	Xi = abstractParameters[3];
	timeZeroDL1 = abstractParameters[4];
	timeZeroDL2 = abstractParameters[5];
	vector<pointGroup> abstractPositionPoints = GenerateAbstractPointspace(abstractParameters);// generate array of abstract positions
	vector<pointGroup> positionPoints = ConvertToDevicePosition(abstractPositionPoints, Xi, timeZeroDL2, timeZeroDL1);// convert to device position points
	return positionPoints;
}

//method to get scan name
CString ScanTauPlusLog::getScanName()
{
	return scanName;
}
