
// MM3000.h: header file for one of the two current delay line classes
// This is a child class derived from the DeviceDependent (dependent devices) base class

/* This file declares the functions used for:
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

#pragma once 
#include "DeviceDependent.h"

class MM3000 : public DeviceDependent
{
public:
	MM3000(void);
	MM3000(int theBaudRate);
	~MM3000(void);
	//*******************************************************************//
	// Sets the zero position of the device in device units
	// Starts the data connection.
	// Returns true if the initialization was sucessful
	bool Initialize();
	// Moves the device to the specified position and return
	// the position of the device in device units.
	// The movement is absolute
    double GoToInDeviceUnit(double position); 
	// Moves the device to the specified position and return
	// the actual position of the device.
	// The movement is relative to current position
	double MoveInDeviceUnit(double position);
	//Convert user unit to device unit
	//take input as user unit and return a number in device unit
	double ConvertToDeviceUnit(double number, string unit);
	//Convert device unit to user unit
	//take input as device unit and return a number in user unit
	double ConvertToUserUnit(double number, string unit);
	// Checks to see if the device is moving
	// Returns false if there is no movement
	bool IsMoving();
	// Returns the actual position of the device
	// The return value is in device units
	double getActualPosition();
	//take the tau2(relative position) and timeZero as input
	//output the absolute position based on DL2position = timeZero - tau2, unit in "ps"
	double getPositionAtTau2(double aTau2, double timeZero);
	//take the absolute position and timeZero as input
	//output the tau2 based on tau2 = timeZero - DL2position, unit in "ps"
	double getTau2AtPosition(double aPosition, double timeZero);
	//take a delay position, return whether it is out of range
	bool isSafeMove(double delayPositionInUserUnit);

	//*******************************************************************//
	
	int getAxis();//Return the delay line axis
	void setAxis(int dln);//Set the delay line axis

	long int getVelocity(int axe);// Returns the velocity of this delay line
	void setVelocity(long int theVelocity);//Changes the velocity of this delay line to the parameter given 

	long int getAcceleration(int axe);// Returns the acceleration of this delay line
	void setAcceleration(long int theAcelaration);// Changes the acceleration of this delay line to the parameter given \

private:
	int Axis;			     //Currently 2. Determine which axis is being used in this controller
	long int Velocity[2];    //Record the velocity of this dedvice
    long int Aceleration[2]; //Record the acelaration of this device
	int delayTimeInMs;
	// Pass it the command to be convereted from ascii character to integer value
	int asciiToInt(char *command);
};
