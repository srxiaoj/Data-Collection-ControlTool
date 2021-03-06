
=========================================

    ControlTool Project file overview   
	Last reviewed: 02/12/15         
                                       
=========================================

This text file is an outline of the functions/implementation of the files that currently compose 
the ControlTool application project.

The main function of this application is to construct an interface through which the user can control
a number of devices, parameters and procedures used in experiment. Such devices currently include 
the lock-in amplifier and two delay lines (Mercury and MM3000).

The project was built using an application wizard which utilizes the MFC application class CwinApp

CWinApp:
	The main application class in MFC encapsulates the initialization, running, 
	and termination of an application for the Windows operating system. An application 
	built on the framework must have one and only one object of a class derived from CWinApp. 
	This object is constructed before windows are created.


//////// List of the main files used in the project ////////////////////////////////////////////// 

	ControlTool.h / ControlTool.cpp
	ControlToolDlg.h / ControlToolDlg.cpp
	Acquisition.h / Acquisition.cpp
	Datastructure.h
	DeviceDependent.h / DeviceDependent.cpp
	DeviceIndependent.h / DeviceIndependent.cpp
	Mercury.h / Mercury.cpp
	MM300.h / MM3000.cpp
	Lockin.h / Lockin.cpp
	LockinInterface.h / LockinInterface.cpp
	ScanBase.h
	Serial.h / Serial.cpp
	PI_Mercury_GCS_DLL.h


//////// File content descriptions ///////////////////////////////////////////////////////////////

ControlTool.h:

	- Header file used to declare the applications class CControlToolApp.
	- In this file, the one and only object (theApp) of the CControlToolApp
	  class (which is derived from CWinApp) is declared.
	- Declares the InitInstance function, which allows several copies of the 
	  same program to run at the same time. Typically, you override InitInstance 
	  to construct your main window object and set the CWinThread::m_pMainWnd 
	  data member to point to that window. 

ControlTool.cpp:

	- This is the main implementation file, it implements the ControlTool class. 
	- Calls the DoModal member function to invoke the dialog box (application interface).
	- The aformentioned dialog box is constructed in ControlToolDlg.cpp

ControlToolDlg.h:

	- Header file for the class responsible for the functionality of the dialog box.
	- This file declares the functions needed to obtain and relay data (user input) 
	  from the interface. Resource ID = IDD_CONTROLTOOL_DIALOG

ControlToolDlg.cpp:

	- Imlements the main dialog box class.
	- Defines the functions used to retrieve info and initialize experiment (delay line 
	  selection, scanning method, etc..) as well as all of the functionality of the dialog box

Acquisition.h:

	- Header file for the Acquisition class, a CDialog child class with the same resource ID
	  as the main dialog box (IDD_CONTROLTOOL_DIALOG) to allow control of the progress bar.
	- Contains declarations of functions used to perform data acquisition 
	  and write the results to a text file.

Acquisition.cpp:

	- Implements the Acquisition class.
	- Defines the functions declared  used to perform data acquisition 
	  and write the results to a text file.

DataStructure.h:

	- This file contains four 'struct' definitions used to handle data in terms of 
	  device parameters and scanning data points.
	- Refer to file for further definition of content.

DeviceDependent.h:

	- Header file for the Dependent Device class, a base class used to derive all other classes 
	  for actual experimental devices.
	- Contains declarations of pure virtual functions defined in their respective child
	  device class implementation files.
	- Contains declarations of functions used to send/receive data from devices, identify device 
	  by name and port number and functions used to set and get zero positions.
	- There are currently 2 child dependent device classes:
		Mercury: class pertaining to one of the two delay lines
		MM3000: class pertaining to the other delay line

DeviceDependent.cpp:

	- Implementation file for the Dependent Device base class.
	- Dependent Devices are those whose activity we control through the interface.
	- Contains function definitions.
	  
DeviceIndependent.h:

	- Header file for the Independent Device class, a base class used to derive all other 
	  classes for actual experimental devices.
	- Contains declarations of pure virtual functions defined in their respective child
	  device class implementation files.
	- Contains declarations of functions used to send/receive data from devices, identify device 
	  by name and port number and functions used to set and get zero positions.
	- There are currently 1 child independent device classes:
		Lockin: class pertaining to the SR850 Lock-in amplifier

