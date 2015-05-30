
#pragma once
#include "ScanBase.h"

class ScanLinear2D : public ScanBase
{
public:
	ScanLinear2D(CWnd* pParent = NULL); //Standard constructor
	virtual ~ScanLinear2D();
	
	//method to generate position names in terms of tau1, tau2
	vector<CString> GenerateParameterLegends();
    // Performs scanning in increments of 'step' from 'start' to 'end'
    // Parameters passed are read from the dialog box
    // Returns a vector of pairs of points at which to scan.
	vector<pointGroup> GeneratePointspace(vector<double> parameters, int aSelector);
	//method to get the scan name
	CString getScanName();
private:
	double start, end, step, start2, end2, step2; //Variables where values from the interface will be stored
};
