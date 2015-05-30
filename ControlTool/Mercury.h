
// Mercury.h: header file for one of the two current delay line classes
// This is a child class derived from the DeviceDependent (dependent devices) base class

/* This file declares the functions used for:
	Movement command (absolute and relative)
	Movement detection
	Device initialization
	Unit conversion
	Position detection
	Error-generation
	Velocity change
	Velocity detection
*/

#pragma once
#include "DeviceDependent.h"
#include "PI_Mercury_GCS_DLL.h"

class Mercury : public DeviceDependent
{
public:
	Mercury(void);
	Mercury(int portNumber, int Brate);  
	~Mercury(void);
	// Sets the zero position of the device in device units
	// Starts the data connection.
	// Returns true if the initialization was sucessful
	bool Initialize();

	// Moves the device to the specified position and return
	// the actual position of the device.
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
	virtual double getActualPosition();
	//take the tau1(relative position) and timeZero as input
	//output the absolute position based on DL1position = timeZero + tau1, unit in "ps"
	double getPositionAtTau1(double aTau1, double timeZero);
	//take the absolute position and timeZero as input
	//output the tau1 based on tau1 = DL1position - timeZero, unit in "ps"
	double getTau1AtPosition(double aPosition, double timeZero);
	//take a delay position, return whether it is out of range
	bool isSafeMove(double delayPositionInUserUnit);

	//*******************************************************************//
	// Returns the velocity of this delay line
	double getVelocity();
	// Changes the velocity of this delay line to the parameter given 
	void setVelocity(double theVelocity);
	//print out different kinds of warnings according to the error types
	char* Mercury::PrintERROR();

private:
	int ID;
	char szAxes[18];
	int delayTimeInMs;
};
