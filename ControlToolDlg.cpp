
// ControlToolDlg.cpp : Dialog box implementation file
/* This file contains structures used to retrieve user
	input from the dialog box. 
	Structures include connection initialization, delay
	line selection, moving commands, read auxiliary, scanning commands */

#pragma once
#include "stdafx.h"
#include "ControlToolDlg.h"
#include "ScanSet.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//*************************** CAboutDlg class definition ************************************

// CAboutDlg: Dialog used for the Information sub-box, the about box
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	enum {IDD = IDD_ABOUTBOX};

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{	
}

// Call this function to exchange/validate (DDX/DDV) dialog data 
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

//*************************** CControlToolDlg class definition ************************************

// This is the constructor
// All device objects should be created here
CControlToolDlg::CControlToolDlg(CWnd* pParent)	: CDialog(CControlToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

/////////  All the device objects should be initialized here:  //////////

	mm3000 = MM3000();
	mercury = Mercury();
	lockin = Lockin();
	//mercuryN = MercuryNative();

/////////////////////////////////////////////////////////////////////////
	theTextToPrint = "";
}

void CControlToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

/* These macros map the fucntions for the rest of the
	interface options in the dialog box */
BEGIN_MESSAGE_MAP(CControlToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(StartConnection, &CControlToolDlg::OnBnClickedStartconnection)
	ON_BN_CLICKED(DLN2, &CControlToolDlg::OnBnClickedDln2)
	ON_BN_CLICKED(DLN1, &CControlToolDlg::OnBnClickedDln1)
	ON_BN_CLICKED(RunScan, &CControlToolDlg::OnBnClickedRunscan)
	ON_NOTIFY(NM_CUSTOMDRAW, TheProgressBar, &CControlToolDlg::OnNMCustomdrawTheprogressbar)
	ON_BN_CLICKED(MoveType, &CControlToolDlg::OnBnClickedMovetype)
	ON_BN_CLICKED(MoveDLN, &CControlToolDlg::OnBnClickedMovedln)
	ON_CBN_CLOSEUP(ScanList, &CControlToolDlg::OnCbnCloseupScanlist)
	ON_BN_CLICKED(InfoBoxButton, &CControlToolDlg::OnBnClickedInfoboxbutton)
	ON_BN_CLICKED(AuxCheck, &CControlToolDlg::OnBnClickedAuxcheck)
	ON_BN_CLICKED(ReadLockin, &CControlToolDlg::OnBnClickedReadlockin)
	ON_BN_CLICKED(ClearText, &CControlToolDlg::OnBnClickedClearText)
END_MESSAGE_MAP()

BOOL CControlToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	//ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	//ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			//pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	//  Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	ShowWindow(SW_SHOW);
	CButton* pEdit0 = (CButton*)GetDlgItem(DLN2);
	pEdit0->SetCheck(true);//set the default button on DL2
	//Set auxiliary input reading on
	CButton* pEdit1 = (CButton*)GetDlgItem(AuxCheck);
	pEdit1->SetCheck(true);//set the default to be checked
	auxFlag = true;

	return TRUE;  // return TRUE  unless you set the focus to a control
}


// If you add a minimize button to your dialog, you will need the code below
// to draw the icon.  For MFC applications using the document/view model,
// this is automatically done for you by the framework.
void CControlToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); //Device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else CDialog::OnPaint();

	//Display the default values:
	setDefaultValues(); 
	//populateScanList();
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CControlToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//Draw the progress bar when scanning
void CControlToolDlg::OnNMCustomdrawTheprogressbar(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	*pResult = 0;
}

//************************************* On-Screen text printing routines *****************************************

//Method used to print string on screen
void CControlToolDlg::PrintInfo(CString st)
{
	theTextToPrint.Append(st);
	CEdit* pEditShow = Get(Infobox);
	//char* sp= " ";
	CString sp1= CString(" ");
    theTextToPrint.Append(sp1);
    pEditShow->SetWindowText(theTextToPrint);
}

//Overload the PrintInfo method to print integer
void CControlToolDlg::PrintInfo(int number)
{
	char str[80] ;
	sprintf(str,"%d",number); //Converts to decimal base string.
	PrintInfo(str);  	
}

