//
// File: Light3D.h
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

#ifndef _LIGHT3D_H_
#define _LIGHT3D_H_

#ifdef WIN32
#include <windows.h>
#endif

#include "GL/gl.h"
#include "GL/glu.h"
#include "SDL/SDL.h"
#include "renderer/3D/MapBlock3D.h"

#include <iostream>
#include <list>

class cLight3D {
    private:
      float m_x, m_y, m_z;
      float m_radius;
      int m_minx, m_miny, m_maxx, m_maxy;
      void get_bounds ();
    public:
      cLight3D (float x, float y, float z, float radius);
      ~cLight3D ();
      
      float x () { return m_x; }
      float y () { return m_y; }
      float z () { return m_z; }
      float radius () { return m_radius; }
      
      int minx () { return  m_minx; }
      int maxx () { return  m_maxx; }
      int miny () { return  m_miny; }
      int maxy () { return  m_maxy; }
      //void ApplyToWorld ();
      void SetPosition (float x, float y, float z);
};
   

class cLightManager {
    private:
      std::list <cLight3D *> dynamic_lights;
      std::list <cLight3D *> static_lights;
    public:
      cLightManager ();
      virtual ~cLightManager ();
      void Clear ();
      void ClearDynamic ();
      void ClearStatic ();
      //void ApplyToWorld ();
      cLight3D * AddDynamicLight (float x, float y, float z, float radius);
      cLight3D * AddStaticLight (float x, float y, float z, float radius);
      std::list <cLight3D *> & static_light_list () { return static_lights; }
      std::list <cLight3D *> & dynamic_light_list () { return dynamic_lights; }
};

extern cLightManager * pLightManager;
 
#endif //_LIGHT3D_H_
