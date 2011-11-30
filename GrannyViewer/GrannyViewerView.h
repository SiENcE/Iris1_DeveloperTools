// GrannyViewerView.h : iCGrannyViewerView 클래스의 인터페이스
//


#pragma once

#include "atltypes.h"
#include "OpenGLView.h"


class CGrannyViewerView : public COpenGLView
{
protected: // serialization에서만 만들어집니다.
	CGrannyViewerView();
	DECLARE_DYNCREATE(CGrannyViewerView)

public:
	CGrannyViewerDoc* GetDocument() const;
	virtual void RenderScene();	//Render the Scene
	virtual ~CGrannyViewerView();
	void ResetTrans(bool bAngel=true);


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CPoint m_ptMouseDown;

	float m_xPos, m_yPos, m_zPos;
	float m_xAngle, m_yAngle;
	float m_fScale;
	
	float m_curTime;

	GLenum m_polyMode;
	bool m_useTexture;
	bool m_drawBone;
	bool m_printBoneName;
	bool m_drawBoundBox;
	bool m_drawAxis;

	UINT m_iFrameSec;

	enum ANIM_STATE { PLAY, PAUSE, STOP };
	ANIM_STATE m_animState;
	UINT m_iLastAnimId;

	DWORD m_preAniTime;

	void DrawAxis(float xPos, float yPos, float zPos, float scale);

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPolygonFill();
	afx_msg void OnPolygonPoint();
	afx_msg void OnPolygonLine();
	afx_msg void OnUpdatePolygonFill(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePolygonPoint(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePolygonLine(CCmdUI *pCmdUI);
	afx_msg void OnRenderTexture();
	afx_msg void OnUpdateRenderTexture(CCmdUI *pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRenderBone();
	afx_msg void OnUpdateRenderBone(CCmdUI *pCmdUI);
	afx_msg void OnRenderScalePlus();
	afx_msg void OnRenderScaleMinus();
	afx_msg void OnRenderBonename();
	afx_msg void OnUpdateRenderBonename(CCmdUI *pCmdUI);
	afx_msg void OnRenderBoundbox();
	afx_msg void OnUpdateRenderBoundbox(CCmdUI *pCmdUI);
	afx_msg void OnRenderAnimationspeed();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnRenderBackgroundcolor();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnModelStartanimation();
	afx_msg void OnModelStopanimation();
	afx_msg void OnModelPauseanimation();
	afx_msg void OnUpdateModelStartanimation(CCmdUI *pCmdUI);
	afx_msg void OnUpdateModelStopanimation(CCmdUI *pCmdUI);
	afx_msg void OnUpdateModelPauseanimation(CCmdUI *pCmdUI);
	afx_msg void OnRenderAxis();
	afx_msg void OnUpdateRenderAxis(CCmdUI *pCmdUI);
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // GrannyViewerView.cpp의 디버그 버전
inline CGrannyViewerDoc* CGrannyViewerView::GetDocument() const
   { return reinterpret_cast<CGrannyViewerDoc*>(m_pDocument); }
#endif

