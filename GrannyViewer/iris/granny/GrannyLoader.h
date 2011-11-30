//
// File: GrannyLoader.h
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

#ifndef _GRANNYLOADER_H_
#define _GRANNYLOADER_H_

#ifdef WIN32
#include <windows.h>
#endif

#include "SDL/SDL.h"
#include <string>
#include <map>
#include <vector>

#include "GrannyModel.h"

class cAnimSet 
{
public:
	cAnimSet() : id(0) {}
	std::map <int, std::string> anim_names;
	int id;
	std::string defaultanim;
};

typedef std::map<Uint32, cGrannyModel*> GrannyModelMap;
typedef GrannyModelMap::iterator GrannyModelMapIter;
typedef std::map <int, cAnimSet *> GrannyAnimMap;
typedef GrannyAnimMap::iterator GrannyAnimMapIter;

class TiXmlElement;

class cGrannyLoader
{
private:
	std::string mulpath;
	std::string tex_basepath;
	GrannyModelMap models;
	int tick;

	inline void InsertAnimset(TiXmlElement *node);
	inline void InsertCharacter(TiXmlElement *node);
	inline void InsertCharacterAOS(TiXmlElement *node);
public:
	cGrannyLoader ();
	~cGrannyLoader ();

	bool Load(std::string filename, std::string mulpath);
	void Clear ();

	void Render(Uint32 id, Uint32 type, float& curtime, 
		GrnMatrix * left_matrix = NULL, GrnMatrix * right_matrix = NULL);
	void Render(Uint32 id, Uint32 type, float &curtime, 
		GrnMatrix * left_matrix, GrnMatrix * right_matrix, std::vector < int >bodyparts);

	GrannyModelMap& GetGrannyModels() { return models; }
	cGrannyModel* GetModel(Uint32 id);
	int GetAnimset(Uint32 id);
	bool isFound(Uint32 id);

// for grannyviewer
	bool Release(Uint32 id);
	bool IsShow(Uint32 id);
	float GetAnimLength(Uint32 id, Uint32 type);

	static std::string GetAnimName(int aid);
	static std::string GetAosBodyName(int bodyid);
	static std::map<int, std::string> GetAnimDataType();
	const GrannyAnimMap &GetAnimsetMap() { return m_animsets; }
protected:
	std::map <int, cAnimSet *> m_animsets;
};

extern cGrannyLoader * pGrannyLoader;

struct TypeData {
	TypeData ( const std::string& NewName, int NewBlock) : name(NewName) , blockid(NewBlock) {}
	std::string name;
	int blockid;

	static int GetAnimType(const std::string& animName);
	static std::string GetAnimTypeName(int animType);
};

typedef TypeData AnimTypeData;

#endif //_GRANNYLOADER_H_
