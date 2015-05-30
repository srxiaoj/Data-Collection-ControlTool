
// MM3000.cpp: Implementation file for one of the two delay lines.
// Makes use of the Serial library for connections
// Refer to motion controller literature for native command set description:
// https://twiki.cern.ch/twiki/bin/viewfile/ILCBDSColl/Phase2Preparations?rev=1;filename=MercuryNativeCommands_MS176E101.pdf


/* Implements the class MM3000 and defines the functions used for:
	Movement command (absolute and relative)
	Movement detection
	Device initialization
	Unit conversion
	Position detection 
	Serial connection test
	Command sending/reading
	Velocity change/detection
	Acceleration change/detection
*/

#include "stdafx.h"
#include "MM3000.h"
#pragma once

MM3000::MM3000(void)
{
	deviceName = "DelayLineMM3000";
	baudRate = 9600;
	delayTimeInMs = 500;
	Axis = 2;
}
MM3000::MM3000(int theBaudRate):DeviceDependent()
{
	deviceName = "DelayLineMM3000";
	baudRate = theBaudRate;
	delayTimeInMs = 500;
	Axis = 2;
}

MM3000::~MM3000(void)
{
}

// Sets the zero position of the device in device units
// Starts the data connection.
// Returns true if the initialization was sucessful
bool MM3000::Initialize()
{
    if(startSerialConnection())
    {
		int size = 6;	
		int status;
		int com_port = DeviceDependent::getPortNumber();
		CString* temp = new CString[size];	
		status = 0;
		status += WriteData("1va20000");
		temp[0]="20000"; // initialize dln1 with a velocity of 20000
		Velocity[0]=20000;
		Sleep(delayTimeInMs);
		status += WriteData("1ac20000");
		temp[1]="20000"; // initialize dnl with an aceleration of 20000
		Aceleration[0]=20000;
		Sleep(delayTimeInMs);
		status += WriteData("1pa0");
		temp[2]="0"; //  sent dln1 to the zer0 position
		status += WriteData("2va20000");
		temp[3]="20000"; // initialize dln2 with a velocity of 20000
		Velocity[1]=20000;
		Sleep(delayTimeInMs);
		status += WriteData("2ac20000");
		temp[4]="20000"; // initialize dln2 with an aceleration of 20000
		Aceleration[1]=20000;
		Sleep(delayTimeInMs);
		status += WriteData("2pa0");
		temp[5]="0"; // sent dln2 to the zero position 	
		Sleep(delayTimeInMs);
		return true;
	}
    return false;
}

//********************* Moving command routines *********************

// Moves the device to the specified position and return
// the position of the device in device units.
// The movement is absolute
double MM3000::GoToInDeviceUnit(double position)
{
	if(isSafeMove(ConvertToUserUnit(position, "ps")))
	{
		validatePosition = 0;
		int curr_axis = getAxis(); //Used to select axis is being used (currently 2)
		int p =(int)position;
		char command[30]; //Place holders for the commands to be sent
		char subcommand[30];
		//int wait = 1;
		sprintf(command,"%d%s%ld",curr_axis,"pa", p); //Writes the command "pa" to direct position absolute
		//sprintf(subcommand,"%d%s%ld",curr_axis,"ws", wait); /*Writes the command "ws" to tell the motor to wait for
		//													 motion to complete for 500ms to execute next command */	
		WriteData(command); //Sends move command					 
		//WriteData(subcommand); //Sends wait command
		while(IsMoving()) //Sleep(1000); //Check if device is still moving every 1000ms			    
		Sleep(20); 
		return getActualPosition(); //Return the current position when device stops
		validatePosition = 1;
	}
}

// Moves the device to the specified position and return
// the actual position of the device.
// The movement is relative to current position
double MM3000::MoveInDeviceUnit(double position)
{
	if(isSafeMove(ConvertToUserUnit(position, "ps")))
	{
		validatePosition = 0;
		position  = getActualPosition() + position;
		return GoToInDeviceUnit(position);
		validatePosition = 1;
	}
}

//********************* Unit conversion function (to device units) *********************
//Convert user unit to device unit
//take input as user unit and return a number in device unit
double MM3000::ConvertToDeviceUnit(double number, string unit)
{
	
	if(unit.compare("ps")|| unit.compare("PS")|| unit.compare("pico")|| unit.compare("PICO"))
	{	
		return number/0.003336;		
	}
	if(unit.compare("mm")|| unit.compare("MM")|| unit.compare("ms")|| unit.compare("MS"))
	{
		return number/0.0005;
	}
	printf("Unkown unit... use either <<ps, pico, ms, mm>> ");
	return -0;
}

//********************* Unit de-conversion function (from device units) *********************
//Convert device unit to user unit
//take input as device unit and return a number in user unit
double MM3000::ConvertToUserUnit(double number, string unit)
{
	
	if(unit.compare("ps")|| unit.compare("PS")|| unit.compare("pico")|| unit.compare("PICO"))
	{	
		return number*0.003336;		
	}
	if(unit.compare("mm")|| unit.compare("MM")|| unit.compare("ms")|| unit.compare("MS"))
	{
		return number*0.0005;
	}
	printf("Unkown unit... use either <<ps, pico, ms, mm>> ");
	return -0;
}

//********************* Moving detection routine *********************

