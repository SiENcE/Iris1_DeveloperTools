// OpenGLView.cpp : implementation of the COpenGLView class
//

#include "stdafx.h"
#include "OpenGLView.h"

#include <mmsystem.h>
#include ".\openglview.h"

#include "iris/config.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpenGLView

IMPLEMENT_DYNCREATE(COpenGLView, CView)

BEGIN_MESSAGE_MAP(COpenGLView, CView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenGLView construction/destruction

COpenGLView::COpenGLView() : m_hRC(NULL), m_pDC(NULL), m_bgColor(0)
{
	// TODO: add construction code here
	m_StartTime = m_ElapsedTime = m_previousElapsedTime = 0;
}

COpenGLView::~COpenGLView()
{
}

BOOL COpenGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	//An OpenGL Window must be created with the following flags
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// COpenGLView drawing

void COpenGLView::OnDraw(CDC* pDC)
{
	// Get the system time, in milliseconds.
	m_ElapsedTime = ::timeGetTime(); // get current time

	if ( ElapsedTimeinMSSinceLastRender() < 30 )
		return;

	// Clear out the color & depth buffers
	::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glPushMatrix();
	RenderScene();
	glPopMatrix();

	// Tell OpenGL to flush its pipeline
	::glFinish();

	// Now Swap the buffers
	::SwapBuffers( m_pDC->GetSafeHdc() );

	//Perform Post Display Processing
	PostRenderScene();

	// the very last thing we do is to save
	// the elapsed time, this is used with the
	// next elapsed time to calculate the
	// elapsed time since a render and the frame rate
	m_previousElapsedTime = m_ElapsedTime;
}

/////////////////////////////////////////////////////////////////////////////
// COpenGLView diagnostics

#ifdef _DEBUG
void COpenGLView::AssertValid() const
{
	CView::AssertValid();
}

void COpenGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COpenGLView message handlers

int COpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	//Initialize OpenGL Here
	InitializeOpenGL();

	// Now we initialize the animation code
    m_StartTime = ::timeGetTime(); // get time in ms

    // need a previous time if we start off animated
    m_previousElapsedTime = m_StartTime; 

    //Start Timer
    //SetTimer(1, 10, NULL);
	
	return 0;
}

BOOL COpenGLView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//comment out the original call
	//return CView::OnEraseBkgnd(pDC);
	//Tell Windows not to erase the background
	return TRUE;
}

void COpenGLView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	GLdouble aspect_ratio; // width/height ratio
	
	if ( 0 >= cx || 0 >= cy )
	{
		return;
	}

	// select the full client area
    ::glViewport(0, 0, cx, cy);

	// compute the aspect ratio
	// this will keep all dimension scales equal
	aspect_ratio = (GLdouble)cx/(GLdouble)cy;

	// select the projection matrix and clear it
    ::glMatrixMode(GL_PROJECTION);
    ::glLoadIdentity();

	// select the viewing volume
	::gluPerspective(45.0f, aspect_ratio, .01f, 200.0f);
	//gluOrtho2D(-10.0f, 10.0f, -10.0f, 10.0f); 
	
	// switch back to the modelview matrix and clear it
    ::glMatrixMode(GL_MODELVIEW);
    ::glLoadIdentity();
}

void COpenGLView::OnDestroy() 
{
	CView::OnDestroy();

	UninitializeOpenGL();
	
	
}

