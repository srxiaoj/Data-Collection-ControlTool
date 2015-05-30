
#pragma once
#include "ScanBase.h"

class ScanLog2D : public ScanBase
{
public:
	ScanLog2D(CWnd* pParent = NULL); //Standard constructor
	virtual ~ScanLog2D();

	//method to generate position names in terms of tau1, tau2
	vector<CString> GenerateParameterLegends();
    // Performs scanning in logarithmically calculated increments 
    // Parameters passed are read from the dialog box
    // Returns array of positions at which to scan
	vector<pointGroup> GeneratePointspace(vector<double> parameters, int aSelector);
	//method to get the scan name
	CString getScanName();
private:
	double start, end, step, start2, end2, step2; //Variables where values from the interface will be stored
};
