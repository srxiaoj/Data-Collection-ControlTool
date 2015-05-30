
// Serial.cpp: Implementation file for the class CSerial
/* This file defines the functions used to open/close
	serial connections and send/receive data to/from devices */

#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h> 
#include "Serial.h"
#include <string>
#include <iostream>

using namespace std;

CSerial::CSerial()
{
	memset( &m_OverlappedRead, 0, sizeof( OVERLAPPED ) );
 	memset( &m_OverlappedWrite, 0, sizeof( OVERLAPPED ) );
	m_hIDComDev = NULL;
	m_bOpened = FALSE;
}

CSerial::~CSerial()
{
	Close();
}

// Used to open the serial port. It takes two interger arguments. 
// The first argument contains the port number where the valid entries 
// are 1 through 4. The second argument is the baud rate. Valid values 
// for this argument are 1200, 2400, 4800, 9600, 19200, 38400 and 76800. 
// Returns true if successful, false otherwise.
BOOL CSerial::Open( int nPort, int nBaud )
{
	if( m_bOpened ) return( TRUE );	
//	CString szPort;
//	CString szComParams;
	DCB dcb;
    
    // LPTSTR pPrinterNam= const_cast< LPTSTR >( static_cast< LPCTSTR >( m_PrinterDeviceName ) );
	sprintf( szPort, "COM%d", nPort );
	m_hIDComDev = CreateFile( szPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL );
	std::cout << "The szPort is: " << szPort << endl;
	
	if( m_hIDComDev == NULL ) return( FALSE );
	memset( &m_OverlappedRead, 0, sizeof( OVERLAPPED ) );
 	memset( &m_OverlappedWrite, 0, sizeof( OVERLAPPED ) );

	COMMTIMEOUTS CommTimeOuts;
	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = 0;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 5000;
	SetCommTimeouts( m_hIDComDev, &CommTimeOuts );

	m_OverlappedRead.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	m_OverlappedWrite.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );

	dcb.DCBlength = sizeof( DCB );
	GetCommState( m_hIDComDev, &dcb );
	dcb.BaudRate = nBaud;
	dcb.ByteSize = 8;
	unsigned char ucSet;
	ucSet = (unsigned char) ( ( FC_RTSCTS & FC_DTRDSR ) != 0 );
	ucSet = (unsigned char) ( ( FC_RTSCTS & FC_RTSCTS ) != 0 );
	ucSet = (unsigned char) ( ( FC_RTSCTS & FC_XONXOFF ) != 0 );
	if( !SetCommState( m_hIDComDev, &dcb ) ||
		!SetupComm( m_hIDComDev, 10000, 10000 ) ||
		m_OverlappedRead.hEvent == NULL ||
		m_OverlappedWrite.hEvent == NULL ){
		DWORD dwError = GetLastError();
		if( m_OverlappedRead.hEvent != NULL ) CloseHandle( m_OverlappedRead.hEvent );
		if( m_OverlappedWrite.hEvent != NULL ) CloseHandle( m_OverlappedWrite.hEvent );
		CloseHandle( m_hIDComDev );
		return( FALSE );
		}

	m_bOpened = TRUE;
	return( m_bOpened );
}

// Closes a serial connection 
// Returns true if successful
BOOL CSerial::Close( void )
{
	if( !m_bOpened || m_hIDComDev == NULL ) return( TRUE );

	if( m_OverlappedRead.hEvent != NULL ) CloseHandle( m_OverlappedRead.hEvent );
	if( m_OverlappedWrite.hEvent != NULL ) CloseHandle( m_OverlappedWrite.hEvent );
	CloseHandle( m_hIDComDev );
	m_bOpened = FALSE;
	m_hIDComDev = NULL;

	return( TRUE );
}

//Writes one byte of data contained in the buffer ucByte to the file accessed by the handle m_hIDDComDev
//Return True if successful
BOOL CSerial::WriteCommByte( unsigned char ucByte )
{
	BOOL bWriteStat;
	DWORD dwBytesWritten;

	bWriteStat = WriteFile( m_hIDComDev, (LPSTR) &ucByte, 1, &dwBytesWritten, &m_OverlappedWrite );
	if( !bWriteStat && ( GetLastError() == ERROR_IO_PENDING ) )
	{
		if( WaitForSingleObject( m_OverlappedWrite.hEvent, 1000 ) ) dwBytesWritten = 0;
		else{
			GetOverlappedResult( m_hIDComDev, &m_OverlappedWrite, &dwBytesWritten, FALSE );
			m_OverlappedWrite.Offset += dwBytesWritten;
			}
	}
	return( TRUE );
}

// Writes data from a buffer to the serial port. The first argument it takes 
// is a const char* to a buffer that contains the data being sent. The second 
// argument is the number of bytes being sent. Returns the actual number of 
// bytes that are succesfully transmitted.
int CSerial::SendData( const char *buffer, int size )
{
	if( !m_bOpened || m_hIDComDev == NULL ) return( 0 );

	DWORD dwBytesWritten = 0;
	int i;
	for( i=0; i<size; i++ )
	{
		WriteCommByte( buffer[i] );
		dwBytesWritten++;
	}

    dwBytesWritten++;       
	WriteCommByte('\x0d'); // write the carriage return to the serial to signalize the end of a word, terminator of a command
    //cout << "The m_hIDComDev in SentData is: " << m_hIDComDev << endl;
	//cout << "The szPort string in SentData is: " << szPort << endl;
	return( (int) dwBytesWritten );
}

// This function returns the number of bytes that waiting in the 
// communication port's buffer. It basically allows you to "peek" at 
// the buffer without actually retrieving the data.
int CSerial::ReadDataWaiting( void )
{
	if( !m_bOpened || m_hIDComDev == NULL ) return( 0 );

	DWORD dwErrorFlags;
	COMSTAT ComStat;

	ClearCommError( m_hIDComDev, &dwErrorFlags, &ComStat );

	return( (int) ComStat.cbInQue );
}

// Reads data from the port's incoming buffer. The first argument that it 
// takes is a void* to a buffer into which the data will be placed. The 
// second argument is an integer value that gives the size of the buffer. 
// Returns the number of bytes successfully read into the buffer.
int CSerial::ReadData( void *buffer, int limit )
{
	if( !m_bOpened || m_hIDComDev == NULL ) return( 0 );

	BOOL bReadStatus;
	DWORD dwBytesRead, dwErrorFlags;
	COMSTAT ComStat;

	ClearCommError( m_hIDComDev, &dwErrorFlags, &ComStat );
	if( !ComStat.cbInQue ) return( 0 );

	dwBytesRead = (DWORD) ComStat.cbInQue;
	if( limit < (int) dwBytesRead ) dwBytesRead = (DWORD) limit;

	bReadStatus = ReadFile( m_hIDComDev, buffer, dwBytesRead, &dwBytesRead, &m_OverlappedRead );
	if( !bReadStatus )
	{
		if( GetLastError() == ERROR_IO_PENDING )
		{
			WaitForSingleObject( m_OverlappedRead.hEvent, 2000 );
			return( (int) dwBytesRead );
		}
		return( 0 );
	}
	//cout << "The m_hIDComDev in ReadData is: " << m_hIDComDev << endl;
	//cout << "The szPort string in ReadData is: " << szPort << endl;
	return( (int) dwBytesRead );
}

