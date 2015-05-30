//ScanSet.h: header file for the ScanSet class
/*This file contains the method to set a pointer array to the ScanBase class
and assign each pointer in the array to the subclass of the ScanBase class*/

#pragma once
#include "stdafx.h"
//Include here all scan type headers:
#include "ScanBase.h"
#include "ScanLinear1D.h"
#include "ScanLinear2D.h"
#include "ScanLog1D.h"
#include "ScanLog2D.h"
#include "ScanTauPlusLog.h"
#include "ScanTauPlusLinear.h"

class ScanSet
{
public:
	//standard constructor used to assign each scan type to one pointer
	//if new scan type has been defined in the ScanBase class, add another pointer in this method
	ScanSet();
	//method to get the ptr array for each scan
	ScanBase *getScan(CString scanName);
	//method to get the number of scans in the sub class
	int getNumberOfScan();
	//method to display the name of scan
	CString getDisplayName(int i);
	
private:
	ScanBase *ptr[10];//A pointer array for all the pointers to specific scans
	CString displayName[10];//An array to save all the scan names
	int numberOfScan;//number of scans in the sub class
};
