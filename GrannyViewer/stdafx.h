#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		
#endif

#ifndef WINVER				
#define WINVER 0x0400	
#endif

#ifndef _WIN32_WINNT		
#define _WIN32_WINNT 0x0400		
#endif						

#ifndef _WIN32_WINDOWS		
#define _WIN32_WINDOWS 0x0410 
#endif

#ifndef _WIN32_IE			
#define _WIN32_IE 0x0400	
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	

#define _AFX_ALL_WARNINGS

#include <afxwin.h>        
#include <afxext.h>      
#include <afxdisp.h>      
#include <afxcview.h>

#include <afxdtctl.h>		
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			
#endif // _AFX_NO_AFXCMN_SUPPORT

//OpenGL Headers
#include <gl/gl.h>         //OpenGL Main Library Header
#include <gl/glu.h>       //OpenGL Utility Library Header
#include <gl/glaux.h>    //OpenGL Auxiliary Library Header
#include <gl/glut.h>      //OpenGL GLUT Library Header

#include "iris/SDL_types.h"
//#include "auto_ref.h"
#include "gfx.h"

