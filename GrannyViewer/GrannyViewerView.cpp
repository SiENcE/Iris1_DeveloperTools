// GrannyViewerView.cpp : CGrannyViewerView 클래스의 구현
//

#include "stdafx.h"
#include "GrannyViewer.h"
#include "GrannyViewerDoc.h"
#include "GrannyViewerView.h"
#include ".\grannyviewerview.h"
#include "granny/GrannyLoader.h"
#include "granny/GrannyFile.h"
#include "loaders/StitchinLoader.h"
#include "loaders/ModelInfoLoader.h"
#include "iris/Config.h"
#include "AnimSetup.h"

#include <mmsystem.h>

#ifdef min
#undef min
#undef max
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGrannyViewerView

extern int polycount;
void printw (GLvoid *font, float x, float y, float z, char* format, ...);
void printw (float x, float y, float z, char* format, ...);

IMPLEMENT_DYNCREATE(CGrannyViewerView, COpenGLView)

BEGIN_MESSAGE_MAP(CGrannyViewerView, COpenGLView)
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_CREATE()
	ON_COMMAND(ID_POLYGON_FILL, OnPolygonFill)
	ON_COMMAND(ID_POLYGON_POINT, OnPolygonPoint)
	ON_COMMAND(ID_POLYGON_LINE, OnPolygonLine)
	ON_UPDATE_COMMAND_UI(ID_POLYGON_FILL, OnUpdatePolygonFill)
	ON_UPDATE_COMMAND_UI(ID_POLYGON_POINT, OnUpdatePolygonPoint)
	ON_UPDATE_COMMAND_UI(ID_POLYGON_LINE, OnUpdatePolygonLine)
	ON_COMMAND(ID_RENDER_TEXTURE, OnRenderTexture)
	ON_UPDATE_COMMAND_UI(ID_RENDER_TEXTURE, OnUpdateRenderTexture)
	ON_WM_DESTROY()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_RENDER_BONE, OnRenderBone)
	ON_UPDATE_COMMAND_UI(ID_RENDER_BONE, OnUpdateRenderBone)
	ON_COMMAND(ID_RENDER_SCALE_PLUS, OnRenderScalePlus)
	ON_COMMAND(ID_RENDER_SCALE_MINUS, OnRenderScaleMinus)
	ON_COMMAND(ID_RENDER_BONENAME, &CGrannyViewerView::OnRenderBonename)
	ON_UPDATE_COMMAND_UI(ID_RENDER_BONENAME, &CGrannyViewerView::OnUpdateRenderBonename)
	ON_COMMAND(ID_RENDER_BOUNDBOX, &CGrannyViewerView::OnRenderBoundbox)
	ON_UPDATE_COMMAND_UI(ID_RENDER_BOUNDBOX, &CGrannyViewerView::OnUpdateRenderBoundbox)
	ON_COMMAND(ID_RENDER_ANIMATIONSPEED, &CGrannyViewerView::OnRenderAnimationspeed)
	ON_WM_TIMER()
	ON_COMMAND(ID_RENDER_BACKGROUNDCOLOR, &CGrannyViewerView::OnRenderBackgroundcolor)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MODEL_STARTANIMATION, OnModelStartanimation)
	ON_COMMAND(ID_MODEL_STOPANIMATION, OnModelStopanimation)
	ON_COMMAND(ID_MODEL_PAUSEANIMATION, OnModelPauseanimation)
	ON_UPDATE_COMMAND_UI(ID_MODEL_STARTANIMATION, OnUpdateModelStartanimation)
	ON_UPDATE_COMMAND_UI(ID_MODEL_STOPANIMATION, OnUpdateModelStopanimation)
	ON_UPDATE_COMMAND_UI(ID_MODEL_PAUSEANIMATION, OnUpdateModelPauseanimation)
	ON_COMMAND(ID_RENDER_AXIS, OnRenderAxis)
	ON_UPDATE_COMMAND_UI(ID_RENDER_AXIS, OnUpdateRenderAxis)
END_MESSAGE_MAP()

