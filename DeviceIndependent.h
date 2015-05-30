
// DeviceIndependent.h: Header file for the Independent Device base class
/* This is intended to be used as a base class to derive
	classes for the different kinds of independent devices (Lock-in, etc)
	pre-requesite: all child classes that use this class as base class
	should provide definition for all virtual methods. */

/* Declares the functions used to:
	Set/get device port numbers (connection location)
	Set/get device name
*/

#pragma once
#include "DataStructure.h"
#include "Serial.h"
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <sstream>
#include <math.h>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

class DeviceIndependent
{
protected:
	string deviceName;          // Child name to identify this device
    int portNumber;             // The port where the device will be connected
    int baudRate;				// Value for the symbol rate
	CSerial serial;				// Variable used to start a serial connection

public:
	// Default constructor
	DeviceIndependent(void);
	~DeviceIndependent(void);
//********** Methods to be implemented by the child classes **********

	// Starts the data connection.
	// Returns true if the initialization was sucessful	
	virtual bool Initialize()=0;    
	
	
//********** Methods provided by the base class, no need to be implemented by the child **********

	// Starts a serial connection to this device
	bool startSerialConnection(); 
	
	// Helper method used to send command such as query string to this device
	// Pass it the command to be sent
	// Calls the serial.SendData function
	// Returns the number of bytes that were sent
	int WriteData(char command[80]);
	
    // Helper method used to receive data from this device
	// Calls the serial.ReadData function to acquire bytes read
	// Returns -1 if reading fail, number of bytes that were read if reading succesful
	int ReadData(int, char*);
	
    // Helper method to get how many data in the waiting list
	// Calls the serial.ReadDataWaiting function to acquire bytes waiting
	int NumWaitingData(void);
	
    // Helper method to get how many data is read
	// return the same thing as the ReadData function in this page, but do not call the serial.ReadData function 
	int NumReadingData(void * buffer, int limit);
	
    // Helper method to get port number
	int  getPortNumber();
    // Method to set the port number
	void setPortNumber(int portNumber);

    // Helper method to get device name
	string getDeviceName();
    // Method to set device name
	void setDeviceName(string theDeviceName);
    
    // Helper method to get baud rate
	int getbaudRate();
    // Method to set baud rate
	void setbaudRate(int thebaudRate);
};
