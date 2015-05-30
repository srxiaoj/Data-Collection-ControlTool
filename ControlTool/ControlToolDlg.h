
// ControlToolDlg.h
// Header file for the structure of the ControlTool dialog box

#pragma once
#include "ControlTool.h"
#include "DataPrint.h"
#include "DataStructure.h"
#include "InfoBox.h"
#include "ScanBase.h"
#include "ScanSet.h"
//Include here all device headers:
#include "Mercury.h"
#include "MercuryNative.h"
#include "MM3000.h"
#include "Lockin.h"

//////////////////////////////////
#include "afxwin.h"
#include <Windows.h>
#include <string>
#include <time.h>
#include <math.h>
#include <vector>
using namespace std;

// CControlToolDlg dialog
class CControlToolDlg : public CDialog
{
protected:
	virtual void DoDataExchange(CDataExchange* pDX); //DDX/DDV support
	HICON m_hIcon; //a handle to the loaded icon
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg int OnCreate();
	DECLARE_MESSAGE_MAP()

public:
	CControlToolDlg(CWnd* pParent = NULL);	// Standard constructor
	enum { IDD = IDD_CONTROLTOOL_DIALOG };
	// Any method or variable should be declared here
	MM3000  mm3000;//declare an instance for the MM3000 class
	Mercury mercury;//declare an instance for the Mercury class
	Lockin lockin;//declare an instance for the Lockin class
	
	double P0, P1, P2, P3, P4, P5, P6, P7; //Variables where scan parameters from user input will be stored
	CString theTextToPrint;//a variable to save strings that will print out on the screen
	
	//Method used to print string on screen
	void PrintInfo(CString st);
	//Overload the PrintInfo method to print integer
	void PrintInfo(int st);
	//Overload the PrintInfo method to print float
	void PrintInfo(double st);
	//Method used to set defualt values such as port number, time constant upon opening program
	void setDefaultValues();
	//Method used to print out the new delayline position after a movement
	void updatePositionBoxes(int flag);
	//Method used to populate the list with any scan sub-class created
	//They will appear in order shown and 0-based indexed
	void populateScanList();
	//Method used to pass information of parameters from scanBase class
	//Displays the label such as "start", "end", "step" and "scan type"
	void displayScanInfo(vector<CString> paramInfo, CString type);
	//Reads the scan parameters from user input
	//takes user input of start, end and step values and stores in a vector
	vector<double> readParameters();
	//Method used to update the parameters for scanning from user input 
	//and save them to string variable for further analysis use
	void updateLockinSettings();//Object that defines the progress bar

	//The data acquisition routine, which takes a pointspace generated
	//by a scan class, move the delaylines, takes and stores the readings.
	void performAcquisition(vector<pointGroup> pointspace, int dimension, CString type);
	// Calculates the average intensity reading from one channel from a given device by a specific number of readings.
	// Returns the average reading.
	double AverageRead(int timeConstant, double AvgTime, double DelayFactor, double SampleFactor, int channel);
	// Calculates the average normalized intensity reading from one channel as well as auxiliary input.
	// Returns an average of the normalized signal, unnormalized singal and axuiliary signal
	vector <double> UserDefinedReading(int timeConstant, double AvgTime, double DelayFactor, double SampleFactor, int channel, int auxPort);

