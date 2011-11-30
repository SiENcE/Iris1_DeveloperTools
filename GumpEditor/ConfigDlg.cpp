// ConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GumpEditor.h"
#include "ConfigDlg.h"


// CConfigDlg dialog

IMPLEMENT_DYNAMIC(CConfigDlg, CDialog)

CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlg::IDD, pParent)
	, m_strXmlViewerPath(_T(""))
	, m_strIrisPath(_T(""))
{

}

CConfigDlg::~CConfigDlg()
{
}

void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_XML_EDIT, m_strXmlViewerPath);
	DDX_Text(pDX, IDC_IRIS_EDIT, m_strIrisPath);
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
END_MESSAGE_MAP()


// CConfigDlg message handlers
