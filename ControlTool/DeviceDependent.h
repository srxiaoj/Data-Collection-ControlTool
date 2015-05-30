
// DeviceDependent.h: Header file for the Dependent Device base class
/* This is intended to be used as a base class to derive
	classes for the different kinds of  dependent devices (delayline, 
	spetrometer, etc). Pre-requesite: all child classes that use this 
	class as base class should provide definition for all virtual methods. */

/* Declares the functions used to:
	Set/get device zero positions
	Set/get device port numbers (connection location)
	Set/get device name
*/

#pragma once
#include "DataStructure.h"
#include "Serial.h"
#include <string>
#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
using namespace std;

class DeviceDependent
{
protected:
	string deviceName;          // Child name to identify this device
	double currentPosition;		// Value for the current position of device
	int validatePosition;		// 1 if at target position, 0 if moving
	parameterGroup parameters;  // Values and interpretation varies with specific device
    double deviceZeroPosition;  // Define the home position of this device in device units
    double userZeroPosition;    // Zero posiition defined in user units 
	int portNumber;             // The port where the device will be connected  
    int baudRate;				// Value for the symbol rate
	CSerial serial;				// Variable used to start a serial connection
    
public:
	//Default Constructor
	DeviceDependent(void);
	~DeviceDependent(void);

//********************** Methods to be implemented by the child classes **********************

	// Sets the zero position of the device in device units and starts
	// the data connection. Returns true if the initialization was sucessful
	virtual bool Initialize()=0;

    // Moves the device to the specified position and return
	// the position of the device. Movement is absolute
	virtual double GoToInDeviceUnit(double position)=0; 

	// Moves the device to the specified position and return
	// the position of the device. Movement is relative to current position
	virtual double MoveInDeviceUnit(double position)=0;

	// Returns the actual position of the device
	virtual double getActualPosition()=0;

	// Checks to see if the device is moving
	// Returns false if there is no movement
	virtual bool IsMoving()=0;

	// Unit conversion to device units
	virtual double ConvertToDeviceUnit(double number, string unit)=0;

	// Unit conversion from device units
	virtual double ConvertToUserUnit(double number, string unit)=0;	
	
//********** Methods provided by the base class, no need to be implemented by the child **********

	//Opens a serial connectrion to this device
	bool startSerialConnection(); 

	// Helper method used to send command to this device
	// Pass it the command to be sent
	// Returns the number of byte that were sent
	int  WriteData(char command[80]); 

	// Helper method used to receive data from this device
	// Calls the Serial.ReadData function to acquire bytes read
	// Returns 1 if reading fail, 0 if reading succesful
	int  ReadData(int, char*); 

	string getDeviceName(); //Return the device alias
	void setDeviceName(string name); //Set the alias for this device	

	int  getPortNumber(); //Return the port number where device is connected
	void setPortNumber(int portNumber); //Set the port number for connection

	int getbaudRate(); //Return the symbol rate
	void setbaudRate(int thebaudRate); //Set the symbol rate

	double getdeviceZeroPosition(); //Return zero position in device units
	void setdeviceZeroPosition(double position); //Calibrate device to the user-defined zero position in device units

	double getUserZeroPosition(); //Return the user zero position
	void setUserZeroPosition(double position); //Calibrate device to the user-defined zero position in user units			
};
