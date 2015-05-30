
#include "stdafx.h"
#include "ScanLog2D.h"
#pragma once

//Standard constructor
ScanLog2D::ScanLog2D(CWnd* pParent /*=NULL*/)
{
	scanName = "2D Log";
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

ScanLog2D::~ScanLog2D()
{
}

//virtual method to generate labeling for scan parameters
vector<CString> ScanLog2D::GenerateParameterLegends()
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
vector<pointGroup> ScanLog2D::GeneratePointspace(vector<double> parameters, int aSelector)
{
	start  = parameters[0];
	end    = parameters[1];
	step   = parameters[2];
	start2 = parameters[3];
	end2   = parameters[4];
	step2  = parameters[5];

	pointGroup points; //Create object 'points' of type pointGroup
	vector<pointGroup> scanPoints; //Create a vector of 'points' objects

	double b = 20;
	double a = 1/(exp(1/b)-exp(-1/b));
	int x = -40;

	double value = start, value2 = start2; //Current value of the item in iteration for DLN1/DLN2
	double logStep, logStep2; //Logarithmic step for DLN1/DLN2 to be taken through routine
	double numPts = 0, numPts2 = 0; //Number of time points in the arrays (to be calculated):

//////////// Sub-Routine to calculate the number of time points for each DLN ////////////

	double fvalue = start, fvalue2 = start2; //Dummy variables 
	int fx = -40, fx2 = -40; //Dummy variables
	if(start < end) 
		for(fvalue; fvalue<=end; fvalue=start+step*a*(exp(fx/b)-exp(-fx/b)),++fx) ++numPts;
    else 
		for(fvalue; fvalue>=end; fvalue=start-step*a*(exp(fx/b)-exp(-fx/b)),++fx) ++numPts;
	if(start2 < end2) 
		for(fvalue2; fvalue2<=end2; fvalue2=start2+step2*a*(exp(fx2/b)-exp(-fx2/b)),++fx2) ++numPts2;
    else 
		for(fvalue2; fvalue2>=end2; fvalue2=start2-step2*a*(exp(fx2/b)-exp(-fx2/b)),++fx2) ++numPts2;
//////////////////////////////////////////////////////////////////////////////////////////////

	//Compares and sets the amount of points to iterate through
	double totalPts = (numPts >= numPts2) ? numPts : numPts2; 

	//Attach the first set of points (starting positions)
	points.DL1.position = start; 
	points.DL2.position = start2; 		
	scanPoints.push_back(points);

	for(int i=0; i < totalPts; ++i)
	{
		logStep = step*a*(exp(x/b)-exp(-x/b)); //Calculates current step
		logStep2 = step2*a*(exp(x/b)-exp(-x/b)); //Calculates current step2

		if(start < end) //If DLN1 scanning forward:
			//Increase by log step if end has not been reached, keep constant if it has
			(i < numPts) ? value = start + logStep : value = value;
		else //If scanning backward:
			//Decrease by log step if end has not been reached, keep constant if it has
			(i < numPts) ? value = start - logStep : value = value; 

		if(start2 < end2) //If DLN2 scanning forward:
			//Increase by log step2 if end2 has not been reached, keep constant if it has
			(i < numPts2) ? value2 = start2 + logStep2 : value2 = value2;
		else //If scanning backward:
			//Decrease by log step2 if end2 has not been reached, keep constant if it has
			(i < numPts2) ? value2 = start2 - logStep2 : value2 = value2; 

		//To make sure that no steps outside the starting value are taken:
		if ((start < end && value > start) || (start > end && value < start)) //DL1 in range
		{
			if ((start2 < end2 && value2 > start2) || (start2 > end2 && value2 < start2))//DL2 in range
			{
				points.DL1.position = value; 
				points.DL2.position = value2; 
				scanPoints.push_back(points);
			}
			else
			{
				points.DL1.position = value;
				points.DL2.position = start2;
				scanPoints.push_back(points);
			}
		}
		else //DL1 out of range
		{
			points.DL1.position = start;
			if ((start2 < end2 && value2 > start2) || (start2 > end2 && value2 < start2)) //DL2 in range
			{
				points.DL2.position = value2;
				scanPoints.push_back(points);
			}
		}
		++x;
	}
	return scanPoints;
}
//method to get scan name
CString ScanLog2D::getScanName()
{
	return scanName;
}