//Overload the PrintInfo method to print float
void CControlToolDlg::PrintInfo(double fnumber)
{
	char str[80] ;
	if (fnumber < 0.01 )
		sprintf(str,"%.3e",fnumber);//Converts to decimal base string.
	else
		sprintf(str,"%.3f",fnumber);
	
	PrintInfo(str);  		 
}

//Clear the text in the text box
void CControlToolDlg::OnBnClickedClearText()
{
	theTextToPrint.Empty();//clear the text saved in this string
	CWnd* pEditShow = GetDlgItem(Infobox);
	pEditShow->SetWindowText(_T(""));//clear the text box
}

//*********************************** Set the default values when window is drawn ***********************************
//Method used to set defualt values such as port number, time constant upon opening program
void CControlToolDlg::setDefaultValues()
{
	CString port = "4";
	CEdit* pEdit = Get(ComMM3000); 
	pEdit->SetWindowText(port); //Set the MM3000 port number to 4
	port = "6";
	pEdit = Get(ComMercury); 
	pEdit->SetWindowText(port); //Set the Mercury port number to 6
	port = "3";
	pEdit = Get(LockinPort); 
	pEdit->SetWindowText(port); //Set the Lock-In port number to 3

	//Set the default values for the lock-in settings:
	LockTC = 9;
	LockAV = 2;
	LockDF = 2;
	LockSF = 3;

	CString TC, AT, DF, SF;
	TC.Format("%d", LockTC);
	AT.Format("%d", LockAV);
	DF.Format("%d", LockDF);
	SF.Format("%d", LockSF);
	pEdit = Get(TConstant);
	pEdit->SetWindowText(TC); //Display the time constant
	pEdit = Get(AverageTime);
	pEdit->SetWindowText(AT); //Display the averaging time
	pEdit = Get(DLFactor);
	pEdit->SetWindowText(DF); //Display the delay factor
	pEdit = Get(SLFactor);
	pEdit->SetWindowText(SF); //Display the sampling factor

	//Set MM3000 as default delay line
	selector = 1;
	//Set "absolute" as defalut move type
	movementType = 1;
	
}

//*********************************************** Methods from the interface *************************************************

//When clicked "Start Connections", port numbers are read, set and devices are initialized
void CControlToolDlg::OnBnClickedStartconnection()
{
	// Read the port number from the interface (leftmost section of dialog box)
	CEdit* pEdit0 = Get(ComMM3000); 
	CEdit* pEdit1 = Get(ComMercury); 
	CEdit* pEdit2 = Get(ComPhase);
	CEdit* pEdit3 = Get(LockinPort);

	CString strText, strComMM3000, strComMercury, strLockinPort, strComPhase; 

	pEdit0->GetWindowText(strComMM3000);
    pEdit1->GetWindowText(strComMercury);
    pEdit2->GetWindowText(strComPhase);
	pEdit3->GetWindowText(strLockinPort);

	// Set the port number of these devices
	mercury.setPortNumber(atoi(strComMercury));
	//mercuryN.setPortNumber(atoi(strComMercury));
    mm3000.setPortNumber(atoi(strComMM3000));
	lockin.setPortNumber(atoi(strLockinPort));
	PrintInfo("Setting Mercury port number to:");
	PrintInfo(mercury.getPortNumber()); PrintInfo("\r\n");
	PrintInfo("Setting MM3000 port number to:");
	PrintInfo(mm3000.getPortNumber()); PrintInfo("\r\n");
	PrintInfo("Setting Lock-In port number to:");
	PrintInfo(lockin.getPortNumber()); PrintInfo("\r\n");

	// Call the initialization functions for all current devices
	if(mercury.Initialize())	PrintInfo("Mercury DLN initialized sucessfully \r\n");
	else PrintInfo("Mercury initialization falied. Check connection \r\n");

	if(mm3000.Initialize())		PrintInfo("MM3000 DLN initialized sucessfully \r\n");
	else PrintInfo("MM3000 initialization falied. Check connection \r\n");

	if(lockin.Initialize()) PrintInfo("Lock-In initialized sucessfully \r\n");
	else PrintInfo("Lock-In initialization falied. Check connection \r\n");
	//Display the default scan options:
	populateScanList();
}

//************************************** Response to Delay Line (Control) and Aux (Scan) selection ***********************************

