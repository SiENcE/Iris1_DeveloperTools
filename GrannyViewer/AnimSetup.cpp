// AnimSetup.cpp : implementation file
//

#include "stdafx.h"
#include "GrannyViewer.h"
#include "AnimSetup.h"


// CAnimSetupDlg dialog

IMPLEMENT_DYNAMIC(CAnimSetupDlg, CDialog)

CAnimSetupDlg::CAnimSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAnimSetupDlg::IDD, pParent)
	, m_iTime(0)
{

}

CAnimSetupDlg::~CAnimSetupDlg()
{
}

void CAnimSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_SLIDER1, m_iTime);
	DDX_Control(pDX, IDC_SLIDER1, m_slider);
}


BEGIN_MESSAGE_MAP(CAnimSetupDlg, CDialog)
END_MESSAGE_MAP()


// CAnimSetupDlg message handlers

int CAnimSetupDlg::GetTime()
{
	return m_iTime + 33;
}

void CAnimSetupDlg::SetTime(int iTime)
{
	m_iTime = max(0, iTime - 33);
}

BOOL CAnimSetupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_slider.SetRange(0, 1000-33); // 30ms - 1sec, reflesh time
	m_slider.SetPos(m_iTime);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
