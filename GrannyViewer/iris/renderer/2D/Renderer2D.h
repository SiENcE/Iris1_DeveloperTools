//
// File: Renderer2D.h
// Created by: Alexander Oster - tensor@ultima-iris.de
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

#ifndef _RENDERER2D_H_
#define _RENDERER2D_H_

#ifdef WIN32
#include <windows.h>
#endif

#include "renderer/Texture.h"
#include "renderer/Renderer.h"
#include "renderer/Camera.h"

#include <string>
#include <vector>
#include <list>

#include "SDL/SDL.h"

#include "renderer/Fade.h"

class Renderer2D : public Renderer
{
protected:
	void RenderTerrain (int base_x, int base_y);

public:
   Renderer2D ();
   virtual ~Renderer2D ();

   virtual int Init(void);
   virtual int DeInit(void);
   virtual int RenderScene(void);
   
   virtual void GrabDynamic(int x, int y, cDynamicObject ** r_object, cCharacter ** r_character);
   virtual void GrabMousePosition(int x, int y, int maxz, int cursor3d[3], int * cursor_character);
   
   virtual void AddDynamic (cDynamicObject * object);
   virtual void DelDynamic (cDynamicObject * object);

   virtual void AddCharacter (cCharacter * character);
   virtual void DelCharacter (cCharacter * character);
};

#endif //_CRenderer2D_H_