//When radio button for DLN1 is selected, set DL1 as working delayline
void CControlToolDlg::OnBnClickedDln2() //Corresponds to the MM3000 delay line
{
	selector = 1;
	mm3000.setAxis(2);
}
//When radio button for DLN2 is selected, set DL2 as working delayline
void CControlToolDlg::OnBnClickedDln1() //Corresponds to the Mercury delay line
{
	selector = 2;
}
//When check box for auxiliary input reading is checked, inform program to record aux signal
void CControlToolDlg::OnBnClickedAuxcheck()
{
	auxFlag? auxFlag = false : auxFlag = true;
}

//***************************************** Response to movement commands **********************************************

//Toggles the selection of movement type between absolute/relative
void CControlToolDlg::OnBnClickedMovetype()
{
	CButton* pEdit0 = (CButton*)GetDlgItem(MoveType); //Get the item (button)
	if(movementType == 1) //If the current state is absolute
	{
		pEdit0->SetWindowText("Relative"); //Change the text
		movementType = 2; //Toggle to relative
	}
	else if(movementType == 2) //If the current state is relative
	{
		pEdit0->SetWindowText("Absolute"); //Change the text
		movementType = 1; //Toggle to absolute
	}
}

//Method used to print out the new delayline position after a movement
void CControlToolDlg::updatePositionBoxes(int flag)
{
	if(flag == 0) //If the delay line(s) are currently scanning
	{
		CStatic* pEdit0 = GetS(MMPos);
		CStatic* pEdit1 = GetS(MercPos);
		if(selector == 1)
		pEdit0->SetWindowTextA("Scanning");
		if(selector == 2)
		pEdit1->SetWindowTextA("Scanning");

	}

	else if (flag == 1) //If the delayline(s) have reached a position
	{
		double pL1 = mercury.ConvertToUserUnit(mercury.getActualPosition(), "ps");
		double pL2 = mm3000.ConvertToUserUnit(mm3000.getActualPosition(), "ps");
		char pos1[100]; sprintf(pos1, "%.3f%", pL1);
		char pos2[100]; sprintf(pos2, "%.3f%", pL2);
		CStatic* pEdit1 = GetS(MercPos);
		CStatic* pEdit2 = GetS(MMPos);
		pEdit1->SetWindowTextA(pos1);
		pEdit2->SetWindowTextA(pos2);
	}
}

// When the "Move" button is clicked, move delayline to corresponding position
void CControlToolDlg::OnBnClickedMovedln()
{
	CEdit* pEdit0 = Get(PositionBox); 
	CString strPosition;
	pEdit0->GetWindowText(strPosition); //Get the value from the edit box PositionBox and save to strPosition
	double pd; //Variable where position desired is stored in device units
	double pu; //Variable where new position is stored in user units

	if(selector == 2) //If moving Mercury delay line
	{
		pd = mercury.ConvertToDeviceUnit(atof(strPosition), "ps"); //Convert value to device units
		pu = (movementType == 1) ? //Perform absolute/relative movement and convert back to user units
			mercury.ConvertToUserUnit(mercury.GoToInDeviceUnit(pd), "ps") : mercury.ConvertToUserUnit(mercury.MoveInDeviceUnit(pd), "ps");
	}
	else if(selector == 1) //If moving MM3000 delay line
	{
		pd = mm3000.ConvertToDeviceUnit(atof(strPosition),"ps"); //Convert value to device units
		pu = (movementType == 1) ? //Perform absolute/relative movement and convert back to user units
			mm3000.ConvertToUserUnit(mm3000.GoToInDeviceUnit(pd), "ps") : mm3000.ConvertToUserUnit(mm3000.MoveInDeviceUnit(pd), "ps");
	}
	updatePositionBoxes(1);
}

//*********************************** Scan list creation ***********************************

// Method used to populate the list with any scan sub-class created
// They will appear in order shown and 0-based indexed
void CControlToolDlg::populateScanList()
{
	CComboBox* pEdit0 = (CComboBox*)GetDlgItem(ScanList);
	numberOfScans = thisScanSet.getNumberOfScan();
	for (int i = 0; i<numberOfScans; i++)
	{
		pEdit0->AddString(thisScanSet.getDisplayName(i)); //display the scan name on the interface
	}
}

//***************************************** Response to Scan Droplist **************************************

