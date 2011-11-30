//
// File: Node.h
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

#ifndef _NODE_H_
#define _NODE_H_

#ifdef WIN32
#include <windows.h>
#endif

#include "SDL/SDL.h"

#include <map>
#include <vector>
#include <list>

#include "StaticObjects.h"
#include "renderer/3D/SceneMaker.h"

#define NODEQUADFLAG_DOUBLE 0x40000000

class cNode
{
  private:
    Uint32 id;
  public:
        vertex v;
        Uint8 global_light[3]; 
        cNode * equiv_node;
        std::vector <void *> quads;
        float normal[3];
        //int lightidx;
//        float vertex[3];
        float vec_P[3];
        float light_fac;
        bool affected;
  	cNode (Uint32 id);
	~cNode ();
	void AddQuad(void * quad);
	void CalcNormal();
        void CalcLight (float light_vector [3]);
        void ResetLight ();
        void Lighten (float light_x, float light_y, float light_z, float m_radius);
  protected:

};

typedef struct {
	Uint32 id;
	cNode * nodes[4];
	float normal[3];
        Uint8 lights[4][2][3];
	Uint32 texture;
        float texcoords[4][2];
	Uint32 flags;
	struct sStaticObject * object;
} sNodeQuad;

typedef std::map<Uint32, cNode *>  nodelist_t;
typedef std::list<sNodeQuad *>  nodequadlist_t;


class cNodeList
{
private:
   nodelist_t nodes[4][4];
public:
   cNodeList ();
   ~cNodeList ();

   void Clear();
   cNode * AddNode(float x, float y, float z);
   void CalcNormals();
   void CalcLight (float light_vector [3]);
   void ResetLight ();
   
   void Lighten (float light_x, float light_y, float light_z, float m_radius);
   
protected:

};

class cNodeQuadList
{
private:
   nodequadlist_t quads;
   bool _transparent;
public:
   cNodeQuadList ();
   ~cNodeQuadList ();

   void Clear();
   sNodeQuad * AddQuad(void);
   
   void Sort();
   
   void Render(float move_x, float move_y);
   void RenderSelection();
   void RenderTransparent();
   
   bool transparent() { return _transparent; }
   
   void ReduceQuads ();
   void ResetDynamicLight ();
   void ResetStaticLight ();
   void SaveStaticLight ();
   void Lighten (float light_x, float light_y, float light_z, float m_radius);
   
   void RemoveItem (int itemid);
   
protected:

};

#endif //_NODE_H_
