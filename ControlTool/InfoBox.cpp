
#include "stdafx.h"
#include "InfoBox.h"
#pragma once

IMPLEMENT_DYNAMIC(InfoBox, CDialog)

InfoBox::InfoBox(CWnd* pParent /*=NULL*/)
	: CDialog(InfoBox::IDD, pParent)
{
}

InfoBox::~InfoBox()
{
}

void InfoBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(InfoBox, CDialog)
END_MESSAGE_MAP()
