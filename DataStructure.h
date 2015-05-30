#pragma once
#include <string>
#include <math.h>
#include <vector>
using namespace std;

// This is the basic structure of a labeled number
// It can be used throughout the program to represent any measurement or parameter
struct labeledNumber //Defines the type 'labeled Number'
{
	//Contains the following members:
	string name; //A string for what the number represents
	double value; //The actual value of the number
	string unit; //A string for the unit of the value
};

// A more general structure containing five labeled numbers 
// to use with device parameter identification
struct parameterGroup //Defines the type 'parameterGroup'
{
	//Contains the following members:
	labeledNumber param1;
	labeledNumber param2;
	labeledNumber param3;
	labeledNumber param4;
	labeledNumber param5;
	//We can add as many more as needed
};

/**********************************************************************************************/

// This is the basic structure of a scan point used throughout the program
// This scan point represents a position (time point) for use with the delay lines
struct scanPoint //Defines the type 'scanPoint'
{
	//Contains the following members:
	double position; //A value for the data point (a position in most cases) 
	string unit; //A string for its unit
}; 

// A more general structure containing two objects of
// type 'scanPoint' used for positioning of multiple delay lines
struct pointGroup
{
	//Contains the following members:
	scanPoint DL2; //Information for MM3000
	scanPoint DL1; //Information for Mercury
	scanPoint tau2; // tau2 = timeZeroDL1 - DL1.position
	scanPoint tau1; // tau1 = DL2.position - timeZeroDL2
    scanPoint tauPlus; //abstract position of tau1+tau2
	scanPoint Xi; //value of Xi
	//We can add future delay line elements here
};