BOOL COpenGLView::InitializeOpenGL()
{
	//Get a DC for the Client Area
	m_pDC = new CClientDC(this);

	//Failure to Get DC
	if(m_pDC == NULL)
	{
		MessageBox("Error Obtaining DC");
		return FALSE;
	}

	//Failure to set the pixel format
	if(!SetupPixelFormat())
	{
		return FALSE;
	}

	//Create Rendering Context
	m_hRC = ::wglCreateContext (m_pDC->GetSafeHdc ());

	//Failure to Create Rendering Context
	if(m_hRC == 0)
	{
		MessageBox("Error Creating RC");
		return FALSE;
	}
	
	//Make the RC Current
	if(::wglMakeCurrent (m_pDC->GetSafeHdc (), m_hRC)==FALSE)
	{
		MessageBox("Error making RC Current");
		return FALSE;
	}

	//Specify Black as the clear color
	glClearColor(float(GetRValue(m_bgColor)) / 255, float(GetGValue(m_bgColor)) / 255, 
		float(GetBValue(m_bgColor)) / 255, 1.0f);

	//Specify the back of the buffer as clear depth
	::glClearDepth(1.0f);

	//Enable Depth Testing
	::glEnable(GL_DEPTH_TEST);

	::glEnable(GL_CULL_FACE);
	::glCullFace(GL_BACK);

	::glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	::glEnable(GL_BLEND);

	::glAlphaFunc (GL_GREATER, 0.9);
	::glEnable (GL_ALPHA_TEST);


	::glShadeModel(GL_SMOOTH);

	//::glEnable(GL_LIGHTING);
	//::glEnable(GL_LIGHT0);

	//	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	if (nConfig::useVertexArray)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		//glEnableClientState(GL_INDEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		//glClientActiveTextureARB(GL_TEXTURE1_ARB);
	}


	return TRUE;
}

BOOL COpenGLView::UninitializeOpenGL()
{
	if (nConfig::useVertexArray)
	{
   		glDisableClientState(GL_VERTEX_ARRAY);
		//glDisableClientState(GL_INDEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	}


	//Make the RC non-current
	if(::wglMakeCurrent (0,0) == FALSE)
	{
		MessageBox("Could not make RC non-current");
	}
	
	//Delete the rendering context
	if(::wglDeleteContext (m_hRC)==FALSE)
	{
		MessageBox("Could not delete RC");
	}
	
	//Delete the DC
	if(m_pDC)
	{
		delete m_pDC;
	}
	//Set it to NULL
	m_pDC = NULL;

	return TRUE;
}

//Setup Pixel Format
/////////////////////////////////////////////////////////////////////////////
BOOL COpenGLView::SetupPixelFormat()
{
  static PIXELFORMATDESCRIPTOR pfd = 
	{
        sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
        1,                              // version number
        PFD_DRAW_TO_WINDOW |            // support window
        PFD_SUPPORT_OPENGL |		    // support OpenGL
        PFD_DOUBLEBUFFER,				// double buffered
        PFD_TYPE_RGBA,                  // RGBA type
        24,                             // 24-bit color depth
        0, 0, 0, 0, 0, 0,               // color bits ignored
        0,                              // no alpha buffer
        0,                              // shift bit ignored
        0,                              // no accumulation buffer
        0, 0, 0, 0,                     // accum bits ignored
        16,                             // 16-bit z-buffer
        0,                              // no stencil buffer
        0,                              // no auxiliary buffer
        PFD_MAIN_PLANE,                 // main layer
        0,                              // reserved
        0, 0, 0                         // layer masks ignored
    };
    
    int m_nPixelFormat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);

	if ( m_nPixelFormat == 0 )
	{
       return FALSE;
	}

    if ( !::SetPixelFormat(m_pDC->GetSafeHdc(), m_nPixelFormat, &pfd))
    {
       return FALSE;
	}

    return TRUE;
}

void COpenGLView::RenderScene ()
{
}

void COpenGLView::PostRenderScene()
{
}


int COpenGLView::FramesPerSecond( void )
{
	double eTime = ElapsedTimeinMSSinceLastRender();

	if ( 0 == (int)eTime )
		return 0;

	return (int)(1000/(int)eTime);
}

void COpenGLView::OnTimer(UINT nIDEvent)
{
	Invalidate(FALSE);

	CView::OnTimer(nIDEvent);
}

void COpenGLView::InvalidateScene(void)
{
	m_previousElapsedTime = 0;

	Invalidate(FALSE);
}
