
// Mercury.cpp: Implementation file for one of the two delay lines.
// Makes use of the Serial library for connections
// Refer to motion controller literature for general command set description:
// https://twiki.cern.ch/twiki/bin/viewfile/ILCBDSColl/Phase2Preparations?rev=1;filename=PI_Mercury_GCS_DLL_MS154E101.pdf


/* Implements the class Mercury and defines the functions used for:
	Movement command (absolute and relative)
	Movement detection
	Device initialization
	Unit conversion
	Position detection 
	Error-generation
	Velocity change
	Velocity detection
*/

#include "stdafx.h"
#include "Mercury.h"
#pragma once

Mercury::Mercury(void):DeviceDependent()
{
	deviceName = "DelayLineMercury";
	ID = -1;
	delayTimeInMs = 500;
}
Mercury::Mercury(int portNumber, int Brate):DeviceDependent()
{
	deviceName = "DelayLineMercury";
	ID = -1;
	delayTimeInMs = 500;
}
Mercury::~Mercury(void)
{
}

//********************* Initialization routine *********************

// Sets the zero position of the device in device units
// Starts the data connection.
// Returns true if the initialization was sucessful
bool Mercury::Initialize()
{
	CString* buffer;
	int maxlen = 6;
	char* Error;
	//ID = Mercury_InterfaceSetupDlg("PI\\MercuryDLLSample");
	ID = Mercury_ConnectRS232(getPortNumber(), 9600);  // Start the connection return a positive number if sucssed
	buffer = new CString[maxlen];
	if (ID < 0 && !Mercury_IsConnected(ID))  // ID return to handle multiple connection
	{
		Error = PrintERROR();
		return false;
	}
	char szIDN[1000];
	  if (!Mercury_qIDN(ID, szIDN, 999)) // Get identification string of the controller.
      {
		printf("qIDN() failed!\n");
		Error = PrintERROR();
		return false;
	  }
	  printf("ID of PI Mercury = \"%s\"\n\n", szIDN);
	  buffer[0]=szIDN;
	 // state[0]=szIDN; // Set the identification of this controller in the array
    
	 if (!Mercury_qSAI_ALL(ID, szAxes, 17)) // Get all possible axes, and not only all connected	
	 {
		 //buffer = PrintERROR();  
         Error = PrintERROR();       
		 return false;
	 }
	 if ( strcmp(szAxes, "A") != 0)                      //  and configured axes as returned by the Mercury_qSAI       
	 {                                                    // function. Each character in the returned string is an axis identifier for one possible axis.
		 printf("we do expect axes \"A\" connected\n");
		 // goto error;
		 Error = PrintERROR();
		 return false;
	 }
	 buffer[1]=szAxes;
	 // AXES=szAxes;
	 // state[1]=szAxes; // Set the identification of this controller in the array
	 size_t nrAxes = strlen(szAxes); // Return the lengh of the string, sZaxes normally will be 1
	 char szStages[1000] = "M-414.3PD\n";
	 if (!Mercury_CST(ID, szAxes, szStages) ) // Set the types of the stages connected to szAxes.
	 {
	    Error = PrintERROR();
		return false;		  
	 }
	 if (!Mercury_qCST(ID, szAxes, szStages, 999) )  // get the types of the stages connected to szAxes.
	 {
		Error = PrintERROR();
		return false;			
	 }
     printf("connected stages: \n\"%s\"\n", szStages);
     buffer[2]=szStages;
	 // state[2]= szStages;

	 if (!Mercury_INI(ID, szAxes)) // Initialize szAxes: resets motion control chip for the axis
	 {
		Error = PrintERROR();
	    return false;      
	 }
     BOOL hasReference[16];   // will receive a BOOLean value
	 BOOL hasLimitSwitch[16]; // will receive a BOOLean value

	 if (!Mercury_qREF(ID, szAxes, hasReference) ) // Check if the given axes have reference 
	 {
		 Error = PrintERROR();
	     return false;
	 }
     if (!Mercury_qLIM(ID, szAxes, hasLimitSwitch) ) // Check if the given axes have limit switches
	 {
		 Error = PrintERROR();
	     return false;
	 }

	 char sRefAxes[17];
	 char sLimAxes[17];
	 int nrRefAxes=0;
	 int nrLimAxes=0;

	 for (size_t i=0; i<nrAxes; i++)
			{
				if (hasReference[i])
				{
					sRefAxes[nrRefAxes] = szAxes[i];
					nrRefAxes++;
				}
				else if (hasLimitSwitch[i])
				{
					sLimAxes[nrLimAxes] = szAxes[i];
					nrLimAxes++;
				}
			}
	sRefAxes[nrRefAxes] = '\0';
	sLimAxes[nrLimAxes] = '\0';
	printf("%d axes with reference: %s\n", nrRefAxes, sRefAxes);
	printf("%d axes with limit switches: %s\n", nrLimAxes, sLimAxes);

	if (nrRefAxes > 0)
	{
		printf("Start move to reference for axes %s\n", sRefAxes);
		if (!Mercury_REF(ID, sRefAxes) )//For each of the axes in szAxes.turn, reset soft
		{
			Error = PrintERROR();// limits and home position, move the axis to its reference switch				
			return false ;
		}			
		BOOL bReferencing = false;
		do
		{
			if (!Mercury_IsReferencing(ID, NULL, &bReferencing))
			{
				Error = PrintERROR();
	            return false;  //Check if axis is busy referencing.
			}
			Sleep(500);
			printf(".");
		} while (bReferencing);
		printf("\n");
	}

	if (nrLimAxes > 0)
	{
		printf("Start move to negative limit switch for axes %s\n", sLimAxes);
		if (!Mercury_MNL(ID, sLimAxes) )
		{
			Error = PrintERROR();
			return false;
		}
		BOOL bReferencing = false;
		do
		{
			if (!Mercury_IsReferencing(ID, NULL, &bReferencing))
			{
				Error = PrintERROR();
				return false;
			}
		Sleep(500);
		printf(".");
		} while (bReferencing);
		printf("\n");
	}

	BOOL refOK[16];
	if (!Mercury_IsReferenceOK(ID, szAxes, refOK) ) // Check the reference state of the given axes.
	{
		Error = PrintERROR();
		return false; // Check the reference state of the given axes.
	}
	int nrNotOK = 0;
	for (int i=0; i<nrAxes; i++)
	{
		printf("axis %c: reference %s\n", szAxes[i], refOK[i] ? "OK" : "not OK");
		if (refOK[i] != 1)
			nrNotOK++;
	}
	if (nrNotOK >0)
	{
		printf("some axes not referenced!\n");
		return false;
	}
	double negRangeLimit[16];
	double posRangeLimit[16];
	double pos[16];
	if (!Mercury_qTMN(ID, szAxes, negRangeLimit) )
	{
		// Get the low end of travel range of szAxes in physical units and relative to the current home position.
		Error = PrintERROR();
	    return false;
	}
	if (!Mercury_qTMX(ID, szAxes, posRangeLimit) )
	{
	    // Get the high end of the travel range of szAxes in physical units and relative to the current home position.
		Error = PrintERROR();
	    return false;
	}
	if (!Mercury_qPOS(ID, szAxes, pos) )
	{
		// Get the positions of szAxes.
		Error = PrintERROR();
	    return false;	
	}
    double target[16];
	for (int i=0; i<nrAxes; i++)
	{
		target[i] = ((std::min)(posRangeLimit[i],10000.0) + (std::max)(negRangeLimit[i], -10000.0))/2;
		printf("axis %c: Range %g - %g, current position: %g, move to %g\n",
			szAxes[i], negRangeLimit[i], posRangeLimit[i], pos[i], target[i]);
	
	char str[80] ;
	sprintf(str,"%d",negRangeLimit[i]);// converts to decimal base string.	
	buffer[3]=str;
//	state[3]=str;
    sprintf(str,"%d",posRangeLimit[i]);
//	state[4]=str;
	buffer[4]=str;
	}

    if (Mercury_MOV(ID, szAxes, target)!=1)
    {
		// Move szAxes to absolute position.
	    Error = PrintERROR();
	    return false;
    }
	BOOL bMoving = false;
	do
	{
		if (!Mercury_IsMoving(ID, NULL, &bMoving))
		{
		 // Check if szAxes are moving. If an axis is moving, the 
		 // corresponding element of the array will be TRUE, otherwise FALSE.
			Error = PrintERROR();
            return false;
		}
		Sleep(500);                                           
		printf(".");
	} while (bMoving);
	printf("\n");
	
	if (!Mercury_qPOS(ID, szAxes, pos) )
	{
		Error = PrintERROR();
		return false;
	}
	for (int i=0; i<nrAxes; i++)
	{
	target[i] = target[i]/2;
	printf("axis %c: current position: %g, mvr %g\n",
					szAxes[i], pos[i], target[i]);	
	char str[80] ;
	sprintf(str,"%d",pos[i]);
	buffer[5]=str;
//	state[5]=str;  // Set in the array the actual position of the DLN
	}
	if (Mercury_DFH(ID, szAxes)!=1) // Makes current positions of szAxes the new home position
	{
		Error = PrintERROR();
		return false;
	}
	return true;
}
//********************* End of Initialization routine *********************