//When the Scan name dropdown list is clicked, call the displayScanInfor function to display names of scan
void CControlToolDlg::OnCbnCloseupScanlist()
{
	CComboBox* pEdit0 = (CComboBox*)GetDlgItem(ScanList); //Get the Item (Combo Box)
	index = pEdit0->GetCurSel(); //Store the scan selected to an integer first and then get the text from this integer using GetLBText method
	pEdit0->GetLBText(index, scanName);//Store the text on the CComboBox identified by an integer index to a string scanName
	displayScanInfo(thisScanSet.getScan(scanName)->GenerateParameterLegends(), thisScanSet.getScan(scanName)->getScanName());
	Enable(RunScan);//Enable the Run Scan button
}

//***************************************** Response to Run Scan *************************************

//When "Run Scan" is clicked, call the performAcquisition function to record data
void CControlToolDlg::OnBnClickedRunscan()
{
	if(thisScanSet.getScan(scanName)->isSafeScan(readParameters(), selector))
		performAcquisition(thisScanSet.getScan(scanName)->GeneratePointspace(readParameters(), selector), thisScanSet.getScan(scanName)->dimension, thisScanSet.getScan(scanName)->getScanName());
	else
		PrintInfo("Scan out of range, reset parameter!\r\n");
}

//Method used to pass information of parameters from scanBase class
//Displays the label such as "start", "end", "step" and "scanName"
void CControlToolDlg::displayScanInfo(vector<CString> paramInfo, CString scanName)
{
	CStatic* pEdit0 = GetS(SP0); pEdit0->SetWindowText(paramInfo[0]);
	CStatic* pEdit1 = GetS(SP1); pEdit1->SetWindowText(paramInfo[1]);
	CStatic* pEdit2 = GetS(SP2); pEdit2->SetWindowText(paramInfo[2]);
	CStatic* pEdit3 = GetS(SP3); pEdit3->SetWindowText(paramInfo[3]);
	CStatic* pEdit4 = GetS(SP4); pEdit4->SetWindowText(paramInfo[4]);
	CStatic* pEdit5 = GetS(SP5); pEdit5->SetWindowText(paramInfo[5]);
	CStatic* pEdit6 = GetS(SP6); pEdit6->SetWindowText(paramInfo[6]);
	CStatic* pEdit7 = GetS(SP7); pEdit7->SetWindowText(paramInfo[7]);
	CStatic* pEditL = GetS(ScanLegend); pEditL->SetWindowText(scanName);
}

//***************************************** Parameter/Lockin settings reading routines *************************************

//Reads the scan parameters from user input
//takes user input of start, end and step values and stores in a vector
vector<double> CControlToolDlg::readParameters()
{
	//Get the values from their corresponding controls
	CEdit* pEdit0 = Get(PAR0); CEdit* pEdit1 = Get(PAR1);
	CEdit* pEdit2 = Get(PAR2); CEdit* pEdit3 = Get(PAR3); 
	CEdit* pEdit4 = Get(PAR4); CEdit* pEdit5 = Get(PAR5);
	CEdit* pEdit6 = Get(PAR6); CEdit* pEdit7 = Get(PAR7);

	CString strP0,strP1,strP2,strP3,strP4,strP5,strP6,strP7;
	vector<double> parameters; //Vector where parameters will be stored

	//Save the values under their corresponding variables and construct vector
	pEdit0->GetWindowText(strP0); P0 = atof(strP0); parameters.push_back(P0);
	pEdit1->GetWindowText(strP1); P1 = atof(strP1); parameters.push_back(P1);
	pEdit2->GetWindowText(strP2); P2 = atof(strP2); parameters.push_back(P2);
	pEdit3->GetWindowText(strP3); P3 = atof(strP3); parameters.push_back(P3);
	pEdit4->GetWindowText(strP4); P4 = atof(strP4); parameters.push_back(P4);
	pEdit5->GetWindowText(strP5); P5 = atof(strP5); parameters.push_back(P5);
	pEdit6->GetWindowText(strP6); P6 = atof(strP6); parameters.push_back(P6);
	pEdit7->GetWindowText(strP7); P7 = atof(strP7); parameters.push_back(P7);
	return parameters;
}

