// DynCtrlTypeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GumpEditor.h"
#include "GumpControlTypeDlg.h"
#include "entity/GumpEntityFactory.h"

IMPLEMENT_DYNAMIC(CGumpControlTypeDlg, CDialog)
CGumpControlTypeDlg::CGumpControlTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGumpControlTypeDlg::IDD, pParent)
	, m_iCtrlType(0), m_iStartCtrlType(CGumpEntityFactory::PICTURE)
{
}

CGumpControlTypeDlg::~CGumpControlTypeDlg()
{
}

void CGumpControlTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lbControl);
	DDX_LBIndex(pDX, IDC_LIST1, m_iCtrlType);
}

BEGIN_MESSAGE_MAP(CGumpControlTypeDlg, CDialog)
END_MESSAGE_MAP()


BOOL CGumpControlTypeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	for (int i = m_iStartCtrlType; i < CGumpEntityFactory::GetTypeCount(); i++)
		m_lbControl.AddString(CGumpEntityFactory::GetTypeName(CGumpEntityFactory::TYPE(i)));

	m_lbControl.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
}

CGumpEntityFactory::TYPE CGumpControlTypeDlg::GetControlType()
{
	return CGumpEntityFactory::TYPE(m_iCtrlType + m_iStartCtrlType);
}