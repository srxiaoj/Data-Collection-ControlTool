// ScanTauPlusLog.h: header file for the ScanTauPlusLog class
// This is a class used to calculate the parameters needed for an abstract scan
// tauPlus and Xi values are from user input, an abstract position table is generated
// and then converted to delay line position table
// This is a child class derived from the ScanBase class

#pragma once
#include "ScanBase.h"

class ScanTauPlusLog : public ScanBase
{
public:
	ScanTauPlusLog(CWnd* pParent = NULL); //Standard constructor
    virtual ~ScanTauPlusLog();

	//method to generate position names in terms of tau1, tau2
	vector<CString> GenerateParameterLegends();
	// Parameters passed are read from user input of tauPlus and Xi
	// convert to device position points
	// Returns array of position points
	vector<pointGroup> GeneratePointspace(vector<double> abstractParameters, int aSelector);
	//method to get the scan name
	CString getScanName();
private:
    double startTauPlus, endTauPlus, stepTauPlus; //Variables where values from the interface will be stored
	// Parameters passed are read from user input of tauPlus range and step
	// Returns array of abstract positions for tauPlus
	vector<pointGroup> GenerateAbstractPointspace(vector<double> abstractParameters);
};
