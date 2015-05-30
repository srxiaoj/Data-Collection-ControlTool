
// ControlTool.cpp : Defines the class behaviors for the application.
/* This file implements the class CControlToolApp, initiates the ControlTool 
	dialog box (main interface) constructed by by ControlToolDlg.cpp */

#include "stdafx.h"
#include "ControlTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//CControlToolApp

BEGIN_MESSAGE_MAP(CControlToolApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp) //Maps the special help command to F1 key
END_MESSAGE_MAP()

CControlToolApp::CControlToolApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


//The one and only CControlToolApp object
//This unique object is from the CControlToolApp class (derived from
//the CWinApp class)
CControlToolApp theApp;


//CControlToolApp initialization
BOOL CControlToolApp::InitInstance()
{
	CWinAppEx::InitInstance();

	AfxEnableControlContainer(); /* Enables support for containment of OLE 
									(Object Linking and Embedding) controls */

	// Change the registry key under which our settings are stored:
	SetRegistryKey(_T("Berg ControlTool Application"));

	CControlToolDlg dlg;
	m_pMainWnd = &dlg; //Stores the dlg pointer to our main window object
	INT_PTR nResponse = dlg.DoModal(); //Summons the dialog box
	if (nResponse == IDOK)
	{
		/* TODO: Place code here to handle when the
		   dialog is dismissed with OK */ 
	}
	else if (nResponse == IDCANCEL)
	{
		/* TODO: Place code here to handle when the
		   dialog is dismissed with Cancel */
	}
	/* Since the dialog has been closed, return FALSE so that we exit the
	   application, rather than start the application's message pump */
	return FALSE;
}
