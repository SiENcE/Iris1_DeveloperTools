//
// File: MapBlock2D.h
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

#ifndef _MAPBLOCK2D_H_
#define _MAPBLOCK2D_H_

#ifdef WIN32
#include <windows.h>
#endif

#include "GL/gl.h"
#include "GL/glu.h"

#include <iostream>
#include <list>
#include "renderer/StaticObjects.h"
#include "renderer/DynamicObjects.h"
#include "renderer/Characters.h"
#include "renderer/MapBlock.h"
#include "renderer/Node.h"

#define TILECLASS_UNKNOWN   0
#define TILECLASS_GROUND    1
#define TILECLASS_STATIC    2
#define TILECLASS_DYNAMIC   3
#define TILECLASS_CHARACTER 4

class cWorldEntity 
{
protected:
   int m_x, m_y, m_z;
   int m_height;    
   int m_tileclass;
   int m_flags;
   int m_id;
public:
   cWorldEntity ();
   virtual void Render (int base_x, int base_y) = 0;
   virtual bool CheckClick (int x, int y, int base_x, int base_y) = 0;
   
   int x () { return m_x; }
   int y () { return m_y; }
   int z () { return m_z; }
   int id () { return m_id; }
   int height () { return m_height; }
   int tileclass () { return m_tileclass; }
   int flags() { return m_flags; }
   
};

class cWorldGround : public cWorldEntity
{
protected:
   int m_tileid;
   int draw_pos[4][2];
public:
   // z1: z at (x/y)
   // z2: z at (x + 1/y)
   // z3: z at (x/y + 1)
   // z4: z at (x + 1/y + 1)
   cWorldGround (int x, int y, int z1, int z2, int z3, int z4, int tileid);
   virtual void Render (int base_x, int base_y);
   virtual bool CheckClick (int x, int y, int base_x, int base_y);
};

class cWorldStatic : public cWorldEntity
{
protected:
   sStaticObject * object;
   int draw_pos[4][2];
public:
   cWorldStatic (sStaticObject * object);
   virtual void Render (int base_x, int base_y);
   virtual bool CheckClick (int x, int y, int base_x, int base_y);
};

class cWorldDynamic : public cWorldEntity
{
protected:
   cDynamicObject * object;
   int draw_pos[4][2];
public:
   cWorldDynamic (cDynamicObject * object);
   virtual void Render (int base_x, int base_y);
   virtual bool CheckClick (int x, int y, int base_x, int base_y);
   cDynamicObject * dynamic_object() { return object; }
};

class cWorldCharacter : public cWorldEntity
{
protected:
   cCharacter * m_character;
   int draw_x, draw_y;
   Uint32 last_time;
   int frame_index;
   
   void RenderAnimation(int anim_body, int base_x, int base_y);
   
public:
   cWorldCharacter (cCharacter * character);
   virtual void Render (int base_x, int base_y);
   virtual bool CheckClick (int x, int y, int base_x, int base_y);
   cCharacter * character() { return m_character; }
};

class cWorldEntityList
{
private:
   std::list <cWorldEntity *> entities;
public:
   cWorldEntityList();
   virtual ~cWorldEntityList();
   void AddStatic(sStaticObject * object);
   void AddDynamic(cDynamicObject * object);
   void AddCharacter (cCharacter * character);
   void DelDynamic(Uint32 id);
   void AddGround(int x, int y, int z1, int z2, int z3, int z4, int tileid);
   void Render (int base_x, int base_y);
   void Sort();
   void GrabMousePosition(int x, int y, int max_z, int base_x, int base_y, int & result_x, int & result_y, int & result_z, int blockx, int blocky);
   void GrabDynamic(int x, int y, int max_z, int base_x, int base_y, cDynamicObject ** r_object, cCharacter ** r_character);
};


class cMapblock2D : public cMapblock
{
protected:
 cWorldEntityList entity_list[8][8];
 
public:
    cMapblock2D ();
   ~cMapblock2D ();
   virtual bool Generate(int blockx, int blocky);
   void Render(int x, int y);
   void GrabMousePosition(int x, int y, int max_z, int base_x, int base_y, int & result_x, int & result_y, int & result_z);
   void GrabDynamic(int x, int y, int max_z, int base_x, int base_y, cDynamicObject ** r_object, cCharacter ** r_character);
   void AddDynamic (cDynamicObject * object);
   void AddCharacter (cCharacter * character);
   void DelDynamic (cDynamicObject * object);
   void DelCharacter (cCharacter * character);
};


#endif //_MAPBLOCK2D_H_
