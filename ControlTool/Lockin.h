
// Lockin.h: header file for the Lockin class
// This is a class used as interface with the lock-in amplifier
// This is a child class derived from the DeviceIndependent (independent devices) base class

/* This file declares the functions used for:
	Device initialization
	Start commands setup
	Setting the time constant
	Reading the Lock-in channel
	Average reading of the Lock-in
*/

#pragma once
#include "DeviceIndependent.h"

class Lockin : public DeviceIndependent
{
public:
	Lockin(void);
	Lockin(int theBaudRate);
	~Lockin(void); 
	//*******************************************************************//
    /* Initializes the lockin device
       Returns true if successful */
	bool Initialize(); 

    /* set parameters such as scan mode, sensitivity 
	   Returns true if successful*/
	bool setUpstartCommands(); 

    /* Set time constant
       Returns true if successful*/
	bool setTimeConstant(int Tao);

    /* Return the time constant on lock-in using ask time constant query */
	double askTimeConstant(void);

    /* Routine to convert the time constant key value 
       obtained from the interface to units of seconds.
       Returns the time constant in seconds */
	double timeConstantConvert(int value);

    /* Takes a single reading from the SR850 Lock-in of the given channel.
       Returns the value of the reading in Volts */
	double ReadChannel(int channel);

    /* Takes a single reading from one of the Lock-in auxiliary inputs.
       Returns value of the reading in Volts */
	double ReadAux(int auxPort);
	//return currentChannel
	int getCurrentChannel();
	//return currentAux
	int getCurrentAux();

private:
	int currentChannel, currentAux;
	int Sensitivity, SampleRate;
	int const static commandSize = 30; //Size of the buffer where commands are stored
};
