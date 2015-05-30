
// Lockin.cpp: Implementation file for the Lockin class
// This is a class used as interface with the lock-in amplifier (Model SR850 DSP)
// Makes use of the Serial library for connections
// Device manual: http://www.thinksrs.com/downloads/PDFs/Manuals/SR850m.pdf


/* Implements the class Lockin and defines the functions used for:
	Device initialization
	Serial connection setup
	Start commands setup:
		(sensitivity, sample rate)
	Time constant read from device
	Data read/write
	Average reading of the lock-in amplifier
	Writing results to text file
*/

#include "stdafx.h"
#include "Lockin.h"
#pragma once

Lockin::Lockin(void)
{
	deviceName = "Lockin Amp";
	currentChannel = 1; //Lock-in channels are: 1=X, 2=Y, 3=R, 4=theta
	currentAux = 1; //One of the four auxiliary inputs
	Sensitivity = 26; SampleRate = 4; 
}
Lockin::Lockin(int theBaudRate):DeviceIndependent()
{
	deviceName = "Lockin Amp";
	currentChannel = 1; //Lock-in channels are: 1=X, 2=Y, 3=R, 4=theta
	currentAux = 1; //One of the four auxiliary inputs
	Sensitivity = 26; SampleRate = 4;
	baudRate = theBaudRate;
}
Lockin::~Lockin(void)
{
}


// Initializes the lockin device
// Returns true if successful
bool Lockin::Initialize()
{
    if(startSerialConnection())
	{
		if(setUpstartCommands()) return true;
	}
	return false;
}

// Sets up the port type and scan mode.
// Retrieves and sets the values for the 
// Lock-in sensitivity, sample rate.
// Returns true if values are set successfuly
/* NOTE: sensitivity and sampling rate are returned as a key
	integer that needs to be conevrted to actual units, write
	such procedures if needed. Refer to device manual for key */
bool Lockin::setUpstartCommands()
{
   char command[commandSize];

   if(WriteData("outx 0") == -1) //Sets the port to RS232 type output
	   return false;		  
   if(WriteData("send 1") == -1) //Sets the scanning mode to Loop (see manual)
	   return false;
   if(WriteData("sens?") == -1) //Retrieves the lockin sensitivity
       return false;
   if(ReadData(commandSize,command) == -1) //Reads sensitivity into 'command'
   {
       if(ReadData(commandSize,command) == -1) //Checks twice for read failure
		   return false;
   }
   Sensitivity = atoi(command); //Saves sensitivity as Sensitivity
 
   if(WriteData("srat?") == -1) //Retrieves the lockin sampling rate
	   return false;
   if(ReadData(commandSize,command) == -1) //Reads sampling rate into 'command'
   {
       if(ReadData(commandSize,command) == -1) //Checks twice for read failure
		   return false;
   }
   SampleRate = atoi(command); //Saves sampling rate as SampleRate
   return true;
}

// Set time constant
// Returns false if constant was not set successfully
bool Lockin::setTimeConstant(int Tao)
{
	char Tcommand[30];
    sprintf(Tcommand,"%s%d","oflt", Tao); //Command 'oflt' sets the time constant
	if(WriteData(Tcommand) == -1) return false;
	else return true;
}

// Return the time constant on lock-in using ask time constant query
double Lockin::askTimeConstant(void)
{
	char Tcommand[30];
    sprintf(Tcommand,"oflt?"); //Command 'oflt' sets the time constant
	WriteData(Tcommand);
	while (NumWaitingData() == 0) // if no data to read, do nothing and wait until there is data for reading, for outp? command, the size of waitingdata is 12 bytes
	{	
	}
	Sleep(10); // wait for all the data in the waiting list to be transfered, if this time is missing, only part of the data may be transfered
	//printf("number of waiting data is: %d\n", NumWaitingData());
	ReadData(30,Tcommand);
	double value = atof(Tcommand);
	return value;
}

// Routine to convert the time constant key value 
// obtained from the interface to units of seconds.
// Returns the time constant in seconds
double Lockin::timeConstantConvert(int value)
{
	double sec = 0;
	if(value==0) sec = 0.00001;
	else if(value==1) sec = 0.00003;
	else if(value==2) sec = 0.0001;
	else if(value==3) sec = 0.0003;
	else if(value==4) sec = 0.001;
	else if(value==5) sec = 0.003;
	else if(value==6) sec = 0.01;
	else if(value==7) sec = 0.03;
	else if(value==8) sec = 0.1;
	else if(value==9) sec = 0.3;
	else if(value==10) sec = 1;
	else if(value==11) sec = 3;
	else if(value==12) sec = 10;
	else if(value==13) sec = 30;
	else if(value==14) sec = 100;
	else if(value==15) sec = 300;
	else if(value==16) sec = 1000;
	else if(value==17) sec = 3000;
	else if(value==18) sec = 10000;
	else if(value==19) sec = 30000;

	return sec;
}