CGrannyViewerView::CGrannyViewerView()
: m_ptMouseDown(0), m_polyMode(GL_FILL), m_useTexture(true), m_drawBone(false), m_printBoneName(false), m_drawAxis(false),
  m_drawBoundBox(false),
  m_fScale(1.0f),
  m_iFrameSec(33),
  m_animState(PLAY),
  m_iLastAnimId(INVALID_GRN_ID),
  m_preAniTime(0)
{
	ResetTrans();

	m_bgColor = RGB(0, 128, 192);

	CWinApp *pApp = AfxGetApp();

	m_polyMode      = pApp->GetProfileInt(_T("render"), _T("polymode"),      m_polyMode);
	m_useTexture    = pApp->GetProfileInt(_T("render"), _T("usetexture"),    m_useTexture);
	m_drawBone      = pApp->GetProfileInt(_T("render"), _T("drawbone"),      m_drawBone);
	m_printBoneName = pApp->GetProfileInt(_T("render"), _T("printbonename"), m_printBoneName);
	m_drawBoundBox  = pApp->GetProfileInt(_T("render"), _T("boundbox"),      m_drawBoundBox);
	m_iFrameSec     = pApp->GetProfileInt(_T("render"), _T("framesec"),      m_iFrameSec);
	m_bgColor		= pApp->GetProfileInt(_T("render"), _T("bgcolor"),       m_bgColor);
	m_drawAxis      = pApp->GetProfileInt(_T("render"), _T("drawaxis"),      m_drawAxis);
	
	nConfig::drawBone = m_drawBone;
	nConfig::printBoneName = m_printBoneName;
	nConfig::drawBoundBox = m_drawBoundBox;
}

CGrannyViewerView::~CGrannyViewerView()
{
	CWinApp *pApp = AfxGetApp();

	pApp->WriteProfileInt(_T("render"), _T("polymode"),      m_polyMode);
	pApp->WriteProfileInt(_T("render"), _T("usetexture"),    m_useTexture);
	pApp->WriteProfileInt(_T("render"), _T("drawbone"),      m_drawBone);
	pApp->WriteProfileInt(_T("render"), _T("printbonename"), m_printBoneName);
	pApp->WriteProfileInt(_T("render"), _T("boundbox"),      m_drawBoundBox);
	pApp->WriteProfileInt(_T("render"), _T("framesec"),      m_iFrameSec);
	pApp->WriteProfileInt(_T("render"), _T("bgcolor"),       m_bgColor);
	pApp->WriteProfileInt(_T("render"), _T("drawaxis"),      m_drawAxis);
}

void CGrannyViewerView::ResetTrans(bool bAngle)
{
	m_xPos =  0.0f;
	m_yPos = -1.7f;
	m_zPos = -5.5f;
	if (bAngle)
	{
		m_xAngle = -90.0f;
		m_yAngle = 0.0f;
	}
	m_curTime = 0.0f;
	m_fScale  = 1.0f;
}

void CGrannyViewerView::OnInitialUpdate()
{
	COpenGLView::OnInitialUpdate();

	ResetTrans();
}

int CGrannyViewerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COpenGLView::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetTimer(1,m_iFrameSec,NULL);

	return 0;
}

void CGrannyViewerView::OnDestroy()
{
	COpenGLView::OnDestroy();

	CWinApp *pApp = AfxGetApp();
	pApp->WriteProfileInt("view", "polymode", m_polyMode);
	pApp->WriteProfileInt("view", "usetexture", m_useTexture);
	pApp->WriteProfileInt("view", "drawbone", m_drawBone);
}


// CGrannyViewerView 진단

#ifdef _DEBUG
void CGrannyViewerView::AssertValid() const
{
	COpenGLView::AssertValid();
}

void CGrannyViewerView::Dump(CDumpContext& dc) const
{
	COpenGLView::Dump(dc);
}

CGrannyViewerDoc* CGrannyViewerView::GetDocument() const 
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGrannyViewerDoc)));
	return (CGrannyViewerDoc*)m_pDocument;
}
#endif //_DEBUG


// CGrannyViewerView 메시지 처리기

void CGrannyViewerView::DrawAxis(float xPos, float yPos, float zPos, float scale)
{
	char *s[3] = { "x", "y", "z" };
	Point pt[3] = { Point(1,0,0), Point(0,1,0), Point(0,0,1) };
	
	glBegin(GL_LINES);
	for (int i = 0; i < 3; i++)
	{
		float *p = pt[i].points;
		glColor3f(p[0],p[1],p[2]); 
		glVertex3f(0,0,0);
		glVertex3fv(p);
	}
	glEnd();

	for (int i = 0; i < 3; i++)
	{
		float *p = pt[i].points;
		glColor3f(p[0],p[1],p[2]); 
		printw(GLUT_BITMAP_HELVETICA_12, p[0],p[1],p[2], s[i]);
	}
	
	glColor3f(0.7f,0.7f,0.7f);
	printw(GLUT_BITMAP_HELVETICA_10, 0,0,0, "(%.2f,%.2f,%.2f,%.6f)",xPos,yPos,zPos,scale);
	glColor3f(1.0f,1.0f,1.0f);
}