//Method used to update the parameters for scanning from user input 
//and save them to string variable for further analysis use
void CControlToolDlg::updateLockinSettings()
{
	//Retrieve the values from their respective edit boxes
	CEdit* pEdit0 = (CEdit*)GetDlgItem(TConstant);
	CEdit* pEdit1 = (CEdit*)GetDlgItem(AverageTime);
	CEdit* pEdit2 = (CEdit*)GetDlgItem(DLFactor);
	CEdit* pEdit3 = (CEdit*)GetDlgItem(SLFactor); 
	CString strAverageTime, strDelayFactor, strSampleFactor, strTimeConstant;
	pEdit0->GetWindowText(strTimeConstant);
	pEdit1->GetWindowText(strAverageTime);
	pEdit2->GetWindowText(strDelayFactor);
	pEdit3->GetWindowText(strSampleFactor);

	//Save the values under their corresponding variables
	LockTC = atoi(strTimeConstant);
	LockAV = atoi(strAverageTime);
	LockDF = atoi(strDelayFactor);
	LockSF = atoi(strSampleFactor);
}

//On click the Read lockin button, print readings on the screen
void CControlToolDlg::OnBnClickedReadlockin()
{
	//updateLockinSettings();
	
	PrintInfo("(Signal: ");
	Sleep(200);	
	PrintInfo(lockin.ReadChannel(1)); // printing the result from ReadChannel response
	PrintInfo(" V");
	PrintInfo(", ");
	PrintInfo("Aux: ");
	Sleep(200);	
	PrintInfo(lockin.ReadAux(1)); // printing the result from ReadAux response
	PrintInfo(" V");
	PrintInfo(") \r\n");
}

//********************************************** Response to InfoBox *********************************************
//display a list of time constant and their corresponding key for user input when setting time constant
void CControlToolDlg::OnBnClickedInfoboxbutton()
{
	InfoBox timeConstantKey;
	timeConstantKey.DoModal();
}

