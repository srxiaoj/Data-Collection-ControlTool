// ScanLog1D.h: header file for the ScanLog1D class
// This is a class used as calculate the parameters needed for 1D log scan
// This is a child class derived from the ScanBase class
#include "stdafx.h"
#include "ScanLog1D.h"
#pragma once

//Standard constructor
ScanLog1D::ScanLog1D(CWnd* pParent /*=NULL*/)
{
	scanName = "1D Log";
	parameter0 = "Start";
	parameter1 = "End";
	parameter2 = "Step";
	parameter3 = "other DL position";
	parameter4 = "t0_DL1(mercury)";
	parameter5 = "t0_DL2(mm3000)";
	parameter6 = "";
	parameter7 = "";
	dimension = 1;
}

ScanLog1D::~ScanLog1D()
{
}

//virtual method to generate labeling for scan parameters
vector<CString> ScanLog1D::GenerateParameterLegends()
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
// Performs scanning in logarithmically calculated increments 
// Parameters passed are read from the dialog box
// Returns array of positions at which to scan
vector<pointGroup> ScanLog1D::GeneratePointspace(vector<double> parameters, int aSelector)
{
	start = parameters[0];
	end   = parameters[1];
	step  = parameters[2];
	otherDLposition = parameters[3];
	timeZeroDL1 = parameters[4];
	timeZeroDL2 = parameters[5];
	pointGroup points; //Declares object 'points' of type pointGroup
	vector<pointGroup> scanPoints; //Create a vector of 'points' objects

	double b = 20;
	double a = 1/(exp(1/b)-exp(-1/b));
	int x = -40;
	
	double value = start; //Current value of the item in iteration
	double logStep; //Logarithmic step to be taken through routine
	double numPts = 0; //Number of time points in the array (to be calculated)

//////////// Routine to calculate the total number of time points in the array ///////////
	double fvalue = start; //Dummy variable 
	int fx = -40; //Dummy variable
	if(start < end) 
		for(fvalue; fvalue<=end; fvalue=start+step*a*(exp(fx/b)-exp(-fx/b)),++fx) ++numPts;
    else 
		for(fvalue; fvalue>=end; fvalue=start-step*a*(exp(fx/b)-exp(-fx/b)),++fx) ++numPts;
/////////////////////////////////////////////////////////////////////////////////////////
	if(aSelector == 1)//Scan MM3000, record DL2 position and tau2 value
	{
		for(int i=0; i < numPts; ++i)
		{
			logStep = step*a*(exp(x/b)-exp(-x/b)); //Calculates current log step
			if(start < end) 
				value = start + logStep; //If scanning forward, increase by log step
			else 
				value = start - logStep; //If scanning backward, decrease by log step
			points.DL1.position = otherDLposition; //Saves the delay line 1 position on the delay line element
			points.DL2.position = value; //Saves the delay line 2 position on the delay line element
			points.tau1.position = aMercury.getTau1AtPosition(points.DL1.position, timeZeroDL1);//convert delay position to tau1 value
			points.tau2.position = aMM3000.getTau2AtPosition(points.DL2.position, timeZeroDL2);//convert delay position to tau2 value
			scanPoints.push_back(points); //Adds the pair of points to the vector scanPoints
			++x;
		}
	}
	else if(aSelector == 2)//Scan Mercury, record DL1 position and tau1 value
	{
		for(int i=0; i < numPts; ++i)
		{
			logStep = step*a*(exp(x/b)-exp(-x/b)); //Calculates current log step
			if(start < end) 
				value = start + logStep; //If scanning forward, increase by log step
			else 
				value = start - logStep; //If scanning backward, decrease by log step
			points.DL1.position = value; //Saves the delay line 1 position on the delay line element
			points.DL2.position = otherDLposition; //Saves the delay line 2 position on the delay line element
			points.tau1.position = aMercury.getTau1AtPosition(points.DL1.position, timeZeroDL1);//convert delay position to tau1 value
			points.tau2.position = aMM3000.getTau2AtPosition(points.DL2.position, timeZeroDL2);//convert delay position to tau2 value
			scanPoints.push_back(points); //Adds the pair of points to the vector scanPoints
			++x;
		}
	}
	return scanPoints; //Returns the vector containing pairs of calculated delay line position
}
//method to get scan name
CString ScanLog1D::getScanName()
{
	return scanName;
}
