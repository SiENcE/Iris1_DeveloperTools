//
// File: GrannyModel.h
// Created by:   Alexander Oster - tensor@ultima-iris.de
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

#ifndef __GRANNYMODEL_H__
#define __GRANNYMODEL_H__

#include <string>
#include "renderer/Texture.h"
#include "granny/grnmatrix.h"

#define HAND_NONE   0
#define HAND_LEFT   1
#define HAND_RIGHT  2
#define HAND_OWNER  3

class cIrisModelFile;
class cGrannyFile;
class cGrannyAnimation;

class cGrannyModel
{
public:
	cGrannyModel() : color(0xffffffff), show(true) {}
	virtual ~cGrannyModel() {};
	virtual void Render(int animid, int tick, float & curtime,
		GrnMatrix * left_matrix, GrnMatrix * right_matrix) = 0;
	virtual int Age(int tick) = 0;
	virtual void Free () = 0;
	virtual void AddAnimation(int animid, std::string filename) = 0;
	virtual void SetHandBones(int left, int right) = 0;
	virtual void SetHand (int hand) = 0;
	virtual void SetAnimset(int anim_set) {}
	virtual int animset() { return 0; }
	virtual int  GetHand () { return HAND_NONE; }
	
	virtual std::string GetFileName(bool fullpath = false) = 0;
	virtual std::string GetAnimFileName(int animid, bool fullpath=false) { return ""; }

	virtual bool ExportModel(std::string filename) { return false; }
	virtual bool ExportAnimation(std::string filename, int animid) { return false; }
	
	virtual float GetAnimLength(int animid) { return 0.0f; }
	
	virtual cGrannyAnimation *GetAnimation(int animid) { return NULL; }
	virtual cGrannyFile *GetModel(int modelid=0) { return NULL; }
	
	virtual void SetColor(dword color) { this->color = color; }
	virtual dword GetColor() { return color; }

	virtual void ReleaseFile() {}
	
	std::string desc;
	dword color;
	bool show;
};

#endif
