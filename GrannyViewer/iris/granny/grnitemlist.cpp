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

#include "stdafx.h"
#include "granny/grntype.h"
#include "granny/grnitemlist.h"

#include <stdlib.h>
#include <iostream>
#include <cassert>

#include "Debug.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ItemList::ItemList()
{
	textChunk = new TextChunk;
	objects = new Objects;
	meshes = new Meshes;
	bones = new Bones;
	boneTies = new BoneTies;
	textureInfos = new TextureInfos;
	textures = new Textures;
	animations = new Animations;
	unhandled = new Unhandled;
}

ItemList::~ItemList()
{
	delete textChunk;
	delete objects;
	delete meshes;
	delete bones;
	delete boneTies;
	delete textureInfos;
	delete textures;
	delete animations;
	delete unhandled;
}

void ItemList::load(cGrannyStream * file)
{
	assert(file);
	file->readDword();	//unknown
	listOffset=file->readDword();
	int i;
	for (i=0;i<2;i++)
		file->readDword();	//unknown
	dword oldPos=file->tellg();

	file->seekg(listOffset);
	dword children=file->readDword();
	for (i=0;i<3;i++)
		file->readDword();	//unknown
	loadList(children,file);
	file->seekg(oldPos);
}

void ItemList::loadList(dword peers,cGrannyStream * file)
{
	assert(file && textChunk && objects && meshes && bones && boneTies && textureInfos && textures && animations && unhandled);
	for (unsigned int i=0;i<peers;)
	{
		dword chunk=file->readDword();
		dword offset=file->readDword();
		dword children=file->readDword();

		switch (chunk)
		{
		case 0xCA5E0200:
			pDebug.Log(LEVEL_INFO1," [textchunk]");
			textChunk->load(file,offset,listOffset,children);
			break;
		case 0xCA5E0304:
			pDebug.Log(LEVEL_INFO1," [textureinfo]");
			textureInfos->load(file,offset,listOffset,children);
			break;
		case 0xCA5E0507:
			pDebug.Log(LEVEL_INFO1," [bone]");
			bones->load(file,offset,listOffset,children);
			break;
		case 0xCA5E0602:
			pDebug.Log(LEVEL_INFO1," [mesh]");
			meshes->load(file,offset,listOffset,children);
			break;
		case 0xCA5E0a01:
			pDebug.Log(LEVEL_INFO1," [unhandled] 0xCA5E0a01");
			unhandled->load(file,offset,listOffset,children);
			break;
		case 0xCA5E0b01:
			pDebug.Log(LEVEL_INFO1," [bonetie] 0xCA5E0b01");
			boneTies->load(file,offset,listOffset,children);
			break;
		case 0xCA5E0c01:
			pDebug.Log(LEVEL_INFO1," [bonetie] 0xCA5E0c01");
			boneTies->load(file,offset,listOffset,children);
			break;
		case 0xCA5E0d01:
			pDebug.Log(LEVEL_INFO1," [unhandled] 0xCA5E0d01");
			unhandled->load(file,offset,listOffset,children);
			break;
		case 0xCA5E0e01:
			pDebug.Log(LEVEL_INFO1," [texture]");
			textures->load(file,offset,listOffset,children);
			break;
		case 0xCA5E0f03:
			pDebug.Log(LEVEL_INFO1," [object]");
			objects->load(file,offset,listOffset,children);
			break;
		case 0xCA5E1003:
			pDebug.Log(LEVEL_INFO1," [unhandled] 0xCA5E1003");
			unhandled->load(file,offset,listOffset,children);
			break;
		case 0xCA5E1205:
			pDebug.Log(LEVEL_INFO1," [animation]");
			animations->load(file,offset,listOffset,children);
			break;
		case 0xCA5Effff:			//end of list
			pDebug.Log(LEVEL_INFO1," [end]");
			break;
		default:
			{
				pDebug.Log(LEVEL_INFO3, "Unknown ItemList Chunk: 0x%x", chunk);
				//assert(!"Unknown ItemList Chunk");
				return;
			}
		}
		i+=children+1;
	}
}

string ItemList::getTextureName()
{
	assert(textureInfos && textChunk && objects);

	dword id=textureInfos->getTextureID();
	if (!id)
		return "";
	dword key=textChunk->findString("__FileName");
	dword value=objects->getValue(id-1,key);

	string fn = textChunk->findID(value);
	if (!fn.length())
		return "";

	int i=fn.find_last_of('\\');
	if (i)
		i++;
	fn = fn.substr(i);
	return fn;
}

Meshes &ItemList::getMeshes()
{
	assert(meshes);
	return *meshes;
}

Bones &ItemList::getBones()
{
	assert(bones);
	return *bones;
}

BoneTies &ItemList::getTies()
{
	assert(boneTies);
	return *boneTies;
}

Textures &ItemList::getTextures()
{
	assert(textures);
	return *textures;
}

BoneAnim &ItemList::getBoneAnim(dword id)
{
	assert(animations);
	return animations->getBoneAnim(id);
}

float ItemList::getAnimLength()
{
	assert(animations);
	return animations->length();
}


Animations &ItemList::getAnimations()
{
	assert(animations);
	return *animations;
}

TextChunk &ItemList::getTextChunk() 
{ 
	assert(textChunk);
	return *textChunk; 
}

dword ItemList::findString(string str)
{
	assert(textChunk);
	return textChunk->findString(str);
}

dword ItemList::getValue(dword obj,dword key)
{
	assert(objects);
	return objects->getValue(obj-1,key);
}

dword ItemList::findValue(dword key,dword value)
{
	assert(objects);
	return objects->findValue( key, value );
}

std::string ItemList::findID(dword id)
{
	assert(textChunk);
	return textChunk->findID( id );
}

Objects &ItemList::getObjects() 
{ 
	assert(objects);
	return *objects; 
}

void ItemList::freeUnused()
{
	delete textChunk; textChunk = NULL;
	delete objects;   objects = NULL;
	delete unhandled; unhandled = NULL;
	delete boneTies;  boneTies = NULL;
}