// Checks to see if the device is moving
// Returns false if there is no movement
bool MM3000::IsMoving()
{
		char command[30], result[30]; //Place holders for the commands to be sent
		char *buffer= new char[30];
		sprintf(command,"%s","ts"); //Writes the command "ts" to ask for status of the motor		
        WriteData(command); //Sends the command to the device
		Sleep(300);
        ReadData(30,command); //Returns a byte with the information of the motor of this DelayLine
		char tt = command[0];
		int byte = tt;
	    itoa(byte,result,2);
        char byte2 = result[6];
		if(byte==64 || byte2==0) return false;  //The device is not moving
		else{ validatePosition = 0; return true; }
}

//********************* Position detection routine *********************

// Returns the absolute position of the device
// The return value is in device units
double MM3000::getActualPosition()
{
	int curr_axis = getAxis(); //Used to select axis is being used (currently 2)
	char command[30], result[30]; //Place holders for the commands to be sent
	sprintf(command,"%d%s",curr_axis,"tpe"); //Writes the command "tpe" to ask for position in encoder units
    WriteData(command); //Sends the command to the device
	Sleep(200);
    ReadData(30,command); //Returns the information of the position of this DelayLine
	int value = asciiToInt(command);
  	
    return value; //Returns the position 
}

//take the tau2(relative position) and timeZero as input
//output the absolute position based on DL2position = timeZero - tau2, unit in "ps"
double MM3000::getPositionAtTau2(double aTau2, double timeZero)
{
	return timeZero - aTau2;
}
//take the absolute position and timeZero as input
//output the tau2 based on tau2 = timeZero - DL2position, unit in "ps"
double MM3000::getTau2AtPosition(double aPosition, double timeZero)
{
	return timeZero - aPosition;
}
//take a delay position, return whether it is out of range
bool MM3000::isSafeMove(double delayPositionInUserUnit)
{
	if(delayPositionInUserUnit >= -1000.0 && delayPositionInUserUnit <= 1000.0)
		return true;
	else
		return false;
}

//********************* Axis setting/getting routines *********************
//Return the delay line axis
int MM3000::getAxis()
{
	return Axis;
}
//Set the delay line axis
void MM3000::setAxis(int dln)
{
	if(dln>0 && dln<3)
    Axis = dln;
}

//********************* Velocity setting/getting routines *********************

// Changes the velocity of this delay line to the parameter given 
void MM3000::setVelocity(long int theVelocity)
{
	if(theVelocity > 0)
	{
		int curr_axis = getAxis(); //Select which Dln will move    
		char command[30]; //Place holder for the command to be sent and output of the command
		sprintf(command,"%d%s%ld",curr_axis,"va", theVelocity); //Write the command "va" to set velocity
		WriteData(command); //Send the command
	}
}

// Returns the velocity of this delay line
// Returns 0 if failure
long int MM3000::getVelocity(int axe)
{  
	if(axe>0 && axe<3)
	{
		char command[30];  
		sprintf(command,"%d%s",axe,"tv"); //Write the command "tv" to tell velocity 	
		if(WriteData(command) > 0) //Send the command
		{
			if(ReadData(30,command) == 0) //If reading succesful
			{
				Velocity[axe-1] = atoi(command); //Store the velocity in Velocity[]
				return Velocity[axe-1]; //Return the velocity
			}
		}		  
	}
	return 0;  
}

//********************* Acceleration setting/getting routines *********************

// Changes the acceleration of this delay line to the parameter given 
void MM3000::setAcceleration(long int theAcelaration)
{
	 if(theAcelaration > 0)
	 {
		 int curr_axis = getAxis(); //Select which Dln will be changed    
		 char command[30]; //Place holder for the command to be sent and output of the command
		 sprintf(command,"%d%s%ld",curr_axis,"ac", theAcelaration); //Write the command "ac" to set acceleration	
		 WriteData(command); //Send the command
	 }
}

// Returns the acceleration of this delay line
long int MM3000::getAcceleration(int axe)
{
	if(axe>0 && axe<3)
	{
		char command[30]; 
		sprintf(command,"%d%s",axe,"da"); //Write the command "da" to tell desired acceleration 
		if(WriteData(command) > 0) //Send the command
		{
			if(ReadData(30,command) == 0) //If reading succesful
			{
				Aceleration[axe-1] = atoi(command); //Store the acceleration in Acceleration[]
				return Aceleration[axe-1]; //return the acceleration
			}
		}			
	}
	return 0;
}

//********************* Command conversion routine *********************

// Pass it the command to be convereted from ascii character to integer value
int MM3000::asciiToInt(char *command)
{   
   CString temp[30], strNumber;
   int sign = 1;
   int j = -1;
   int Number = 0;
   for(int i=0; i<30; i++)
   {
	   char AscIInumber = command[i]; 

	   if(AscIInumber==45)
		   sign=-1;
	   else if(AscIInumber==43)
		   sign=1;
	   else if(AscIInumber==48)
		   temp[++j]="0";
	   else if(AscIInumber==49)
		   temp[++j]="1";
	   else if(AscIInumber==50)
		   temp[++j]="2";
	   else if(AscIInumber==51)
		   temp[++j]="3";
	   else if(AscIInumber==52)
		   temp[++j]="4";
	   else if(AscIInumber==53)
		   temp[++j]="5";
	   else if(AscIInumber==54)
		   temp[++j]="6";
	   else if(AscIInumber==55)
		   temp[++j]="7";
   	   else if(AscIInumber==56)
		   temp[++j]="8";
	   else if(AscIInumber==57)
		   temp[++j]="9";
       else if(AscIInumber==67)
		   break;
	   else
		   continue;
   }
   for(int i=0; i<=j; i++)
   {
	   strNumber.Append(temp[i]);
   }
   Number=atoi(strNumber);
   if(sign==-1) Number = -1*Number;
   return Number;
}