//********************* Moving command routines *********************

// Moves the device to the specified position and return
// the actual position of the device.
// The movement is absolute
double Mercury::GoToInDeviceUnit(double position)
{
	if(isSafeMove(ConvertToUserUnit(position, "ps")))
	{
		// validate the unit here
		double finalPosition;
		double * pdValarray = &position;		
	
		if (Mercury_MOV(ID, szAxes, pdValarray)!=1)
		{
			PrintERROR(); // error happened 
			return -5000000;  // invalid value is returned
		}
		while( IsMoving());
		Sleep(20); 
		return getActualPosition(); // Return the actual value where the DLN stopped
	}
}

// Moves the device to the specified position and return
// the actual position of the device.
// The movement is relative to current position
double Mercury::MoveInDeviceUnit(double position)
{
	if(isSafeMove(ConvertToUserUnit(position, "ps")))
	{
		// validate the unit here
		double finalPosition;
		double * pdValarray = &position;		
	
		if (Mercury_MVR (ID, szAxes, pdValarray)!=1)
		{
			PrintERROR(); // error happened 
			return -5000000;  // invalid value is returned
		}
		while( IsMoving());
		return getActualPosition(); // return the atual value where the DLN stopped
	}
}

//********************* Unit conversion function (to device units) *********************
//Convert user unit to device unit
//take input as user unit and return a number in device unit
double Mercury::ConvertToDeviceUnit(double number, string unit)
{
	if(unit.compare("ps")|| unit.compare("PS")|| unit.compare("pico")|| unit.compare("PICO"))
	{
		return number*3/20;
	}
	if(unit.compare("mm")|| unit.compare("MM")|| unit.compare("ms")|| unit.compare("MS"))
	{
		return number;
	}
	return -0;
}

