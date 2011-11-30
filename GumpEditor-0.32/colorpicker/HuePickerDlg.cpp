// HuePickerDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "../GumpEditor.h"
#include "HuePickerDlg.h"
#include ".\huepickerdlg.h"


// CHuePickerDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CHuePickerDlg, CDialog)
CHuePickerDlg::CHuePickerDlg(DWORD hueId /*=1*/,CWnd* pParent /*=NULL*/)
	: CDialog(CHuePickerDlg::IDD, pParent)
	, m_strHueID(_T("0x1"))
	, m_strRGB(_T("0,0,0"))
	, m_color(0), m_hueId(hueId)
	, m_bRGBHex(TRUE)
	, m_bHueIDHex(TRUE)
{
}

CHuePickerDlg::~CHuePickerDlg()
{
}

void CHuePickerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_HUEID_EDIT, m_strHueID);
	DDX_Text(pDX, IDC_RGB_EDIT, m_strRGB);
	DDX_Check(pDX, IDC_CHECK1, m_bRGBHex);
	DDX_Check(pDX, IDC_CHECK2, m_bHueIDHex);
}


BEGIN_MESSAGE_MAP(CHuePickerDlg, CDialog)
	ON_MESSAGE(WM_HUE_CHANGED, OnHueChanged)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnBnClickedCheck1)
END_MESSAGE_MAP()


// CHuePickerDlg 메시지 처리기입니다.

BOOL CHuePickerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CRect rect;
	GetDlgItem(IDC_HUE)->GetWindowRect(rect);
	ScreenToClient(rect);
	m_wndHue.Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_VSCROLL, rect, this, IDC_HUE);
	SelectHueId(m_hueId);
	return TRUE;  
}

void CHuePickerDlg::UpdateRGB()
{
	if (m_bRGBHex)
		m_strRGB.Format("%X,%X,%X", GetRValue(m_color), GetGValue(m_color), GetBValue(m_color));
	else
		m_strRGB.Format("%d,%d,%d", GetRValue(m_color), GetGValue(m_color), GetBValue(m_color));

	if (m_bHueIDHex)
		m_strHueID.Format("0x%X", GfxAtoX(m_strHueID));
	else
		m_strHueID.Format("%d", GfxAtoX(m_strHueID));
}	

LRESULT CHuePickerDlg::OnHueChanged(WPARAM wParam, LPARAM lParam)
{
	m_hueId = wParam;
	m_color = lParam;
	m_strHueID = GfxXtoA(m_hueId);
	UpdateRGB();

	CRect rect;
	GetDlgItem(IDC_COLOR_STATIC)->GetWindowRect(rect);
	ScreenToClient(rect);
	InvalidateRect(rect);
	UpdateData(FALSE);
	return 0;
}
void CHuePickerDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;
	GetDlgItem(IDC_COLOR_STATIC)->GetWindowRect(rect);
	ScreenToClient(rect);
	dc.FillSolidRect(rect,m_color);
}

void CHuePickerDlg::OnBnClickedCheck1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	UpdateRGB();
	UpdateData(FALSE);
}

void CHuePickerDlg::SelectHueId(DWORD hueId)
{
	m_wndHue.SelectHue(hueId);
}

void CHuePickerDlg::SelectColor(CString strColor)
{
}

BOOL CHuePickerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		UpdateData();
		if (pMsg->hwnd == GetDlgItem(IDC_HUEID_EDIT)->GetSafeHwnd()) {
			SelectHueId(GfxAtoX(m_strHueID));
		} else if (pMsg->hwnd == GetDlgItem(IDC_RGB_EDIT)->GetSafeHwnd()) {
			SelectColor(m_strRGB);
		}
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

