
// DeviceDependent.cpp: Implementation file for the Dependent Device base class
/* The class DeviceDependent is an abstract class used to derive all child
	classes for actual devices, definitions of the virtual functions declared
	in the header file are found in the child class immplementation files */
    
/* Implements the Device class and defines the functions used to:
	Set/get device name
	Set/get device port numbers (connection location)
	Set/get baud rate (symbol rate)
	Set/get zero positions 
*/

#pragma once
#include "stdafx.h"
#include "DeviceDependent.h"
//Default Constructor
DeviceDependent::DeviceDependent(void)
{  
}
DeviceDependent::~DeviceDependent(void)
{
}
//Opens a serial connectrion to this device
bool DeviceDependent::startSerialConnection()
{
	int port = getPortNumber();
	if(serial.Open(port)) return true;	
    else return false;
}
// Helper method used to send command to this device
// Pass it the command to be sent
// Returns the number of byte that were sent
int DeviceDependent::WriteData(char command[80])  
{	
	char *cmd = command;
	string temp = command;
	int size = temp.length();

	int sent = serial.SendData(cmd,size); 

	if(sent==0) return 0; //Return 0 if nothing was sent
	else return sent;     //Return the number of byte that were sent
}

// Helper method used to receive data from this device
// Calls the Serial.ReadData function to acquire bytes read
// Returns 1 if reading fail, 0 if reading succesful
int  DeviceDependent::ReadData(int limit , char* buffer)
{	
	int byteRead = serial.ReadData(buffer, limit);
	if(byteRead = 0) return 1;
	else return 0;
}

string DeviceDependent::getDeviceName() { return deviceName; }//Return the device alias
void DeviceDependent::setDeviceName(string name) { deviceName = name; }//Set the alias for this device

int DeviceDependent::getPortNumber() { return portNumber; }//Return the port number where device is connected
void DeviceDependent::setPortNumber(int portnumber) { portNumber = portnumber; }//Set the port number for connection

int DeviceDependent::getbaudRate() { return baudRate; }//Return the symbol rate
void DeviceDependent::setbaudRate(int thebaudRate) { baudRate = thebaudRate; }//Set the symbol rate

double DeviceDependent::getdeviceZeroPosition() { return deviceZeroPosition; }//Return zero position in device units
void DeviceDependent::setdeviceZeroPosition(double position) { deviceZeroPosition = position; }//Calibrate device to the user-defined zero position in device units

double DeviceDependent::getUserZeroPosition() { return userZeroPosition; }//Return the user zero position
void DeviceDependent::setUserZeroPosition(double position) { userZeroPosition = position; }//Calibrate device to the user-defined zero position in user units