	//When clicked "Start Connections", port numbers are read, set and devices are initialized
	afx_msg void OnBnClickedStartconnection();
	//When radio button for DLN1 is selected, set DL1 as working delayline
	afx_msg void OnBnClickedDln1();
	//When radio button for DLN2 is selected, set DL2 as working delayline
	afx_msg void OnBnClickedDln2();
	//When "Run Scan" is clicked, call the performAcquisition function to record data
	afx_msg void OnBnClickedRunscan();
	//Toggles the selection of movement type between absolute/relative
	afx_msg void OnBnClickedMovetype();
	//When the "Move" button is clicked, move delayline to corresponding position
	afx_msg void OnBnClickedMovedln();
	//When the Scan type dropdown list is clicked, call the displayScanInfor function to display types of scan
	afx_msg void OnCbnCloseupScanlist();
	//On click the Read lockin button, print readings on the screen
	afx_msg void OnBnClickedReadlockin();
	//display a list of time constant and their corresponding key for user input when setting time constant
	afx_msg void OnBnClickedInfoboxbutton();
	//When check box for auxiliary input reading is checked, inform program to record aux signal
	afx_msg void OnBnClickedAuxcheck();
	//Draw the progress bar when scanning
	afx_msg void OnNMCustomdrawTheprogressbar(NMHDR *pNMHDR, LRESULT *pResult);
	//Clear the text in the text box
	afx_msg void OnBnClickedClearText();

	//method to retrieve selection variable of delayline, 1 = MM3000 delay line, 2 = Mercury delay line
	int getSelector();
	//method to retrieve movement type, 1 = absolute, 2 = relative
	int getMoveType();
	//method to retrieve index of the item selected in dropdown scan list
	//0 = 1D linear, 1 = 2D linear, 2 = 1D log, 3 = 2D log, 4 = abstract Scan (tauPlus and Xi)
	int getScanNameIndex();
	//method to check whether to read aux signal or not
	//return true if needs to read
	bool getAuxFlag();
	//method to retrieve lock in time constant
	int getLockTC();
	//method to retrieve average time at one delay position
	int getLockAV();
	//method to retrieve delay factor
	int getLockDF();
	//method to retrieve sampling factor
	int getLockSF();
	//method to get the progress bar
	CProgressCtrl* getProgressBar();

private:
	ScanBase *currentPtr[50];//a ScanBase pointer array used to save all the pointer point to each scan type
	ScanSet thisScanSet;//an instance of ScanSet class, call the standard constructor automatically which assign each pointer to each scan type
	CSerial serial; //declare an instance for the CSerial class
	CString scanName;//A string to store the text of scan name from user selection of scan type
	int selector; //1 = MM3000 delay line, 2 = Mercury delay line
	int movementType; //1 = absolute, 2 = relative
	int index; //Index of the item selected in dropdown scan list, 0 = 1D linear, 1 = 2D linear, 2 = 1D log, 3 = 2D log, 4 = tauPlus Linear, 5 tauPlus Log
	bool auxFlag; //Flag that indicates whether the lockin aux input is being read, true means read aux

	vector<pointGroup> PositionTable; //Array where the final positions from 1D acquisition will be stored
	double TwoDNormWithAuxTable [200][120]; // 2D Array of Int/Aux at each position
	double TwoDNormWithAuxSquareTable [200][120];// 2D Array of Int/Aux^2 at each position
	double TwoDValueTable [200][120]; //2D Array where the final lock-in readings will be stored
	double TwoDAuxTable [200][120]; //2D Array where the lock-in auxiliary input readings will be stored
	double ValueTable [400]; //Array where the final lock-in readings will be stored
	double AuxTable [400]; //Array where the lock-in auxiliary input readings will be stored
	double AvgNormWithAuxTable [200]; //Average of the Int/Aux at each delay position
	double AvgNormWithAuxSquareTable [200]; //Average of the Int/Aux^2 at each delay position
	double avgInt [200]; //Averge of intensity at each delay position
	double avgAux [200]; //Averge of aux at each delay position
	int LockTC, LockAV, LockDF, LockSF; //Variables for the lockin settings
	int numberOfScans;//number of scans get from the getNumberOfScan method in ScanBase
	double actualTC; //Time constant in seconds after conversion
	double delayTime, sampleTime, targetTime, targetReads; //Parameters within lockin::ReadAverageByTime/Reads
	double totalAverageTime;//count the time for averaging data
	int totalReads; //Count the size of reading2 vector during averaging time, and used for creating valuetable and to printdata
};

