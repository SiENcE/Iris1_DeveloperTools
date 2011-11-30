//
// File: SDLScreen.h
// Created by: Gustav Nylander - blobba@ultima-iris.de
//
/*****
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *****/

#ifndef _SDLSCREEN_H_
#define _SDLSCREEN_H_

#ifdef WIN32
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include <cstring>
#include "GL/gl.h"
#include "GL/glu.h"
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include <stdlib.h>
#include <string>
#include <map>

using namespace std;

class SDLScreen
{
private:
  int Init (int width, int height, int bpp);
  int InitGL (GLvoid);
  void DisplayFps ();
  const SDL_VideoInfo *videoInfo;
  GLfloat ratio;
  std::map <Uint32, TTF_Font *> fonts;
  std::map <Uint32, Uint16> default_hues;
  Uint8 act_alpha;
  
  float light_factor;

public:
    SDLScreen ();
   ~SDLScreen ();

  int ResizeWindow (int width, int height);
  void ToggleFullScreen ();
  void ClearScreen ();
  void ClearZBuffer ();
  int DrawGL (GLvoid);
  int ScreenSave ();
  SDL_Surface *screen;
  int videoFlags;
  void SetPerspective(void);
  float GetRatio(void);
  
  void SetAlpha(Uint8 alpha, bool force = false);
  void SetHue(Uint16 hue = 0);
  
  void ClearFonts ();
  void RegisterFont (Uint32 id, std::string filename, Uint32 size, Uint16 defaulthue = 0);
  void UnregisterFont (Uint32 id);
  TTF_Font * GetFont(Uint32 id);
  
  Uint16 GetDefaultHue(Uint32 id);
  
  void SetLight(float factor);
  
protected:

};


#endif //_SDLSCREEN_H_
