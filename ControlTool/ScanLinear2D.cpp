
#include "stdafx.h"
#include "ScanLinear2D.h"
#pragma once

//Standard constructor
ScanLinear2D::ScanLinear2D(CWnd* pParent /*=NULL*/)
{
	scanName = "2D Linear";
	parameter0 = "Start 1";
	parameter1 = "End 1";
	parameter2 = "Step 1";
	parameter3 = "Start 2";
	parameter4 = "End 2";
	parameter5 = "Step 2";
	parameter6 = "";
	parameter7 = "";
	dimension = 2;
}

ScanLinear2D::~ScanLinear2D()
{
}

//virtual method to generate labeling for scan parameters
vector<CString> ScanLinear2D::GenerateParameterLegends()
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
vector<pointGroup> ScanLinear2D::GeneratePointspace(vector<double> parameters, int aSelector)
{
	start  = parameters[0];
	end    = parameters[1];
	step   = parameters[2];
	start2 = parameters[3];
	end2   = parameters[4];
	step2  = parameters[5];

	pointGroup points; //Create object 'points' of type pointGroup
	vector<pointGroup> scanPoints; //Create a vector of 'points' objects

	int i, j; //Placeholder used to iterate through array
	double numPts = fabs(end-start)/step; //Number of time points in the first array
	double numPts2 = fabs(end2-start2)/step2; //Number of time points in the second array
	double value = start; //Current value of the item in iteration for the first dln
	double value2 = start2; //Current value of the item in iteration for the second dln

	//Compares and sets the amount of points to iterate through
	double totalPts = (numPts >= numPts2) ? numPts : numPts2; 

	if(start > end) step = -step; //Condition if the movement for dln1 is backwards
	if(start2 > end2) step2 = -step2; //Condition if the movement for dln2 is backwards

	for(i=0; i <= totalPts; ++i) //Iterates through the larger amount of points
	{
		points.DL1.position = value; //Saves the current time points on the first delay line element
		points.DL2.position = value2; //Saves the current time points on the second delay line element
		scanPoints.push_back(points); //Adds the pair of points to the vector scanPoints
		//Increments by step if end has not been reached, keeps constant if it has
		(i < numPts) ? value += step : value = value;
		//Increments by step2 if end2 has not been reached, keeps constant if it has
		(i < numPts2) ? value2 += step2 : value2 = value2; 
	}
	return scanPoints; //Returns the vector containing pairs of calculated time points 
}
//method to get scan name
CString ScanLinear2D::getScanName()
{
	return scanName;
}
