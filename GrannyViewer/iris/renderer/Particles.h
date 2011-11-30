//
// File: Particles.h
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

#ifndef _PARTICLES_H_
#define _PARTICLES_H_

#ifdef WIN32
#include <windows.h>
#endif

#include "SDL/SDL.h"
#include <list>
#include "renderer/Texture.h"

class cParticle
{
  private:
    float vec_pos[3];
    float vec_speed[3];
    float vec_acc[3];
    float m_lifetime;
    float texcoords[2][2];
  public:
    float m_time;
    cParticle (float x, float y, float z, float lifetime);
    virtual ~cParticle ();
    virtual void Handle (float time_fac);
    virtual void Draw(float vec1[3], float vec2[3]);
    virtual void Accelerate(float x, float y, float z);
    virtual void SetAcceleration(float x, float y, float z);
    void SetTexCoords(float x, float y, float w, float h);
};

class cParticleEngine 
{
  private:
    std::list<cParticle *> particles;
    Uint32 last_handle;
    Texture * m_texture;
  public:
    cParticleEngine ();
    ~cParticleEngine ();
    void Handle ();
    void ClearParticles();
    void AddParticle(cParticle * particle);
    void Render(float dx, float dy);
    void LoadTexture(Texture * texture);    
};

extern cParticleEngine * pParticleEngine;


#endif //_PARTICLES_H_
