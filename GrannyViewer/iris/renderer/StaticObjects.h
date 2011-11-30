//
// File: StaticObjects.h
// Created by:  Alexander Oster - tensor@ultima-iris.de
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

#ifndef _STATICOBJECTS_H_
#define _STATICOBJECTS_H_

#ifdef WIN32
#include <windows.h>
#endif

#include "SDL/SDL.h"

#include <vector>

#include "Fade.h"

struct sStaticObject {
  int x, y, z;
  int blockx, blocky;
  int tileid;
  unsigned int flags;
  unsigned int tiledata_flags;
  int height;
  bool clip;
  Uint8 alpha;
  Uint16 hue;
  float sphere[5];
  int dynamic_id;
  cFader * fader;
};


typedef std::vector<struct sStaticObject *>  objectlist_t;

class StaticObjectList
{
private:
 objectlist_t objectlist;
public:
    StaticObjectList ()
    { }
    
   ~StaticObjectList ()
   {
   	Clear();
   }

   void Clear (void) {
   	for (unsigned int i = 0; i < objectlist.size(); i++)
		delete objectlist[i];
	objectlist.clear();
   	
   }
   
   struct sStaticObject * Add (void) {
   	struct sStaticObject * result = new sStaticObject;
	memset(result, 0, sizeof (struct sStaticObject));
	objectlist.push_back(result);
	return result;
   }
   
   struct sStaticObject * Get (unsigned int index)
   {
   	if (index < objectlist.size())
		return objectlist[index];
	return NULL;
   }
   
   objectlist_t * GetList(void) { return &objectlist; }
   
   int GetCount(void) { return objectlist.size(); }

protected:

};



#endif //_STATICOBJECTS_H_