//***************************************** Data Acquisition *************************************
//The data acquisition routine, which takes a pointspace generated
//by a scan class, move the delaylines, takes and stores the readings.
void CControlToolDlg::performAcquisition(vector<pointGroup> pointspace, int dimension, CString type)
{
	
	updateLockinSettings(); ///Read and update lock-in settings from interface
	lockin.setTimeConstant(LockTC); //Set the appropriate time constant 
	PositionTable.clear(); //Clear the positions from any earlier scans

	pointGroup posElement; //Creates a pointGroup object to hold position and unit
	double totalPoints = pointspace.size();
	double DevicePositions[10000][2]; //Create a 2D array for new position units (unit conversion ahead)
	double readingValue = 1; //Place holder (through iterations) for the Lockin average reading
	vector<double> readingValue2 (3000);
	double dumpValue = 1; //Temporary value for faulty lock-in reading
	vector<double> dumpValue2;
	int i = 0; int j = 0; //Iterating variables

	CProgressCtrl* bar = (CProgressCtrl*)GetDlgItem(TheProgressBar);  //Object that defines the progress bar
	bar->SetRange(10, totalPoints*10); //Sets the range of the progress bar
	double AvgNormWithAuxTable[200] = {0}; // initialize the table to 0;
	double AvgNormWithAuxSquareTable[200] = {0}; // initialize the table to 0;
	double avgInt[200] = {0};
	double avgAux[200] = {0};
	//Update position boxes in main intefarce and clock current time:
	updatePositionBoxes(0);
	time_t beginTime = time(NULL);
	time_t beginMoveTime;
	double totalMoveTime = 0;
	totalAverageTime = 0;
	double lastDL2 = mm3000.ConvertToUserUnit(mm3000.getActualPosition(), "ps"); //variable to store the last DL1 position, initialized to current value
	double lastDL1 = mercury.ConvertToUserUnit(mercury.getActualPosition(), "ps"); //variable to store the last DL2 position, initialized to current value

	//Dump an initial lock-in reading in case it is faulty:
	//dumpValue = AverageRead(LockTC, LockAV, LockDF, LockSF, lockin.currentChannel);
	dumpValue2 = UserDefinedReading(LockTC, LockAV, LockDF, LockSF, lockin.getCurrentChannel(), lockin.getCurrentAux());
	if (auxFlag == false) // do not record aux signal
	{
		if (dimension == 1) //If performing 1 Dimensional data acquisition
		{
			for(i = 0; i < totalPoints; ++i)
			{	
				double converted = selector == 1 ? //Convert position value to desired units:
				mm3000.ConvertToDeviceUnit(pointspace[i].DL1.position,"ps") : mercury.ConvertToDeviceUnit(pointspace[i].DL1.position,"ps"); 

				DevicePositions[i][j] = selector == 1 ? //Move and store position:
				mm3000.GoToInDeviceUnit(converted) : mercury.GoToInDeviceUnit(converted); 

				posElement.DL1.position = selector == 1 ? //Convert back to user units and store the position:
				mm3000.ConvertToUserUnit(DevicePositions[i][j], "ps") :
				mercury.ConvertToUserUnit(DevicePositions[i][j], "ps"); 
				posElement.DL1.unit = "ps"; //Store the units
				PositionTable.push_back(posElement); //Append points to the position vector
				//Take the average reading from the Lock-in:			
				readingValue = AverageRead(LockTC, LockAV, LockDF, LockSF, lockin.getCurrentChannel());
				ValueTable[i] = (readingValue * 1000); //Stores average reading in mV in ValueTable
				bar->SetPos(i*10+10); //Updates the progress bar
			}
		}
	}
	else if (auxFlag == true)// record aux signal
	{
		if (dimension == 1) //If performing 1 Dimensional data acquisition
		{
			// Iterate through the number array returned by the scanning functions for as many points as appropriate
			// These loops convert the ith element of the pointspace to desired units,
			// Move the delay line, store its position,
			// Take and store the lock-in average reading at each position.
			for(i = 0; i < totalPoints; ++i)
			{	
				beginMoveTime = time(NULL);//start time of a move
				if(pointspace[i].DL2.position != lastDL2)//only move DL1 if the new position is different than the last position
				{
					j = 0;//index for MM3000
					double DL2inDeviceUnit = mm3000.ConvertToDeviceUnit(pointspace[i].DL2.position,"ps");//convert DL1 position from ps to device unit
					lastDL2 = pointspace[i].DL2.position;//store the new position
					DevicePositions[i][j] = mm3000.GoToInDeviceUnit(DL2inDeviceUnit);//move to position and store the position in array
					posElement.DL2.position = mm3000.ConvertToUserUnit(DevicePositions[i][j], "ps");//Convert back to user units and store the position	
					posElement.tau2.position = pointspace[i].tau2.position;
				}
				else//pointspace[i].DL2.position == lastDL2
				{
					posElement.DL2.position = lastDL2; //use last DL position
					posElement.tau2.position = pointspace[i].tau2.position;
				}

				if(pointspace[i].DL1.position != lastDL1)//only move DL1 if the new position is different than the last position
				{
					j = 1;//index for Mercury
					double relativeStep = pointspace[i].DL1.position-lastDL1;
					double DL1inDeviceUnit = mercury.ConvertToDeviceUnit(relativeStep,"ps");//convert DL2 position from ps to device unit
					lastDL1 = pointspace[i].DL1.position;//store the new position
					DevicePositions[i][j] = mercury.MoveInDeviceUnit(DL1inDeviceUnit);
					posElement.DL1.position = mercury.ConvertToUserUnit(DevicePositions[i][j], "ps");//Convert back to user units and store the position
					posElement.tau1.position = pointspace[i].tau1.position;
				}
				else//pointspace[i].DL1.position == lastDL1
				{
					posElement.DL1.position = lastDL1; //use last DL position
					posElement.tau1.position = pointspace[i].tau1.position;
				}
				posElement.tauPlus.position = pointspace[i].tauPlus.position;
				posElement.Xi.position = pointspace[i].Xi.position;
				posElement.DL1.unit = "ps"; //Store the units
				posElement.DL2.unit = "ps"; //Store the units
				posElement.tau1.unit = "ps"; //Store the units
				posElement.tau2.unit = "ps"; //Store the units
				PositionTable.push_back(posElement); //Append points to the position vector
				totalMoveTime += time(NULL) - beginMoveTime;//add time of a move to the total time
				
				//Take reading from the Lock-in:
				readingValue2 = UserDefinedReading(LockTC, LockAV, LockDF, LockSF, lockin.getCurrentChannel(), lockin.getCurrentAux());
				for (int k = 1; k <= totalReads; ++k)
				{
					TwoDValueTable[i][k] = (readingValue2[(k+1)*2-1] * 1000); //Stores signal reading in mV in TwoDValueTable
					TwoDAuxTable[i][k] = readingValue2[k*2] * 1000; //Stores aux reading in mV in TwoDAuxTable
					TwoDNormWithAuxTable[i][k] = readingValue2[(k+1)*2-1]/readingValue2[k*2];//Calculate each pair of Int/Aux and store them in TwoDNormWithAuxTable
					AvgNormWithAuxTable[i] += TwoDNormWithAuxTable[i][k];//Calculate the sum of all Int/Aux values, used for further calculate the avg Int/aux
					TwoDNormWithAuxSquareTable[i][k] = readingValue2[(k+1)*2-1]/(readingValue2[k*2] * readingValue2[k*2] * 1000);//Stores int/aux^2 TwoDNormWithAuxSquareTable
					AvgNormWithAuxSquareTable[i] += TwoDNormWithAuxSquareTable[i][k];//Calculate the sum of all Int/Aux^2 values, used for further calculate the avg Int/aux^2
					avgInt[i] += TwoDValueTable[i][k];
					avgAux[i] += TwoDAuxTable[i][k];
				}
				AvgNormWithAuxTable[i] = AvgNormWithAuxTable[i]/totalReads; //calculate the avg Int/Aux
				AvgNormWithAuxSquareTable[i] = AvgNormWithAuxSquareTable[i]/totalReads; //calculate the avg Int/Aux^2
				avgInt[i] = avgInt[i]/totalReads;//calculate the avg Int
				avgAux[i] = avgAux[i]/totalReads;//calculate the avg Aux
			
				bar->SetPos(i*10+10); //Updates the progress bar
			}
		}
		else if (dimension == 2) //If performing 2 Dimensional data acquisition
		{
			// Iterates through the number array returned by the scanning functions for as many points as appropriate
			// This loop converts the ith element of each delay line to desired units,
			// Moves the corresponding delay line, stores the positions,
			// Takes and stores the lock-in average reading at each position,
			for(i = 0; i < totalPoints; ++i)
			{
				j = 0; //Dealing with the first delay line information
				double converted = mm3000.ConvertToDeviceUnit(pointspace[i].DL2.position,"ps"); //Converted position value to desired units
				DevicePositions[i][j] = mm3000.GoToInDeviceUnit(converted); //Move device to position, store the current position
				posElement.DL2.position = mm3000.ConvertToUserUnit(DevicePositions[i][j], "ps"); //Convert back to user units and store the position
				posElement.DL2.unit = "ps"; //Store the units
				PositionTable.push_back(posElement);
				j = 1; //Dealing with the second delay line information
				converted = mercury.ConvertToDeviceUnit(pointspace[i].DL1.position,"ps"); //Converted position value to desired units
				DevicePositions[i][j] = mercury.GoToInDeviceUnit(converted); //Move device to position, store the current position
				posElement.DL1.position = mercury.ConvertToUserUnit(DevicePositions[i][j], "ps"); //Convert back to user units and store the position
				posElement.DL1.unit = "ps"; //Store the units
				PositionTable.push_back(posElement);

				//Take the average reading from the Lock-in:
				readingValue = AverageRead(LockTC, LockAV, LockDF, LockSF, lockin.getCurrentChannel());
				ValueTable[i] = (readingValue * 1000); //Stores average reading in mV in ValueTable
				bar->SetPos(i*10+10); //Updates the progress bar
			}
		}

		double completionTime = time(NULL) - beginTime; //Calculates taken for complete data acquisition
		//Write results to text file:
		DataPrint print; print.WritetoFile_NormalizedSignal(PositionTable, ValueTable, AuxTable, AvgNormWithAuxTable, AvgNormWithAuxSquareTable, avgInt, avgAux, TwoDValueTable, 
				TwoDAuxTable, TwoDNormWithAuxTable, TwoDNormWithAuxSquareTable, dimension, auxFlag, selector, scanName, completionTime, totalMoveTime, totalAverageTime, totalReads);
			PrintInfo("Acquisition Complete. Results saved in /Debug/LockinData.csv \r\n");
		updatePositionBoxes(1); //Display the final positions
	}
}

