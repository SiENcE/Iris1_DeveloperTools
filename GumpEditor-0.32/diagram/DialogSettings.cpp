// DialogSettings.cpp : implementation file
//

#include "stdafx.h"
#include "DialogSettings.h"
#include ".\dialogsettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSettings dialog


CDialogSettings::CDialogSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSettings::IDD, pParent)
	, m_strName(_T(""))
	, m_iAlpha(0)
	, m_iFlags(0)
	, m_iFadeAlpha(0)
	, m_iFadeTime(0)
	, m_strShapeName(_T(""))
{
	m_height = 0;
	m_width = 0;
	m_gridHeight = 0;
	m_gridWidth = 0;
	m_marginLeft = 0;
	m_marginTop = 0;
	m_marginRight = 0;
	m_marginBottom = 0;
}


void CDialogSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_height);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_width);
	DDX_Text(pDX, IDC_EDIT_GRID_HEIGHT, m_gridHeight);
	DDX_Text(pDX, IDC_EDIT_GRID_WIDTH, m_gridWidth);
	DDX_Text(pDX, IDC_EDIT_LEFT, m_marginLeft);
	DDX_Text(pDX, IDC_EDIT_TOP, m_marginTop);
	DDX_Text(pDX, IDC_EDIT_RIGHT, m_marginRight);
	DDX_Text(pDX, IDC_EDIT_BOTTOM, m_marginBottom);

	DDX_Text(pDX, IDC_EVENT_ONCLOSE, m_strEvClose);
	//DDX_Text(pDX, IDC_EVENT_ONCLICK, m_strEvClick);
	//DDX_Text(pDX, IDC_EVENT_ONMOUSEUP, m_strEvMouseUp);
	//DDX_Text(pDX, IDC_EVENT_ONMOUSEDOWN, m_strEvMouseDown);
	//DDX_Text(pDX, IDC_EVENT_ONKEYPRESSED, m_strEvKeyPressed);
	DDX_Control(pDX, IDC_ALPHASPIN, m_spinAlpha);
	DDX_Text(pDX, IDC_ALPHA, m_iAlpha);
	//DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_FADE_ALPHA, m_iFadeAlpha);
	DDX_Text(pDX, IDC_FADE_TIME, m_iFadeTime);
	DDX_Control(pDX, IDC_SPIN1, m_spinFormWidth);
	DDX_Control(pDX, IDC_SPIN2, m_spinFormHeight);
	DDX_Control(pDX, IDC_SPIN3, m_spinGridWidth);
	DDX_Control(pDX, IDC_SPIN6, m_spinGridHeight);
	DDX_Control(pDX, IDC_ALPHASPIN2, m_spinFadeAlpha);
	DDX_Control(pDX, IDC_ALPHASPIN3, m_spinFadeTime);
	DDX_Control(pDX, IDC_SPIN7, m_spinLeft);
	DDX_Control(pDX, IDC_SPIN9, m_spinTop);
	DDX_Control(pDX, IDC_SPIN10, m_spinRight);
	DDX_Control(pDX, IDC_SPIN11, m_spinBottom);
	DDX_Text(pDX, IDC_SHAPE, m_strShapeName);
}


BEGIN_MESSAGE_MAP(CDialogSettings, CDialog)
END_MESSAGE_MAP()

BOOL CDialogSettings::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_spinAlpha.SetRange32(0,0xffff);
	m_spinAlpha.SetRange32(0,0xffff);
	m_spinFormWidth.SetRange32(0,0xffff);
	m_spinFormHeight.SetRange32(0,0xffff);
	m_spinGridWidth.SetRange32(0,0xffff);
	m_spinGridHeight.SetRange32(0,0xffff);
	m_spinFadeAlpha.SetRange32(0,0xffff);
	m_spinFadeTime.SetRange32(0,0xffff);
	m_spinLeft.SetRange32(0,0xffff);
	m_spinTop.SetRange32(0,0xffff);
	m_spinRight.SetRange32(0,0xffff);
	m_spinBottom.SetRange32(0,0xffff);

	return TRUE;
}