//********************* Signal Normalization routines *****************************

// Takes a single reading from the SR850 Lock-in of the given channel.
// Returns the value of the reading in Volts
double Lockin::ReadChannel(int channel)
{
	char * command = new char [commandSize]; // The string file used to save the query strings
	char * lockin_response = new char [commandSize]; // Buffer where reading will be stored
	sprintf(command, "outp? %d", channel); // Save the query reading from lockin to the command string
	WriteData(command); //Sends the command

	while (NumWaitingData() == 0) // if no data to read, do nothing and wait until there is data for reading, for "outp?" query, the size of waitingdata is 12 bytes
		{	

		}
	Sleep(16); // wait for all the data in the waiting list to be transfered, if this time is missing, only part of the data may be transfered
	//printf("number of waiting data is: %d\n", NumWaitingData());
	ReadData(commandSize, lockin_response); // get the respond string from lockin
	double read = atof(lockin_response); // convert the respond string to float and save in a double variable "read"
	delete [] command; // free the memory used by command
	delete [] lockin_response; // free the memory used by lockin_response
	return read;	

}
/*
double Lockin::ReadChannel(int channel)
{
	char * command = new char [commandSize]; //Buffer where reading will be stored
	sprintf(command, "outp? %d", channel); //Command to query reading from lockin
	WriteData(command); //Sends the command
	//printf("number of waiting data is: %d\n", NumWaitingData());
	if (ReadData(commandSize, command) != -1) //Check for reading faikure
	{
		double read = atof(command);
		return read;
	}
	else return 100000;
}
*/
// Takes a single reading from one of the Lock-in auxiliary inputs.
// Returns value of the reading in Volts
double Lockin::ReadAux(int auxPort)
{
	char * command = new char [commandSize]; // The string file used to save the query strings
	char * lockin_response = new char [commandSize]; // Buffer where reading will be stored
	sprintf(command, "oaux? %d", auxPort); // Save the query reading from lockin to the command string
	WriteData(command); //Sends the command

	while (NumWaitingData() == 0) // if no data to read, do nothing and wait until there is data for reading
	{	
		//cout << "read successfully? " << ReadData(30, lockin_response) << endl;
		//ReadData(30, lockin_response);
	}
	Sleep(16); // wait for all the data in the waiting list to be transfered, if this time is missing, only part of the data may be transfered
	//printf("number of waiting data is: %d\n", NumWaitingData());
	ReadData(commandSize, lockin_response); // get the respond string from lockin
	double read = atof(lockin_response); // convert the respond string to float and save in a double variable "read"
	delete [] command; // free the memory used by command
	delete [] lockin_response; // free the memory used by lockin_response
	return read;
}
/*
double Lockin::ReadAux(int auxPort)
{
	char * command = new char [commandSize]; //Buffer where reading will be stored
	sprintf(command, "oaux? %d", auxPort); //Command to query reading from lockin
	WriteData(command); //Sends the command
	//printf("number of waiting data is: %d\n", NumWaitingData());
	Sleep(20);
	if (ReadData(commandSize, command) != -1) //Check for reading faikure
	{
		double read = atof(command);
		return read;
	}
	else return 100000;
}
*/
/*// Take the channel number and aux port number as input
// Return the two signals
vector<double> Lockin::ReadSignalwithAux(int channel, int auxPort)
{
	vector<double> SignalwithAux (2); // create a two element vector
	SignalwithAux[0] = ReadChannel(channel); // first element is the signal from channel "channel"
	Sleep(200); //Wait appropriate time for next reading (sampling time)
	SignalwithAux[1] = ReadAux(auxPort); // second element is the signal from Auxiliary port
	return SignalwithAux;
}

// After a single reading with ReadSignalwithAux, the procedure of normalizing signal are described by this funtion
// Take the channel number and aux port number as iput
// Return the normalized signal
double Lockin::SignalNormalizing(int channel, int auxPort)
{
	double NormalizedSignal;
	vector<double> SignalwithAux (2); // create a two element vector
	SignalwithAux = ReadSignalwithAux(channel, auxPort); 
	NormalizedSignal = SignalwithAux[0]/SignalwithAux[1]; // normalized signal = I(i)/A(i), i is the number of reading
	return NormalizedSignal;
}*/

//return currentChannel
int Lockin::getCurrentChannel()
{
	return currentChannel;
}

//return currentAux
int Lockin::getCurrentAux()
{
	return currentAux;
}
