
// Serial.h: Header file for the class CSerial
/* This class is responsible for opening/closing serial
	connections to devices used in initialization routines,
	as well as functions used to send/receive data to/from
	delay lines */

#ifndef __SERIAL_H__
#define __SERIAL_H__
#include <windows.h>

#define FC_DTRDSR       0x01
#define FC_RTSCTS       0x02
#define FC_XONXOFF      0x04
#define ASCII_BEL       0x07
#define ASCII_BS        0x08
#define ASCII_LF        0x0A
#define ASCII_CR        0x0D
#define ASCII_XON       0x11
#define ASCII_XOFF      0x13

class CSerial
{
public:
	CSerial();
	~CSerial();

	// Opens a serial connection 
	// Returns true if succesful
	BOOL Open( int nPort = 2, int nBaud = 9600 );

	// Closes a serial connection 
	// Returns true if succesful
	BOOL Close( void );

	// Reads data from the port's incoming buffer. The first argument that it 
	// takes is a void* to a buffer into which the data will be placed. The 
	// second argument is an integer value that gives the size of the buffer. 
	// Returns the number of bytes successfully read into the buffer.
	int ReadData( void *, int );

    // Writes data from a buffer to the serial port. The first argument it takes 
    // is a const char* to a buffer that contains the data being sent. The second 
    // argument is the number of bytes being sent. Returns the actual number of 
    // bytes that are succesfully transmitted.
	int SendData( const char *, int );

	// This function returns the number of bytes that waiting in the 
	// communication port's buffer. It basically allows you to "peek" at 
	// the buffer without actually retrieving the data.
	int ReadDataWaiting( void );
	
	BOOL IsOpened( void ){ return( m_bOpened ); }//A boolean variable to check whether the port is open or not

protected:
    //Writes one byte of data contained in the buffer ucByte to the file accessed by the handle m_hIDDComDev
    //Return True if successful
	BOOL WriteCommByte( unsigned char );
	
	HANDLE m_hIDComDev; //A handle to the file where query commands are stored
	OVERLAPPED m_OverlappedRead, m_OverlappedWrite;
	BOOL m_bOpened;
private:
	char szPort[15];
	char szComParams[50];
	
};

#endif
