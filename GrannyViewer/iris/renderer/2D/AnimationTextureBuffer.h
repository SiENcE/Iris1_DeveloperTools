//
// File: AnimationTextureBuffer.h
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

#ifndef _ANIMATIONTEXTUREBUFFER_H_
#define _ANIMATIONTEXTUREBUFFER_H_

#include "SDL/SDL.h"
#include "../Texture.h"
#include <map>
#include "Buffer.h"

class cAnimationTextureBuffer  
{
public:
   cAnimationTextureBuffer ();
   virtual ~cAnimationTextureBuffer ();

   Texture * LoadAnimation(int index, int hue);

protected:
	std::map < Uint32, Texture *> textures;
};

extern	cAnimationTextureBuffer * pAnimationTextureBuffer;

#endif //_ANIMATIONTEXTUREBUFFER_H_
