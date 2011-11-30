// SetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GrannyViewer.h"
#include "SetupDlg.h"


// CSetupDlg dialog

IMPLEMENT_DYNAMIC(CSetupDlg, CDialog)

CSetupDlg::CSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupDlg::IDD, pParent)
	, m_bTextChunk(FALSE)
	, m_bMesh(FALSE)
	, m_bObject(FALSE)
	, m_bObjPtr(FALSE)
	, m_bBone(FALSE)
	, m_bBoneAnim(FALSE)
	, m_bBoneObj(FALSE)
{
}

CSetupDlg::~CSetupDlg()
{

}

void CSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_TEXT_CHECK, m_bTextChunk);
	DDX_Check(pDX, IDC_MESH_CHECK, m_bMesh);
	DDX_Check(pDX, IDC_OBJECT_CHECK, m_bObject);
	DDX_Check(pDX, IDC_OBJPTR_CHECK, m_bObjPtr);
	DDX_Check(pDX, IDC_BONE_CHECK, m_bBone);
	DDX_Check(pDX, IDC_BONEANIM_CHECK, m_bBoneAnim);
	DDX_Check(pDX, IDC_BONEOBJ_CHECK, m_bBoneObj);
}


BEGIN_MESSAGE_MAP(CSetupDlg, CDialog)
END_MESSAGE_MAP()


// CSetupDlg message handlers
