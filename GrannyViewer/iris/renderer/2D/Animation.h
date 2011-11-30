//
// File: Animation.h
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

#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#ifdef WIN32
#include <windows.h>
#endif

#include "SDL/SDL.h"

#include <map>
#include <vector>
#include <list>

#include "../Texture.h"

class cFrame
{
  protected:
     int m_x, m_y;
     int m_width, m_height;
     int m_centerx, m_centery;
     int m_pitch;
     float m_texcoords[4][2];
     float m_texcoords_mirrored[4][2];
     Uint32 * m_data;
  public:
     cFrame (int x, int y, int width, int height, int pitch_x, int pitch_y, Uint32 * data)
     {
        m_x = x;
        m_y = y;
        m_width = width;
        m_height = height;
        m_pitch = pitch_x;
        m_data = data;
        m_centerx = 0;
        m_centery = 0;
        
        if (pitch_x && pitch_y) {
           m_texcoords[3][0] = (float) (x + width) / (float) pitch_x;
           m_texcoords[3][1] = (float) y / (float) pitch_y;
           
           m_texcoords[1][0] = (float) x / (float) pitch_x;
           m_texcoords[1][1] = (float) (y + height) / (float) pitch_y;
           
           m_texcoords[2][0] = m_texcoords[3][0];
           m_texcoords[2][1] = m_texcoords[1][1];
           
           m_texcoords[0][0] = m_texcoords[1][0];
           m_texcoords[0][1] = m_texcoords[3][1];
           
           for (int i = 0; i < 4; i++) {
              m_texcoords_mirrored[i][0] = m_texcoords[3 - i][0];
              m_texcoords_mirrored[i][1] = m_texcoords[i][1];
           }
           
        } else
           for (int i=0; i < 4; i++)
             for (int j=0; j < 2; j++) {
                m_texcoords[i][j] = 0.0f;
                m_texcoords_mirrored[i][j] = 0.0f;
              }
     }
     
     int width () { return m_width; }
     int height () { return m_height; }
     int centerx () { return m_centerx; }
     int centery () { return m_centery; }
     int pitch () { return m_pitch; }
     Uint32 * data () { return m_data; }
     
     void setCenter(int x, int y)
     {
        m_centerx = x;
        m_centery = y;
     }
     
     float * texcoords () { return &m_texcoords[0][0]; }
     float * texcoords_mirrored () { return &m_texcoords_mirrored[0][0]; }
           
};

class cAnimation
{
protected:
   std::vector <cFrame *> frames;
   int m_width, m_height;
   int m_framewidth, m_frameheight;
   Uint32 * m_data;
public:
   cAnimation (int framecount, int frame_width, int frame_height);
   ~cAnimation ();
   cFrame * frame (int index);
   Texture * BuildTexture ();
   int framecount () { return frames.size(); }
   int framewidth () { return m_framewidth; }
   int frameheight () { return m_frameheight; }
};

Uint32 getAnimationBase (Uint32 model);

#endif //_ANIMATION_H_
