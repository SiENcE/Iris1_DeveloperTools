//
// File: ModelInfoLoader.h
// Created by: ArT-iX ArT-iX@libero.it
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

#ifndef _MODELINFO_LOADER_
#define _MODELINFO_LOADER_

#ifdef WIN32
#include <windows.h>
#endif

#include "SDL/SDL.h"
#include <string>
#include <map>

class cModelInfoEntry
{
private:
	int m_id;
	float m_scalex;
	float m_scaley;
	float m_scalez;
	int m_alpha;
	int m_defhue;
	int m_alt_body;
public:
	cModelInfoEntry (int id, float scalex, float scaley, float scalez, int alpha, int defhue, int alt_body);
	int id ();
	float scalex ();
	float scaley ();
	float scalez ();
	int alpha ();
	int defhue ();
	int alt_body ();   
};

class cModelInfoLoader
{
private:
	std::map<int, cModelInfoEntry *> model_infos;
	float m_scale_factor;
public:
	cModelInfoLoader();
	~cModelInfoLoader();

	bool Init (std::string filename);
	void DeInit ();

	float getScaleFactor();

	cModelInfoEntry * GetModelEntry(int id);   
};

extern cModelInfoLoader pModelInfoLoader;

#endif
