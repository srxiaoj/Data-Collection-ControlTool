//ScanSet.cpp: this is the implementation file for the ScanSet class
/*This file contains the method to set a pointer array to the ScanBase class
and assign each pointer in the array to the subclass of the ScanBase class*/
#pragma once
#include "stdafx.h"
#include "ScanSet.h"
//standard constructor used to assign each scan type to one pointer
//if new scan type has been defined in the ScanBase class, add another pointer in this method
ScanSet::ScanSet()
{
	ptr[0] = new ScanLinear1D(); //assign the first pointer to the subclass scanLinear1D
	ptr[1] = new ScanLinear2D();//assign the first pointer to the subclass scanLinear2D
	ptr[2] = new ScanLog1D();//assign the first pointer to the subclass scanLog1D
	ptr[3] = new ScanLog2D();//assign the first pointer to the subclass scanLog2D
	ptr[4] = new ScanTauPlusLog();//assign the first pointer to the subclass scanTauPlusLog
	ptr[5] = new ScanTauPlusLinear();//assign the first pointer to the subclass scanTauPlusLinear
	displayName[0] = "1D Linear";
	displayName[1] = "2D Linear";
	displayName[2] = "1D Log";
	displayName[3] = "2D Log";
	displayName[4] = "TauPlus Log Scan";
	displayName[5] = "TauPlus Linear Scan";
	numberOfScan = 6;
}

//method to get the ptr array for each scan
ScanBase *ScanSet::getScan(CString scanName)
{
	for (int i = 0; i<numberOfScan; i++)
	{
		//if the scan name from the user selection matches one on the display name list, return that pointer
		if(scanName.Compare(displayName[i]) == 0)
		{
			return ptr[i];
		}
	}
}

//method to get the number of scans in the sub class
int ScanSet::getNumberOfScan()
{
	return numberOfScan;
}


//method to display the name of scan
CString ScanSet::getDisplayName(int i)
{
	return displayName[i];
}
