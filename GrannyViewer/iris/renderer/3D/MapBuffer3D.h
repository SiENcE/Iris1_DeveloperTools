//
// File: MapBuffer.h
// Created by: Darkstorm
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

#ifndef _MAPBUFFER3D_H_
#define _MAPBUFFER3D_H_

#ifdef WIN32
#include <windows.h>
#endif

#include "include.h"
#include "renderer/MapBlock.h"
#include "renderer/3D/MapBlock3D.h"
#include "renderer/MapBuffer.h"
#include "SDL/SDL.h"
#include <map>


class cMapbuffer3D : public cMapbuffer
{
public:
	void GetEnvironment(int x, int y, cMapblock3D * environment[8]);
	virtual cMapblock * CreateBlock (int x, int y);
};

#endif //_MAPBUFFER3D_H_

