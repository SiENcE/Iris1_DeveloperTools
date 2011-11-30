// NumSpinCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GumpEditor.h"
#include "NumSpinCtrl.h"
#include ".\numspinctrl.h"


// CNumSpinCtrl

IMPLEMENT_DYNAMIC(CNumSpinCtrl, CSpinButtonCtrl)
CNumSpinCtrl::CNumSpinCtrl()
{
}

CNumSpinCtrl::~CNumSpinCtrl()
{
}


BEGIN_MESSAGE_MAP(CNumSpinCtrl, CSpinButtonCtrl)
	ON_NOTIFY_REFLECT(UDN_DELTAPOS, OnDeltapos)
END_MESSAGE_MAP()



// CNumSpinCtrl 메시지 처리기입니다.


void CNumSpinCtrl::OnDeltapos(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pUD = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	*pResult = 0;
	
	int val = GetPos() + pUD->iDelta;
	if (pUD->iDelta < 0) { // spin down
	} else {
	}
	
	SetValueForBuddy(val);
}

void CNumSpinCtrl::SetValueForBuddy(int val)
{
	CWnd* pEdit = GetBuddy();
	if (!pEdit) return;
	
	int lower=0,upper=0;
	GetRange32(lower,upper);
	val=max(lower,min(upper,val));

	CString str;
	pEdit->GetWindowText(str);
	if (str.Left(2).CompareNoCase("0x")==0)
		str.Format("0x%X", val);
	else
		str.Format("%d", val);
	
	pEdit->SetWindowText (str);
}

int CNumSpinCtrl::GetPos(void)
{
	CWnd* pEdit = GetBuddy();
	if (!pEdit) return 0;
	
	CString str;
	pEdit->GetWindowText(str);
	
	return GfxAtoX(str);
}
