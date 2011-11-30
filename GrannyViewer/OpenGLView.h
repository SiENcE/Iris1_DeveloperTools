#pragma once

class COpenGLView : public CView
{
protected: // create from serialization only
	COpenGLView();
	DECLARE_DYNCREATE(COpenGLView)

// Attributes
public:
	HGLRC m_hRC;	//Rendering Context
	CDC* m_pDC;		//Device Context

	COLORREF m_bgColor;

// Operations
public:

	virtual BOOL InitializeOpenGL();	//Initialize OpenGL
	virtual BOOL UninitializeOpenGL();
	BOOL SetupPixelFormat();	//Set up the Pixel Format
	virtual void RenderScene();		//Render the Scene
	virtual void PostRenderScene();	//Post Render Operations

	//For elapsed timing calculations
	DWORD m_StartTime, m_ElapsedTime, m_previousElapsedTime;	

	//Timing Member Functions
	int FramesPerSecond();	//Calculate the FPS
	DWORD ElapsedTimeinMSSinceLastStartup()	{return(m_ElapsedTime - m_StartTime);}
	DWORD ElapsedTimeinMSSinceLastRender()	{return(m_ElapsedTime - m_previousElapsedTime);}


public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);


// Implementation
public:
	virtual ~COpenGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT nIDEvent);
	void InvalidateScene(void);
};



