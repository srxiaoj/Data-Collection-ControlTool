
// ControlTool.h : main header file for the ControlTool application.
// This file declares the classes CControlToolApp and CWinAppEx.
/* The CWinAppEx class handles the application state, saves the state
	the registry, loads the state from the registry, initializes application
	managers, and provides links to those same application managers. */

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "stdafx.h"
#include "resource.h"
#include "ControlToolDlg.h"

// CControlToolApp:
// See ControlTool.cpp for the implementation of this class

class CControlToolApp : public CWinAppEx
{
public:
	CControlToolApp();

	//A couple of macros to help with control editing on interface:
	#define Enable(x) GetDlgItem(x)->EnableWindow(TRUE) //Macro to enable a control
	#define Disable(x) GetDlgItem(x)->EnableWindow(FALSE) //Macro to disable a control
	#define Get(x) (CEdit*)GetDlgItem(x); //Macro to get edit control item from interface
	#define GetS(x) (CStatic*)GetDlgItem(x); //Macro to get static text control from interface

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP() // Declares that the class CControlToolApp
						  // defines a message map.
};

extern CControlToolApp theApp; // Declares the theApp object as an extern type
							   // (visible to outside files)						   // (visible to outside files)
