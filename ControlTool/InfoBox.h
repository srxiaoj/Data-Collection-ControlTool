
#pragma once
#include "afxdialogex.h"
#include "ControlTool.h"
#include <Windows.h>


class InfoBox : public CDialog
{
	DECLARE_DYNAMIC(InfoBox)

public:
	InfoBox(CWnd* pParent = NULL); //Standard constructor
	virtual ~InfoBox();
	enum { IDD = IDD_INFOBOX };
 

protected:
	virtual void DoDataExchange(CDataExchange* pDX); //DDX/DDV support
	DECLARE_MESSAGE_MAP()

};
