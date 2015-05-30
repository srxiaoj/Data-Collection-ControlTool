// ScanLinear1D.h: header file for the ScanLinear1D class
// This is a class used as calculate the parameters needed for 1D linear scan
// This is a child class derived from the ScanBase class
#pragma once
#include "ScanBase.h"

class ScanLinear1D : public ScanBase
{
public:
    ScanLinear1D(CWnd* pParent = NULL); //Standard constructor
	virtual ~ScanLinear1D();

	//method to generate position names in terms of tau1, tau2
	vector<CString> GenerateParameterLegends();
    // Performs scanning in increments of 'step' from 'start' to 'end'
    // Parameters passed are read from the dialog box
    // Returns a vector of delayline positions at which to scan.
	vector<pointGroup> GeneratePointspace(vector<double> parameters, int aSelector);
	//method to get the scan name
	CString getScanName();
private:
    double start, end, step, otherDLposition; //Variables where values from the interface will be stored
};
