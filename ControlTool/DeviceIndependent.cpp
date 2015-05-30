
// DeviceIndependent.cpp: Implementation file for the Independent Device base class
/* The class DeviceIndependent is an abstract class used to derive all child classes 
	for independent devices (Lockin-in, etc.), definitions of the virtual functions 
	declared in the header file are found in the child class immplementation files */
    
/* Implements the DeviceIndependent class and defines the functions used to:
	Set/get device port numbers (connection location)
	Set/get device name
*/

#pragma once
#include "stdafx.h"
#include "DeviceIndependent.h"
//Default Constructor
DeviceIndependent::DeviceIndependent(void)
{
}

DeviceIndependent::~DeviceIndependent(void)
{
}

// Starts a serial connection to this device
bool DeviceIndependent::startSerialConnection()
{	
	int port = getPortNumber();
	if(serial.Open(port)) return true;
    return false;
}
// Helper method used to send command such as query string to this device
// Pass it the command to be sent
// Calls the serial.SendData function
// Returns the number of bytes that were sent
int DeviceIndependent::WriteData(char command[80])  
{
	char *cmd = command;
	string temp = command;
	int size = temp.length();

	int sent = serial.SendData(cmd,size); //Returns 0 if nothing was sent

	if(sent==0) return -1; //Returns if reading fail

	return sent;  //Returns the number of bytes that were sent
}
// Helper method used to receive data from this device
// Calls the serial.ReadData function to acquire bytes read
// Returns -1 if reading fail, number of bytes that were read if reading succesful
int DeviceIndependent::ReadData(int buffersize , char* buffer)
{	
	int byteRead = serial.ReadData(buffer, buffersize); //Number of bytes were read
	//cout << "Bytes were read: " << byteRead << endl;
	if(byteRead = 0) return -1; //Returns if reading fail
	else return 1; //If reading successful
}

int DeviceIndependent::getPortNumber() { return portNumber; }// Helper method to get port number
void DeviceIndependent::setPortNumber(int portnumber) { portNumber = portnumber; }// Method to set the port number

string DeviceIndependent::getDeviceName() { return deviceName; }// Helper method to get device name
void DeviceIndependent::setDeviceName(string theDeviceName) { deviceName = theDeviceName; }// Method to set device name

int DeviceIndependent::getbaudRate() { return baudRate; }// Helper method to get baud rate
void DeviceIndependent::setbaudRate(int thebaudRate) { baudRate = thebaudRate; }// Method to set baud rate

// Helper method to get how many data in the waiting list
// Calls the serial.ReadDataWaiting function to acquire bytes waiting
int DeviceIndependent::NumWaitingData(void)
{
	int WaitingBytes = serial.ReadDataWaiting();
	return WaitingBytes;
}

// Helper method to get how many data is read
// return the same thing as the ReadData function in this page, but do not call the serial.ReadData function
int DeviceIndependent::NumReadingData(void * buffer, int limit)
{
	int NumReadBytes = limit;
	if (limit > NumWaitingData()) NumReadBytes = NumWaitingData();
	return NumReadBytes;
}