//********************* Unit de-conversion function (from device units) *********************
//Convert device unit to user unit
//take input as device unit and return a number in user unit
double Mercury::ConvertToUserUnit(double number, string unit)
{
	if(unit.compare("ps")|| unit.compare("PS")|| unit.compare("pico")|| unit.compare("PICO"))
	{
		return number*20/3;
	}
	if(unit.compare("mm")|| unit.compare("MM")|| unit.compare("ms")|| unit.compare("MS"))
	{
		return number;
	}
	return -0;
}

//take the tau1(relative position) and timeZero as input
//output the absolute position based on DL1position = timeZero + tau1, unit in "ps"
double Mercury::getPositionAtTau1(double aTau1, double timeZero)
{
	return timeZero + aTau1;
}
//take the absolute position and timeZero as input
//output the tau1 based on tau1 = DL1position - timeZero, unit in "ps"
double Mercury::getTau1AtPosition(double aPosition, double timeZero)
{
	return aPosition - timeZero;
}

//********************* Moving detection routine *********************

// Checks to see if the device is moving
// Returns false if there is no movement
bool Mercury::IsMoving()
{
    BOOL bMoving = false;
	// bool stillMoving = true;
	do
	{
		if (!Mercury_IsMoving(ID, NULL, &bMoving))
		{
		 // Check if szAxes are moving. If an axis is moving, the 
		 // corresponding element of the array will be TRUE, otherwise FALSE.
			PrintERROR();
			return false; 
		}
		Sleep(delayTimeInMs);                                           
		printf(".");         
	} while (bMoving);

	return false ;
}
//take a delay position, return whether it is out of range
bool Mercury::isSafeMove(double delayPositionInUserUnit)
{
	if(delayPositionInUserUnit >= -1000.0 && delayPositionInUserUnit <= 1000.0)
		return true;
	else
		return false;
}

//********************* Position detection routine *********************

// Returns the actual position of the device
double Mercury::getActualPosition()
{
     double pos[16];
	 if(Mercury_qPOS(ID,szAxes,pos )!=1)
	 {
        PrintERROR();
	 }
     return pos[0];   
}

//********************* Velocity setting/getting routines *********************

// Returns the velocity of this delay line
double Mercury::getVelocity()
{
	double pdValarray[16];		
	
	if (Mercury_qVEL(ID, szAxes, pdValarray)!=1)
	{
        PrintERROR(); // error happened 		  
	}
	//Velocity was obtained sucessfully
	Sleep(delayTimeInMs);

	return pdValarray[0];
}

// Changes the velocity of this delay line to the parameter given 
void Mercury::setVelocity(double theVelocity)
{
	if(theVelocity > 0)
	{
		double * pdValarray = &theVelocity;		
	
		if (Mercury_VEL (ID, szAxes, pdValarray)!=1)
		{
			PrintERROR(); //Error happened 		  
		}
		//Velocity was changed sucessfully
		Sleep(delayTimeInMs);
	}
}

//********************* Error-generating routine *********************
//print out different kinds of warnings according to the error types
char* Mercury::PrintERROR()
{
	int err = Mercury_GetError(ID); // Get the error state of the controller.
	char errMsg[1000];
		if (err != 0)
		{			
			if (Mercury_TranslateError(err, errMsg, 999))
			{
				printf("Error %d: %s\n", err, errMsg);
				return errMsg;
			}
			else
			{
				printf("Cannot translate error %d\n", err);
				char errMsg1[24] = "unkown error ocurred...";
				return errMsg1;
			}
		}
		Mercury_CloseConnection(ID);
		char errMsg2[24] = "false positive error...";
     	return errMsg2; 
}