//***************************** Averageing routines *****************************
// Calculates the average intensity reading from one channel from a given device by a specific number of readings.
// Returns the average reading.
double CControlToolDlg::AverageRead(int timeConstant, double AvgTime, double DelayFactor, double SampleFactor, int channel)
{
	
	double reading; //Current reading from lockin
    double result = 0; //Sum of all current lockin readings
    int totalReads = 0; //Current number of readings during averaging time
    int failReads = 0; //Number of reading attempts failed
	int i = 0; //Iterating variable used in reading loop
	
	actualTC = lockin.timeConstantConvert(timeConstant); //units in seconds
	delayTime = DelayFactor * actualTC; //Time to wait before begin averaging, units in seconds
	sampleTime = actualTC / SampleFactor; //Time in betweeen lockin reads through averaging time, units in seconds
	targetReads = AvgTime / sampleTime; //Target number of readings to perform during averaging time
	
    Sleep(1000*delayTime); //Wait for a delay time to begin scanning, units in ms
    for(i; i <= targetReads; ++i) //Read device and wait for as many times as appropriate
    {
	    reading = lockin.ReadChannel(channel); //Takes the reading from the Lock-in
		result += reading; //Updates sum of readings
		++totalReads; //Updates number of readings
	    Sleep(1000*sampleTime); //Wait appropriate time for next reading, units in ms
    }
    return result/totalReads; //Returns average intensity reading
}