void CGrannyViewerView::RenderScene()
{
	CGrannyViewerDoc* pDoc = GetDocument(); ASSERT_VALID(pDoc);
	if (!pDoc) return;

	cGrannyLoader *pLoader = pDoc->GetGrannyLoader();
	cStitchinLoader *pStitchinLoader = pDoc->GetStitchinLoader();
	cModelInfoLoader *pModelInfoLoader = pDoc->GetModelInfoLoader();
	if (!pLoader || !pStitchinLoader || !pModelInfoLoader) return;

	glClearColor(
		float(GetRValue(m_bgColor)) / 255, 
		float(GetGValue(m_bgColor)) / 255, 
		float(GetBValue(m_bgColor)) / 255, 1.0f);

	Point s(0,0,0), d(0,0,0);
	if (m_ptMouseDown != CPoint(0,0)) d.points[0]=0.1;


	Uint32 mid = pDoc->GetCurModelId();
	Uint32 aid = pDoc->GetCurAnimId();
	cGrannyModel *pModel = pLoader->GetModel(mid);

	std::vector<UINT> models = pDoc->GetReisterModels();

	if (pModel)
	{
		if (std::find(models.begin(), models.end(), mid) == models.end())
		{
			if (pModel->GetHand() == HAND_OWNER)
				models.insert(models.begin(), mid);
			else
				models.push_back(mid);
		}
	}
	
	if (models.size() == 0) return;

	if (mid == INVALID_GRN_ID) 
	{
		mid = models[0];
		pModel = pLoader->GetModel(mid);
	}
	
	if (!pModel) return;

	cGrannyFile *pFile = pModel->GetModel();
	Point pt;
	float depth = 0.0f;
	if (pFile) 
		pt = pFile->getModelOrg(NULL,NULL,&depth);

#if 0
	if (0.9f < m_fScale && m_fScale < 1.1f) 
	{
		if (depth > 1000.0f)     m_fScale = 0.0001f;
		else if (depth > 100.0f) m_fScale = 0.01f;
		else if (depth > 10.0f)  m_fScale = 0.1f;
	}
#endif

	float fScale = m_fScale;

	cModelInfoEntry *entry = pModelInfoLoader->GetModelEntry(mid);
	if (entry) 
	{
		fScale *= entry->scalex();
	}
	
	float xPos = m_xPos, yPos = m_yPos, zPos = m_zPos;
	
	glLoadIdentity();
	glTranslatef(xPos, yPos, zPos);
	glScalef(fScale, fScale, fScale);
    glRotatef(m_xAngle, 1.0f, 0.0f, 0.0f); 
	glRotatef(m_yAngle, 0.0f, 0.0f, 1.0f);
	//glTranslatef(pt.points[0], pt.points[1], pt.points[2]);

	if (m_drawAxis)
		DrawAxis(xPos, yPos, zPos, fScale);
	
	glPolygonMode(GL_FRONT_AND_BACK, m_polyMode);

	if (m_useTexture && m_polyMode==GL_FILL) 
		glEnable(GL_TEXTURE_2D);

	GrnMatrix left, right;
	std::vector<int> bodyparts(14,0);

	if (nConfig::aos)
	{
		for (Uint32 i=0; i < models.size(); i++)
		{
			Uint32 id = models[i];

			if (ISEQUIP(id)) // equip item
			{
				cModelEntry *entry = pStitchinLoader->GetModel(GETID(id));
				if (entry) 
				{
					int *c = entry->GetCovers();
					for (int i = 0; i < 13; i++)
						bodyparts[i+1] += c[i];
				}
			}
		}

		bodyparts[4]=0; // render hands
	}


	float lenAnim = 0.0f;
	for (Uint32 i=0; i < models.size(); i++)
	{
		Uint32 id = models[i];
		lenAnim = std::max(lenAnim,  pLoader->GetAnimLength(id, aid));
		
		if (pLoader->IsShow(id))
		{
			if (nConfig::aos && (id == 400 || id == 401))
				pLoader->Render(id, aid, m_curTime, &left, &right, bodyparts);
			else
				pLoader->Render(id, aid, m_curTime, &left, &right);
		}
	}

	if (m_useTexture && m_polyMode==GL_FILL) 
		glDisable(GL_TEXTURE_2D);
	
	DWORD time = ::timeGetTime();;
	if (m_animState == PLAY && pDoc->GetCurAnimId() != INVALID_GRN_ID && time - m_preAniTime >= m_iFrameSec) 
	{
		m_preAniTime = time;
		m_curTime += 0.03f;
	}

	//if (m_curTime > pDoc->GetCurAnimLength()) 
	if (m_curTime > lenAnim)
		m_curTime =  0.0f;
}


void CGrannyViewerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_UP:         
		m_yPos = m_yPos + 0.1f;
		break;
	case VK_DOWN:    
		m_yPos = m_yPos - 0.1f;
		break;
	case VK_LEFT:       
		m_xPos = m_xPos - 0.1f;
		break;
	case VK_RIGHT:     
		m_xPos = m_xPos + 0.1f;
		break;
	default:
		COpenGLView::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	} 

	InvalidateScene();

	COpenGLView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CGrannyViewerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	m_ptMouseDown = point;

	COpenGLView::OnLButtonDown(nFlags, point);
}

void CGrannyViewerView::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	m_ptMouseDown = CPoint(0,0);
	InvalidateScene();

	COpenGLView::OnLButtonUp(nFlags, point);
}

void CGrannyViewerView::OnMButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	m_ptMouseDown = point;

	if (GetCapture()==this)
	{
		InvalidateScene();
	}

	COpenGLView::OnMButtonDown(nFlags, point);
}

void CGrannyViewerView::OnMButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	point = CPoint(0,0);

	COpenGLView::OnMButtonUp(nFlags, point);
}

void CGrannyViewerView::OnRButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	m_ptMouseDown = point;

	COpenGLView::OnRButtonDown(nFlags, point);
}

void CGrannyViewerView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	point = CPoint(0,0);

	COpenGLView::OnRButtonUp(nFlags, point);
}


void CGrannyViewerView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (GetCapture() != this) return;
	
	if (GetAsyncKeyState(VK_LBUTTON)) 
	{
		//Increment the object rotation angles
		m_xAngle += (point.y-m_ptMouseDown.y)/3.6;
		m_yAngle += (point.x-m_ptMouseDown.x)/3.6;
	} 
	else if (GetAsyncKeyState(VK_MBUTTON)) 
	{
		m_xPos += (point.x - m_ptMouseDown.x)*0.01;
		m_yPos -= (point.y - m_ptMouseDown.y)*0.01;
	} 
	else if (GetAsyncKeyState(VK_RBUTTON)) 
	{
		m_zPos += (point.x - m_ptMouseDown.x)*0.03; 
	}
    //Redraw the view
    Invalidate(FALSE);
    //Set the mouse point
    m_ptMouseDown=point;
}

BOOL CGrannyViewerView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (zDelta > 0) m_zPos += 0.3;
	else if (zDelta < 0) m_zPos -= 0.3;
	InvalidateScene();

	return COpenGLView::OnMouseWheel(nFlags, zDelta, pt);
}

void CGrannyViewerView::OnPolygonFill()
{
	m_polyMode = GL_FILL;
	InvalidateScene();
}

void CGrannyViewerView::OnPolygonPoint()
{
	m_polyMode = GL_POINT;
	InvalidateScene();
}

void CGrannyViewerView::OnPolygonLine()
{
	m_polyMode = GL_LINE;
	InvalidateScene();
}

void CGrannyViewerView::OnUpdatePolygonFill(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_polyMode==GL_FILL);
}

void CGrannyViewerView::OnUpdatePolygonPoint(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_polyMode==GL_POINT);
}

void CGrannyViewerView::OnUpdatePolygonLine(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_polyMode==GL_LINE);
}

void CGrannyViewerView::OnRenderTexture()
{
	m_useTexture = !m_useTexture;
	InvalidateScene();
}

void CGrannyViewerView::OnUpdateRenderTexture(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_useTexture);
}

void CGrannyViewerView::OnRenderBone()
{
	m_drawBone = !m_drawBone;
	nConfig::drawBone = m_drawBone;
	InvalidateScene();
}

void CGrannyViewerView::OnUpdateRenderBone(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_drawBone);
}

void CGrannyViewerView::OnRenderScalePlus()
{
	m_fScale *= 10.0f;
	InvalidateScene();
}

void CGrannyViewerView::OnRenderScaleMinus()
{
	m_fScale *= 0.1f;
	InvalidateScene();
}

void CGrannyViewerView::OnRenderBonename()
{
	m_printBoneName = !m_printBoneName;
	nConfig::printBoneName = m_printBoneName;
	InvalidateScene();
}

void CGrannyViewerView::OnUpdateRenderBonename(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_printBoneName);
}

