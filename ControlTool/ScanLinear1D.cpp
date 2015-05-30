// ScanLinear1D.cpp: implementation file for the ScanLinear1D class
// This is a class used as calculate the parameters needed for 1D linear scan
// This is a child class derived from the ScanBase class
#include "stdafx.h"
#include "ScanLinear1D.h"
#pragma once

//Standard constructor
ScanLinear1D::ScanLinear1D(CWnd* pParent /*=NULL*/)
{
	scanName = "1D Linear";
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

ScanLinear1D::~ScanLinear1D()
{
}

//virtual method to generate labeling for scan parameters
vector<CString> ScanLinear1D::GenerateParameterLegends()
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
// Performs scanning in increments of 'step' from 'start' to 'end'
// Parameters passed are read from the dialog box
// Returns a vector of pairs of points at which to scan.
vector<pointGroup> ScanLinear1D::GeneratePointspace(vector<double> parameters, int aSelector)
{
	start = parameters[0];
	end   = parameters[1];
	step  = parameters[2];
	otherDLposition = parameters[3];
	timeZeroDL1 = parameters[4];
	timeZeroDL2 = parameters[5];
	pointGroup points; //Create object 'points' of type pointGroup
	vector<pointGroup> scanPoints; //Create a vector of 'points' objects
	int i; //Placeholder used to iterate through array
	double numPts = fabs(end-start)/step; //Number of time points in the array
	double value = start; //Current value of the item in iteration

	if(start > end) step = -step; //Condition if the movement is backwards
	if(aSelector == 1)//Scan MM3000, record DL2 position and tau2 value
	{
		for(i=0; i <= numPts; ++i) //Iterates through the amount of points
		{
			points.DL1.position = otherDLposition; //Saves the delay line 1 position on the delay line element
			points.DL2.position = value; //Saves the delay line 2 position on the delay line element
			points.tau1.position = aMercury.getTau1AtPosition(points.DL1.position, timeZeroDL1);//convert delay position to tau1 value
			points.tau2.position = aMM3000.getTau2AtPosition(points.DL2.position, timeZeroDL2);//convert delay position to tau2 value
			scanPoints.push_back(points); //Adds the pair of points to the vector scanPoints
			value += step; //Increments by step
		}
	}
	else if(aSelector == 2)//Scan Mercury, record DL1 position and tau1 value
	{
		for(i=0; i <= numPts; ++i) //Iterates through the amount of points
		{
			points.DL1.position = value; //Saves the delay line 1 position on the delay line element
			points.DL2.position = otherDLposition; //Saves the delay line 2 position on the delay line element
			points.tau1.position = aMercury.getTau1AtPosition(points.DL1.position, timeZeroDL1);//convert delay position to tau1 value
			points.tau2.position = aMM3000.getTau2AtPosition(points.DL2.position, timeZeroDL2);//convert delay position to tau2 value
			scanPoints.push_back(points); //Adds the pair of points to the vector scanPoints
			value += step; //Increments by step
		}
	}
	return scanPoints; //Returns the vector containing pairs of calculated delay line position points
}
//method to get scan name
CString ScanLinear1D::getScanName()
{
	return scanName;
}