// Record intensity reading from one channel as well as from the auxiliary input.
// Returns an array of the unnormalized singal and axuiliary signal
vector<double> CControlToolDlg::UserDefinedReading(int timeConstant, double AvgTime, double DelayFactor, double SampleFactor, int channel, int auxPort)
{
	time_t beginAverage = time(NULL);
	vector<double> reading (3000); // record final reading
    totalReads = 0; //Current number of readings during averaging time
	
	actualTC = lockin.timeConstantConvert(timeConstant); //units in seconds (eg: 0.3s)
	delayTime = DelayFactor * actualTC; //Time to wait before begin averaging, units in seconds (eg: 1*0.3s)
	//sampleTime = actualTC / SampleFactor; //Time in betweeen lockin reads through averaging time, units in seconds (eg: 0.3s)
	sampleTime = 0.15;/*the reason we set the sampleTime to be 0.15s and the wait time after two reading to be zero is
						that the time it takes for readChannel and readAux are 0.075s each. Number of reading = average time/0.15 */
	targetReads = AvgTime / sampleTime; //Target number of readings to perform during averaging time
    Sleep(1000*delayTime); //Wait for a delay time to begin scanning, units in ms (eg: 300ms)

    for(int k = 1; k <= targetReads+1; ++k) //Read device and wait for as many times as appropriate
    {
		reading[2*k-1] = lockin.ReadChannel(channel);
		reading[2*k] = lockin.ReadAux(auxPort);
		//Sleep(1000*sampleTime); //Wait appropriate time for next reading, units in ms (eg: 100ms), because the time for read channel 
		                          //and read aux is around 70ms,so we drop this waiting time 10/28/2014
		totalReads += 1; //Updates number of readings
    }
	totalReads-=1;//shift one point of time to match int and aux
	totalAverageTime += time(NULL) - beginAverage;
	return reading;
}

//*****************************Accessors**********************************************************
//method used to retrieve selection variable of delayline, 1 = MM3000 delay line, 2 = Mercury delay line
int CControlToolDlg::getSelector()
{
	return selector;
}
//method to retrieve movement type, 1 = absolute, 2 = relative
int CControlToolDlg::getMoveType()
{
	return movementType;
}
//method to retrieve index of the item selected in dropdown scan list
//0 = 1D linear, 1 = 2D linear, 2 = 1D log, 3 = 2D log, 4 = abstract Scan (tauPlus and Xi)
int CControlToolDlg::getScanNameIndex()
{
	return index;
}
//method to check whether to read aux signal or not
//return true if needs to read
bool CControlToolDlg::getAuxFlag()
{
	return auxFlag;
}
//method to retrieve lock in time constant
int CControlToolDlg::getLockTC()
{
	return LockTC;
}
//method to retrieve average time at one delay position
int CControlToolDlg::getLockAV()
{
	return LockAV;
}
//method to retrieve delay factor
int CControlToolDlg::getLockDF()
{
	return LockDF;
}
//method to retrieve sampling factor
int CControlToolDlg::getLockSF()
{
	return LockSF;
}
//method to get progress bar
CProgressCtrl* CControlToolDlg::getProgressBar()
{
	CProgressCtrl* bar = (CProgressCtrl*)GetDlgItem(TheProgressBar);  //Object that defines the progress bar
	return bar;
}
