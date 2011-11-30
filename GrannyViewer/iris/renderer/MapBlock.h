//
// File: MapBlock.h
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

#ifndef _MAPBLOCK_H_
#define _MAPBLOCK_H_

#ifdef WIN32
#include <windows.h>
#endif

#include "GL/gl.h"
#include "GL/glu.h"

#include <iostream>
#include "renderer/StaticObjects.h"
#include "renderer/DynamicObjects.h"

class cMapblock
{
protected:
 StaticObjectList objects;
 bool _in_use;
 bool _generated;
 
 signed char groundmap[8][8];
 unsigned short groundids[8][8];

public:
    cMapblock ();
   virtual ~cMapblock ();
   
   int UpdateFader(int min_z, Uint8 alpha, cFader * fader, bool below);
   void ResetFader(cFader * fader);
   void SetAlpha(int min_z, Uint8 alpha, bool below);
   
   int GetRoofHeight(int x, int y, int z);
   int GetGroundZ(Uint32 x, Uint32 y);
   int GetWalkZ(Uint32 dstx, Uint32 dsty, int srcz);
   
   int blockx, blocky;
   
   bool in_use () { return _in_use; }
   void set_in_use (bool value = false) { _in_use = value; }
   
   bool generated () { return _generated; }
   virtual void ResetLight () {}

};


#endif //_MAPBLOCK_H_


