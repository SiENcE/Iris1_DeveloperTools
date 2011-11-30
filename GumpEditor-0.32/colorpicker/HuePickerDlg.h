#pragma once

#include "HueWnd.h"



class CHuePickerDlg : public CDialog
{
	DECLARE_DYNAMIC(CHuePickerDlg)

public:
	CHuePickerDlg(DWORD hueId=1,CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CHuePickerDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_HUE_DIALOG };
	CHueWnd m_wndHue;
	COLORREF m_color;
	DWORD m_hueId;

	COLORREF GetColor() { return m_color; }
	DWORD GetHueId() { return m_hueId; }
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strHueID;
	CString m_strRGB;
	BOOL m_bRGBHex;
	void UpdateRGB();
	void SelectHueId(DWORD hueId);
	void SelectColor(CString strColor);

	afx_msg LRESULT OnHueChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedCheck1();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL m_bHueIDHex;
	afx_msg void OnBnClickedCheck2();
};
