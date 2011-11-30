//
// File: Renderer.h
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

#ifndef _RENDERER3D_H_
#define _RENDERER3D_H_

#ifdef WIN32
#include <windows.h>
#endif

#include "renderer/Texture.h"
#include "renderer/Camera.h"

#include <string>
#include <vector>
#include <list>

#include "SDL/SDL.h"

#include "renderer/Fade.h"
#include "renderer/Renderer.h"

class Renderer3D : public Renderer
{
protected:
	void RenderTerrain (bool do_culling);
	void RenderWater (bool do_culling);
	void RenderTransparent ();
	void RenderDynamics (bool do_culling);
	void RenderDynamicsTransparent ();
	void RenderCharacters (bool do_culling);
	void RenderDragModel();
        void GenerateLight ();
	
        Texture * tex_water;
	
public:
   Renderer3D ();
   virtual ~Renderer3D ();

   virtual int Init(void);
   virtual int DeInit(void);
   virtual int RenderScene(void);
   virtual void GrabDynamic(int x, int y, cDynamicObject ** r_object, cCharacter ** r_character);
   virtual void GrabMousePosition(int x, int y, int maxz, int cursor3d[3], int * cursor_character);
   virtual void AddDynamic (cDynamicObject * object);
   virtual void DelDynamic (cDynamicObject * object);
};

#endif //_Renderer3D_H_