DeviceIndependent.cpp:

	- Implementation file for the Independent Device base class.
	- Independent Devices are those whose activity does not directly depend on the interface.
	- Contains function definitions.

Mercury.h:

	- Header file for the Mercury class, one of the two delay lines.
	- This file includes the declarations of several functions used to control device
	  initialization, movement, position etc.

Mercury.cpp:

	- Implementation file for the Mercury class.
	- Defines the functions mentioned above. See source code for details.

MM3000.h:

	- Header file for the MM3000 class, the other of the two delay lines.
	- Similarly, this file includes the declarations of several functions used to control
	  device initialization, movement, position etc.

MM300.cpp:

	- Implementation file for the MM3000 class.
	- Defines the functions mentioned above. See source code for details.

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

NOTE: The delay lines make use of a Physik Instrumente (PI) precision motion controller
	  and a Newport MM3000 motion controller for movement/positioning of the Mercury and
	  MM3000 delay lines respectively. The Mercury delay line is controlled by using 
      the PI General Command Set (GCS) via a DDL (dynamically linked library) whereas 
      the MM3000 delay line is controlled by using its own command set. The source code 
      for each delay line class is used to relay the respective commands from the interface 
      to the motion controller. Refer to respective command set literature for command descriptions.
      Both manuals can be found as a PDF online (links in respective source files). 

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Lockin.h:

	- Header file for the Lockin class; this is the class responsible for the control of
	  the lock-in amplifier.
	- Contains declarations of lockin-related functions, most importantly, the function
	  used to read the average intensity values.

Lockin.cpp:

	- Implements the class Lockin and defines the lock-in related functions.
	- Contains definition of the ReadAverage functions.
	- ReadAverageByTime performs averaging for the specified averaging time
	  and no longer, regardless of how many individual reads it performed.
	- ReadAverageRead performs averaging for a specific number of reads 
	  calculated from the averaging time and the sampling time.

LockinInterface.h:

	- Header file for the LockinInterface class, a CDialog child class used to control
	  the Lock-In settings sub-dialog box. Resource ID = IDD_LOCKINBOX
	- Declares the function used to retrieve information from the sub-dialog box

LockinInterface.cpp:

	- Implements the LockinInterface class. 
	- Defines the method used to relay information from the box to the program.

ScanBase.h:

	- Header file for the ScanBase class, a base calss used to derive all other
	  experiment child classes.
	- Cointains declarations of pure virtual functions used to get appropriate parameters,
	  gererate the respective pointspace and perform the scanning routine.

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

NOTE: The program has been comptartmentalized in order to allow for easy implementation of new
	  experiment designs. There are currently four (1D/2D linear/sech scanning routines) 
	  designs represented by a Scan child class derived from the ScanBase parent class.
	  In order to implement a new experiment design, we only needs to create a new class derived
	  from the ScanBase class and define the three functions respectively.

		getParameters() simply assigns however many parameters are necessary from the generic
		list of parameters in the main interface.
		
		GeneratePointspace() define the algorith that will be used to calculate the points at
		which the delay lines (or other devices) will move and data acquisition will happen.
		This function will use the parameters defined in the previous function, and should return
		a vector of 'pointGroup' objects containing the pointspace.

		performScan() simply calls the acquisition function and passes it the pointspace 
		generated by the previous function.

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 

Serial.h:

	- Header file for the CSerial class, used to open/close serial connections to given
	  devices as well as to dens recieve information to devices.
	- Cointains declaration of such functions.

Serial.cpp:

	- Implementation file for the CSerial class. 
	- This file contains the definitions of the Open/Close functions used for connections.
	- Contains definitions of the ReadData and WriteData functions, functions used to send
	  and reveive certain information, such as a command, to devices like delay lines.

PI_Mercury_GCS_DLL.h:

	- This header file contains the declaration of the functions used by the PI GCS.
	- This is a dynamically linked library needed for the Mercury device data exchange.

	

