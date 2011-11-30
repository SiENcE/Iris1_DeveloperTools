// GumpStaticPropertyDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GumpEditor.h"
#include "GumpEditorDoc.h"
#include ".\GumpStaticPropertyPage.h"
#include "GumpStatic.h"
#include "iris/config.h"

IMPLEMENT_DYNAMIC(CGumpStaticPropertyPage, CDiagramPropertyPage)

CGumpStaticPropertyPage::CGumpStaticPropertyPage(CWnd* pParent)
	: CDiagramPropertyPage(CGumpStaticPropertyPage::IDD)
	, m_iTextAlign(0)
	, m_strText(_T("Sample String 1234"))
	, m_strFontId(_T(""))
{
}

CGumpStaticPropertyPage::~CGumpStaticPropertyPage()
{
}

void CGumpStaticPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COLOR, m_btnColor);
	DDX_CBIndex(pDX, IDC_COMBO2, m_iTextAlign);
	DDX_Text(pDX, IDC_TEXT, m_strText);
	DDX_Control(pDX, IDC_FONT_COMBO, m_cbFont);
	DDX_CBString(pDX, IDC_FONT_COMBO, m_strFontId);
}


BEGIN_MESSAGE_MAP(CGumpStaticPropertyPage, CDiagramPropertyPage)
	ON_CBN_SELCHANGE(IDC_FONT_COMBO, OnCbnSelchangeFontCombo)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_TEXT, OnEnChangeText)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnCbnSelchangeCombo2)
	ON_MESSAGE(CPN_SELCHANGE, OnColorChange)
END_MESSAGE_MAP()


BOOL CGumpStaticPropertyPage::OnInitDialog()
{ 
	CDiagramPropertyPage::OnInitDialog();

	m_btnColor.SetStyle(TRUE);
	m_btnColor.SetTrackSelection(TRUE);
	m_btnColor.SetDefaultText("");

	CGumpEditorDoc* pDoc = GfxGetGumpDocument(); ASSERT(pDoc);

	using namespace std;
	CString strId;
	for (vector<nConfig::font_info>::iterator iter=nConfig::fonts.begin(); iter != nConfig::fonts.end(); iter++)
	{
		if (!pDoc->GetFont(iter->id)) continue;

		strId.Format("%d", iter->id);
		m_cbFont.AddString(strId);
	}

	if (LB_ERR != m_cbFont.SetCurSel(3))
	{
		m_cbFont.GetLBText(m_cbFont.GetCurSel(), strId);
	}

	m_btnColor.SetFontId(GetFontId());

	return TRUE;  
}


void CGumpStaticPropertyPage::SetValues()
{
	if( GetSafeHwnd() && GetEntity() )
	{
		CGumpStatic* pStatic = (CGumpStatic*)GetEntity();
		
		m_strFontId.Format("%d", pStatic->GetFontId());
		m_btnColor.SetHueId(pStatic->GetHueId());
		m_iTextAlign = pStatic->GetTextAlign();
		m_strText = pStatic->GetTitle();
		
		UpdateData(FALSE);
	}
}

void CGumpStaticPropertyPage::ApplyValues()
{
	if( GetSafeHwnd() && GetEntity() )
	{
		UpdateData();
		CGumpStatic* pStatic = (CGumpStatic*)GetEntity();
		pStatic->SetTitle(m_strText);
		pStatic->SetHueId(m_btnColor.GetHueId());
		pStatic->SetFontId(atoi(m_strFontId));
		pStatic->SetTextAlign((TEXT_ALIGN)m_iTextAlign);
		
		Redraw();
	}	
}

int CGumpStaticPropertyPage::GetFontId()
{
	CString strId;
	int i = m_cbFont.GetCurSel();
	m_cbFont.GetLBText(i, strId);

	return ::atoi(strId);
}

void CGumpStaticPropertyPage::OnCbnSelchangeFontCombo()
{
	UpdateData();

	m_btnColor.SetFontId(GetFontId());

	CRect rect;
	GetDlgItem(IDC_TEXT_STATIC)->GetWindowRect(rect);
	ScreenToClient(rect);
	InvalidateRect(rect);
}

void CGumpStaticPropertyPage::OnPaint()
{
	CPaintDC dc(this);
	CString strId;
	int i = m_cbFont.GetCurSel();
	m_cbFont.GetLBText(i, strId);
	CGumpEditorDoc* pDoc = GfxGetGumpDocument();
	
	CFont* pOldFont = dc.SelectObject(pDoc->GetFont(GetFontId()));
	COLORREF crOld = dc.SetTextColor(m_btnColor.GetColor());
	dc.SetBkMode(TRANSPARENT);

	CRect rect;
	GetDlgItem(IDC_TEXT_STATIC)->GetWindowRect(rect);
	ScreenToClient(rect);
	rect.DeflateRect(5,0,0,5);

	UINT nFormat[] = { DT_LEFT, DT_CENTER, DT_RIGHT };
	
	dc.DrawText(m_strText, rect, DT_SINGLELINE | DT_VCENTER | nFormat[m_iTextAlign]);

	dc.SelectObject(pOldFont);
	dc.SetTextColor(crOld);
}

void CGumpStaticPropertyPage::OnEnChangeText()
{
	OnCbnSelchangeFontCombo();
}

void CGumpStaticPropertyPage::OnCbnSelchangeCombo2()
{
	OnCbnSelchangeFontCombo();
}

LRESULT CGumpStaticPropertyPage::OnColorChange(WPARAM,LPARAM)
{
	OnCbnSelchangeFontCombo();
	return 0;
}