void CGrannyViewerView::OnRenderBoundbox()
{
	m_drawBoundBox = !m_drawBoundBox;
	nConfig::drawBoundBox = m_drawBoundBox;
	InvalidateScene();
}

void CGrannyViewerView::OnUpdateRenderBoundbox(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_drawBoundBox);
}

void CGrannyViewerView::OnRenderAnimationspeed()
{
	CAnimSetupDlg dlg;
	dlg.SetTime(m_iFrameSec);

	if (dlg.DoModal() != IDOK) return;

	m_iFrameSec = dlg.GetTime();
	KillTimer(1);
	SetTimer(1, m_iFrameSec,NULL);
}

void CGrannyViewerView::OnModelStartanimation()
{
	CGrannyViewerDoc *pDoc = GetDocument(); ASSERT(pDoc);
	m_animState = PLAY;
	if (pDoc->GetCurAnimId() == INVALID_GRN_ID)
		pDoc->SetCurAnimId(m_iLastAnimId);
}

void CGrannyViewerView::OnModelPauseanimation()
{
	m_animState = PAUSE;
}

void CGrannyViewerView::OnModelStopanimation()
{
	CGrannyViewerDoc *pDoc = GetDocument(); ASSERT(pDoc);
	m_animState = STOP;
	m_iLastAnimId = pDoc->GetCurAnimId();
	pDoc->SetCurAnimId(INVALID_GRN_ID);
}


void CGrannyViewerView::OnUpdateModelStartanimation(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_animState == PLAY);
}

void CGrannyViewerView::OnUpdateModelPauseanimation(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_animState == PAUSE);
}

void CGrannyViewerView::OnUpdateModelStopanimation(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_animState == STOP);
}

void CGrannyViewerView::OnTimer(UINT_PTR nIDEvent)
{
	if (GetDocument()->GetCurAnimId() != INVALID_GRN_ID && m_animState == PLAY) 
		InvalidateRect(NULL,FALSE);
}

void CGrannyViewerView::OnRenderBackgroundcolor()
{
	CColorDialog dlg(m_bgColor);
	if (dlg.DoModal() == IDOK)
	{
		m_bgColor = dlg.GetColor();
		InvalidateScene();
	}
}

void CGrannyViewerView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	if (!menu.CreatePopupMenu()) return;

	menu.AppendMenu(MF_STRING | (m_polyMode == GL_POINT ? MF_CHECKED : 0), ID_POLYGON_POINT,    _T( "&Point" ));
	menu.AppendMenu(MF_STRING | (m_polyMode == GL_LINE  ? MF_CHECKED : 0), ID_POLYGON_LINE,     _T( "&Line" ));
	menu.AppendMenu(MF_STRING | (m_polyMode == GL_FILL  ? MF_CHECKED : 0), ID_POLYGON_FILL,     _T( "&Fill" ));
	menu.AppendMenu(MF_STRING | (m_useTexture  ? MF_CHECKED : 0),          ID_RENDER_TEXTURE,   _T( "&Texture" ));
	menu.AppendMenu(MF_SEPARATOR);
	menu.AppendMenu(MF_STRING | (m_drawBone				? MF_CHECKED : 0), ID_RENDER_BONE,      _T( "&Bone" ));
	menu.AppendMenu(MF_STRING | (m_printBoneName		? MF_CHECKED : 0), ID_RENDER_BONENAME,  _T( "Bone &Name" ));
	menu.AppendMenu(MF_STRING | (m_drawBoundBox			? MF_CHECKED : 0), ID_RENDER_BOUNDBOX,  _T( "Bound Bo&x" ));
	menu.AppendMenu(MF_SEPARATOR);
	menu.AppendMenu(MF_STRING | (m_drawAxis             ? MF_CHECKED : 0), ID_RENDER_AXIS,      _T( "&Axis" ));
	menu.AppendMenu(MF_SEPARATOR);
	menu.AppendMenu(MF_STRING, ID_MODEL_CLEARMODEL,      _T("Clear"));
	menu.AppendMenu(MF_STRING, ID_MODEL_CLEARREGISTERED, _T("Clear Registered"));
	menu.AppendMenu(MF_STRING, ID_MODEL_CLEARALL,        _T("Clear All"));

	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,point.x, point.y, this);
}

void CGrannyViewerView::OnRenderAxis()
{
	m_drawAxis = !m_drawAxis;
	InvalidateScene();
}

void CGrannyViewerView::OnUpdateRenderAxis(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_drawAxis);
}

