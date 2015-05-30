// ScanLog1D.h: header file for the ScanLog1D class
// This is a class used as calculate the parameters needed for 1D log scan
// This is a child class derived from the ScanBase class
#pragma once
#include "ScanBase.h"

class ScanLog1D : public ScanBase
{
public:
    ScanLog1D(CWnd* pParent = NULL); //Standard constructor
	virtual ~ScanLog1D();

	//method to generate position names in terms of tau1, tau2
	vector<CString> GenerateParameterLegends();
    // Performs scanning in logarithmically calculated increments 
    // Parameters passed are read from the dialog box
    // Returns array of positions at which to scan
	vector<pointGroup> GeneratePointspace(vector<double> parameters, int aSelector);
	//method to get the scan name
	CString getScanName();
private:
    double start, end, step, otherDLposition; //Variables where values from the interface will be stored
};
